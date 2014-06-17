#include <string>
#include <boost/lexical_cast.hpp> 
#include <stdlib.h>     /* abs */

#include "CAST.hpp"
#include <memory>


#include "Similarity.hpp"

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

}
