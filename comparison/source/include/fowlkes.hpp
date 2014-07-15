/****************************************************************************************
 * File: fowlkes.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 05/07/2014

 ***************************************************************************************/
#ifndef CLUSTERING_FOWLKES_HPP
#define CLUSTERING_FOWLKES_HPP

#include "measure.hpp"
#include "partition.hpp"
#include <cmath>
namespace clustering
{
struct FowlkesIndex: public ComparisonMeasure {
  virtual double compute( const Clustering& clusteringA, const Clustering& clusteringB ) const {
    Partition pA (clusteringA), pB(clusteringB);
    return compute( pA, pB );
  }

  //  
  virtual double compute( const Partition& pA, const Partition& pB ) const {    
    assert( pA.nbr_objects() == pB.nbr_objects() );
    int n_11 = 0, n_00 = 0, n_01 = 0, n_10 = 0;
    size_t N = pA.nbr_objects();
    
    for ( size_t i = 0; i < N; ++i )
      for ( size_t j = i+1; j < N; ++j ) {
        int c_iA = pA.cluster(i), c_iB = pB.cluster(i), c_jA = pA.cluster(j), c_jB = pB.cluster(j);
        if ( c_iA == c_jA && c_iB == c_jB ) ++n_11;
        else if ( c_iA != c_jA && c_iB != c_jB) ++n_00;
        else if ( c_iA == c_jA && c_iB != c_jB) ++n_01;
        else if ( c_iA != c_jA && c_iB == c_jB) ++n_10;
      }      

    // printf("11: %d, 00: %d, 01: %d, 10:%d\n", n_11, n_00, n_01, n_10);
    return std::sqrt( double(n_11*n_11)/ ((n_11+n_01)*(n_11+n_10)) );
  }

  virtual std::string name() const { return "Fowlkes-Mallows-Index"; }

};

}
/****************************************************************************************/
#endif // CLUSTERING_FOWLKES_HPP
