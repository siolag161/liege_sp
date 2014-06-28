/****************************************************************************************
 * File: average_strategy.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 19/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_AVERAGE_STRATEGY_HPP
#define CLUSTERING_AVERAGE_STRATEGY_HPP

// #include "Cluster.hpp"
namespace clustering
{

template< class Distance >
struct AverageStrategy {

  AverageStrategy( Distance& distance): dist(distance) {}
  
  template< class DataMat, class Cluster >
  double operator()( const DataMat& dm,
                     const Cluster& clusterA,
                     const Cluster& clusterB )
  {
    double result = 0.0;
    double totalSize = 0.0;
    typename Cluster::iterator itA, itB;
    for ( itA = clusterA.begin(); itA != clusterA.end(); ++itA ) {
      for ( itB = clusterB.begin(); itB != clusterB.end(); ++itB ) {
        result += dist(*itA,*itB);
        ++totalSize;
      }
    }
    return totalSize == 0 ? 0.0 : result / totalSize;
  }

  Distance& dist;

};

} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{

} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_AVERAGE_STRATEGY_HPP
