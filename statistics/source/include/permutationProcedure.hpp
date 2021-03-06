/****************************************************************************************
 * File: permutationProcedure.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 25/06/2014

 ***************************************************************************************/
#ifndef STATS_PERMUTATIONPROCEDURE_HPP
#define STATS_PERMUTATIONPROCEDURE_HPP

#include <boost/random.hpp> // boost::mt19937
//#include <vector>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <random>
#include <omp.h>

#include "Graph.hpp"
namespace stats
{

struct CollectionPermutate {
  CollectionPermutate( unsigned long seed = 1 ) {
    rng.seed(seed);
  }

  static unsigned long currentTime() {
    unsigned long time =
        std::chrono::system_clock::now().time_since_epoch() / 
        std::chrono::nanoseconds(1);
    return time;
  }
  
  struct Rand: std::unary_function<unsigned, unsigned> {
    boost::mt19937 &_state;
    unsigned operator()(unsigned i) {
      boost::uniform_int<> rng(0, i - 1);
      return rng(_state);
    }
    Rand(boost::mt19937 &state) : _state(state) {}    
  };

  template<typename VecT>
  void operator()(VecT& vec, boost::mt19937 &state) {    
    Rand rand(state);
    std::random_shuffle(vec.begin(), vec.end());
  }

  template<typename VecT>
  void operator()(VecT& vec) {
    Rand rand(CollectionPermutate::rng);
    std::random_shuffle(vec.begin(), vec.end(), rand );
  }
 private:
  boost::mt19937 rng;    
};

template<typename T>
double correct_pval_distribution( const T p, const std::vector<T>& dist ) {
  double count = 0.0;
  for (auto& val: dist ) {
    if ( val < p) ++count;
  }
  return count / dist.size();
}

/** todo: change to list (with name and such)
 *
 //  */
template< class Matrix, class Vector >
void performTesting( std::vector< std::vector< std::vector<double> > >& dists,
                     std::vector<std::vector<double> >& result,
                     std::vector<StatTest*> statTests,
                     const Matrix& genoMat,
                     const Vector& phenotype,
                     const fltm::Graph& graph,
                     const int permu )
{
  int NP = 2;
  size_t nvars = genoMat.size(), ntests = statTests.size();
  int levels = fltm::graph_height( graph );
  
  result.resize( 2*ntests,  std::vector<double>(nvars, 0.0) );
  Vector pheno = phenotype;

  
  for ( size_t test = 0; test < ntests; ++ test ) {
    for ( size_t snp = 0; snp < nvars; ++snp ) {
  
      result[2*test][snp] = statTests[test]->execute( genoMat.at(snp), pheno,
                                                      graph[snp].variable.cardinality(),
                                                      NP );
      // printf("snp: %d: %f\n", snp,t][snp]  );

    }    
  }

  if ( permu > 0 ) {
    dists.resize( ntests,
                  std::vector< std::vector<double> >( levels+1,
                                                      std::vector<double>( permu, 2.0 )));
    

    CollectionPermutate permutate;
    #pragma omp parallel for
    for ( int p = 0; p < permu; ++p ) {
       #pragma omp critical
       permutate(pheno);
       for ( size_t test = 0; test < ntests; ++ test ) {
         if (statTests[test]->name == "Fisher") omp_set_num_threads(1);
         for ( size_t snp = 0; snp < nvars; ++snp ) {
           double pval = statTests[test]->execute( genoMat.at(snp), pheno, graph[snp].variable.cardinality(), NP);
           int level = graph[snp].level;          
           #pragma omp critical
           dists[test][level][p]= std::min( dists[test][level][p], pval );
         }    
      }       
    }

    for ( size_t snp = 0; snp < nvars; ++snp ) {
      int level = graph[snp].level;
      for ( size_t test = 0; test < ntests; ++ test ) {
         result[2*test+1][snp] = correct_pval_distribution( result[2*test][snp], dists[test][level] );
       }   
     }
  }
}


} // namespace stats ends here. stats


/****************************************************************************************/
#endif // STATS_PERMUTATIONPROCEDURE_HPP
