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
  std::string dataInFile; // input filename
  std::string labPosInFile; // input filename

  std::string outputDir; // input filename
  
  int hasHeader; // indicates whether the input file contains a header row

  unsigned K;
  unsigned maxPos;
  double eps;
  double simiThres;  
 
  ApplicationOptions(): hasHeader(true) {}
};

/****************************************************************************************/
#endif // _APPLICATIONOPTIONS_HPP
