#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include "statistics.hpp"
#include "association_test.hpp"
#include "permutationProcedure.hpp"
#include <fstream>
#include <omp.h>
#include <chrono>

#include "DataLoad.hpp"

using namespace stats;
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_Perform_StatCollection, Data ) 


typedef std::vector< std::vector<int> > Matrix;
typedef std::vector<int> Pheno;

BOOST_AUTO_TEST_CASE( Test_Collection )
{
  int codage = 3, permu = 15000;
  Matrix geno;
  loadDataTable(geno, "./data.csv");
  BOOST_CHECK_EQUAL( geno.size(), 32);
  BOOST_CHECK_EQUAL( geno[0].size(), 1018 );

  Pheno pheno;
  std::vector< std::string > labels;
  std::vector<unsigned> ids;
  std::vector< int > positions;
  loadPhenotype( pheno, labels, ids, positions, "./pheno.csv" );
  BOOST_CHECK_EQUAL( pheno.size(), 1018);

  std::vector<StatTest*> statTests { new G(), new ChiSq(), new Fisher() };
  std::vector<std::vector<double> > result;
  std::cout << "lad loaded..."<< std::endl;

  omp_set_num_threads(1);
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();  
  performTesting( result, statTests, geno, pheno, codage, permu );
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();  
  std::cout << "Algo (sequential) took "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " (ms).\n";

  omp_set_num_threads(8);
  start = std::chrono::steady_clock::now();  
  performTesting( result, statTests, geno, pheno, codage, permu );
  end = std::chrono::steady_clock::now();  
  std::cout << "Algo (parallel) took "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " (ms).\n";

  //std::cout.precision(55);
  std::setprecision(55);
  std::ofstream os("testCollection.txt");
  os << "SNP";
  for ( auto& test: statTests ) { os << "," << test->name << " (b4),   " << test->name; }
  os << std::endl;
  
  for ( size_t snp; snp < geno.size(); ++ snp ) {
    os << snp;
    for ( size_t test = 0; test < statTests.size(); ++ test ) {
      os << ",   " << std::setprecision (55) << result[test][snp] << ",   " <<result[test+1][snp];
    }
    os << std::endl;
  }
  os.close();
}





BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
