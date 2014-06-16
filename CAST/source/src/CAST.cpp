#include <string>
#include <boost/lexical_cast.hpp> 
#include <stdlib.h>     /* abs */

#include "MutInfo.hpp"
#include "CAST.hpp"
#include <memory>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>


namespace fltm {
/////////////////////////////////////////////////////////////////////////

CAST::partition_ptr CAST::operator()( SimilarityCompute& simCompute,
                                      const std::vector<unsigned>& local2Global ) {
  partition_ptr result(new CAST_Partition);
  if (local2Global.empty())
    return result;
  CAST_Cluster unAssignedCluster;
  for (Index i = 0; i < local2Global.size(); ++i) { 
    unAssignedCluster.push_back(CAST_Item(i, local2Global.at(i), 0.0) );    
  }
  
  return performClustering( simCompute, unAssignedCluster);  
}

///////////////////////////////////////////////////////////////////////////////
CAST::partition_ptr CAST::performClustering( SimilarityCompute& simCompute, 
                                             CAST_Cluster& unAssignedCluster) {
  partition_ptr result(new CAST_Partition); // @todo: change the name of the alias

  while ( unAssignedCluster.size() ) { 
    std::unique_ptr<CAST_Cluster> openCluster(new CAST_Cluster); 
    resetAffinity(unAssignedCluster);

    bool changesOccurred = true;
    while (changesOccurred && unAssignedCluster.size()) {     
      changesOccurred = false;
      std::unique_ptr<AffinityCompute> maxCompute(new AffinityCompute);
      std::unique_ptr<AffinityCompute> minCompute(new AffinityCompute);
      while ( unAssignedCluster.size() ) {
        Index maxAffIdx = (*maxCompute)(unAssignedCluster, std::greater<double>());
        if ( unAssignedCluster.at(maxAffIdx).affinity >= thresCAST*openCluster->size() ) {
          changesOccurred = true;
          addGoodItem( unAssignedCluster, *openCluster, simCompute, maxAffIdx );
        } else {
          break;
        }
      }
      maxCompute.reset();
      while( unAssignedCluster.size() ) {      
        Index minAffIdx = (*minCompute)(*openCluster, std::less<double>());
        if ( openCluster->at(minAffIdx).affinity < thresCAST*openCluster->size() ) {
          changesOccurred = true;
          removeBadItem( unAssignedCluster, *openCluster, simCompute, minAffIdx );
        } else {
          break;
        }       
      }
      minCompute.reset();
    }    
    openCluster->set_name(boost::lexical_cast<std::string>(result->size())); 
    result->insert( std::move(openCluster) );
  }  
  return result;  
}

void resetAffinity( CAST_Cluster& cluster) {
  for ( auto it = cluster.begin(); it != cluster.end(); ++it ) {
    it->affinity = 0.0;
  } 
}

/** Searches for the optimal item in given cluster
 *
 */
template<typename Compare>
const Index AffinityCompute::operator()(const CAST_Cluster& clust, Compare comp)
{
  Index result = 0; 
  for (Index idx = 1; idx < clust.size(); idx++) {
    if ( comp(clust.at(idx).affinity, clust.at(result).affinity) ) { 
      result = idx;    
    }
  }
  
  return result;
}


//////////////////////////////////////////////////////////////////////////////////////
void addGoodItem( CAST_Cluster& unAssignedCluster, CAST_Cluster& openCluster, 
                  SimilarityCompute& simCompute, const Index clusterIdx ) {  
  updateClustersAffinity( unAssignedCluster, openCluster, simCompute,
                          clusterIdx );  
}

void removeBadItem( CAST_Cluster& unAssignedCluster, CAST_Cluster& openCluster, 
                    SimilarityCompute& simCompute, const Index clusterIdx ) {
  updateClustersAffinity( openCluster, unAssignedCluster, simCompute,
                          clusterIdx); 
}

void updateClustersAffinity( CAST_Cluster& sourceCluster, CAST_Cluster& targetCluster,
                             SimilarityCompute& simCompute, 
                             const Index clusterIndex )
{
  const CAST_Item item = sourceCluster.at(clusterIndex);
  moveItemBetweenClusters( sourceCluster, targetCluster, clusterIndex );
  for (Index i = 0; i < sourceCluster.size(); i++) {
    sourceCluster[i].affinity += simCompute( sourceCluster[i].matrixIndex, item.matrixIndex );
  }

  for (Index i = 0; i < targetCluster.size(); i++) {
    targetCluster[i].affinity += simCompute( targetCluster[i].matrixIndex, item.matrixIndex );
  } 

}

void moveItemBetweenClusters( CAST_Cluster& source, CAST_Cluster& target, const Index clusterIndex ) {
  target.push_back( source.at(clusterIndex) );
  source.remove(clusterIndex);
}


SimilarityCompute::SimilarityCompute( const Positions& pos ,
                                      const Matrix& mat,
                                      unsigned maxPos,                                      
                                      double simiThres ):
    positions(pos),
    dataMat(mat),
    maxPosition(maxPos),
    m_simiThres(simiThres),
    entropyMap(std::vector<double> (pos.size(), -1.0))
{

  std::cout << "simi: " << simiThres << std::endl;
  if ( simiThres > 0.0 ) {
    std::cout << "init distance..." << std::endl;
    size_t nbrVars = pos.size();
    std::vector<double> entropyMap( nbrVars, -1.0 );
    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::median(boost::accumulators::with_p_square_quantile) > > acc; 
    for ( size_t varA = 0; varA < nbrVars; ++varA) {
      for ( size_t varB = varA+1; varB < nbrVars; ++varB ) {
        if ( abs( positions[varA] - positions[varB] ) >= maxPos ) continue; //result =  MAX_DISTANCE; // 1.0 = max distance possible
      
        size_t nbrVars = positions.size();
        unsigned key = 2*nbrVars*varA + varB;
        unsigned nvars = positions.size();
        std::vector<double> vals;
  
        Entropy<EMP> entropy; // empirical entropy (estimates the probability by frequency)
        JointEntropy<EMP> mutEntropy; // empirial mutual entropy;
        if ( entropyMap.at(varA) < 0.0 ) { 
          entropyMap[varA] = entropy( dataMat.at(varA) ); // computes entropy of varA only if not already done
        }        
        if ( entropyMap.at(varB) < 0.0 ) { // computes entropy of varB only if not already done
          entropyMap[varB] = entropy( dataMat.at(varB)) ; // since this operation could be expensive
        }
  
        double minEntropyAB = std::min( entropyMap[varA], entropyMap[varB]); // takes the min
        if (minEntropyAB != 0) {
          double mutEntropyAB = mutEntropy( dataMat.at(varA), dataMat.at(varB) );
          double mutualInfoAB = entropyMap[varA] + entropyMap[varB] - mutEntropyAB; // classic formula
          double normalizedMutInfo = mutualInfoAB / minEntropyAB;        
          simiCache[key] = normalizedMutInfo;
          acc(normalizedMutInfo);
        }
      }
    }
    m_simiThres = boost::accumulators::median(acc);
  }
}


double SimilarityCompute::operator()( unsigned varA,
                                      unsigned varB )
{
   // return result;
  if (  m_simiThres < 0 ) {
    if (varA == varB) return 1.0;
    if ( abs( positions[varA] - positions[varB] ) > maxPosition ) {
      return 0.0;
    }
  
    unsigned key = 1;
    if ( varA < varB ) {
      key = 2*nbrVars*varA + varB;
    } else {
      key = 2*nbrVars*varB + varA;
    }
    SimiCache::iterator iter = simiCache.find(key);

    unsigned nvars = positions.size();
    std::vector<double> vals;
  
    if ( iter == simiCache.end() ) {
      Entropy<EMP> entropy; // empirical entropy (estimates the probability by frequency)
      JointEntropy<EMP> mutEntropy; // empirial mutual entropy;
      if ( entropyMap.at(varA) < 0.0 ) { 
        entropyMap[varA] = entropy( dataMat.at(varA) ); // computes entropy of varA only if not already done
      }        
      if ( entropyMap.at(varB) < 0.0 ) { // computes entropy of varB only if not already done
        entropyMap[varB] = entropy( dataMat.at(varB)) ; // since this operation could be expensive
      }    
    
      double minEntropyAB = std::min( entropyMap[varA], entropyMap[varB]); // takes the min
      float result = 0.0;
      if (minEntropyAB != 0) {
        double mutEntropyAB = mutEntropy( dataMat.at(varA), dataMat.at(varB) );
        double mutualInfoAB = entropyMap[varA] + entropyMap[varB] - mutEntropyAB; // classic formula
        double normalizedMutInfo = mutualInfoAB / minEntropyAB;
        result = (normalizedMutInfo) ;// > simiThres) ? 1.0 : 0.0;
        simiCache[key] =  result;
      }
      return result;
    } else {
      return iter->second;
    }
  } else {
    if ( varA == varB ) return MAX_SIMILARITY;

    double result =  MIN_SIMILARITY; // 1.0 = max distance possible
    if ( abs( positions[varA] - positions[varB] ) < maxPosition ) {

      size_t nbrVars = dataMat.size();
      size_t key = 1;
      if ( varA < varB ) {
        key = 2*nbrVars*varA + varB;
      } else {
        key = 2*nbrVars*varB + varA;
      }

      if ( m_simiThres > 0 )
        result = simiCache[key] > m_simiThres ? MAX_SIMILARITY : MIN_SIMILARITY;
      else
        result = simiCache[key];
    }
  } 
}

}
