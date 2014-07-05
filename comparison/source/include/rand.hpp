/****************************************************************************************
 * File: Rand.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 02/07/2014

 ***************************************************************************************/
#ifndef CLUSTERING_RAND_HPP
#define CLUSTERING_RAND_HPP

#include "measure.hpp"
#include "partition.hpp"
namespace clustering
{
struct RandIndex: public ComparisonMeasure {
  virtual double compute( const Clustering& clusteringA, const Clustering& clusteringB ) const {
    Partition pA (clusteringA), pB(clusteringB);
    return compute( pA, pB );
  }

  //  
  virtual double compute( const Partition& pA, const Partition& pB ) const {    
    assert( pA.nbr_objects() == pB.nbr_objects() );
    unsigned n_11 = 0, n_00 = 0; // n_01 = 0, n_10 = 0,
    size_t N = pA.nbr_objects();
    
    for ( size_t i = 0; i < N; ++i )
      for ( size_t j = i+1; j < N; ++j ) {
        int c_iA = pA.cluster(i), c_iB = pB.cluster(i), c_jA = pA.cluster(j), c_jB = pB.cluster(j);
        if ( c_iA == c_jA && c_iB == c_jB ) ++n_11;
        else if ( c_iA != c_jA && c_iB != c_jB) ++n_00;     
      }      

    return (n_11+n_00)*2 / double(N*(N-1));
  }

  virtual std::string name() const { return "RandIndex"; }

};


///////////////////////////////////////////////////////////////////////////////////////////////
struct AdjustedRandIndex: public RandIndex {
  
  virtual double compute( const Partition& pA, const Partition& pB ) const {    
    const size_t R = pA.nbr_clusters(), S = pB.nbr_clusters(), N = pA.nbr_objects();    
    std::vector< std::vector<int> > conTab(R, std::vector<int>(S, 0));   
    std::vector<int> sumA(R, 0), sumB(S,0);    
    for ( size_t i = 0; i < N; ++i ) {
      int cA = pA.cluster(i), cB = pB.cluster(i);
      ++conTab[cA][cB];
      ++sumA[cA];
      ++sumB[cB];
    }
    double expected_idx = 0, idx = 0, max_idx = 0, rs = 0;
    for ( size_t cA = 0; cA < R; ++cA ) {
      for ( size_t cB = 0; cB < S; ++cB ) {
        int val = conTab[cA][cB];
        idx += double(val*(val-1)) / 2;        
      }
    }
    double expected_idxA = 0, expected_idxB;
    for ( auto& a: sumA ) {
      expected_idxA += double(a*(a-1))/2;
    }

    for ( auto& b: sumB ) {
      expected_idxB += double(b*(b-1))/2;
    }
    expected_idx = 2 * expected_idxA * expected_idxB / (N*(N-1));
    max_idx = 0.5*( expected_idxA + expected_idxB );
    rs = ( max_idx == expected_idx ) ? 0.0 : ( idx - expected_idx ) / ( max_idx - expected_idx );
    return rs;
  }

  virtual std::string name() const { return "AdjustedRandIndex"; }

};

} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_RAND_HPP
