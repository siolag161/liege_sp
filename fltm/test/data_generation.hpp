/****************************************************************************************
 * File: DataGeneration.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 08/04/2014

 ***************************************************************************************/
#ifndef DATA_GEN_DATAGENERATION_HPP
#define DATA_GEN_DATAGENERATION_HPP

#include "pl.h"

#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_01.hpp>

#include <fstream>

//namespace dtt = data_table;

namespace data_gen
{

typedef std::vector< std::vector<int> > Matrix;

typedef plSymbol Variable;
typedef plVariablesConjunction Variables;

typedef boost::uniform_real<> RealDistribution; // choose a distribution
typedef boost::uniform_int<> NumberDistribution; // choose a distribution

typedef boost::mt19937 RandomNumberGenerator;    // pick the random number generator method,
typedef boost::variate_generator< RandomNumberGenerator&, NumberDistribution > Generator;  // link the generator to the distribution

typedef std::vector<float> DistValues ;
typedef std::vector<DistValues> DistValueVec;
typedef std::vector<DistValueVec> DistValueMat;


static RandomNumberGenerator generator(std::time(0));

// generator.seed( std::time(0) );  

struct GenerateNaiveBayes {

  GenerateNaiveBayes( Variables& childVs, Variable& latentV, const size_t nbrIs ):
      variables(childVs), latentVar(latentV), nbrInds(nbrIs)
  {}
  
  inline Matrix operator()(const bool includeLatent = true);  

