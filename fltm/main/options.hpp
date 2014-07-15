/****************************************************************************************
 * File: options.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 09/07/2014

 ***************************************************************************************/
#ifndef FLTM_OPTIONS_HPP
#define FLTM_OPTIONS_HPP

#include <string>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp> // to obtain the program's name
#include "OptionPrinter.hpp" //for printing
#include "FLTM.hpp"
namespace po = boost::program_options;   

namespace fltm
{

struct Options {
  /* Input */
  std::string inputDataFile;
  std::string inputLabelFile;
  
  /* Output */
  int verbose;
  std::string outputFile;

  int clustAlgo;
  
  /* DBSCAN */
  int dbscan_minPts;
  double dbscan_eps;

  /* CAST */
  double cast_cast;

  double fltm_alpha;
  double fltm_beta;
  int fltm_maxCard;
  int fltm_maxDist;
  double fltm_simiThres;
  
  int fltm_nbrRestarts; 
  int fltm_imputeMode; 
  FLTM_Options fltm_opts;
  
};


inline Options getProgramOptions(int argc, char** argv) {
  Options result;
  std::string appName = boost::filesystem::basename(argv[0]);
  po::options_description optDesc("Options");

  try  {
    /** Define and parse the program options 
     */
    optDesc.add_options()
        ("help,h", "Print help messages")        
        ("in_dat,d", po::value<std::string>(&result.inputDataFile)->required(), "Input Data File")
        ("in_lab,l", po::value<std::string>(&result.inputLabelFile)->required(), "Input Label File")
        ("in_card,N", po::value<int>(&result.fltm_opts.cardinality)->required(), "Input cardinality")
        ("verbose,v", po::value<int>(&result.verbose)->default_value(0), "Verbose")
        
        ("out,o", po::value<std::string>(&result.outputFile)->required(), "Output File")

        ("clust,c", po::value<int>(&result.clustAlgo)->required(), "Clust Algo")

        ("max_dist,x", po::value<int>(&result.fltm_maxDist)->required(), "Max Dist")
        ("simi_thres,t", po::value<double>(&result.fltm_simiThres)->required(), "Simi Thres")

        ///////////////////////////////////////////////////////////////////////////
        ("db_minp,M", po::value<int>(&result.dbscan_minPts)->default_value(0), "DBSCAN MinPts")
        ("db_eps,E", po::value<double>(&result.dbscan_eps)->default_value(0), "DBSCAN Eps")

        ///////////////////////////////////////////////////////////////////////////
        ("cast_cast,C", po::value<double>(&result.cast_cast)->default_value(0), "CAST cast")
        ("f_imode,m", po::value<int>(&result.fltm_imputeMode)->required(), "FLTM impute mode") 

        ///////////////////////////////////////////////////////////////////////////
        ("f_alpha,a", po::value<double>(&result.fltm_alpha)->required(), "FLTM alpha")
        ("f_beta,b", po::value<double>(&result.fltm_beta)->required(), "FLTM beta")
        ("f_maxCard,X", po::value<int>(&result.fltm_maxCard)->required(), "FLTM maxCard")

        ("f_nbr_restarts,r", po::value<int>(&result.fltm_nbrRestarts)->required(), "FLTM nbr restarts")
        ("f_nbr_steps,s", po::value<int>(&result.fltm_opts.nbrSteps)->required(), "FLTM nbr steps") 

        ("f_thres_info,i", po::value<double>(&result.fltm_opts.infoThres)->required(), "FLTM thres info") 
        ("f_thres_em,e", po::value<double>(&result.fltm_opts.emThres)->required(), "FLTM thres EM")
        
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


} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_OPTIONS_HPP
