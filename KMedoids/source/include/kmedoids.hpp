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

// #include <boost/random.hpp>

#include "MatrixUtils.hpp"
#include "partition.hpp"
// #include "random.hpp"
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
  PAM(): partition(0), eps(1e-10)//, random(get_time_seed()), rng(random)
  {}

  /**
   *
   */
  PAM( double epsilon ): partition(0), eps(epsilon)//,  random(get_time_seed()), rng(random)
  {}
  
  /**
   */
  template< class DataMatrix, class Distance >
  PAM_Partition& operator()( const DataMatrix& dataMat, Distance& dist, const size_t K, const size_t n_steps = 150 );

  const PAM_Partition& get_partition() const { return partition; }
 private:
 
 private:
  PAM_Partition partition;
  double eps; // epsilon
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


template< class Distance >  
double cost( const PAM_Partition& partition, ClusterId i, ObjectId h, Distance& distance) {
  double total = 0;
  for (ObjectId j = 0; j < partition.clusters.size(); j++) {
    ObjectId mi  = partition.medoids[i];                // object id of medoid i
    double dhj = distance(h, j);               // distance between object h and object j      
    ObjectId mj1 = partition.medoids[partition.clusters[j]];   // object id of j's nearest medoid
    double dj1 = distance(mj1, j);             // distance to j's nearest medoid

    // check if distance bt/w medoid i and j is same as j's current nearest medoid.
    if (distance(mi, j) == dj1) {
      double dj2 = std::numeric_limits<double>::max();
      if (partition.medoids.size() > 1) {   // look at 2nd nearest if there's more than one medoid.
        ObjectId mj2 = partition.medoids[ partition.sec_clusters[j] ];  // object id of j's 2nd-nearest medoid
        dj2 = distance(mj2, j);                      // distance to j's 2nd-nearest medoid
      }
      total += std::min(dj2, dhj) - dj1;

    } else if (dhj < dj1) {
      total += dhj - dj1;
    }
  }
  return total;
}



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
    std::cout << "\n\nbegin step: " << (step+1) << std::endl;
    total_dissimilarity = assign_objects_to_clusters( partition, dataMat, dist );// assign_objects_to_clusters(matrix_distance(distance));
    //vars to keep track of minimum
    double maxGain = 0.0;
    ClusterId maxCluster = 0;
    MedoidId maxObject = 0;

    std::cout << "check swapping..." << std::endl;

   #pragma omp parallel for schedule(static) collapse(2)
    for ( ClusterId cid = 0; cid < K; ++ cid ) {
      for ( ObjectId oid = 0; oid < partition.n_objects(); ++ oid ) {
        if ( partition.is_medoid(oid) ) continue;
        double currGain = swap_gain( partition, cid, oid, dist ); // computes the gain
        if ( currGain > maxGain ) {
          #pragma omp critical
          {            
            maxGain = currGain;
            maxCluster = cid;
            maxObject = oid;
          }
        }
      }
    }
    
    if ( step++ > MAX_STEPS ) break; // make no sense if MAX_STEPS = 0
    if ( maxGain < tolerance ) break;
    partition.set_cluster( maxObject, maxCluster );
    partition.set_medoid( maxCluster, maxObject );

  }

  //if ( step % 5 == 0 )

  return partition;
}


/** considers switching medoid of cluster c and h and measures
 *  the impact on j
 */
template< class Distance >  
double swap_gain( const PAM_Partition& partition,
                  const ClusterId& c,
                  const ObjectId& h,
                  const ObjectId& j,
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
template< class Distance >  
double swap_gain( const PAM_Partition& partition,
                  const ClusterId& c,
                  const ObjectId& h,
                  Distance& dist )
{
  double total_gain = 0.0;
  for ( ObjectId j = 0; j < partition.n_objects(); ++j ) {
    // += swap_gain( partition, c, h, i, dist );

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

    total_gain += gain;
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
  //  std::cout << "objectId: " << i << std::endl;
  size_t nbrObjects = dataMat.size();
  ObjectId minDisSimId = 0;
  double minDisSim = std::numeric_limits<double>::max();

  for ( ObjectId i = 0; i < nbrObjects; ++i ) {
    double total = 0.0;
    for ( ObjectId j = 0; j < partition.n_objects(); ++j ) {
      total += dist(i, j);     
    }   
    if ( total < minDisSim ) {
      minDisSim = total;
      minDisSimId = i;
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

  for ( size_t i = 0; i < nbrObjects; ++i ) {
    if ( partition.is_medoid(i) ) continue;
    double gain = object_gain( i, partition, dataMat, dist );
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

  Diss minD, min2D;
  minD = min2D = std::numeric_limits<double>::max();
  ClusterId minId = 0, min2Id = 0;
  for ( ClusterId cid = 0; cid < partition.n_clusters(); ++cid ) {
    MedoidId mid = partition.medoid_of(cid);
    double d = dist( mid, oid );
    {
      if ( d < minD || partition.medoids[cid] == oid )  {
        min2D = minD; min2Id = minId;
        minD = d; minId = cid;
      } else if ( d < min2D) {
        min2D = d; min2Id = cid;
      }
    }
  }
  return std::pair<Obj_Diss, Obj_Diss>(Obj_Diss( minId, minD), Obj_Diss( min2Id, min2D) );
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

  #pragma omp parallel for reduction(+: total_diss)
  for ( ObjectId oid = 0; oid < partition.n_objects(); ++ oid ) {
    Diss minD, min2D;
    minD = min2D = std::numeric_limits<double>::max();
    ClusterId minId = 0, min2Id = 0;

    for ( ClusterId cid = 0; cid < partition.n_clusters(); ++cid ) {
      MedoidId mid = partition.medoid_of(cid);
      double d = dist( mid, oid );
      {
        if ( d < minD || partition.medoids[cid] == oid )  {
          min2D = minD; min2Id = minId;
          minD = d; minId = cid;
        } else if ( d < min2D) {
          min2D = d; min2Id = cid;
        }
      }
    }
 
    total_diss += minD; // first -> minClust, second -> minDist
    #pragma omp critical
    partition.set_cluster( oid, minId ); // set best
    #pragma omp critical
    partition.sec_clusters[oid] = min2Id;

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
  std::cout << "init-find object most similar..." << std::endl;
  Obj_Diss first_medoid = object_most_similar( partition, dataMat, dist );
  partition.medoids[0] = first_medoid.first;
  assign_objects_to_clusters( partition, dataMat, dist );
  
  for ( size_t clust = 1; clust < K; ++clust ) {
    ObjectId best_obj = 0;
    double max_gain = -1.0;
    if ( clust % 10 == 0 )
      std::cout << clust << " cluster " << std::endl;

    //#pragma omp parallel for schedule(static) 
    for (size_t i = 0; i < partition.n_objects(); i++) {
      if ( partition.is_medoid(i) ) continue;
      double gain = 0.0;
      //#pragma omp parallel for reduction(+: gain) 
      for (size_t j = 0; j < partition.n_objects(); j++) {
        double Dj = dist(j, partition.medoids[ partition.clusters[j] ]);  // distance from j to its medoid
        double v = std::max(Dj - dist(i,j), 0.0);                 // gain from selecting i
        gain += v;
      }

      //#pragma omp critical
      if (gain >= max_gain) {   // set the next medoid to the object that 
        max_gain = gain;        // maximizes the gain function.
        best_obj = i;
      }
    }
    
    partition.insert_medoid( best_obj );
    assign_objects_to_clusters( partition, dataMat, dist );    
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
