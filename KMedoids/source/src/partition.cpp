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

#include <algorithm>

#include "partition.hpp"
#include "counter.hpp"

namespace clustering {

/**
 *
 */
Partition::Partition( const size_t nbrObjects): clusters( nbrObjects, 0) {
  if ( nbrObjects ) medoids.resize(1);
}

/**
 *
 */
size_t Partition::n_objects( const int clust_id ) {
  return std::count( clusters.begin(), clusters.end(), clust_id );
}

Clustering Partition::to_clustering() const {
  Clustering clt;
  //clt.reserve( medoids.size() );
  clt.resize( medoids.size(), std::set<ObjectId>() );
  for ( size_t obj = 0; obj < n_objects(); ++ obj ) {
    ClusterId clustId = clusters[obj];
    clt[clustId].insert(obj);
  }
  return clt;
}

/// only clear the content, not free the memory
void Partition::clear( size_t nbrObjects ) {
  //medoids.clear();
  //std::vector<MedoidId>().swap( medoids );
  medoids.resize(1,0);
  // std::vector<ClusterId>( clusters );
  clusters.resize( nbrObjects, 0);
}

///////////////////////////////////////////////////////////////////////////////
/**
 *
 */
double mirkin_distance( const Clustering& c1, const Clustering& c2 ) {
  size_t c1_sum2 = 0;
  size_t n = 0;
  for (size_t i=0; i < c1.size(); i++) {
    c1_sum2 += c1[i].size() * c1[i].size();
    n += c1[i].size();
  }

  size_t c2_sum2 = 0;
  for (size_t i=0; i < c2.size(); i++) {
    c2_sum2 += c2[i].size() * c2[i].size();    
  }
  

  size_t c1c2_sum2 = 0;
  for (size_t i=0; i < c1.size(); i++) {
    for (size_t j=0; j < c2.size(); j++) {
      size_t size;
      std::set_intersection(c1[i].begin(), c1[i].end(), 
                       c2[j].begin(), c2[j].end(),
                       counter(size));
      c1c2_sum2 += size * size;
    }
  }

  return (c1_sum2 + c2_sum2 - (2 * c1c2_sum2)) / (double)(n*n);
}

/**
 *
 */
double mirkin_distance( const Partition& p1, const Partition& p2 ) {
  Clustering c1 = p1.to_clustering();
  Clustering c2 = p2.to_clustering();

  return mirkin_distance( c1, c2 );
}



}
