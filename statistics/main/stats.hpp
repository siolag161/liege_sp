/****************************************************************************************
 * File: statistics.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 16/07/2014

 ***************************************************************************************/
#ifndef _STATISTICS_HPP
#define _STATISTICS_HPP

#include "statistics.hpp"
#include "association_test.hpp"
#include "permutationProcedure.hpp"
#include "Graph.hpp"
typedef int Position;
typedef std::string Label;
typedef std::vector< std::vector<int> > Matrix;
typedef std::vector<int> Phenotype;
typedef std::vector< std::vector< std::vector<double> > > Distributions;

ApplicationOptions getProgramOptions(int argc, char** argv);
void checkInputFiles( std::string& path, std::string filename );


void writeResult( Distributions& dists, 
                  std::vector<std::vector<double> >& pvalues,
                  fltm::Graph& graph,                  
                  std::vector<stats::StatTest*>& tests,
                  std::string outputFileName );

 

/****************************************************************************************/
#endif // _STATISTICS_HPP
