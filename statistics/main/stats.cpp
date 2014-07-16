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
#include "association_test.hpp"
#include "permutationProcedure.hpp"

#include "Graph.hpp"
#include "GraphIO.hpp"

#include "stats.hpp"

namespace utl = utility;
namespace po = boost::program_options;   
using namespace stats;
using namespace fltm;

/**
 * @todo: merge this and CAST
 */
int main(int argc, char** argv) {

  
  utl::Timer timer, totalTimer; timer.start(); totalTimer.start();
  ApplicationOptions progOpt = getProgramOptions(argc, argv);
  checkInputFiles( progOpt.genoInFile, " data"); checkInputFiles( progOpt.phenoInFile, "pheno file");
  checkInputFiles( progOpt.labelInFile, "label file");

  // std::vector<Label> labels; std::vector<Position> positions; std::vector<int> ids, cards, pheno;
  std::vector<int> pheno;
  Matrix genoMat;
  
  std::cout << "Loading geno data from " << progOpt.genoInFile << std::endl; // todo: logging
  loadDataTable ( genoMat, progOpt.genoInFile );
  std::cout << "Loading pheno data from " << progOpt.genoInFile << std::endl; // todo: logging
  loadPhenotype( pheno, progOpt.phenoInFile );
  std::cout << "Loading label data from " << progOpt.genoInFile << std::endl; // todo: logging

  Graph graph;
  BayesGraphLoad graphLoad;
  graphLoad( graph,
             progOpt.labelInFile,
             progOpt.graphBayesVertexInFile,
             progOpt.graphBayesDistInFile );


  
  std::cout << "Data loaded. takes: " <<  timer.display() << std::endl << std::endl; // todo: logging
  timer.restart();

  std::cout << "Testing begin...please be patient..." << std::endl;
  std::vector<std::vector<double>> pvalues;
  Distributions dists;
  std::vector<StatTest*> tests { new ChiSq };
  performTesting( dists, pvalues, tests, genoMat, pheno, graph, progOpt.permutation );
  std::cout << "Testing end..takes: " <<  timer.display() << std::endl << std::endl; // todo: logging
  timer.restart();

  boost::filesystem::path outputPath = boost::filesystem::absolute(progOpt.outputDir);
  boost::filesystem::path outputFileName; // = outputPath.string();
  // if ( !progOpt.verbose ) {     
  boost::filesystem::create_directories(outputPath);
  char testing_fn[256];
  char optBuf[80];
  sprintf( optBuf, "%d", progOpt.permutation );
  sprintf( testing_fn, "testing_%s_%s.txt", progOpt.labelInFile.c_str(), optBuf );
  outputFileName = (outputPath / testing_fn);
  boost::filesystem::create_directories(outputFileName.parent_path());


  std::cout << "writing result to: " << outputFileName  << "...\n\n";
  writeResult( dists, pvalues, graph, tests, outputFileName.string() );

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
        ("tinput,t", po::value<std::string>(&result.phenoInFile)->required(), "Phenotype Input filename")        
        ("linput,l", po::value<std::string>(&result.labelInFile)->required(), "Label Input filename")

        ("vbinput,v", po::value<std::string>(&result.graphBayesVertexInFile)->required(), "Bayes Graph ")
        ("dbnput,d", po::value<std::string>(&result.graphBayesDistInFile)->required(), "Label Input filename")

        
        ("outputDir,o", po::value<std::string>(&result.outputDir)->required(), "Output Directory")
        ("permutation,p", po::value<int>(&result.permutation)->required(), "Permutation")        

        // ("verbose,v", po::value<int>(&result.verbose)->default_value(0), "Verbose")
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
  int count = 0;
  // while ( j < permu && vals[j] > p ) ++j;
  for ( size_t i = 0; i  < permu; ++ i ) {
    if (vals[i] < p) ++count;
  }

  return count*1.0/permu;
}



void writeResult( Distributions &dists,
                  std::vector<std::vector<double> >& pvalues,
                  fltm::Graph& graph,                  
                  std::vector<StatTest*>& tests,
                  std::string outputFileName ) {
  std::ofstream os(outputFileName);
  // std::cout << "id(graph),label,level";
  for ( size_t t = 0; t < tests.size(); ++t ) {
    // std::cout << "," << tests[t]->name << "," << tests[t]->name << "(after)";
  }
  // std::cout << std::endl;
  // printf("pvalues: %d\n", pvalues[0].size());
  for ( size_t i = 0; i < pvalues[0].size(); ++ i) {
    // std::cout << i << "," << graph[i].label << "," << graph[i].level ;
    for ( size_t t = 0; t < tests.size(); ++t ) {
      // std::cout << "," << pvalues[2*t][i] << "," << pvalues[2*t+1][i];
    }
    // std::cout << std::endl;
  }
  // std::cout << "w1\n";
  int permus = dists[0][0].size();
  // std::cout << "w2\n";
  int levels =  dists[0].size();
  // printf("p: %d, l: %d\n", permus, levels);
  // for ( int t = 0; t < tests.size(); ++t ) {

  for ( int p = 0; p < permus; ++p ) {
    for ( int l = 0; l < levels; ++l ) {
      os << dists[0][l][p] << ",";
    }
    os << dists[0][levels-1][p] << std::endl;
  }
  
  os.close();
}


