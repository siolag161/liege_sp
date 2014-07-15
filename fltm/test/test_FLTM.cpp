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
#include <boost/lexical_cast.hpp>

#include "CAST.hpp"
#include "DBSCAN.hpp"
#include "Simi.hpp"
#include "EM.hpp"
#include "DataMatrix.hpp"
#include "Graph.hpp"
#include "FLTM.hpp"
#include "GraphIO.hpp"

#include "data_generation.hpp"
using namespace data_gen;

using namespace fltm;
class Data 
{ 
};

BOOST_FIXTURE_TEST_SUITE( Test_FLTM, Data ) 

void initData( FLTM_Data& input, int nclusts, int N, int ncols, int CARD) {
  int nrows = nclusts*N;
  for ( int i = 0; i < nrows; ++i ) {
    input.positions.push_back(i);
    input.labels.push_back(boost::lexical_cast<std::string>(i));
  }
  input.matrix = GenerateClusteredData( nclusts, N, CARD, ncols )();  
}

void initOptions( FLTM_Options& opt ) {
  opt.cardinality = 3;
  opt.nbrSteps = 2;
  // opt.maxDist = 1000;
  // opt.simiThres = -1;
  opt.emThres = 0.0000001;
  opt.infoThres = 0.0;
}

BOOST_AUTO_TEST_CASE( Test_Gaussian_K_DBSCAN_10 ) {
  std::cout << "TESTING FLTM: \n";
  size_t nclusts = 5, ncols = 40;
  size_t N = 3, CARD = 3, MAX_POS = 50;
  int nrows = nclusts*N;

  FLTM_Data input;
  FLTM_Options opt;
  initData( input, nclusts, N, ncols, CARD );
  initOptions(opt);
  
  Distance* diss = new MutInfoDiss( input.matrix, input.positions, MAX_POS, -1);  
  DBSCAN dbscan( diss, 2, 0.2 );

  LinearCardinality emLC(0.2, 1, 5);
  MultiEM multiEM(CARD, 3);
  // FLTM fltm( dbscan, emLC, multiEM );
  // printf("before: sz of data: %d, sz of pos: %d\n", input.matrix.size(), input.positions.size()) ;

  // fltm(input,opt);

  // printf("after: sz of data: %d, sz of pos: %d\n", input.matrix.size(), input.positions.size());
  // printf("after: num vertices: %d\n", boost::num_vertices(input.graph));
}

BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
