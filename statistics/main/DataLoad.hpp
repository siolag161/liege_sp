
/****************************************************************************************
 * File: DataLoad.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 12/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_DATALOAD_HPP
#define CLUSTERING_DATALOAD_HPP

#include <vector>
#include <string>
#include "CSVParser.hpp"
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>

template< typename T >
void loadDataTable( std::vector< std::vector<T> >& dt,
                    const std::string& infile,
                    const char& sep = ',',
                    const char& quote = '"' ) {

  std::cout << "loading data" << std::endl << std::endl;

  std::ifstream matrixFile(infile.c_str());
  if (!matrixFile) return;
   dt.reserve(100000);

  utility::CSVIterator<T> matrixLine(matrixFile);
  
  for( ; matrixLine != utility::CSVIterator<T>(); ++matrixLine ) {         
    std::vector<T> row(matrixLine->size(), 0);
    for (unsigned i = 0; i < matrixLine->size(); ++i) {
      row[i] = matrixLine->at(i);
    }
    dt.push_back(row);    
  }

  dt.resize(dt.size());
  size_t ncols = dt.empty() ? 0 : dt[0].size();
  std::cout << "done loading matrix of (" << dt.size() << "," << ncols << ")" << std::endl << std::endl;

}

void loadPhenotype( std::vector< int >& phenotype,
                    // std::vector< std::string > & labels,
                    // std::vector<unsigned>& ids,
                    // std::vector< int >& positions,
                    const std::string& infile ) {
  std::ifstream labPosFile(infile.c_str());
  if (!labPosFile) return;
  // std::vector<std::string>().swap(labels); //lab2Pos.clear();
  // std::vector<int>().swap(positions); //.clear();
  utility::CSVIterator<std::string> labPosLine(labPosFile);// ++labPosLine;
  for( ; labPosLine != utility::CSVIterator<std::string>(); ++labPosLine ) {    
    // std::string label =  (*labPosLine)[2];
    // int position = boost::lexical_cast<int>( (*labPosLine)[3]);
    int pheno = boost::lexical_cast<int>( (*labPosLine)[0]);
    // unsigned id = boost::lexical_cast<unsigned>( (*labPosLine)[1]);
    phenotype.push_back(pheno);
    // ids.push_back(id);
    // labels.push_back(label);
    // positions.push_back(position);
  }

  // std::cout << "load " << labels.size() << " variables.\n";
}

void loadLabelPosition( // std::vector< int >& phenotype,
                     std::vector< std::string > & labels,
                     std::vector<int>& ids,
                     std::vector<int>& positions,
                     std::vector<int>& cards,
                     const std::string& infile ) {
  std::ifstream labPosFile(infile.c_str());
  if (!labPosFile) return;
  labels.clear();
  positions.clear();
  utility::CSVIterator<std::string> labPosLine(labPosFile);// ++labPosLine;
  for( ; labPosLine != utility::CSVIterator<std::string>(); ++labPosLine ) {    
    std::string label =  (*labPosLine)[1];
    int position = boost::lexical_cast<int>( (*labPosLine)[2]);
    int card = boost::lexical_cast<int>( (*labPosLine)[3]);
    int id = boost::lexical_cast<int>( (*labPosLine)[0]);
    ids.push_back(id);
    labels.push_back(label);
    positions.push_back(position);
    cards.push_back(card);
  }
}



/****************************************************************************************/
#endif // CLUSTERING_DATALOAD_HPP
