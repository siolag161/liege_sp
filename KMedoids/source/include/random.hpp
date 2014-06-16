/****************************************************************************************
 * File: random.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 13/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_RANDOM_HPP
#define CLUSTERING_RANDOM_HPP

#include <sys/time.h>

namespace clustering
{

///
/// Returns a seed for random number generators based on the product
/// of sec and usec from gettimeofday().
///
inline long get_time_seed() {
  struct timeval time;
  gettimeofday(&time, 0);
  return time.tv_sec * time.tv_usec;
}

} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_RANDOM_HPP
