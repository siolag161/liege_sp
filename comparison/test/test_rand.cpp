#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include "partition.hpp"
#include "mirkin.hpp"
#include "rand.hpp"

#include <set>

using namespace clustering; 
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_RandIndex, Data ) 

BOOST_AUTO_TEST_CASE( Test_Rand ) {
  // inline double mirkin_distance( const Clustering& c1, const Clustering& c2 ) {
  Clustering cl1, cl2;
// true.id <- c(1, 1, 1, 2, 2, 2, 3, 3, 3)
// pred.id <- c(2, 1, 2, 1, 1, 1, 2, 1, 1)
// label   <- c(0, 0, 0, 0, 1, 0, 2, 0, 0)
  std::set<int> c11 { 0,1,2 };
  std::set<int> c12 { 3,4,5 };
  std::set<int> c13 { 6,7,8 };

  std::set<int> c21 { 1,3,4,5,7,8 };
  std::set<int> c22 { 0,2,6 };
  // std::set<int> c23 { };

  cl1.push_back(c11); cl1.push_back(c12); cl1.push_back(c13);
  cl2.push_back(c21); cl2.push_back(c22); // cl2.push_back(c23);

  RandIndex rand;
  double rv = rand(cl1, cl2);
  BOOST_CHECK_CLOSE( rv, 0.5277778, 0.0001 );
}


BOOST_AUTO_TEST_CASE( Test_Rand_Partition ) {
  // Clustering cl1, cl2;
  // true.id <- c(1, 1, 1, 2, 2, 2, 3, 3, 3)
  // pred.id <- c(2, 1, 2, 1, 1, 1, 2, 1, 1)
  // label   <- c(0, 0, 0, 0, 1, 0, 2, 0, 0)
  Partition pA(9), pB(9);
  pA.cluster(0,0).cluster(1,0).cluster(2,0).cluster(3,1).cluster(4,1).cluster(5,1).cluster(6,2).cluster(7,2).cluster(8,2);
  pB.cluster(0,1).cluster(1,0).cluster(2,1).cluster(3,0).cluster(4,0).cluster(5,0).cluster(6,1).cluster(7,0).cluster(8,0);  

  // cl1.push_back(c11); cl1.push_back(c12); cl1.push_back(c13);
  // cl2.push_back(c21); cl2.push_back(c22); // cl2.push_back(c23);

  RandIndex rand;
  double rv = rand(pA,pB);
  BOOST_CHECK_CLOSE( rv, 0.5277778, 0.0001 );

}

BOOST_AUTO_TEST_CASE( Test_Adj_Rand_Partition ) {
  Partition pA(9), pB(9);
  pA.cluster(0,0).cluster(1,0).cluster(2,0).cluster(3,1).cluster(4,1).cluster(5,1).cluster(6,2).cluster(7,2).cluster(8,2);
  pB.cluster(0,1).cluster(1,0).cluster(2,1).cluster(3,0).cluster(4,0).cluster(5,0).cluster(6,1).cluster(7,0).cluster(8,0);  

  AdjustedRandIndex arand;
  double rv = arand(pA,pB);
   BOOST_CHECK_CLOSE( rv, 0.05555556, 0.0001 ); 
}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
