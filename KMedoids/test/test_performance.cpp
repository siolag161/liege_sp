#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "kmedoids.hpp"
#include "chrono_measure.hpp"
#include <map>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <math.h>
#include "Distance.hpp"
#include <omp.h>

#include "data_generation.hpp"
#pragma GCC diagnostic ignored "-Wpedantic"

using namespace clustering;
using namespace data_gen;
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_Performance, Data ) 


      
BOOST_AUTO_TEST_CASE( Test_Perf ) {
  // std::cout << "\nTesting GAUSIAN...\n";
  size_t nclusts = 100, ncols = 40;
  size_t N = 10, CARD = 3, MAX_POS = 50;
  double simi = -1;
  
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  std::cout << "Generating data... " << std::endl;
  Matrix mat = GenerateClusteredData( nclusts, N, CARD, ncols )();  
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Generating data takes: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " (ms).\n";

  size_t nrows = nclusts * N;
  BOOST_CHECK_EQUAL( mat.size(), nrows );
  BOOST_CHECK_EQUAL( mat[0].size(), ncols );
  
  std::vector<int> positions( nrows, 0); for ( int i = 0; i < nrows; ++i ) positions[i] = i;  
  MutInfoDistance<Matrix> mutInfoDist( mat, positions, MAX_POS, 1 );

  PAM pam( 0.0000001 );

  // start = std::chrono::steady_clock::now();
  // std::cout << "clustering data... " << std::endl;
  // pam( mat, mutInfoDist, nclusts, 20 );
  // end = std::chrono::steady_clock::now();
  // std::cout << "clustering data (//) takes: "
  //           << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
  //           << " s.\n";

  start = std::chrono::steady_clock::now();
  std::cout << "Clustering data... " << std::endl;
  pam( mat, mutInfoDist, nclusts, 20 );
  end = std::chrono::steady_clock::now();
  std::cout << "Clustering data (//) takes: "
            << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
            << " (s).\n";

  omp_set_num_threads(1);
  start = std::chrono::steady_clock::now();
  std::cout << "Clustering data... " << std::endl;
  pam( mat, mutInfoDist, nclusts, 20 );
  end = std::chrono::steady_clock::now();
  std::cout << "Clustering data (seq) takes: "
            << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
            << " (s).\n";
 
}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
