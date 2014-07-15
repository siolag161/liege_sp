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

#include "data_generation.hpp"
using namespace data_gen;
using namespace fltm;
class Data 
{ 
};

BOOST_FIXTURE_TEST_SUITE( Test_Clustering_Gene, Data ) 


struct A {
  A(std::vector<int>* a_): a(a_) {}
  std::vector<int>* a;


  void set(std::vector<int>* a_) { a = a_; }
};



BOOST_AUTO_TEST_CASE( Test_Gaussian_K_DBSCAN_10 ) {
  std::cout << "TESTING THE DAMN QUYT QUYT: \n";
  size_t nclusts = 5, ncols = 40;
  size_t N = 3, CARD = 3, MAX_POS = 50;
  int nrows = nclusts*N;
  std::vector<int> positions; for ( int i = 0; i < nrows; ++i ) positions.push_back(i);
  auto data = GenerateClusteredData( nclusts, N, CARD, ncols )();  
  Distance* diss = new MutInfoDiss(data, positions, MAX_POS, -1);  
  DBSCAN dbscan( diss, 2, 0.2 );  
  Partition result = dbscan();
  std::cout << result << std::endl;
  Simi* simi = new MutInfoSimi(data, positions, MAX_POS, -1);  
  CAST cast(simi, 0.5);
  result = cast();
  std::cout << result << std::endl;

  nclusts=6;
  N=3;
  auto data_2 = GenerateClusteredData( nclusts, N, CARD, ncols )();  
  std::vector<int>positions_2 = std::vector<int>(); for ( int i = 0; i < (nclusts*N); ++i ) positions_2.push_back(i);
  Distance* diss_2 = new MutInfoDiss(data_2, positions_2, MAX_POS, -1);
  // dbscan.getComp()->reset( data_2, positions_2 );
  result = dbscan();  
  // std::cout << "TESTING THE DAMN QUYT QUYT - END \n";
  // std::cout << result << std::endl;

  std::vector<int> va { 1, 2, 3 };
  A a(&va);
  a.a->push_back(2);
  // printf("%d,%d\n", a.a->size(), va.size());
  std::vector<int> vb =  { 1, 2, 3, 4, 5, 6, 7 };
  a.set(&vb);
  // printf("%d,%d,%d\n", a.a->size(), va.size(), vb.size());

  // DBSCAN dbscan_2( diss_2, 2, 0.2 );

  // std::cout << "TESTING THE DAMN QUYT QUYT - END \n";
  // result = dbscan_2();
  // std::cout << result << std::endl;

}





BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
