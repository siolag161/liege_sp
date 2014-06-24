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
  std::string genoInFile; // input filename
  std::string phenoInFile; // input filename
  std::string outputDir; // input filename
  int codage;
  int permutation;
  int verbose;
  
  ApplicationOptions() {}
};

/****************************************************************************************/
#endif // _APPLICATIONOPTIONS_HPP
