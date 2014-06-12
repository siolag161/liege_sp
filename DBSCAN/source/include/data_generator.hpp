/****************************************************************************************
 * File: data_generator.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 11/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_DATA_GENERATOR_HPP
#define CLUSTERING_DATA_GENERATOR_HPP

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

namespace clustering
{

typedef ublas::vector<double> FeaturesWeights;
typedef ublas::matrix<double> ClusterData;
typedef ublas::matrix<double> DistanceMatrix;
typedef std::vector<uint32_t> Neighbors;
typedef std::vector<int32_t> Labels;

static ClusterData gen_cluster_data( size_t features_num, size_t elements_num );


} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_DATA_GENERATOR_HPP
