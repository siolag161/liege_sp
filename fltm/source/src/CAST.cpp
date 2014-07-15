
#include "CAST.hpp"

namespace fltm {

////////////////////////////////////////////////////////////////////////////////////////
Partition CAST::operator()( // const std::vector<int>& local2Global
                            ) {  
  // Partition result;
  // if ( local2Global.empty() ) return result;
  
  std::vector<CAST_Item> unAssignedCluster;
  for ( int i = 0; i < comp->size(); ++i ) { 
    unAssignedCluster.push_back(CAST_Item(i, 0.0) );    
  }

  return run( unAssignedCluster );
}

Partition CAST::run( std::vector<CAST_Item>& unAssignedCluster ) {
  Partition result;

  while ( unAssignedCluster.size() ) { 
    std::vector<CAST_Item> openCluster; 
    resetAffinity( unAssignedCluster );
    bool changesOccurred = true;
    while (changesOccurred && unAssignedCluster.size()) {     
      changesOccurred = false;
      AffinityCompute maxCompute;
      AffinityCompute minCompute;
      while ( unAssignedCluster.size() ) {
        int maxAffIdx = maxCompute( unAssignedCluster, std::greater<double>() );
        if ( unAssignedCluster.at(maxAffIdx).affinity >= thresCAST*openCluster.size() ) {
          changesOccurred = true;
          addGoodItem( unAssignedCluster, openCluster, *comp, maxAffIdx );
        } else {
          break;
        }
      }
      while( unAssignedCluster.size() ) {      
        int minAffIdx = minCompute( openCluster, std::less<double>() );
        if ( openCluster.at(minAffIdx).affinity < thresCAST*openCluster.size() ) {
          changesOccurred = true;
          removeBadItem( unAssignedCluster, openCluster, *comp, minAffIdx );
        } else {
          break;
        }       
      }
    }    

    int cluster_id =  result.nbrClusters();
    for ( auto& item: openCluster ) {
      result.cluster( item.index, cluster_id );
    }
  }  
  return result;  
}

void CAST::resetAffinity( std::vector<CAST_Item>& cluster ) {
  for ( std::vector<CAST_Item>::iterator it = cluster.begin(); it != cluster.end(); ++it ) {
    it->affinity = 0.0;
  } 
}



////////////////////////////////////////////////////////////////////////////////////////
/** Searches for the optimal item in given cluster
 *
 */
template<typename Compare>
const int AffinityCompute::operator()( const std::vector<CAST_Item>& clust, Compare comp) const
{
  int result = 0; 
  for (int idx = 1; idx < clust.size(); idx++) {
    if ( comp(clust.at(idx).affinity, clust.at(result).affinity) ) { 
      result = idx;    
    }
  }
  
  return result;
}


//////////////////////////////////////////////////////////////////////////////////////
void addGoodItem( std::vector<CAST_Item>& unAssignedCluster, std::vector<CAST_Item>& openCluster, 
                  Comp& simCompute, const int clusterIdx ) {  
  updateClustersAffinity( unAssignedCluster, openCluster, simCompute,
                          clusterIdx );  
}

void removeBadItem( std::vector<CAST_Item>& unAssignedCluster, std::vector<CAST_Item>& openCluster, 
                    Comp& simCompute, const int clusterIdx ) {
  updateClustersAffinity( openCluster, unAssignedCluster, simCompute,
                          clusterIdx); 
}

void updateClustersAffinity( std::vector<CAST_Item>& sourceCluster, std::vector<CAST_Item>& targetCluster,
                             Comp& simCompute, 
                             const int clusterIndex )
{
  const CAST_Item item = sourceCluster.at(clusterIndex);
  moveItemBetweenClusters( sourceCluster, targetCluster, clusterIndex );
  for (int i = 0; i < sourceCluster.size(); i++) {
    sourceCluster[i].affinity += simCompute( sourceCluster[i].index, item.index );
  }

  for (int i = 0; i < targetCluster.size(); i++) {
    targetCluster[i].affinity += simCompute( targetCluster[i].index, item.index );
  } 

}

void moveItemBetweenClusters( std::vector<CAST_Item>& source, std::vector<CAST_Item>& target, const int clusterIndex ) {
  target.push_back( source.at(clusterIndex) );
  //source.remove(clusterIndex);
  source.erase( source.begin() + clusterIndex );
}



}
