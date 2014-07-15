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
#include <map>
#include <vector>
#include <string>

#include <limits>       // std::numeric_limits
#include <iomanip>      // std::setprecision
#include <algorithm>
#include <cmath>

// #include "partition.hpp"

namespace utl = utility;
namespace po = boost::program_options;   
using namespace clustering;

#include "signi.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  
  utl::Timer timer, totalTimer; timer.start(); totalTimer.start();
  Options progOpt = getProgramOptions(argc, argv);  
  checkInputFiles( progOpt.labelMapFile, "lab mapping file"); 
  checkInputFiles( progOpt.partitionFile, "partition file");
  checkInputFiles( progOpt.snpFile, "snp file"); 

  std::map<int,int> global2local_ids;
  std::vector<SNP> all_snps;  
  read_lab_map( all_snps, global2local_ids, progOpt.labelMapFile );

  Partition partition;
  read_partition( partition, all_snps, progOpt.partitionFile );
  std::cout << std::endl << std::endl;

  std::vector<SNP> signi_snps;
  read_signi_snps( signi_snps, progOpt.snpFile );
  Clustering clustering = partition2clustering(partition);

  std::vector< NearestCluster* > nearestClusters { new Avg_NearestCluster(), new Max_NearestCluster(), new Min_NearestCluster() };
  impute_signi_snps( signi_snps, partition, clustering, global2local_ids, nearestClusters, progOpt.outPath );
  
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
        ("snp,s", po::value<std::string>(&result.snpFile)->required(), "SNPs")
        ("lab,l", po::value<std::string>(&result.labelMapFile)->required(), "Label")
        ("clust,c", po::value<std::string>(&result.partitionFile)->required(), "Partition")
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

////////////////////////////////////////////////////////////////////
void read_lab_map( Partition& all_snps, std::map<int,int>& global2local_ids, const std::string fn ) {
  std::ifstream mFile( fn.c_str() );
  if (!mFile) return;  

  unsigned nrows = std::count( std::istreambuf_iterator<char>(mFile), 
                               std::istreambuf_iterator<char>(), '\n' );
  mFile.seekg (0, std::ios::beg);
  utility::CSVIterator<std::string> mLine(mFile);// ++labPosLine;
  all_snps.reserve(nrows);

  // int local_id = 0;
  for( ; mLine != utility::CSVIterator<std::string>(); ++mLine ) {
    SNP snp;
    snp.chr = (*mLine)[0];
    snp.id =  boost::lexical_cast<int>((*mLine)[1]);
    snp.afx_label = (*mLine)[2];
    snp.rs_label = (*mLine)[3];
    snp.position = boost::lexical_cast<int>((*mLine)[4]);
    snp.p_value = boost::lexical_cast<double>((*mLine)[5]);
    global2local_ids[ snp.id ] = all_snps.size();
    all_snps.push_back(snp);
    // local_id++;
  }
  
  mFile.close();
  std::cout << "loaded..." << all_snps.size() << " vars\n";
}

void read_partition( Partition& partition, const std::vector<SNP>& all_snps, const std::string cFileName ) {
  std::ifstream cFile( cFileName.c_str());
  if (!cFile) return;
  
  unsigned nrows = std::count( std::istreambuf_iterator<char>(cFile), 
                               std::istreambuf_iterator<char>(), '\n' );
  cFile.seekg (0, std::ios::beg);
  partition.reserve(nrows);

  utility::CSVIterator<std::string> cLine(cFile);// ++labPosLine;
  int local_id = 0;
  for( ; cLine != utility::CSVIterator<std::string>(); ++cLine ) {    
    SNP snp;
    snp.chr = (*cLine)[0];
    snp.id = boost::lexical_cast<int>((*cLine)[1]);
    snp.afx_label = (*cLine)[2];
    snp.rs_label = all_snps[local_id].rs_label;
    snp.cluster = boost::lexical_cast<int>((*cLine)[3]);
    snp.algo = (*cLine)[4];
    snp.position = all_snps[local_id].position;
    snp.p_value =  all_snps[local_id].p_value;
    partition.push_back(snp);
                            
    local_id++;
  }
  cFile.close();
  printf("load %d snps (over %d)\n", partition.size(), nrows);
}

  
void read_signi_snps( std::vector<SNP>& signi_snps, std::string fn ) {
  std::ifstream cFile( fn.c_str());
  if (!cFile) return;
 
  utility::CSVIterator<std::string> cLine(cFile);// ++labPosLine;
  int local_id = 0;
  for( ; cLine != utility::CSVIterator<std::string>(); ++cLine ) {    
    SNP snp;
    snp.chr = (*cLine)[0];
    snp.id =  boost::lexical_cast<int>((*cLine)[1]);
    snp.afx_label = (*cLine)[2];
    snp.rs_label = (*cLine)[3];
    snp.position = boost::lexical_cast<int>((*cLine)[4]);

    signi_snps.push_back(snp);
  }
  cFile.close();
  printf("load %d signi snps\n", signi_snps.size());
}

Clustering partition2clustering( const Partition& partition) {
  Clustering clustering;

  std::cout << "begin converting-1...\n";

  int nclusters = 0;
  for ( auto& snp: partition ) {
    nclusters = std::max( nclusters, snp.cluster );
  }
  clustering.resize( nclusters + 1, std::vector<SNP>() );
  for ( auto& snp: partition ) {
    clustering[snp.cluster].push_back(snp); 
  }

  return clustering;
}


void impute_signi_snps( std::vector<SNP>& signi_snps,
                        const Partition& partition,
                        const Clustering& clustering,
                        const std::map<int,int> global2local_ids,
                        const std::vector<NearestCluster*>& nearestClusters,
                        const std::string outFN ) {
  
  std::ofstream os(outFN);
  for ( NearestCluster* nearestCluster: nearestClusters ) {
    os << "++ " << nearestCluster->name() << std::endl;
    for ( auto& snp: signi_snps ) {
      int cid = 0;
      if ( snp.id >= 0 ) {      
        int local_id = global2local_ids.at(snp.id);
        cid = partition[local_id].cluster;
        snp.cluster = cid;
        os << "- known SNP( id: " << snp.id << ", lab: " << snp.rs_label << ", pos: " << snp.position << " ) -> cluster: " << cid << std::endl;       
      } else {
        cid = nearestCluster->find( snp, clustering );
        os << "* novel SNP( lab: " << snp.rs_label << ", pos: " << snp.position << " ) -> cluster: " << cid << std::endl;
      }
      for ( auto& s : clustering[cid] ) {
        if ( clustering[cid].size() == 1 ) {
          os << "SNP (itself-singleton-cluster): " << s << std::endl;
        }           
        else if ( s.id != snp.id )
          os << "SNP: " << s << std::endl;
      }
      os << std::endl;   

    }
    os << std::endl << std::endl;   
  }
  os.close();
}
