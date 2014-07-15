#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <map>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <math.h>

#include "CAST.hpp"
#include "DBSCAN.hpp"
#include "Simi.hpp"
#include "EM.hpp"

#include "Graph.hpp"
using namespace boost;
class Data 
{ 
};

BOOST_FIXTURE_TEST_SUITE( Test_Graph, Data ) 


BOOST_AUTO_TEST_CASE( Test_Gaussian_K_DBSCAN_10 ) {


  // std::cout << edge_id[0] << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
