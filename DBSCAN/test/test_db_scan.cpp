#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif  
#include <boost/test/unit_test.hpp>

#include "dbs.hpp"
#include "DataLoad.hpp"
#include <memory>
#include "Distance.hpp"

using namespace clustering;
class Data
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_DBSCAN, Data ) 

BOOST_AUTO_TEST_CASE( Test_DB_Scan ) {
  // std::vector< std::vector<double> > dm;
  // loadDataTable( dm, "./data/dt.csv" );

  // DBSCAN dbscan;
  // DistCompute dist( dm);
  // std::vector<unsigned> labs = dbscan( dm, dist, 4, 3 );

  // BOOST_CHECK_EQUAL( labs.size(), dm.size() );

  // for ( size_t i = 0; i < labs.size(); ++i ) {
  //   std::cout << i << ": " << labs[i] << std::endl;
  // }

  // std::cout << dbscan.nclusters() << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
