#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include <vector>

#include "cluster.hpp"
#include "repartition.hpp"
#include "hc.hpp"
#include "average_strategy.hpp"
#include "cluster_iterator.hpp"
using namespace clustering; 
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_Iterator, Data ) 
typedef std::shared_ptr<Cluster> ClustPtr;

typedef std::vector< std::vector<double> > Matrix;

struct Diss {
  Diss( const Matrix& d ): data(d)
  {
    for ( size_t a = 0; a < d.size(); ++a ) {
      for ( size_t b = 0; b < d.size(); ++b ) {
        size_t key = 2*d.size()*a+b;
        int result = 0.0;
        for ( size_t i = 0; i < data.at(0).size(); ++ i ) {
          result += (data[a][i] - data[b][i] ) * ( data[a][i] - data[b][i] );
        }    
        // return std::sqrt(result);
        valCache[key] = std::sqrt(result);
      }
    }    
  }
  
  double operator()( size_t a, size_t b ) const {
    size_t key = 2*data.size()*a+b;
    return valCache.at(key);
  }

  const Matrix& data;
  std::map< size_t, double > valCache;
};

void print(Cluster& clt) {
  for ( auto& c: clt ) {
    std::cout << c << std::endl;
  }
}

BOOST_AUTO_TEST_CASE( Test_Clustering_Init ) {

  std::vector< ClustPtr > clts;
  for ( int i = 0; i < 6; ++i ) {
    clts.push_back( ClustPtr(new Cluster(i)) );
  }

  size_t nrows = 9, ncols = 40;  
  //std::vector< std::vector<double> > data( nrows, std::vector<double>(ncols, 0.0) );
  Matrix data { {1}, {2}, {15}, {17}, {20}, {23} };

  // Distance diss ( data, positions, 10, -1 );
  Diss diss(data);
  AverageStrategy< Diss > strategy(diss);  
  auto cp = HierarchicalClustering::closestClusterPair(clts, data, strategy );
  //printf("closest pair: (%d,%d)\n", (*cp.first)->id, (*cp.second)->id );
  //clts.erase( cp.first );
  (*cp.first) = ClustPtr( new Cluster(6, *cp.first, *cp.second ) );
  clts.erase( cp.second );

  cp = HierarchicalClustering::closestClusterPair(clts, data, strategy );
  (*cp.first) = ClustPtr( new Cluster(7,  *cp.first, *cp.second) );
  clts.erase( cp.second );

  cp = HierarchicalClustering::closestClusterPair(clts, data, strategy );
  (*cp.first) = ClustPtr( new Cluster(8,  *cp.first, *cp.second) );
  clts.erase( cp.second );
  
  cp = HierarchicalClustering::closestClusterPair(clts, data, strategy );
  // printf("closest pair: (%d,%d)\n", (*cp.first)->id, (*cp.second)->id);
  // (*cp.first) = ClustPtr( new Cluster(7,  (*cp.first)->id, (*cp.second)->id) );
  // clts.erase( cp.second );
}


BOOST_AUTO_TEST_CASE( Test_ClusterIt ) {
  std::vector< ClustPtr > clts;
  for ( int i = 0; i < 6; ++i ) {
    clts.push_back( ClustPtr(new Cluster(i)) );
  }

  //Cluster c(6, clts[0], clts[1]);
  ClustPtr clt6(new Cluster(6, clts[0], clts[1]) );
  clts.push_back( clt6 );
  clts.push_back( ClustPtr( new Cluster(7, clts[2], clts[3]) ) );    
  clts.push_back( ClustPtr( new Cluster(8, clts[7], clts[4]) ) );
  clts.push_back( ClustPtr( new Cluster(9, clts[8], clts[5]) ) );
  clts.push_back( ClustPtr( new Cluster(10, clts[6], clts[9]) ) );

}




BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
