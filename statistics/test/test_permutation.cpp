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


BOOST_AUTO_TEST_CASE( Test_Permutation ) {

}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
