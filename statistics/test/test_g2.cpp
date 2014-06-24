#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include "statistics.hpp"
#include <omp.h>
#include <chrono>
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_G2, Data ) 


BOOST_AUTO_TEST_CASE( Test_G2 )
{
  std::vector<int> geno;
  for ( int i = 0; i < 12; ++i ) geno.push_back(0);
  for ( int i = 0; i < 5; ++i ) geno.push_back(1);
  for ( int i = 0; i < 7; ++i ) geno.push_back(0);
  for ( int i = 0; i < 7; ++i ) geno.push_back(1);
  std::vector<int> pheno;
  for ( int i = 0; i < 17; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 14; ++i ) pheno.push_back(1);
  
  stats::StatisticTest<stats::G2> g2;
  BOOST_CHECK_CLOSE( g2(geno, pheno, 2, 2), 0.240864, 0.0001 );
}

BOOST_AUTO_TEST_CASE( Test_G2_Corrected )
{
  std::vector<int> geno;
  for ( int i = 0; i < 12; ++i ) geno.push_back(0);
  for ( int i = 0; i < 5; ++i ) geno.push_back(1);
  for ( int i = 0; i < 7; ++i ) geno.push_back(0);
  for ( int i = 0; i < 7; ++i ) geno.push_back(1);
  
  std::vector<int> pheno;
  for ( int i = 0; i < 17; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 14; ++i ) pheno.push_back(1);
  
  stats::StatisticTest<stats::G2_YATES> g2;
  // BOOST_CHECK_CLOSE( g2(geno, pheno, 2, 2), 0.240864, 0.0001 );
}

BOOST_AUTO_TEST_CASE( Test_Stress )
{
  unsigned NTESTS = 10000;
  std::vector<int> geno;
  for ( int i = 0; i < 1000; ++i ) geno.push_back(0);
  for ( int i = 0; i < 1000; ++i ) geno.push_back(1);
  for ( int i = 0; i < 1000; ++i ) geno.push_back(0);
  for ( int i = 0; i < 1000; ++i ) geno.push_back(1);

  std::vector<int> pheno; 
  for ( int i = 0; i < 2000; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 2000; ++i ) pheno.push_back(1);

  // omp_set_num_threads(1);
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();  
  stats::StatisticTest<stats::G2> g2;
  for ( int i = 0; i < NTESTS; ++i ) {
    g2(geno, pheno, 2, 2);
  }  
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();  
  std::cout << "Algo (sequential) took "
             << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " (ms).\n";
  // omp_set_num_threads(8);
  start = std::chrono::steady_clock::now();  
  for ( int i = 0; i < NTESTS; ++i ) {
    g2(geno, pheno, 2, 2);
  }
  end = std::chrono::steady_clock::now();  
  std::cout << "Algo (parallel) took "
             << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " (ms).\n";
}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
