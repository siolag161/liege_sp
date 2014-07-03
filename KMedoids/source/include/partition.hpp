/****************************************************************************************
 * File: partition.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 13/06/2014
 * Based on the code of the Muster Project, retrived http://github.com/tgamblin/muster whose license
 * given right below: */
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
#ifndef CLUSTERING_PARTITION_HPP
#define CLUSTERING_PARTITION_HPP

#include <iostream>
#include <cstddef>
#include <vector>
#include <cstdint>
#include <set>

namespace clustering
{

typedef int MedoidId;
typedef int ObjectId;
typedef int ClusterId;

typedef std::vector< std::set<ObjectId> > Clustering;
struct Partition {

  // Partition(){}
  std::vector<MedoidId> medoids; // Gives the o
  std::vector<ClusterId> clusters; // Gives the cluster id for objects

  Partition( const size_t nbrObjects );
  ~Partition() {}

  Partition& insert_medoid( const MedoidId mid ) { medoids.push_back(mid); return *this; }
  Partition& set_cluster( const ObjectId oid, const ClusterId cid ) { clusters[oid] = cid; return *this; }
  Partition& set_medoid( const ObjectId oid, const MedoidId mid ) { medoids[oid] = mid; return *this; }

  /// Checks if an object is a memoid)
  bool is_medoid( const ObjectId oid ) const {
    return medoid_of( cluster_of(oid) ) == oid;
  }

  MedoidId medoid_of ( const ClusterId cid) const { return medoids[cid]; }
  ClusterId cluster_of ( const ObjectId oid ) const { return clusters[oid]; }

  /// return the nbr of clusters
  size_t n_clusters() const { return medoids.size(); }

  size_t n_objects() const { return clusters.size(); }

  size_t n_objects( const int clust_id );

  Clustering to_clustering() const;

  void clear( const size_t );
};

double mirkin_distance( const Clustering& c1, const Clustering& c2 ); 
double mirkin_distance( const Partition& p1, const Partition& p2 ) ;

} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


///
/// Compute the total dissimilarity between all objects and their medoids.
///
template <typename Distance>
double total_dissimilarity( const Partition& p, Distance& dist ) {
  double dissim = 0.0;
  for ( size_t i = 0; i < p.clusters.size(); i++ ) {
    dissim += dist(i, p.medoids[p.clusters[i]]);
  }
  return dissim;
}

///
/// Compute the total dissimilarity between all objects in a particular cluster and its medoid.
///
template <typename Distance>
double total_dissimilarity(const Partition& p, Distance& dist, MedoidId m) {
  double dissim = 0.0;
  for (size_t i=0; i < p.clusters.size(); i++) {
    if (p.clusters[i] == m) {
      dissim += dist(i, p.medoids[p.clusters[i]]);
    }
  }
  return dissim;
}
  
///
/// Compute the total squared dissimilarity between all objects and their medoids.
///
template <typename Distance>
double total_squared_dissimilarity(const Partition& p, Distance& dist) {
  double dissim2 = 0.0;
  for (size_t i = 0; i < p.clusters.size(); i++) {
    double d = dist( i, p.medoids[p.clusters[i]] );
    dissim2 += d*d;
  }
  return dissim2;
}

///
/// Compute the total squared dissimilarity between all objects in a particular 
/// cluster and its medoid.
///
template <typename Distance>
double total_squared_dissimilarity(const Partition& p, Distance& dist, MedoidId m) {
  double dissim2 = 0.0;
  for (size_t i = 0; i < p.clusters.size(); i++) {
    if (p.clusters[i] == m) {
      double d = dist(i, p.medoids[p.clusters[i]]);
      dissim2 += d * d;
    }
  }
  return dissim2;
}

inline std::ostream& operator<< ( std::ostream& out, const Partition& partition ) {
  for ( size_t i = 0; i < partition.n_objects(); ++i ) {
    auto cid = partition.clusters[i];
    auto mid = partition.medoids[cid];
    out << i << ": " << cid << " (" << mid << ")" << std::endl;
  }

  return out;
}


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_PARTITION_HPP
