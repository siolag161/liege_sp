#include "TimerUtils.hpp"
#include "ApplicationOptions.hpp"
#include "OptionPrinter.hpp" //for printing
#include "CSVParser.hpp"
#include "DataLoad.hpp"

#include <iostream> 
#include <fstream>
#include <cstdio>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name

#include "statistics.hpp"

namespace utl = utility;
namespace po = boost::program_options;   
using namespace stats;

typedef int Position;
typedef std::string Label;
typedef std::vector< std::vector<int> > Matrix;
typedef std::vector<int> Phenotype;

ApplicationOptions getProgramOptions(int argc, char** argv);
void checkInputFiles( std::string& path, std::string filename );
void performTesting( std::vector<std::vector<double> >& result,
                     const Matrix& mat,
                     const Phenotype& pheno,
                     const int codage,
                     const int permu );

void  writeResult( std::vector<std::vector<double> >& pvalues,
                   std::vector<Label> labels,
                   std::vector<unsigned> ids,
                   std::vector<int> positions,
                   std::string outputFileName ); 


/**
 * @todo: merge this and CAST
 */
int main(int argc, char** argv) {

  
  utl::Timer timer, totalTimer; timer.start(); totalTimer.start();
  ApplicationOptions progOpt = getProgramOptions(argc, argv);
  checkInputFiles( progOpt.genoInFile, " data"); checkInputFiles( progOpt.phenoInFile, "label file");

  std::vector<Label> labels; std::vector<Position> positions; std::vector<unsigned> ids; std::vector<int> pheno;
  std::vector<std::vector<double> > pvalues;
  Matrix genoMat;
  
  std::cout << "Loading geno data from " <<  progOpt.genoInFile << std::endl; // todo: logging
  loadDataTable ( genoMat, progOpt.genoInFile );
  loadPhenotype( pheno, labels, ids, positions, progOpt.phenoInFile );

  std::cout << "Data loaded. takes: " <<  timer.display() << std::endl << std::endl; // todo: logging
  timer.restart();

  std::cout << "Testing begin...please be patient..." << std::endl;
  performTesting( pvalues, genoMat, pheno, progOpt.codage, progOpt.permutation );

  boost::filesystem::path outputPath = boost::filesystem::absolute(progOpt.outputDir);
  std::string outputFileName = outputPath.string();
  if ( !progOpt.verbose ) {     
    boost::filesystem::create_directories(outputPath);
    char testing_fn[256];
    char optBuf[80];
    sprintf( optBuf, "%d_%d",  progOpt.codage, progOpt.permutation );
    sprintf( testing_fn, "testing_%s.txt", optBuf );
    outputFileName = (outputPath / testing_fn).string();
  } else {
    boost::filesystem::create_directories(outputPath.parent_path());
  }

  writeResult( pvalues, labels, ids, positions, outputFileName );

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
        ("dinput,i", po::value<std::string>(&result.genoInFile)->required(), "Geno Input filename")
        ("hinput,h", po::value<std::string>(&result.phenoInFile)->required(), "Pheno Input filename")
        ("outputDir,o", po::value<std::string>(&result.outputDir)->required(), "Output Directory")
        ("codage,c", po::value<int>(&result.codage)->required(), "Codage")
        ("permutation,p", po::value<int>(&result.permutation)->required(), "Permutation")        

        ("verbose,v", po::value<int>(&result.verbose)->default_value(0), "Verbose")
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
static const std::string ID = "id"; static const std::string LABEL = "label";
static const std::string LATENT = "latent"; static const std::string PARENT = "parent";
static const std::string LEVEL = "level"; static const std::string POSITION = "position";
static const std::string CARDINALITY = "cardinality";
static const std::string PARENT_ID = "parent_id";
static const char SEPARATOR = ',';



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

double correct_permute( const double p, const std::vector<double>& vals, const int permu ) {
  size_t j = 0;
  while ( j < permu && vals[j] > p ) ++j;

  return j*1.0/permu;
}


/** todo: change to list (with name and such)
 *
 */
void performTesting( std::vector<std::vector<double> >& result,
                     const Matrix& mat,
                     const Phenotype& pheno,
                     const int codage,
                     const int permu ) {
  StatisticTest<G2> g2;
  StatisticTest<G2_YATES> g2_cor;
  StatisticTest<FISHER> fisher;
  size_t nvars = mat.size();

  result.resize( 3,  std::vector<double>(nvars, 0.0) );
  std::vector<double>& g2_pvals = result[0], g2_cor_pvals = result[1], fisher_pvals = result[2];
  for ( size_t v = 0; v < nvars; ++v ) {
    g2_pvals[v] = g2( mat.at(v), pheno, 2, 2);
    g2_cor_pvals[v] = g2_cor( mat.at(v), pheno, 2, 2);
    fisher_pvals[v] = fisher( mat.at(v), pheno, 2, 2);
  }

  if ( permu > 0 ) {
    std::vector<double> g2_vals( permu, 0.0), g2_cor_vals( permu, 0.0 ), fisher_vals( permu, 0.0 );
    double g2_max = -1.0, g2_cor_max = -1.0, fisher_max = -1.0;
    for ( int i = 0; i < permu; ++ i ) {
      for ( auto& snp: mat ) {
        double pm_g2 = g2(snp, pheno, codage, 2);
        double pm_g2_cor = g2_cor(snp, pheno, codage, 2);
        double pm_fisher = fisher(snp, pheno, codage, 2);
        
        g2_max = std::max( g2_max, pm_g2 );
        g2_cor_max = std::max( g2_cor_max, pm_g2_cor );
        fisher_max = std::max( fisher_max, pm_fisher );        
      }
      g2_vals[i] = g2_max; g2_cor_vals[i] = g2_cor_max; g2_cor_vals[i] = g2_cor_max; 
    }

    for ( size_t v = 0; v < nvars; ++v ) {
      g2_pvals[v] = correct_permute( g2_pvals[v], g2_vals, permu );
      g2_cor_pvals[v] = correct_permute( g2_cor_pvals[v], g2_cor_vals, permu );
      fisher_pvals[v] = correct_permute( fisher_pvals[v], fisher_vals, permu );
    }
  }

}


void  writeResult( std::vector<std::vector<double> >& pvalues,
                   std::vector<Label> labels,
                   std::vector<unsigned> ids,
                   std::vector<int> positions,
                   std::string outputFileName ) {
  // StatisticTest<G2> g2;
  // StatisticTest<G2_YATES> g2_cor;
  // StatisticTest<FISHER> fisher;

  size_t nbrSnps = labels.size();
  for ( size_t v = 0; v < nbrSnps; ++v ) {

  }
}
