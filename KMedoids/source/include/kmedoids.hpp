/****************************************************************************************
 * File: kmedoids.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 13/06/2014
 * @Based on: http://github.com/tgamblin/muster
 ***************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010, Lawrence Livermore National Security, LLC.  
// Produced at the Lawrence Livermore National Laboratory  
// LLNL-CODE-433662
// All rights reserved.  
//
// This file is part of Muster. For details, see http://github.com/tgamblin/muster. 
// Please also read the LICENSE file for further information.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the disclaimer below.
//  * Redistributions in binary form must reproduce the above copyright notice, this list of
//    conditions and the disclaimer (as noted below) in the documentation and/or other materials
//    provided with the distribution.
//  * Neither the name of the LLNS/LLNL nor the names of its contributors may be used to endorse
//    or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// LAWRENCE LIVERMORE NATIONAL SECURITY, LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CLUSTERING_KMEDOIDS_HPP
#define CLUSTERING_KMEDOIDS_HPP

#include <algorithm>
#include <numeric>
#include <cassert>
#include <cstdlib>
#include <sys/time.h>
#include <limits>

#include <boost/random.hpp>

#include "MatrixUtils.hpp"
#include "partition.hpp"
#include "random.hpp"
namespace clustering
{

typedef double Diss; // short for dissimilarity
typedef std::pair<ObjectId, Diss> Obj_Diss;
/**
 *
 */
struct PAM_Partition: public Partition {
  PAM_Partition(const size_t nbrObjects): Partition(nbrObjects) {}
  // PAM_Partition( double e ): Partition(e) {}
 
  std::vector<ClusterId> sec_clusters; // Gives the o
};

 

/**
   Classic K-Medoids clustering, using the Partitioning-Around-Medoids (PAM)
   algorithm as described in Kaufman and Rousseeuw. 

   @param distance         dissimilarity matrix for all objects to cluster
   @param k                number of clusters to produce
   @param initial_medoids  Optionally supply k initial object ids to be used as initial medoids.

   @see \link build_dissimilarity_matrix()\endlink, a function to automatically
   construct a dissimilarity matrix given a vector of objects and a distance function.
*/
struct PAM {

  /**
   *
   */
  PAM(): partition(0), eps(1e-10), random(get_time_seed()), rng(random) {}

  /**
   *
   */
  PAM( double epsilon ): partition(0), eps(epsilon), random(get_time_seed()), rng(random) {}
  
  /**
   */
  template< class DataMatrix, class Distance >
  PAM_Partition& operator()( const DataMatrix& dataMat, Distance& dist, const size_t K, const size_t n_steps = 150 );

  const PAM_Partition& get_partition() const { return partition; }
 private:
  /**
   */
  // template< class DataMatrix, class Distance >
  // void init_medoids( const DataMatrix& dataMat, Distance& dist, const size_t k );
  
 private:
  PAM_Partition partition;
  double eps; // epsilon
  
  ///
  typedef boost::mt19937 random_type;
  random_type random;
  typedef boost::random_number_generator<random_type, unsigned long> rng_type;
  rng_type rng;

  double total_dissimilarity;
  
};

/**
 *
 */
template< class DataMatrix, class Distance >  
void init_medoids( PAM_Partition& partition,
                   const size_t k,
                   const DataMatrix& dataMat,
                   Distance& dist );



/** find the non-memoid obj that minimizes the diss to the rest
 *
 */
template< class DataMatrix, class Distance >  
Obj_Diss object_most_similar( const PAM_Partition& partition,
                                               const DataMatrix& dataMat,
                                               Distance& dist );

/** find the non-memoid obj that minimizes the diss to the rest
 *
 */
template< class DataMatrix, class Distance >  
Obj_Diss object_best_gain( const PAM_Partition& partition,
                           const DataMatrix& dataMat,
                           Distance& dist );



/** for each object, assigns to the cluster with the closest medoid
 *  when there is only one cluster -> no need to compute the distance
 */