  Variables& variables;
  Variable& latentVar;
  size_t nbrInds;
  plJointDistribution jointDist;  
};

struct GenerateClusteredData {
  GenerateClusteredData( const size_t nbrClusts, const size_t clustSz, const size_t card, const size_t nbrIs ):
      nbrClusters(nbrClusts), clustSize(clustSz), cardinality(card), nbrInds(nbrIs) {}  
  inline Matrix operator()(); 
 protected:
  size_t nbrClusters;
  size_t clustSize;
  size_t cardinality;
  size_t nbrInds;
};


inline void writeData(const Matrix& matrix, std::string outFileName);
inline Generator getUniformGenerator(const float rangeMin, const float rangeMax);
//////////////////////////////////////////////////////////////////////////////////////////////
inline plJointDistribution createNBJointDist(Variables& variables, Variable& latentVar);
inline const DistValues createNBUniVarProbTab(const size_t card);
inline DistValueMat createNBProbTables( const Variables& variables, const Variable& latentVar);
inline plComputableObjectList createNBCndTable( Variables& variables, Variable& latentVar);
///////////////////////////////////////////////////////////////////////////////////////////////
typedef std::vector<size_t> Cluster;
typedef std::vector<Cluster> Clustering;
inline plJointDistribution createClusteringJointDist( const Variables& variables, const Clustering& clusterint);
inline plComputableObjectList  createClusterJointDist( const Variables& variables, const Cluster& cluster );
inline DistValueMat createClusterProbTables( const Variables& variables, const Cluster& cluster );

inline void normalizeDistValues(DistValues& distValues);
} // namespace data_genends here. data_gen

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace data_gen
{

Matrix GenerateNaiveBayes::operator()(const bool includeLatent) {
  Matrix matrix;
  matrix.reserve(nbrInds); 
  plValues values(latentVar ^ variables);
  jointDist = createNBJointDist(variables, latentVar);
  //std::cout << jointDist << std::endl << jointDist.get_computable_object_list() << std::endl;
  for (size_t ind = 0; ind < nbrInds; ++ind) {
    jointDist.draw(values);
    if (includeLatent) {
      std::vector<int> row(variables.size()+1); 
      row[0] = values[ latentVar ];
      for (size_t var = 0; var < variables.size(); ++var) {
        row[var+1] = values[variables[var]]; 
      }
      matrix.push_back(row);
    } else {
      std::vector<int> row(variables.size()); 
      for (size_t var = 0; var < variables.size(); ++var) {
        row[var] = values[variables[var]]; 
      }
      matrix.push_back(row);
    } 
  }    
  return matrix;
}

plJointDistribution createNBJointDist(Variables& variables, Variable& latentVar) {
  const DistValues probValZ = createNBUniVarProbTab( latentVar.cardinality()) ;
  const plProbTable probTabZ(latentVar, probValZ, true);
  
  plComputableObjectList cndProbTab_X_Z = createNBCndTable(variables, latentVar);
  plJointDistribution jointDist(latentVar ^ variables, probTabZ * cndProbTab_X_Z);

  return jointDist;
}

const DistValues createNBUniVarProbTab(const size_t cardinality) {

  //std::cout << var.name() << " - " << var.cardinality() << std::endl;

  Generator generator = getUniformGenerator(2,5);
  float total = 0.0;

  DistValues probTable( cardinality );
  for (size_t val = 0; val < cardinality; ++val) {    
    probTable[val] = generator();
    total += probTable[val];
  }
   
  for (size_t val = 0; val < cardinality; ++val) {    
    probTable[val] /= total;
  }
  
  return probTable;
}

Generator getUniformGenerator(const float rangeMin, const float rangeMax) {
  NumberDistribution distribution( rangeMin, rangeMax );
 
  Generator numberGenerator(generator, distribution);
  return numberGenerator;  
}


plComputableObjectList createNBCndTable(Variables& variables, Variable& latentVar) {
  plComputableObjectList dataCndTable;
  
  DistValueMat probTableXZ = createNBProbTables(variables, latentVar);
  
  for (size_t x = 0; x < variables.size(); ++x) { 
    plDistributionTable distTab_Xi_Z(variables[x], latentVar); 
    for (size_t h = 0; h < latentVar.cardinality(); ++h) {
      distTab_Xi_Z.push( plProbTable(variables[x], probTableXZ[x][h]), (int)h );
    }
    dataCndTable *= distTab_Xi_Z; // adds the conditional table to result 
  }
    
  return dataCndTable;
}

DistValueMat createNBProbTables( const Variables& variables,  const Variable& latentVar) {
  DistValueMat probTableX;    
  for (size_t var = 0; var < variables.size(); ++var) {
    DistValueVec X_Z;
    for (size_t i = 0; i < latentVar.cardinality(); ++i) {
      const DistValues X_Z_i = createNBUniVarProbTab( variables[var].cardinality() );
      X_Z.push_back(X_Z_i);
    }
    probTableX.push_back(X_Z);
  }    
  return probTableX;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

static Matrix Transpose( const Matrix& mat ) {
  Matrix m( mat[0].size(), std::vector<int>( mat.size(), 0) );

  for ( size_t r = 0; r < mat[0].size(); ++ r ) {
    for ( size_t c = 0; c < mat.size(); ++c ) {
      m[r][c] = mat[c][r];
    }
  }

  return m;
}

Matrix GenerateClusteredData::operator()() {
  Matrix matrix;
  matrix.reserve(nbrInds);
  Variables variables;
  
  for (size_t var = 0; var < nbrClusters * clustSize; ++var) {
    variables ^= Variable( boost::lexical_cast<std::string>(var),
                           plIntegerType(0, cardinality-1) );
  }

  Clustering clustering; clustering.reserve(nbrClusters);
  for ( size_t clust = 0; clust < nbrClusters; ++clust ) {
    Cluster cluster;
    for ( size_t item = 0; item < clustSize; ++item ) {
      cluster.push_back( clust*clustSize + item ); 
    }
    clustering.push_back( cluster );
  }
  
  plJointDistribution jointDist = createClusteringJointDist( variables, clustering);
  plValues values( variables );
  // std::cout << jointDist << std::endl << jointDist.get_computable_object_list() << std::endl;
  for (size_t ind = 0; ind < nbrInds; ++ind) {
    jointDist.draw(values);   
    std::vector<int> row(variables.size()); 
    for (size_t var = 0; var < variables.size(); ++var) {
      row[var] = values[variables[var]];  
    }
    matrix.push_back(row);
  }

  //std::cout << jointDist << std::endl;
  return Transpose(matrix);
}

plJointDistribution createClusteringJointDist( const Variables& variables, const Clustering& clustering) {
  plComputableObjectList cndProbTabs;
  for ( Clustering::const_iterator it = clustering.begin(); it != clustering.end(); ++it ) {
    cndProbTabs *= createClusterJointDist( variables, *it);
  }
  //std::cout << cndProbTabs << std::endl;
  plJointDistribution jointDist( variables, cndProbTabs );
  return jointDist;
}


plComputableObjectList createClusterJointDist( const Variables& variables, const Cluster& cluster ) {
  plComputableObjectList cndProbTab;
  const Variable& var =  variables[*cluster.begin()];
  const DistValues probs = createNBUniVarProbTab( var.cardinality() ) ;
  const plProbTable probTab( var, probs, true );
  cndProbTab *= probTab;

  DistValueMat clusterProbTables = createClusterProbTables( variables, cluster );
  //./bVariables clustVars;
  // clustVars ^= var;
  
  Cluster::const_iterator curr, next;
  curr = cluster.begin(); next = curr + 1;

  DistValueMat::const_iterator ptIt = clusterProbTables.begin();
  while ( next != cluster.end() ) {
    plDistributionTable distTab_Xi_Z( variables[*next], variables[*curr] ); 
    for (size_t h = 0; h < variables[*curr].cardinality(); ++h) {
      distTab_Xi_Z.push( plProbTable( variables[*next], ptIt->at(h)), (int)h );
    }
    cndProbTab *= distTab_Xi_Z; 
    ++curr; ++next; ++ptIt;
  }

  return cndProbTab; //plJointDistribution( clustVars, cndProbTab );
}


void normalizeDistValues( DistValues& distValues ) {
  float sum = 0.0;
  for (size_t i = 0; i < distValues.size(); ++i) {
    sum += distValues[i];
  }
  for (size_t i = 0; i < distValues.size(); ++i) {
    distValues[i] /= sum;
  } 
}

static const float RATE = 0.99;
DistValueMat createClusterProbTables( const Variables& variables, const Cluster& cluster ) {
  DistValueMat distValueMat;
  Cluster::const_iterator curr, next;
  curr = cluster.begin(); next = curr + 1;
  while ( next != cluster.end() ) {
    DistValueVec X_Z;    
    for (size_t i = 0; i < variables[*curr].cardinality(); ++i) {      
      DistValues X_Z_i = createNBUniVarProbTab( variables[*next].cardinality() );
      X_Z_i[i] = 1.0 / (1.0 - RATE);
      normalizeDistValues(X_Z_i);
      X_Z.push_back(X_Z_i);      
    }    
    distValueMat.push_back(X_Z);
    ++curr; ++next;
  }
  return distValueMat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
 // void writeData(const Matrix& matrix, std::string outFileName) {
 //   std::ofstream out(outFileName);
 //   out << matrix;  
 //   out.close();
 // }

} // namespace data_genends here. data_gen

/****************************************************************************************/
#endif // DATA_GEN_DATAGENERATION_HPP
