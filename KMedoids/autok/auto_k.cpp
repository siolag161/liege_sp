#include "TimerUtils.hpp"
#include "ApplicationOptions.hpp"
#include "OptionPrinter.hpp" //for printing
#include "DataTable.hpp"
#include "CSVParser.hpp"
#include "DataLoad.hpp"
#include "Distance.hpp"

#include <iostream> 
#include <fstream>
#include <cstdio>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>

#include "kmedoids.hpp"
#include <omp.h>

namespace utl = utility;
namespace po = boost::program_options;   

typedef int Position;
typedef std::string Label;
typedef std::vector< std::vector<int> > Matrix;

using namespace clustering;
ApplicationOptions getProgramOptions(int argc, char** argv);
void checkInputFiles( std::string& path, std::string filename );
void saveClustering( const PAM& pam, const std::vector<unsigned>& ids, std::string clustFN );

/**
 * @todo: merge this and CAST
 */
int main(int argc, char** argv) {
  int nProcessors=omp_get_max_threads();
  mp_set_num_threads( std::max(nProcessors - 7, 1) );
  utl::Timer timer, totalTimer; timer.start(); totalTimer.start();

  ApplicationOptions progOpt = getProgramOptions(argc, argv);  
  checkInputFiles( progOpt.dataInFile, " data"); checkInputFiles( progOpt.labPosInFile, "label file");

  std::vector<Label> labels; std::vector<Position> positions; std::vector<unsigned> ids;
  Matrix matrix;
  std::cout << "loading data from " <<  progOpt.dataInFile << std::endl; // todo: logging
  loadDataTable ( matrix, progOpt.dataInFile );
  loadLabelPosition( labels, ids, positions, progOpt.labPosInFile );

  std::cout << "data loaded. takes: " <<  timer.display() << std::endl << std::endl; // todo: logging
  timer.restart();


  std::cout << "Clustering begin...please be patient..." << std::endl;
  MutInfoDistance<Matrix> mutInfoDist( matrix, positions, progOpt.maxPos, progOpt.simiThres );
  PAM pam( progOpt.eps );

  unsigned N = progOpt.K;  
  unsigned K = positions.size() / N;
  
  printf("Parameters: K: %u, eps: %.2f\n", K, progOpt.eps );

  PAM_Partition partition = pam( matrix, mutInfoDist, K, 50 );  
  std::cout << "clustering finished. takes: " <<  timer.display() << std::endl << std::endl; // todo: logging
  timer.restart();

  boost::filesystem::path outputPath = boost::filesystem::absolute(progOpt.outputDir);
  std::string outputFileName = outputPath.string();
  if ( !progOpt.verbose ) {     
    boost::filesystem::create_directories(outputPath);
    char clustering_fn[256];
    char optBuf[80];
    sprintf( optBuf, "%u_%.2f_%u",  progOpt.K, progOpt.eps, progOpt.maxPos );
    sprintf( clustering_fn, "kmedoids_clustering_%s.txt", optBuf );
    outputFileName = (outputPath / clustering_fn).string();
  } else {
    boost::filesystem::create_directories(outputPath.parent_path());
  }
  saveClustering( pam, ids, outputFileName ) ;
  
  
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
        ("dinput,i", po::value<std::string>(&result.dataInFile)->required(), "Data Input filename")
        ("lpinput,l", po::value<std::string>(&result.labPosInFile)->required(), "Label-Pos Input filename")
        ("outputDir,o", po::value<std::string>(&result.outputDir)->required(), "Output Directory")

        ("n,n", po::value<unsigned>(&result.K)->required(), "n: avg number of items per cluster")
        ("eps,e", po::value<double>(&result.eps)->required(), "Epsilon")
        
        ("maxPos,x", po::value<unsigned>(&result.maxPos)->required(), "Max Position")
        ("simi,s", po::value<double>(&result.simiThres)->required(), "Similarity Threshold (if < 0 then no threshold)")
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

void saveClustering( const PAM& pam, const std::vector<unsigned>& ids, std::string clustFN ) {  
  std::ofstream clustOut(clustFN);
  clustOut << ID << SEPARATOR << LATENT << SEPARATOR << PARENT << SEPARATOR
           << LEVEL << SEPARATOR << CARDINALITY << "\n";  // writes header

  std::cout << "saving clustering of " << pam.get_partition().n_clusters() << " clusters into " << clustFN << std::endl;
  unsigned max_id = ids[ ids.size() - 1 ] + 1;

  unsigned K = pam.get_partition().n_clusters();
  std::vector<unsigned> clusterCardinality(K,0) ; // 
  for ( size_t var = 0; var < pam.get_partition().n_objects(); ++var ) {
    ClusterId cid = pam.get_partition().cluster_of( var );
    ++clusterCardinality[cid];
  }

  //for ( size_t )
  for ( size_t var = 0; var < pam.get_partition().n_objects(); ++var ) {
    ClusterId cid = pam.get_partition().cluster_of( var );
    int level = 0, id = ids[var], card = 3, latent = 0;
    int parent = -1;
    if ( clusterCardinality[cid] > 1 ) {
      parent = max_id + cid;
    }
    clustOut << id << SEPARATOR << latent  << SEPARATOR << parent << SEPARATOR
             << level << SEPARATOR << card << "\n";
  }
  
  for ( size_t var = 0; var < pam.get_partition().n_objects(); ++var ) {
    int level = 0, id = ids[var], card = 3, latent = 0;
    int parent = -1;
    clustOut << id << SEPARATOR << latent  << SEPARATOR << parent << SEPARATOR
             << level << SEPARATOR << card << "\n";
  }

  clustOut.close();
}

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
