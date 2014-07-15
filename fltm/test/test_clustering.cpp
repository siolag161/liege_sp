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
using namespace fltm;
class Data 
{ 
};

BOOST_FIXTURE_TEST_SUITE( Test_Clustering, Data ) 


struct Diss: public Distance {
  Diss( const std::vector< std::vector<double> >& d ): data(d)
  {
    for ( size_t a = 0; a < d.size(); ++a ) {
      for ( size_t b = a+1; b < d.size(); ++b ) {
        size_t key = 2*d.size()*a+b;
        double result = 0.0;
        for ( size_t i = 0; i < data.at(0).size(); ++ i ) {
          result += (data[a][i] - data[b][i] ) * ( data[a][i] - data[b][i] );
        }    
        valCache[key] = std::sqrt(result);
      }
    } 
  } 

  virtual double compute( size_t a, size_t b ) {
    if ( a > b) return (*this)(b,a);
    if ( a == b ) return 1;    
    size_t key = 2*data.size()*a+b;
    return valCache.at(key);
  }

  const std::vector< std::vector<double> >& data;
  std::map< size_t, double > valCache;

  virtual size_t size() const { return data.size(); }
  virtual void invalidCache() { }
  virtual void reset( std::vector< std::vector<int> >& dm, std::vector<int>& pos ) {}
};


struct ClustGen {
  std::vector< std::vector<double> > operator()( int itemsPClust, int nclusts, int ncols ) {
    int nrows = itemsPClust * nclusts;
    boost::mt19937 rng; //  
    std::vector< std::vector<double> > data( nrows, std::vector<double>(ncols, 0.0) );

    for ( size_t i = 0; i < nrows; ++i ) {
      double mean = (i % nclusts)*10;
      double sv = 0.01;
      boost::normal_distribution<> nd( mean, sv );
      boost::variate_generator<boost::mt19937&, 
                               boost::normal_distribution<> > var_nor(rng, nd);    
      for ( size_t j = 0; j < ncols; ++j )
        data[i][j] = var_nor();
    }

    return data;
  }
};     

struct Similarity: public Simi {
  Similarity( const std::vector< std::vector<double> >& d ): data(d)
  {
    for ( size_t a = 0; a < d.size(); ++a ) {
      for ( size_t b = a+1; b < d.size(); ++b ) {
        size_t key = 2*d.size()*a+b;
        double result = 0.0;
        for ( size_t i = 0; i < data.at(0).size(); ++ i ) {
          result += (data[a][i] - data[b][i] ) * ( data[a][i] - data[b][i] );
        }    
        valCache[key] = std::sqrt(result);
        m_max = std::max( m_max, valCache[key] );
      }
    }    
  }

  virtual double compute( const size_t a, const size_t b ) {
    if ( a > b) return (*this)(b,a);
    if ( a == b ) return 1;
    
    size_t key = 2*data.size()*a+b;
    double scaled = ( m_max - valCache.at(key) ) / m_max;
    return scaled;
  }
  
  virtual size_t size() const { return data.size(); }
  virtual void invalidCache() { }

  const std::vector< std::vector<double> >& data;
  std::map< size_t, double > valCache;
  double m_max = -1;

    virtual void reset( std::vector< std::vector<int> >& dm, std::vector<int>& pos ) {}

};


BOOST_AUTO_TEST_CASE( Test_Gaussian_K_10 ) {
  size_t N = 3, ncols = 10;
  size_t K = 3;

   auto data = ClustGen()(N, K, ncols );
   Similarity* simi = new Similarity(data);

   CAST cast( simi, 0.8);
   std::vector<int> local2Global;
   for ( int i = 0; i < N*K; ++i ) local2Global.push_back(i);

  int nrows = N*K;
  // for ( size_t i = 0; i < nrows; ++i ) {
  //   for ( size_t j = i+1; j < nrows; ++j ) {
  //     // printf("(%d,%d):%.3f ", i, j, simi(i,j));
  //   }
  //   // std::cout << std::endl;
  // }
  Partition result = cast();
  // std::cout << result << std::endl;
}


BOOST_AUTO_TEST_CASE( Test_Gaussian_K_DBSCAN_10 ) {
  // size_t N = 3, ncols = 10;
  // size_t K = 3;

  //  auto data = ClustGen()(N, K, ncols );
  //  Diss* diss = new Diss(data);
  //  DBSCAN dbscan( diss, 2, 1 );
  //  for ( size_t i = 0; i < K*N; ++i ) {
  //    for ( size_t j = i+1; j < K*N; ++j ) {
  //      // printf("(%d,%d):%.3f ", i, j, diss(i,j));
  //    }
  //    // std::cout << std::endl;
  //  }
  
  //  std::vector<int> local2Global;
  //  for ( int i = 0; i < N*K; ++i ) local2Global.push_back(i);
  
  //  Partition result = dbscan();
   // std::cout << result << std::endl;
}





BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
