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
 using namespace clustering; 
 class Data 
 { 
 };

 BOOST_FIXTURE_TEST_SUITE( Test_KMedoids, Data ) 


 struct Diss {
   Diss( const std::vector< std::vector<double> >& d ): data(d)
   {
     for ( size_t a = 0; a < d.size(); ++a ) {
       for ( size_t b = 0; b < d.size(); ++b ) {
         size_t key = 2*d.size()*a+b;
         double result = 0.0;
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

   const std::vector< std::vector<double> >& data;
   std::map< size_t, double > valCache;
 };

      
struct Diss_NoCache {
  Diss_NoCache ( const std::vector< std::vector<double> >& d ): data(d) {}
  
  double operator()( size_t a, size_t b ) const {
    double result = 0.0;
    for ( size_t i = 0; i < data.at(0).size(); ++ i ) {
      result += (data[a][i] - data[b][i] ) * ( data[a][i] - data[b][i] );
    }    
    return result;
  }

  const std::vector< std::vector<double> >& data;
 
};

BOOST_AUTO_TEST_CASE( Test_Gaussian_K_10 ) {
   std::cout << "\nTesting GAUSIAN...\n";
    size_t nrows = 100, ncols = 1;
    size_t K = 5;
    boost::mt19937 rng; //  
    std::vector< std::vector<double> > data( nrows, std::vector<double>(ncols, 0.0) );

    for ( size_t i = 0; i < nrows; ++i ) {
      double mean = (i % K)*10;
       double sv = 0.01;
       boost::normal_distribution<> nd( mean, sv );
       boost::variate_generator<boost::mt19937&, 
                                boost::normal_distribution<> > var_nor(rng, nd);    
      for ( size_t j = 0; j < ncols; ++j )
         data[i][j] = mean; //var_nor();
     }
    std::vector< std::vector<double> > dummy { {0}, {15}, {15}, {125}, {125}, {2225}, {2225}, {3550}, {3550} };
    // data = dummy;
  
    Diss dist(data);
    // printf("(%d,%d): %f\n", 92, 93, dist(92,93));
    PAM pam(0.0000000001);
    PAM_Partition partition = pam( data, dist, K, 5 );
    std::cout << partition << std::endl;

}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
