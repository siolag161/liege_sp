#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif  
#include <boost/test/unit_test.hpp>

#include "Similarity.hpp"
#include "CAST.hpp"

using namespace fltm;

class Data
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_CAST, Data ) 

BOOST_AUTO_TEST_CASE( Test_CAST ) {
  
}

std::vector< std::vector<int> > createData( size_t nrows = 9, size_t ncols = 900 ) {
  std::vector< std::vector<int> > mat( nrows, std::vector<int> ( ncols, 0) );
  for ( size_t v = 0; v < nrows; ++v ) {
    std::vector<int> row(ncols, 0);
    size_t c = v; // % 3;
    if ( v < 3 ) {
      for ( size_t o = 0; o < ncols; ++o ) {
        row[o] = o%3;
      }
    }
    else if ( c < 6 ) {
      for ( size_t o = 0; o < ncols/3; ++o ) {
        row[o] = o%3;
      }
      for ( size_t o = ncols/3; o < ncols; ++o ) {
        row[o] = o%4;
      }
    } else {
      for ( size_t o = 0; o < 2*ncols/3; ++o ) {
        row[o] = o%3;
      }
      for ( size_t o = 2*ncols/3; o < ncols; ++o ) {
        row[o] = o%2;
      }
    }  
    mat[v] = row;
  }

  return mat;
}

void printData(std::vector< std::vector<int> >& mat) {
  for (size_t a = 0; a < mat.size(); ++a) {
    for (size_t b = 0; b < mat[a].size(); ++b) {
      std::cout << mat[a][b] << ", ";
    }
    std::cout << std::endl;
  }
}


BOOST_AUTO_TEST_CASE( Test_Distance_Binary ) {
  size_t nrows = 9, ncols = 900;
  std::vector< std::vector<int> > mat = createData( nrows, ncols );
  std::vector<int> positions { 0,1,2,3,4,5,6,7,8 };
  std::vector<unsigned int> ids { 0,1,2,3,4,5,6,7,8 };

  SimilarityCompute mut( positions, mat, 4, +1 );
  BOOST_CHECK_EQUAL( mut(0,2), mut(0,1) ); // 
  BOOST_CHECK_EQUAL( mut(0,1), mut(1,0) );
  
  BOOST_CHECK_CLOSE( 1 - mut(0,3), 1, 0.00001);
  BOOST_CHECK_EQUAL( 1 - mut(3,4), 0);
  BOOST_CHECK_EQUAL( 1 - mut(3,5), 0);
  BOOST_CHECK_CLOSE( 1 - mut(0,6), 1, 0.00001);
  BOOST_CHECK_EQUAL( 1 - mut(3,4), 0);
  BOOST_CHECK_EQUAL( 1 - mut(3,4), 0);

  BOOST_CHECK_CLOSE( 1 - mut(3,6), 1, 0.0001);
  BOOST_CHECK_CLOSE( 1 - mut(3,7), 1, 0.0001);


  // DBSCAN dbscan;
  // const DBSCAN::Labels labels = dbscan( mat, mut, 1, 0.5);
  // BOOST_CHECK_EQUAL( dbscan.nclusters(), 3);
  // BOOST_CHECK_EQUAL( labels[0], labels[1] );

  CAST cast( 0.01 );
  auto clustering = cast( mut, ids );
  BOOST_CHECK_EQUAL( clustering->size(), 3 );

}

BOOST_AUTO_TEST_CASE( Test_Distance_Real ) {
  size_t nrows = 9, ncols = 900;
  std::vector< std::vector<int> > mat = createData( nrows, ncols );
  std::vector<int> positions { 0,1,2,3,4,5,6,7,8 };
  std::vector<unsigned int> ids { 0,1,2,3,4,5,6,7,8 };

  SimilarityCompute mut( positions, mat, 3, -1 );
  
  CAST cast( 0.5 );
  auto clustering = cast( mut, ids );
  BOOST_CHECK_EQUAL( clustering->size(), 3 );

}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
