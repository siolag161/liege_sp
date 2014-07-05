#include "TimerUtils.hpp"
#include "ApplicationOptions.hpp"
#include "OptionPrinter.hpp" //for printing
#include "DataTable.hpp"
#include "Distance.hpp"
#include "CSVParser.hpp"

#include <iostream> 
#include <fstream>
#include <cstdio>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name
#include <algorithm>
#include <map>
#include <vector>
#include <string>

#include "partition.hpp"

namespace utl = utility;
namespace po = boost::program_options;   
using namespace clustering;

typedef int Position;
typedef std::string Label;

struct Options {
  std::string mapFile;
  std::string haploFile;
  std::string outPath;
};

Options getProgramOptions(int argc, char** argv);
void checkInputFiles( std::string& path, std::string filename );

void read_map( std::map< std::string, int >& label2id, std::map<int, std::string>& id2label,
               std::vector<int>& par2global, std::map<int,int>& global2par, std::string mapFileName );
void read_partition( Partition&, std::string haplo, std::map< std::string, int >& label2id, std::vector<int>& par2global, std::map<int,int>& global2par  );

void write_partition( Partition& partition, std::map<int, std::string>& id2label,
                      std::vector<int>& par2global, std::map<int,int>& global2par, std::string& outPath );


int main(int argc, char** argv) {
  
  utl::Timer timer, totalTimer; timer.start(); totalTimer.start();
  Options progOpt = getProgramOptions(argc, argv);  
  checkInputFiles( progOpt.haploFile, "haplo file"); 
  checkInputFiles( progOpt.mapFile, "map file"); 

  std::map< std::string, int > label2id;
  std::map<int, std::string> id2label;
  std::vector<int> par2global;
  std::map<int,int> global2par;
  std::cout << "reading map..." << std::endl;
  read_map( label2id, id2label, par2global, global2par, progOpt.mapFile );
  std::cout << "reading partition..." << std::endl;

  Partition partition(label2id.size());    
  read_partition( partition, progOpt.haploFile, label2id, par2global, global2par );
  std::cout << "writing partition..." << std::endl;
  write_partition( partition, id2label, par2global, global2par, progOpt.outPath );
  
}

/////////////////////////////////////////////////////////////////
Options getProgramOptions(int argc, char** argv)
{
  Options result;
  std::string appName = boost::filesystem::basename(argv[0]);
  po::options_description optDesc("Options");
  
  try  {
    /** Define and parse the program options 
     */
    optDesc.add_options()
        ("help,h", "Print help messages")
        ("haplo,p", po::value<std::string>(&result.haploFile)->required(), "Haplo")
        ("map,m", po::value<std::string>(&result.mapFile)->required(), "Map")

        ("out,o", po::value<std::string>(&result.outPath)->required(), "Output")
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

void read_map( std::map< std::string, int >& label2id, std::map<int, std::string>& id2label,
               std::vector<int>& par2global, std::map<int,int>& global2par, std::string mapFileName ) {
  std::ifstream mFile( mapFileName.c_str());
  if (!mFile) return;  

  unsigned nrows = std::count( std::istreambuf_iterator<char>(mFile), 
                               std::istreambuf_iterator<char>(), '\n' );
  mFile.seekg (0, std::ios::beg);
  // id2label.resize( nrows, "");
  
  utility::CSVIterator<std::string> mLine(mFile);// ++labPosLine;
  int par_id = 0;
  par2global.resize(nrows, 0);
  for( ; mLine != utility::CSVIterator<std::string>(); ++mLine ) {
    int global_id =  boost::lexical_cast<int>((*mLine)[1]);
    std::string label = (*mLine)[2];
    label2id[label]= global_id;
    id2label[global_id] = label;
    par2global[par_id] = global_id;
    global2par[global_id] = par_id;

    ++par_id;
  }
  mFile.close();
  std::cout << "loaded..." << label2id.size() << " vars\n";
}
  
void write_partition( Partition& partition, std::map<int, std::string>& id2label,
                      std::vector<int>& par2global,
                      std::map<int,int>& global2par, std::string& outPath ) {
  std::ofstream pFile( outPath.c_str());
  int singleton = partition.nbr_clusters();
  for ( size_t par_id = 0; par_id < partition.labels.size(); ++par_id ) {
    int clust =  partition.cluster( par_id );    
    if ( clust == -1) {
      clust = singleton;
      ++singleton;
    }    
    pFile << "\"chr02\"" << ","
          << par2global[ par_id ] << ","
          << id2label[ par2global[ par_id ] ] << ","
          << clust << ",\"haplo-plink\"" << std::endl;  
  }
  pFile.close();
}

void read_partition( Partition& partition, std::string hapFileName, std::map< std::string, int >& label2id,
                          std::vector<int>& par2global, std::map<int,int>& global2par ) {
  std::ifstream hFile( hapFileName.c_str());
  if (!hFile) return;  

  
  utility::CSVIterator<std::string> hLine(hFile);// ++labPosLine;
  int cluster = 0;
  int clustered = 0;
  for( ; hLine != utility::CSVIterator<std::string>(); ++hLine ) {
    for ( int i = 0; i < hLine->size(); ++i ) {
      std::string label = (*hLine)[i];
      int global_id = label2id[label];
      int par_id = global2par[global_id];
      partition.cluster( par_id, cluster );
      ++clustered;      
    }
    cluster++;
  }
  hFile.close();
}

 
