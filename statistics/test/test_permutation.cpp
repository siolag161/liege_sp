#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include "fisher.hpp"
#include "statistics.hpp"
#include <boost/random.hpp> // boost::mt19937
#include <vector>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <random>
using namespace stats;
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_Permutation, Data ) 


struct CollectionPermutate {
  CollectionPermutate() {
    rng.seed(currentTime());
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
    //  std::shuffle(std::begin(cards_), std::end(cards_), std::default_random_engine());

  }
 private:
  boost::mt19937 rng;    
};


typedef std::vector< std::vector<int> > Matrix;
typedef std::vector<int> Phenotype;
double correct_permute( const double p, const std::vector<double>& dist ) {
  std::ofstream os("permu_test.txt");
  for ( auto& v: dist) os << v << std::endl;
  size_t count = 0;
  for ( size_t j = 0; j < dist.size(); ++j ) {
    if (dist[j] < p) ++count;
  }
  os.close();  
  return count*1.0/dist.size();
}

/** todo: change to list (with name and such)
 *
 */
void performTesting( std::vector<std::vector<double> >& result,
                     const Matrix& mat,
                     const Phenotype& phenotype,
                     const int codage,
                     const int permu ) {
  StatisticTest<CHISQ_YATES> chisq;
  size_t nvars = mat.size();
  Phenotype pheno = phenotype;
  // std::cout << "nvars: " << nvars << std::endl;
  result.resize( 1,  std::vector<double>(nvars, 0.0) );
  std::vector<double>& chisq_pvals = result[0]; // g2_cor_pvals = result[1], fisher_pvals = result[2];
  for ( size_t v = 0; v < nvars; ++v ) {
    chisq_pvals[v] = chisq( mat.at(v), pheno, codage, 2);
  }

  if ( permu > 0 ) {
    CollectionPermutate permutate;
    std::vector<double> chisq_vals( permu, 0.0);
    for ( int i = 0; i < permu; ++ i ) {
      double chisq_max = -1.0;
      permutate(pheno);
      std::random_shuffle( pheno.begin(), pheno.end() );
      for ( auto& snp: mat ) {
        double pm_chisq = chisq(snp, pheno, codage, 2);        
        chisq_max = std::max( chisq_max, pm_chisq );        
      }
      chisq_vals[i] = chisq_max; 
    }

    for ( size_t v = 0; v < nvars; ++v ) {
      double tmp = chisq_pvals[v];
      chisq_pvals[v] = correct_permute( tmp, chisq_vals );
      printf("pval. before: %.15f, after: %.15f\n", tmp, chisq_pvals[v] );
    }
  }

}

BOOST_AUTO_TEST_CASE( Test_Permutation ) {
  std::vector<std::vector<double> > result;
  std::vector<int> pheno;
  for (int i = 0; i < 663; ++i) pheno.push_back(0);
  for (int i = 0; i < 772; ++i) pheno.push_back(1);
  BOOST_CHECK_EQUAL(pheno.size(), 1435);

  std::vector< std::vector<int> > geno(1, std::vector<int>());;
  for (int i = 0; i < 251; ++i) geno[0].push_back(0);
  for (int i = 0; i < 165; ++i) geno[0].push_back(1);
  for (int i = 0; i < 195; ++i) geno[0].push_back(2);
  for (int i = 0; i < 50; ++i) geno[0].push_back(3);
  for (int i = 0; i < 2; ++i) geno[0].push_back(4);

  for (int i = 0; i < 331; ++i) geno[0].push_back(0);
  for (int i = 0; i < 221; ++i) geno[0].push_back(1);
  for (int i = 0; i < 185; ++i) geno[0].push_back(2);
  for (int i = 0; i < 34; ++i) geno[0].push_back(3);
  for (int i = 0; i < 1; ++i) geno[0].push_back(4);
  BOOST_CHECK_EQUAL(geno[0].size(), 1435);
  int codage = 5, permu = 1500;
  performTesting( result, geno, pheno, codage, permu );
  
  // StatisticTest<stats::CHISQ> chisq;
  // BOOST_CHECK_CLOSE( chisq(geno[0], pheno, 3, 2), 0.2415293, 0.0001 );

  // std::vector<int> a { 0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1 };
  // CollectionPermutate permutate;
  // for ( int i = 0; i < 10; ++i ) {
  //   permutate(a);
  //   for ( auto& v: a ) {
  //     std::cout << v << " ";
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << result[0][0] << std::endl;
 
}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
