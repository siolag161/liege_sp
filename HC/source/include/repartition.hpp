/****************************************************************************************
 * File: repartition.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 18/06/2014

 ************************************************* **************************************/
#ifndef PDT_REPARTITION_HPP
#define PDT_REPARTITION_HPP

#include <vector>
#include <utility>
#include "cluster.hpp"
namespace clustering
{

struct Repartition {
  typedef size_t Size;
  typedef int Id;
 
  typedef Cluster::ClusterPtr ClusterPtr;
  typedef std::vector<ClusterPtr>::iterator ClusterPtrIt;

  Repartition(): currentMaxId(0){}
  ~Repartition(){}
  
  Size nclusters() const { return clusters.size(); }
  
  Repartition& add( ClusterPtrIt left, ClusterPtrIt right ) {
    *left = ClusterPtr( new Cluster( currentMaxId++, *left, *right) );
    clusters.erase( right );
    return *this;
  }

  Repartition& initClusters( const Size nclusters ) {
    currentMaxId = 0;
    clusters.resize(nclusters);
    for ( size_t i = 0; i < nclusters; ++ i ) {
      auto clustPtr = std::make_shared<Cluster>(currentMaxId++);
      clusters[i] = clustPtr;
    }
  }
  
  // Repartition& setLabel( Id id, Id lab ) { labels[id] = lab; }
  
  // void initLabels( size_t nclusters ) {
  //   labels.resize(nclusters, -1);
  //   // for ( unsigned i = 0; i < nclusters; ++i ) {
  //   //   labels[i] = i;
  //   // }
  // }
  
  std::vector<ClusterPtr> clusters; // current labels
  // std::vector<Id> labels;
  Size currentMaxId;
};

} // namespace pdtends here. pdt

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


// template< class DataMat, class Strategy >
// Repartition::ClusterPtrItPair Repartition::closestClusterPair( const DataMat& dm,
//                                                                Strategy& strategy ) {
//   ClusterPtrIt itA, itB;
//   ClusterPtrIt bestItA, bestItB;
//   double bestDist = std::numeric_limits<double>::max();
//   for ( itA = clusters.begin(); itA < clusters.end(); ++itA ) {
//     for ( itB = itA + 1; itB < clusters.end(); ++itB ) {
//       double dist = strategy( dm, **itA, **itB );
//       if ( dist < bestDist ) {
//         bestDist = dist;
//         bestItA = itA;
//         bestItB = itB;
//       }
//     }
//   }

//   //bestItA = clusters.begin();
//   //bestItB = bestItA+1;

//   return ClusterPtrItPair( bestItA, bestItB );
// }

} // namespace pdtends here. pdt

/****************************************************************************************/
#endif // PDT_REPARTITION_HPP
