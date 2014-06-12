/****************************************************************************************
 * File: CAST.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 30/04/2014

 ***************************************************************************************/
#ifndef CLUSTERING_CAST_HPP
#define CLUSTERING_CAST_HPP

#include "Clustering.hpp"
namespace fltm
{

typedef std::string Name;
typedef unsigned Index;

struct CAST_Item {  
  Index matrixIndex;
  Index globalIndex;
  double affinity;

  CAST_Item(const Index matIdx, const Index glbIdx, const double aff):
      matrixIndex(matIdx), globalIndex(glbIdx), affinity(aff) {}
};


typedef int Position;
typedef std::vector< Position > Positions;
typedef std::map< unsigned, double > SimiCache;
typedef std::vector < std::vector <int> > Matrix;


struct SimilarityCompute {
  const double MAX_SIMILARITY = 1.0, MIN_SIMILARITY = 0.0;
  explicit SimilarityCompute( const Positions& pos ,
                              const Matrix& mat,
                              unsigned maxDistance,                                      
                              double simiThres );
  virtual ~SimilarityCompute() {}
  virtual double operator()( unsigned, unsigned );
 protected:
  const Positions& positions;
  const Matrix& dataMat;
  unsigned maxPosition;  
  double m_simiThres;

  std::vector<double> entropyMap;

  unsigned nbrVars;
  SimiCache simiCache;

};

typedef Partition< CAST_Item > CAST_Partition;
typedef Cluster < CAST_Item > CAST_Cluster;

typedef std::map< unsigned, std::map<unsigned, double> > SimiMatrix;


// template<class T>
// class std::unique_ptr<T>;

struct CAST {
  typedef std::unique_ptr<CAST_Partition> partition_ptr;
  CAST (const double& thres): thresCAST(thres) {}  
  partition_ptr operator()( SimilarityCompute& simCompute,
                            const std::vector<unsigned>& localGlobal); 
  
 protected:  
  partition_ptr performClustering( SimilarityCompute& simCompute,
                                   CAST_Cluster& unassignedCluster);
 protected:
  double thresCAST;
};


////////////////////////////////////////////////////////////////////
struct AffinityCompute {
  template<typename Compare>
  const Index operator()(const CAST_Cluster& clust, Compare comp);
};


inline void resetAffinity( CAST_Cluster& cluster);


void addGoodItem( CAST_Cluster& unassignedCluster, CAST_Cluster& openCluster, 
                  SimilarityCompute& simMatrix, const Index clusterIdx );

void removeBadItem( CAST_Cluster& unassignedCluster, CAST_Cluster& openCluster, 
                    SimilarityCompute& simMatrix, const Index clusterIdx );

void updateClustersAffinity( CAST_Cluster& sourceCluster, CAST_Cluster& targetCluster, 
                             SimilarityCompute& simMatrix,
                             const Index clusterIndex );

void moveItemBetweenClusters( CAST_Cluster& source, CAST_Cluster& target, const Index clusterIndex );


} // namespace ends here. 


/****************************************************************************************/
#endif // _CAST_HPP
