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

namespace utl = utility;
namespace po = boost::program_options;   

typedef int Position;
typedef std::string Label;
typedef std::vector< std::vector<int> > Matrix;

using namespace clustering;
ApplicationOptions getProgramOptions(int argc, char** argv);

void saveClustering( const PAM& pam, const std::vector<unsigned>& ids, std::string clustFN );

/**
 * @todo: merge this and CAST
 */
int main(int argc, char** argv) {
  utl::Timer timer, totalTimer; timer.start(); totalTimer.start();

  ApplicationOptions progOpt = getProgramOptions(argc, argv);
  
  std::vector<Label> labels; std::vector<Position> positions; std::vector<unsigned> ids;
  Matrix matrix;
  std::cout << "loading data from " <<  progOpt.dataInFile << std::endl; // todo: logging
  loadDataTable ( matrix, progOpt.dataInFile );
  loadLabelPosition( labels, ids, positions, progOpt.labPosInFile );

  std::cout << "data loaded. takes: " <<  timer.display() << std::endl << std::endl; // todo: logging
  timer.restart();

  boost::filesystem::path outputPath = boost::filesystem::absolute(progOpt.outputDir);
  char timeBuf[80];  
  time_t now = time(0); struct tm tstruct;
  tstruct = *localtime(&now);  
  strftime(timeBuf, sizeof(timeBuf), "%Y_%m_%d_%H_%M_%S", &tstruct);
  outputPath /= timeBuf;
  boost::filesystem::create_directories(outputPath);

  printf("Parameters: K: %u, eps: %.2f\n",  progOpt.K, progOpt.eps );

  std::cout << "Clustering begin...please be patient..." << std::endl;
  MutInfoDistance<Matrix> mutInfoDist( matrix, positions, progOpt.maxPos, progOpt.simiThres );
  PAM pam( progOpt.eps );

  PAM_Partition partition = pam( matrix, mutInfoDist, progOpt.K, 100 );  
  std::cout << "clustering finished. takes: " <<  timer.display() << std::endl << std::endl; // todo: logging
  timer.restart();

  char cast_clustering_fn[256];
  char optBuf[80]; 
  sprintf( optBuf, "%u_%.2f_%u_%.2f",  progOpt.K, progOpt.eps, progOpt.maxPos, progOpt.simiThres );
  sprintf( cast_clustering_fn, "KMedoids_clustering_%s.txt", optBuf );

  std::cout << "writing result into " <<  (outputPath / cast_clustering_fn).string() << std::endl; // todo: logging

  saveClustering( pam, ids, (outputPath / cast_clustering_fn).string() ) ;
  
  
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

        ("k,k", po::value<unsigned>(&result.K)->required(), "K: number of medoids")
        ("eps,e", po::value<double>(&result.eps)->required(), "Epsilon")
        
        ("maxPos,x", po::value<unsigned>(&result.maxPos)->required(), "Max Position")
        ("simi,s", po::value<double>(&result.simiThres)->required(), "Similarity Threshold (if < 0 then no threshold)")

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
