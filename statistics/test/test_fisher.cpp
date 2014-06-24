#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include "fisher.hpp"
#include "statistics.hpp"
//using namespace clustering; 
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_Fisher, Data ) 


BOOST_AUTO_TEST_CASE( Test_Fisher ) {
  std::vector<int> geno;
  for ( int i = 0; i < 12; ++i ) geno.push_back(0);
  for ( int i = 0; i < 5; ++i ) geno.push_back(1);
  for ( int i = 0; i < 7; ++i ) geno.push_back(0);
  for ( int i = 0; i < 7; ++i ) geno.push_back(1);

  std::vector<int> pheno;
  for ( int i = 0; i < 17; ++i ) pheno.push_back(0);
  for ( int i = 0; i < 14; ++i ) pheno.push_back(1);
  
  stats::StatisticTest<stats::FISHER> fisher;
  BOOST_CHECK_CLOSE( fisher(geno, pheno, 2, 2), 0.2883081, 0.0001 );
}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
