
#include "TimerUtils.hpp"
#include "ApplicationOptions.hpp"
#include "OptionPrinter.hpp" //for printing
#include "DataTable.hpp"
#include "CSVParser.hpp"
#include "DataTransformation.hpp"
#include "Range.hpp"

#include "main.hpp"

#include <iostream> 
#include <fstream>
#include <cstdio>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's nam

#include "Clustering.hpp"
#include "CAST.hpp"
namespace utl = utility;
namespace po = boost::program_options;   
using namespace fltm;     

typedef utility::Range<double, utility::ITER_TYPE_ADD> DoubleRange;
typedef utility::Range<unsigned, utility::ITER_TYPE_ADD> IntRange;

CAST::partition_ptr performClustering( const Matrix& matrix,
                                       const std::vector<unsigned>& ids,
                                       const std::vector<Position>& positions,
                                       const Clust_CAST_Opt& opt );

double evaluateClustering( const CAST_Partition& clustering, unsigned nbrVars );
// void saveResults( boost::filesystem::path& path,
//                   Clust_CAST_Opt& opt,
//                   const CAST_Partition& clustering,
//                   const double score,
//                   const double elapsedTime );


int main(int argc, char** argv) {
  
  ApplicationOptions progOpt = getProgramOptions(argc, argv);

  utl::Timer timer, totalTimer; timer.start(); totalTimer.start();
  std::vector<Label> labels; std::vector<Position> positions; std::vector<unsigned> ids;
  std::cout << "loading data from " <<  progOpt.dataInFile << std::endl; // todo: logging
  std::shared_ptr<Matrix> matrix = loadDataTable ( progOpt.dataInFile );
  
  loadLabelPosition( labels, ids, positions, progOpt.labPosInFile );

  std::cout << "data loaded. rows: " << utl::nrow(*matrix) << ", columns: "
            << utl::ncol(*matrix) << ". takes: " <<  timer.display() << std::endl << std::endl; // todo: logging
  timer.restart();

  boost::filesystem::path outputPath = boost::filesystem::absolute(progOpt.outputDir);
  char timeBuf[80];  
  time_t now = time(0); struct tm tstruct;
  tstruct = *localtime(&now);  
  strftime(timeBuf, sizeof(timeBuf), "%Y_%m_%d_%H_%M_%S", &tstruct);
  outputPath /= timeBuf;
  boost::filesystem::create_directories(outputPath);

  //std::vector<double> scores; std::vector<Clust_CAST_Opt> opts; std::vector<double> times;

  printf("Parameters: maxDist: %u, simi: %.2f, cast: %.2f\n",  progOpt.maxDist, progOpt.simi, progOpt.CAST );
  Clust_CAST_Opt option( progOpt.maxDist, progOpt.simi, progOpt.CAST );
  std::string type_CAST = progOpt.simi > 0 ? "binary" : "real";
  std::cout << "performing CAST clustering (" << type_CAST << ")." << std::endl;
  auto clustering = performClustering( *matrix, ids, positions, option );
  // std::cout << "evaluating..." << std::endl;
  // auto score = evaluateClustering( *clustering, positions.size() );
  // scores.push_back(score);
  // opts.push_back(option);
  // saveResults( outputPath, option, *clustering, 
  //              -1, timer.getElapsedTimeInSec() );

  char cast_clustering_fn[256];
  char optBuf[80]; 
  sprintf( optBuf, "%.2f_%d_%.2f",  progOpt.maxDist, progOpt.simi, progOpt.CAST );
  sprintf( cast_clustering_fn, "CAST_clustering_%s.txt", optBuf );  
  saveClustering( *clustering, ids, (outputPath / cast_clustering_fn).string() ) ;
  
  std::cout << "takes: " << timer.getElapsedTimeInSec() << " s.\n";
  // times.push_back(timer.getElapsedTimeInSec());
  // timer.restart();
  
  // unsigned bestIdx = 0;
  // for (auto idx: IntRange(0, scores.size(), 1) ) {
  //   if ( scores.at(bestIdx) < scores.at(idx) ) {
  //     bestIdx = idx;
  //   }
  // }

  // std::ofstream finalReport( (outputPath / "finalReport.txt").string());
  // char bestOpts[256];
  // sprintf(bestOpts, "(simi: %.2f, maxDist: %d, cast: %.2f)", opts[bestIdx].simi,  opts[bestIdx].maxDist,  opts[bestIdx].cast);
  
  // finalReport << "Executes: " << scores.size() << " cluster analysis.\n"
  //             << "Takes: " << totalTimer.display() << ".\n"
  //             << "Clustering with the best result: "
  //             << bestOpts << " with a score of: " << scores[bestIdx] << std::endl << std::endl << std::endl;

  // for (auto idx: IntRange(0, scores.size(), 1) ) {
  //   char optChr[256];
  //   sprintf(optChr, "(simi: %.2f, maxDist: %d, cast: %.2f)", opts[idx].simi,  opts[idx].maxDist,  opts[idx].cast); 
  //   finalReport << optChr << " --- yields: " << scores[idx] << " --- takes: " << utility::timeDisplay(times[idx]) << std::endl;
    
  // }
  // finalReport.close();
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

        ("cast,c", po::value<double>(&result.CAST)->required(), "CAST")
        ("simi,s", po::value<double>(&result.simi)->required(), "Similarity")
        ("maxDist,m", po::value<unsigned>(&result.maxDist)->required(), "Max Distance")

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



std::shared_ptr<Matrix> loadDataTable( const std::string& infile, const char& sep, const char& quote)
{

  std::shared_ptr<Matrix> dt( new Matrix() );
  dt->reserve(100000);
  std::ifstream matrixFile(infile.c_str());
  if (!matrixFile) {
    std::cout << "not exist!!!! exiting programing...\n\n";
    exit(-1);
    return dt;
  }
  
  utl::CSVIterator<int> matrixLine(matrixFile);
  
  for( ; matrixLine != utl::CSVIterator<int>(); ++matrixLine ) {         
    std::vector<int> row(matrixLine->size(), 0);
    for (unsigned i = 0; i < matrixLine->size(); ++i) {
      row[i] = matrixLine->at(i);
    }
    dt->push_back(row);    
  }

  dt->resize(dt->size());
  std::cout << "done loading data" << std::endl << std::endl;
  return dt;
}


void loadLabelPosition( std::vector<Label> & labels,
                        std::vector<unsigned>& ids,
                        std::vector<Position>& positions,
                        const std::string& infile ) { 
  std::ifstream labPosFile(infile.c_str());
  if (!labPosFile) {
    std::cout << "not exist!!!! exiting programing...\n\n";
    exit(-1);
    return;
  }
  std::vector<Label>().swap(labels); //lab2Pos.clear();
  std::vector<Position>().swap(positions); //.clear();
  utl::CSVIterator<std::string> labPosLine(labPosFile);// ++labPosLine;
  for( ; labPosLine != utl::CSVIterator<std::string>(); ++labPosLine ) {    
    std::string label =  (*labPosLine)[2];
    int position = boost::lexical_cast<int>( (*labPosLine)[3]);
    unsigned id = boost::lexical_cast<unsigned>( (*labPosLine)[1]);
    ids.push_back(id);
    labels.push_back(label);
    positions.push_back(position);
  }

  std::cout << "load " << labels.size() << " variables.\n";
}

////////////////////////////////////////////////////////
CAST::partition_ptr performClustering( const Matrix& matrix,
                                       const std::vector<unsigned>& ids,
                                       const std::vector<Position>& positions,
                                       const Clust_CAST_Opt& opt ) {
  SimilarityCompute simiCompute( positions, matrix, opt.simi, opt.maxDist );  
  CAST clusteringCAST( opt.cast );
  utility::Timer t; t.start();
  std::cout << "begin clustering." << std::endl;

  std::vector<unsigned> idMap;
  for ( unsigned id = 0; id < positions.size(); ++id ) {
    idMap.push_back(id);
  }
  
  CAST::partition_ptr clustering = clusteringCAST( simiCompute, ids );
  
  std::cout << "done clustering. takes " << t.display() << std::endl;
  return clustering;
}

double evaluateClustering( const CAST_Partition& clustering, unsigned nbrVars ) {

  if (nbrVars <= 1 ) return 0.0;
  const unsigned cltSz = clustering.size();
  std::vector<unsigned> index2Cluster( nbrVars, -1);
  std::vector<unsigned> cluster2Index( cltSz, 0);
    
  unsigned clusterIdx = 0;
  for ( auto iter = clustering.begin(); iter != clustering.end(); ++iter ) {
    for ( auto citer = iter->second->begin(); citer != iter->second->end(); ++citer) {
      index2Cluster[ citer->globalIndex ] = clusterIdx;
    }
    ++clusterIdx;
  }

  int diffCount = 0;
  for ( unsigned idx = 1; idx < nbrVars; ++idx ) {
    if ( index2Cluster[idx] != index2Cluster[idx - 1] ) {
      ++diffCount;
    }
  }

  double eval = (diffCount +1 - cltSz)*1.0 / (diffCount);
  return eval;
}
// void saveResults( boost::filesystem::path& outputPath,
//                   Clust_CAST_Opt& opt,
//                   const CAST_Partition& clustering,
//                   const double score,
//                   const double elapsedTime )
// {   
//   char cast_clustering_fn[256], cast_statistics_fn[256];
//   char optBuf[80]; 
//   sprintf( optBuf, "%.2f_%d_%.2f", opt.simi, opt.maxDist, opt.cast );  

//   sprintf( cast_statistics_fn, "CAST_statistics_%s.txt", optBuf );  
//   saveClustering( clustering, (outputPath / cast_clustering_fn).string() ) ;
//   // saveStatistics( clustering, score, elapsedTime, (outputPath / cast_statistics_fn).string() );
// }


////////////////////////////////////////////////////////////////////
static const std::string ID = "id"; static const std::string LABEL = "label";
static const std::string LATENT = "latent"; static const std::string PARENT = "parent";
static const std::string LEVEL = "level"; static const std::string POSITION = "position";
static const std::string CARDINALITY = "cardinality";
static const std::string PARENT_ID = "parent_id";
static const char SEPARATOR = ',';

void saveClustering( const CAST_Partition& clustering, const std::vector<unsigned>& ids, std::string clustFN )  {
  size_t currentLatentIndex = ids[ids.size()-1] + 1;
  std::ofstream clustOut(clustFN);
  clustOut << ID << SEPARATOR << LATENT << SEPARATOR << PARENT << SEPARATOR
           << LEVEL << SEPARATOR << CARDINALITY << "\n";  // writes header

  std::cout << "saving clustering of " << clustering.size() << " clusters into " << clustFN << std::endl;
  for ( auto iter = clustering.begin(); iter != clustering.end(); ++iter ) {
    for ( auto citer = iter->second->begin(); citer != iter->second->end(); ++citer ) {
      clustOut << citer->globalIndex << SEPARATOR << 0 << SEPARATOR <<  currentLatentIndex << SEPARATOR
               << 0 << SEPARATOR << 2 << std::endl;  // writes header
    }
    ++currentLatentIndex;
  }

  for (size_t latentId = ids[ids.size()-1] + 1; latentId < currentLatentIndex; ++latentId) {
    clustOut << latentId << SEPARATOR << 1 << SEPARATOR << -1 << SEPARATOR
             << 1 << SEPARATOR << 2 << std::endl;  // writes header
  }

  clustOut.close();
}

// void saveStatistics( const CAST_Partition& clustering, const double score, const double elapsedTime, std::string statisticFN ) {

//   std::ofstream statOut(statisticFN);
//   statOut << "CAST clustering gives: 0 " << clustering.size() << " clusters. "
//           << "Scores: " << score << ". "
//           << "Takes: " << utility::timeDisplay(elapsedTime) << std::endl;
//   statOut.close();
// }
 
