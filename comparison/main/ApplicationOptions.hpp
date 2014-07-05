/****************************************************************************************
 * File: ApplicationOptions.hpp
 * Description:  
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 28/04/2014

 ***************************************************************************************/
#ifndef _APPLICATIONOPTIONS_HPP
#define _APPLICATIONOPTIONS_HPP

struct ApplicationOptions
{
  std::string referenceClusteringFile; // input filename
  std::string targetClusteringFile; // input filename
  std::string assoFile; // input filename

  std::string outputPath; // input filename
  
  int minN;
  int maxN;

  // double threshold;
  
  ApplicationOptions() {}
};

/****************************************************************************************/
#endif // _APPLICATIONOPTIONS_HPP