template< class DataMatrix, class Distance >  
double assign_objects_to_clusters( PAM_Partition& partition,
                                   const DataMatrix& dataMat,
                                   Distance& dist );

/** for each object, assign to the cluster with the closest medoid
 *  when there is only one cluster -> no need to compute the distance 
 */
template< class DataMatrix, class Distance >  
double get_data_level_tolerance( const double eps,
                                 // const PAM_Partition& partition,
                                 const DataMatrix& dataMat,
                                 Distance& dist );

/** for each object, assign to the cluster with the closest medoid
 *  when there is only one cluster -> no need to compute the distance 
 */
template< class DataMatrix, class Distance >  
double swap_gain( const PAM_Partition& partition,
                  const ClusterId& cid,
                  const ObjectId& oid,
                  const DataMatrix& dataMat,
                  Distance& dist );


} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{

/**
 */
template< class DataMatrix, class Distance >
PAM_Partition& PAM::operator()( const DataMatrix& dataMat, Distance& dist, const size_t K, const size_t MAX_STEPS ) {
    
  size_t nbrObjs = nrows(dataMat);
  std::cout << "begin init...( using KR's buid...)" << std::endl;
  init_medoids( partition, K, dataMat, dist ); // choose the initial init according to greedy style
  std::cout << "done init..." << std::endl;
  double tolerance = get_data_level_tolerance( eps, dataMat, dist );
  std::cout << "begin swapping..." << std::endl;

  int step = 0;
  while (true) {
    // initial cluster setup
    //std::cout << "\n\nbegin step: " << (step+1) << std::endl;
    //std::cout << "assigning to clusters..." << std::endl;
    assign_objects_to_clusters( partition, dataMat, dist );
    total_dissimilarity = assign_objects_to_clusters( partition, dataMat, dist );// assign_objects_to_clusters(matrix_distance(distance));
    //vars to keep track of minimum
    double maxGain = 0.0;
    ClusterId maxCluster = 0;
    MedoidId maxObject = 0;
    
    // std::cout << "check swapping..." << std::endl;
    for ( ClusterId cid = 0; cid < K; ++ cid ) {
      for ( ObjectId oid = 0; oid << partition.n_objects(); ++ oid ) {
        if ( partition.is_medoid(oid) ) continue;
        // trying to replace the medoid of CID by OID,
        double currGain = swap_gain( partition, cid, oid, dataMat, dist ); // computes the gain
        if ( currGain > maxGain ) {
          maxGain = currGain;
          maxCluster = cid;
          maxObject = oid;
        }
      }
    }          
    if ( step++ < MAX_STEPS ) break; // make no sense if MAX_STEPS = 0
    if ( maxGain < tolerance ) break;
    partition.set_cluster( maxObject, maxCluster );
    partition.set_medoid( maxCluster, maxObject );
  }

  printf("step: %d, diss = %.3f \n", step, total_dissimilarity);

  return partition;
}


/** considers switching medoid of cluster c and h and measures
 *  the impact on j
 */
template< class DataMatrix, class Distance >  
double swap_gain( const PAM_Partition& partition,
                  const ClusterId& c,
                  const ObjectId& h,
                  const ObjectId& j,
                  const DataMatrix& dataMat,
                  Distance& dist )
{
  double NO_IMPACT = 0.0;
  if ( partition.is_medoid(j) ) return NO_IMPACT;
  if (j == h ) return NO_IMPACT;

  MedoidId i = partition.medoid_of(c);
  
  double gain = 0.0;
  Diss d_jj = dist(j, partition.medoid_of( partition.cluster_of(j) ) );
  Diss d_ji = dist(j, i);

  Diss d_jh = dist(j, h);
  if ( d_ji > d_jj ) { // removing m has no impact
    gain = std::max( d_jj - d_jh, 0.0 );
  } else if (d_ji == d_jj ) { // m is currently the nearest neighbour for j
    Diss d_jj2 = dist(j, partition.medoid_of( partition.sec_clusters[j]) );
    if ( d_jh < d_jj2 ) { // if swap then h becomes j's medoid
      gain = d_jj - d_jh;
    } else { // the second now becomes j's medoid if swapped
      gain = d_jj - d_jj2;
    }
  }
  
  return gain;
}

/** switch the 
 *
 */
template< class DataMatrix, class Distance >  
double swap_gain( const PAM_Partition& partition,
                  const ClusterId& c,
                  const ObjectId& h,
                  const DataMatrix& dataMat,
                  Distance& dist )
{
  double total_gain = 0.0;
  #pragma omp parallel for reduction( + : total_gain )
  for ( ObjectId i = 0; i < partition.n_objects(); ++i ) {
    total_gain += swap_gain( partition, c, h, i, dataMat, dist );
  }
  return total_gain;
}


/** find the non-memoid obj that minimizes the diss to the rest
 *
 */
typedef double Diss; // short for dissimilarity
template< class DataMatrix, class Distance >  
double object_total_diss( const ObjectId i,     
                            const PAM_Partition& partition,
                            const DataMatrix& dataMat,
                            Distance& dist ) {
  double total = 0.0;
  #pragma omp parallel for reduction( + : total )
  for ( ObjectId j = 0; j < partition.n_objects(); ++j ) {
     total += dist(i, j);     
  }
  return total;
}

/** find the non-memoid obj that minimizes the diss to the rest
 *
 */
typedef double Diss; // short for dissimilarity
template< class DataMatrix, class Distance >  
Obj_Diss object_most_similar( const PAM_Partition& partition,
                              const DataMatrix& dataMat,
                              Distance& dist ) {
  size_t nbrObjects = dataMat.size();
  ObjectId minDisSimId = 0;
  double minDisSim = std::numeric_limits<double>::max();
  #pragma omp parallel for
  for ( ObjectId i = 0; i < nbrObjects; ++i ) {
    if ( partition.is_medoid(i) ) continue;
    double total = object_total_diss( i, partition, dataMat, dist );
    #pragma omp critical
    {
      if ( total < minDisSim ) {
        minDisSim = total;
        minDisSimId = i;
      }
    }
  }
  return Obj_Diss( minDisSimId, minDisSim );
}

/** gain from selecting oid as medoid  
 *
 */
template< class DataMatrix, class Distance >  
double object_gain( const ObjectId oid,
                    const PAM_Partition& partition,
                    const DataMatrix& dataMat,
                    Distance& dist ) {
  double gain = 0.0;
  #pragma omp parallel for reduction( + : gain )
  for ( size_t j = 0; j < partition.n_objects(); ++j ) {
    MedoidId medoid_j = partition.medoid_of( partition.cluster_of(j) );
    double dMedoid = dist( j, medoid_j );
    double dI = dist(j, oid);
    gain += std::max( dMedoid - dI, 0.0 );
  }
  return gain;
}

/** find the non-memoid obj that minimizes the gain to the rest
 *
 */
template< class DataMatrix, class Distance >  
Obj_Diss object_best_gain( const PAM_Partition& partition,
                           const DataMatrix& dataMat,
                           Distance& dist )

{
  size_t nbrObjects = dataMat.size();

  ObjectId bestGainId = 0;
  double bestGain = 0.0; //-std::numeric_limits<double>::max();

  #pragma omp parallel for
  for ( size_t i = 0; i < nbrObjects; ++i ) {
    if ( partition.is_medoid(i) ) continue;
    double gain = object_gain( i, partition, dataMat, dist );
    #pragma omp critical
    {
      if ( gain >= bestGain ) {
        bestGain = gain;
        bestGainId = i;
      }
    }
  }
  return Obj_Diss( bestGainId, bestGain );
}

/** for the given object, try to find the clusters of closest and second closests medoids
 *
 */
template< class DataMatrix, class Distance >  
std::pair< Obj_Diss, Obj_Diss> find_object_clusters( const ObjectId oid,
                                                     PAM_Partition& partition,
                                                     const DataMatrix& dataMat,
                                                     Distance& dist )
{
  Diss minD = std::numeric_limits<double>::max(), min2D =  std::numeric_limits<double>::max(); // minD2 = secondMin
  ClusterId minId = 0, min2Id = 0;
  #pragma omp parallel for
  for ( ClusterId cid = 0; cid < partition.n_clusters(); ++cid ) {
    MedoidId mid = partition.medoid_of(cid);
    double d = dist( mid, oid );
    #pragma omp critical
    {
      if ( d < minD )  {
        min2D = minD; min2Id = minId;
        minD = d; minId = cid;
      } else if ( d < min2D) {
        min2D = d; min2Id = cid;
      }
    }
  }

  return std::pair<Obj_Diss, Obj_Diss>(Obj_Diss(minId, minD), Obj_Diss(min2Id, min2D) );
}

/** for each object, assign to the cluster with the closest medoid
 *  when there is only one cluster -> no need to compute the distance 
 */
template< class DataMatrix, class Distance >  
double assign_objects_to_clusters( PAM_Partition& partition,
                                   const DataMatrix& dataMat,
                                   Distance& dist )
{
  if ( partition.sec_clusters.size() != partition.n_objects() ) {
    partition.sec_clusters.resize( partition.n_objects() );
  }
       
  double total_diss = 0.0;

  #pragma omp parallel for
  for ( ObjectId oid = 0; oid < partition.n_objects(); ++ oid ) {
    std::pair< Obj_Diss, Obj_Diss> clusters = find_object_clusters( oid, partition, dataMat, dist );
    
    #pragma omp critical
    {
      total_diss += clusters.first.second; // first -> minClust, second -> minDist
      partition.set_cluster( oid, clusters.first.first ); // set best
      partition.sec_clusters[oid] = clusters.second.first;
    }
  }
  return total_diss;
}

/**
 *
 */
template< class DataMatrix, class Distance >  
void init_medoids( PAM_Partition& partition,
                   const size_t K,
                   const DataMatrix& dataMat,
                   Distance& dist ) {
  
  partition.clear( dataMat.size() );
  assert( partition.medoids.size() == 1);
  // the first medoid is the object that minimizes the diss w.r.t. the rest
  // std::cout << "computing first medoid..." << std::endl;
  Obj_Diss first_medoid = object_most_similar( partition, dataMat, dist );
  //std::cout << "done computing..." << std::endl;

  partition.medoids[0] = first_medoid.first;
  //std::cout << "assigning to this first medoid..." << std::endl;
  assign_objects_to_clusters( partition, dataMat, dist );

  for ( size_t clust = 1; clust < K; ++clust ) {
    //std::cout << "finding the " << clust << " medoid..." << std::endl;
    Obj_Diss objBestGain = object_best_gain( partition, dataMat, dist );
    MedoidId mid = objBestGain.first;
    //std::cout << "updating the medoid " << clust << " medoid..." << std::endl;
    partition.insert_medoid( objBestGain.first );
    // std::cout << "assigning to this medoid..." << std::endl;
    assign_objects_to_clusters( partition, dataMat,dist );
  }
}

/** for each object, assign to the cluster with the closest medoid
 *  when there is only one cluster -> no need to compute the distance 
 */
template< class DataMatrix, class Distance >  
double get_data_level_tolerance( const double eps,
                                 const DataMatrix& dataMat,
                                 Distance& dist ) {  
  if ( dataMat.empty() ) return 0.0;
  double total_sum = 0.0;
  
  for ( auto &row: dataMat ) {
    for ( auto &item: row ) {
      total_sum += item;
    }
  }

  return eps * total_sum / nitems(dataMat);
}


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_KMEDOIDS_HPP
