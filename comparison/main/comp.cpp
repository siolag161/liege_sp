#include "TimerUtils.hpp"
#include "ApplicationOptions.hpp"
#include "OptionPrinter.hpp" //for printing
#include "DataTable.hpp"
#include "Distance.hpp"

#include <iostream> 
#include <fstream>
#include <cstdio>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name
#include <algorithm>
#include "snp.hpp"

#include <omp.h>

#include "measure.hpp"
#include "mirkin.hpp"
#include "rand.hpp"
#include "fowlkes.hpp"

namespace utl = utility;
namespace po = boost::program_options;   
using namespace clustering;

typedef int Position;
typedef std::string Label;

ApplicationOptions getProgramOptions(int argc, char** argv);
void checkInputFiles( std::string& path, std::string filename );
void performComparison( std::vector<ComparisonMeasure*>& measures, const Partition& ref, const Partition& tar );


// std::vector<SNP> filter_snps( cosnst std::vector<SNP>& snps, int N );

std::vector<double> read_association_test( std::string assoFile);
std::vector<SNP> read_clustering( std::string  );
/**
 * @todo: merge this and CAST
 */
int main(int argc, char** argv) {
  
  utl::Timer timer, totalTimer; timer.start(); totalTimer.start();
  ApplicationOptions progOpt = getProgramOptions(argc, argv);  
  checkInputFiles( progOpt.referenceClusteringFile, "ref file"); checkInputFiles( progOpt.targetClusteringFile, "target file");

  std::vector<double> association_test = read_association_test( progOpt.assoFile );
  std::vector<SNP> ref_snps = read_clustering( progOpt.referenceClusteringFile );
  std::vector<SNP> tar_snps = read_clustering( progOpt.targetClusteringFile );

  for ( size_t i = 0; i < association_test.size(); ++i ) {
    ref_snps[i].p_value = association_test[i];
    tar_snps[i].p_value = association_test[i];
  }

  std::sort( ref_snps.begin(), ref_snps.end() );
  std::sort( tar_snps.begin(), tar_snps.end() );


  std::vector<ComparisonMeasure*> measures { new Mirkin, new FowlkesIndex, new RandIndex, new AdjustedRandIndex };
  std::ofstream cfile( progOpt.outputPath.c_str() );
  cfile << "n," ;
  for ( size_t i = 0; i < measures.size()-1; ++i ) {
    cfile << measures[i]->name() << ",";
  }
  cfile << measures[measures.size()-1]->name() << "\n";      

  //omp_set_num_threads(1);
  timer.start();
  std::vector< std::vector<double> > values ( progOpt.maxN - progOpt.minN + 1, std::vector<double>(measures.size(), 0.0) );
  #pragma omp parallel for schedule(dynamic)
  for ( int N = progOpt.minN; N <= progOpt.maxN; ++N ) {
    std::vector<SNP> ref_snps_f(ref_snps.begin(), ref_snps.begin() + N);
    SNP_Partition ref_snp_partition( ref_snps_f );
    std::vector<SNP> tar_snps_f(tar_snps.begin(), tar_snps.begin() + N);
    SNP_Partition tar_snp_partition( tar_snps_f );

    for ( size_t i = 0; i < measures.size(); ++i ) {
      double val = measures[i]->compute( ref_snp_partition.partition, tar_snp_partition.partition );
      values[N-progOpt.minN][i] = val;
    }
  }

  std::cout << timer.display() << std::endl;
  timer.restart();


  for ( int N = progOpt.minN; N <= progOpt.maxN; ++N ) {
    cfile << N << ",";
    for ( size_t i = 0; i < measures.size() - 1; ++i ) {
      cfile << values[N-progOpt.minN][i] << ",";
    }
    cfile << values[N-progOpt.minN][measures.size() - 1] << std::endl;
  }
  
  cfile.close();
}

  /////////////////////////////////////////////////////////////////
  ApplicationOptions getProgramOptions(int argc, char** argv)
  {
    ApplicationOptions result;
    std::string appName = boost::filesystem::basename(argv[0]);
    po::options_description optDesc("Options");
  
    try  {
      /** Define and parse the program options 
       */
      optDesc.add_options()
          ("help,h", "Print help messages")
          ("refinput,r", po::value<std::string>(&result.referenceClusteringFile)->required(), "Reference Clustering")
          ("tarinput,c", po::value<std::string>(&result.targetClusteringFile)->required(), "Target Clustering")
          ("assinput,a", po::value<std::string>(&result.assoFile)->required(), "Target Clustering")
          ("out,o", po::value<std::string>(&result.outputPath)->required(), "Output")
          ("minn,n", po::value<int>(&result.minN)->required(), "min Cut-level")
          ("maxn,x", po::value<int>(&result.maxN)->required(), "max Cut-level")

          // ("thres,t", po::value<double>(&result.threshold)->required(), "Significance threshold")
          ;
      po::variables_map vm; 
      try { 
        po::store(po::command_line_parser(argc, argv).options(optDesc).run(), vm); // throws on error
        if (vm.count("help") ) {
          rad::OptionPrinter::printStandardAppDesc(appName,std::cout, optDesc, NULL);
          exit(1);
        }
        po::notify(vm);   	    

      } 
      catch(boost::program_options::required_option& e) /** missing arguments **/
      {
        rad::OptionPrinter::formatRequiredOptionError(e);
        std::cout << e.what() << std::endl << std::endl;
        rad::OptionPrinter::printStandardAppDesc( appName,std::cout,
                                                  optDesc, NULL);

        exit(-1);
      }

    }
    catch(std::exception e)    
    {
      std::cout << "Unhandled Exception reached the top of main: "
                << e.what() << ", application will now exit" << std::endl;

      rad::OptionPrinter::printStandardAppDesc(appName, std::cout, optDesc, NULL);
      exit(-1);
    }

    return result;
  }


  ////////////////////////////////////////////////////////////////////
  /** checks if input exists and exists on giving the error message
   *
   */
  void checkInputFiles( std::string& path, std::string filename ) {
    if ( !boost::filesystem::exists( path ) )
    {
      std::cout << "Can't find " << filename << " at " << path << "! Program will now close." << std::endl;
      exit(-1);
    }
  }

  // std::vector<SNP> filter_snps( const std::vector<SNP>& snps, int N ) {  
  //   if ( N > 0 ) {
  //     std::vector<SNP> n_snps( snps.begin(), snps.begin() + N );
  //     return n_snps;
  //   }
