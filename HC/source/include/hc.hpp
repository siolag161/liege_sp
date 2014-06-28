/****************************************************************************************
 * File: hc.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 19/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_HC_HPP
#define CLUSTERING_HC_HPP

#include <limits.h>
#include "repartition.hpp"
#include "Distance.hpp"
#include "cluster.hpp"

#include <iostream>

namespace clustering
{

struct HierarchicalClustering {

  typedef size_t Height;
  typedef size_t Size;
  typedef Repartition::ClusterPtrIt ClusterPtrIt;
  typedef Repartition::ClusterPtr ClusterPtr;
  typedef std::pair< ClusterPtrIt, ClusterPtrIt> ClusterPtrItPair;

  template< class DataMat, class Strategy >
  const Repartition& operator()( const DataMat& dm,
                                 Strategy& strategy,
                                 const Size s = std::numeric_limits<int>::max() );

  
  Size nclusters() const { return repartition.nclusters(); }

  
  template< class Clusters, class DataMat, class Strategy >
  static ClusterPtrItPair closestClusterPair( Clusters& clusters,
                                       const DataMat& dm,
                                       Strategy& strategy );

 private:
  //template< class DataMat >
  
  inline void initClusters( const Size nclusters );
  

 private:
  Repartition repartition;
};

  
  

} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{

template< class DataMat, class Strategy >
const Repartition& HierarchicalClustering::operator()( const DataMat& dm,
                                                       Strategy& strategy,
                                                       const Size s) {

  Height currSize = 0;
  initClusters( dm.size() );
  while ( repartition.nclusters() >= 2 && repartition.nclusters() > s ) {
    auto cip = closestClusterPair( repartition.clusters, dm, strategy );
    repartition.add( cip.first, cip.second );
    if ( repartition.nclusters() % 10 == 0 )  
      std::cout << repartition.nclusters() << std::endl;
  }

  return repartition;
}

void HierarchicalClustering::initClusters( const Size nclusters ) {
  repartition.initClusters(nclusters);

}

template< class Clusters, class DataMat, class Strategy >
HierarchicalClustering::ClusterPtrItPair HierarchicalClustering::closestClusterPair( Clusters& clusters,
                                                                                     const DataMat& dm,
                                                                                     Strategy& strategy ) {
  ClusterPtrIt bestItA = clusters.begin(), bestItB = clusters.begin();
  double bestDist = std::numeric_limits<double>::max();

  #pragma omp parallel for schedule(dynamic) 
  for ( size_t i = 0; i < clusters.size(); ++i ) {
    #pragma omp parallel for 
    for ( size_t j = i+1; j < clusters.size(); ++j ) {
      double dist = strategy( dm, *clusters[i], *clusters[j] );
      #pragma omp critical
      if ( dist < bestDist ) {
        bestDist = dist;
        bestItA = clusters.begin() + i;
        bestItB = clusters.begin() + j;
      }
    }
  }
  
  return ClusterPtrItPair( bestItA, bestItB );
}

} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_HC_HPP
