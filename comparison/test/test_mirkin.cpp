#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include "partition.hpp"
#include "mirkin.hpp"

#include <set>

using namespace clustering; 
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_KMedoids, Data ) 

BOOST_AUTO_TEST_CASE( Test_Mirkin ) {
  // inline double mirkin_distance( const Clustering& c1, const Clustering& c2 ) {
  Clustering cl1, cl2;

  std::set<int> c1 {1,3,5,7};
  std::set<int> c2 {0,2,4,6};

  cl1.push_back(c1); cl1.push_back(c2);
  cl2.push_back(c1); cl2.push_back(c2);

  Mirkin mirkin;
  double mv = mirkin(cl1, cl2);
  BOOST_CHECK_EQUAL( mv, 0.0 );
}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