// }

std::vector<double> read_association_test( std::string aFileName ) {
  std::vector<double> assoVec;

  std::ifstream aFile( aFileName.c_str());
  if (!aFile) assoVec;
  
  unsigned nrows = std::count( std::istreambuf_iterator<char>(aFile), 
                               std::istreambuf_iterator<char>(), '\n' );
  aFile.seekg (0, std::ios::beg);
  assoVec.reserve(nrows);

  utility::CSVIterator<std::string> aLine(aFile);// ++labPosLine;
  for( ; aLine != utility::CSVIterator<std::string>(); ++aLine ) {    
    double p_value = boost::lexical_cast<double>((*aLine)[3]);
    assoVec.push_back(p_value);
  }
  aFile.close();

  //  std::cout << "load " << assoVec.size() << " p_values.\n";
  printf("load %d p_values (over %d)\n", assoVec.size(), nrows);
  return assoVec;
}

std::vector<SNP> read_clustering( std::string cFileName ) {
  std::vector<SNP> snpVec;

  std::ifstream cFile( cFileName.c_str());
  if (!cFile) return snpVec;
  
  unsigned nrows = std::count( std::istreambuf_iterator<char>(cFile), 
                               std::istreambuf_iterator<char>(), '\n' );
  cFile.seekg (0, std::ios::beg);
  snpVec.reserve(nrows);

  utility::CSVIterator<std::string> cLine(cFile);// ++labPosLine;
  for( ; cLine != utility::CSVIterator<std::string>(); ++cLine ) {    
    SNP snp;
    snp.chr = (*cLine)[0];
    snp.id = boost::lexical_cast<int>((*cLine)[1]);
    snp.label = (*cLine)[2];
    snp.cluster = boost::lexical_cast<int>((*cLine)[3]);
    snp.algo = (*cLine)[4];
    snpVec.push_back(snp);
  }
  cFile.close();
  //  std::cout << "load " << assoVec.size() << " p_values.\n";
  printf("load %d snps (over %d)\n", snpVec.size(), nrows);
  return snpVec;
}


// void performComparison( std::vector<ComparisonMeasure*>& measures, const Partition& ref, const Partition& tar, std::string filename ) {
//   std::ifstream cfile( filename.c_str());
//   if (!cfile) return ;

//   for ( size_t i = 0; i < measures.size()-1; ++ i ) {
//     cfile << measures[i]->name() << ",";
//   }
//   cfile << measures[measures.size()-1]->name() << "\n";
      
//   for ( ComparisonMeasure* measure: measures ) {
//     cfile << measure->compute(ref, tar);
//   }
// }
