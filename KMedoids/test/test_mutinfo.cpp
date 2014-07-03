// #define BOOST_TEST_DYN_LINK
// #ifdef STAND_ALONE
// #   define BOOST_TEST_MODULE
// #endif  
// #include <boost/test/unit_test.hpp>

// #include <omp.h>

// #include <vector>
// #include "Distance.hpp"
// #include "kmedoids.hpp"
// #include "chrono_measure.hpp"

// using namespace clustering; 
// class Data 
// { 
// };
 
// BOOST_FIXTURE_TEST_SUITE( Test_Para_Entropy, Data ) 
 

// // BOOST_AUTO_TEST_CASE( Test_Entropy_1 ) {
// //   std::cout << "TESTING MUTINFO..." << std::endl;
  
// //   typedef std::vector< std::vector<int> > Matrix;
// //   size_t nrows = 100, ncols = 50;

// //   std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
// //   Matrix data( nrows, std::vector<int>(ncols, 0) );
// //   #pragma omp parallel for
// //   for ( size_t i = 0; i < nrows; ++i ) {
// //     for ( size_t j = 0; j < ncols; ++j )
// //       data[i][j] = i+j;
// //   }
// //   std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();  
// //   std::cout << "Init data took "
// //             << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
// //             << " (ms).\n";
  
// //   std::vector<int> positions( nrows, 0);
// //   for (int i = 0; i < nrows; ++i) positions[i] = i;
  
// //   omp_set_num_threads(1);
// //   start = std::chrono::steady_clock::now();
// //   MutInfoDistance<Matrix> mutInfoDist1( data, positions, 50, 1 );
// //   end = std::chrono::steady_clock::now();  
// //   std::cout << "MutInfo (sequential) took "
// //             << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
// //             << " (ms).\n";

// //   omp_set_num_threads(8);
// //   start = std::chrono::steady_clock::now();
// //   MutInfoDistance<Matrix> mutInfoDist2( data, positions, 50, 1 );
// //   end = std::chrono::steady_clock::now();  
// //   std::cout << "MutInfo (para) took "
// //             << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
// //             << " (ms).\n";
// // }


// BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
