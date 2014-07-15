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

using namespace fltm;
class Data 
{ 
};

BOOST_FIXTURE_TEST_SUITE( Test_EM, Data ) 


struct Diss: public Distance {
  Diss( const std::vector< std::vector<int> >& d ): data(d)
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

  const std::vector< std::vector<int> >& data;
  std::map< size_t, double > valCache;

  virtual size_t size() const { return data.size(); }
  virtual void invalidCache() { }

};


struct ClustGen {
  std::vector< std::vector<int> > operator()( int itemsPClust, int nclusts, int ncols ) {
    int nrows = itemsPClust * nclusts;
    boost::mt19937 rng; //  
    std::vector< std::vector<int> > data( nrows, std::vector<int>(ncols, 0.0) );

    for ( size_t i = 0; i < nrows; ++i ) {    
      for ( size_t j = 0; j < ncols; ++j )
        data[i][j] = i%nclusts;
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
  virtual void invalidCache() { }


  const std::vector< std::vector<double> >& data;
  std::map< size_t, double > valCache;
  double m_max = -1;
};

typedef std::vector< std::vector<int> > Matrix;
inline void prepareEM( Matrix& emMat,
                       Variables& vars,
                       const Matrix& cltMat,
                       const Graph& graph,
                       const std::vector<int>& cluster,
                       const std::vector<int> local2Global);

Variable createLatentVar( const int lab, const int cardinality ) {
  return Variable( boost::lexical_cast<std::string>(lab), plIntegerType(0, cardinality - 1) );
}

BOOST_AUTO_TEST_CASE( Test_Gaussian_K_DBSCAN_10 ) {
  std::cout << "TEST EM:\n\n";
  size_t N = 3, ncols = 10; size_t K = 3;
  auto data = ClustGen()(N, K, ncols );
  Diss* diss = new Diss(data);
  DBSCAN dbscan( diss, 2, 1 );
  
  std::vector<int> local2Global;
  Partition result = dbscan();
  // std::cout << result << std::endl;
  Graph graph;
  for (size_t i = 0; i < N*K; ++i) {
    vertex_t vertex = createVertex( graph, 3, true,
                                    boost::lexical_cast<std::string>(i), i, 0 );

    local2Global.push_back(vertex) ;
  }
  
  // for ( auto& clt: result.to_clustering() ) {
  //   Matrix emMat;
  //   Variables vars;
  //   prepareEM( emMat, vars, data, graph, clt, local2Global );
  //   Variable latentVar = createLatentVar( boost::num_vertices(graph), 3 );
  //   ResultEM resultEM;
  //   MultiEM multiEM(3,1);    
  //   multiEM( resultEM, latentVar, vars, emMat, 0.000001 );
  //   std::cout << "len: " <<  resultEM.imputedData.size() << std::endl;
  //   for ( auto& i: resultEM.imputedData ) {
  //     std::cout << i << " ";
  //   }
  //   std::cout << std::endl;
  // }
}

void prepareEM( Matrix& emMat,
                Variables& vars,
                const Matrix& cltMat,
                const Graph& graph,
                const std::vector<int>& cluster,
                const std::vector<int> local2Global ) {
  
  Matrix *tEMMat = new Matrix();
  tEMMat->reserve(cluster.size());
  vars.clear();  
  tEMMat->push_back( std::vector<int>( ncols(cltMat), -1) );  
  for ( auto& it: cluster ) {    
    vars ^= graph[local2Global.at(it)].variable;
    tEMMat->push_back( cltMat.at(it) );      
  }
  emMat = Transpose(*tEMMat);
  delete(tEMMat);
}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
