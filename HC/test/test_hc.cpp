#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include <vector>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <math.h>
#include "cluster.hpp"
#include "repartition.hpp"
#include "hc.hpp"
#include "average_strategy.hpp"

#include <omp.h>
#include <chrono>
using namespace clustering; 
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_HC, Data ) 

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

typedef std::shared_ptr<Cluster> ClustPtr;

BOOST_AUTO_TEST_CASE( Test_Ptr_Init ) {
  std::vector<ClustPtr> clts;
}

BOOST_AUTO_TEST_CASE( Test_Clustering_Init ) {
  std::cout << "\nTesting HC...\n";
  size_t nrows = 50, ncols = 5000;
  size_t K = 10;
  boost::mt19937 rng; //  
  std::vector< std::vector<double> > data( nrows, std::vector<double>(ncols, 0.0) );

  for ( size_t i = 0; i < nrows; ++i ) {
    double mean = (i % K)*1.0;
    double sv = 0.1;
    boost::normal_distribution<> nd( mean, sv );
    boost::variate_generator<boost::mt19937&, 
                             boost::normal_distribution<> > var_nor(rng, nd);    
    for ( size_t j = 0; j < ncols; ++j )
      data[i][j] = var_nor();
  }

  std::vector<int> positions(nrows, 0);
  
  
  HierarchicalClustering hc;
  // Diss diss(data);
  typedef std::vector< std::vector<double> >  Matrix;
  typedef  MutInfoDistance< Matrix > Distance;
  Distance diss( data, positions, 10, -1 );
  AverageStrategy< Distance > strategy(diss);

  // omp_set_num_threads(1);
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  auto repartition = hc( data, strategy, 1);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();  
  std::cout << "Algo (parallel) took "
            << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
            << " (ms).\n";

  // omp_set_num_threads(1);
  // start = std::chrono::steady_clock::now();
  // Diss dist(data);
  // AverageStrategy< Diss > stra(dist);
  // HierarchicalClustering hc2;
  // hc( data, stra, 1); 
  // end = std::chrono::steady_clock::now();  
  // std::cout << "Algo (sequential) took "
  //           << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
  //           << " (ms).\n";

  BOOST_CHECK_EQUAL( repartition.nclusters(), 1 );
  // for ( auto clusterIt: repartition.clusters() ) {
  for ( auto it = (repartition.clusters[0])->begin(); it != (repartition.clusters[0])->end(); ++it ) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  // } 
}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
