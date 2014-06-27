/****************************************************************************************
 * File: load_eigen.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_LOAD_EIGEN_HPP
#define CLUSTERING_LOAD_EIGEN_HPP

#include <vector>
#include <string>
#include "CSVParser.hpp"
#include <string>
#include <boost/lexical_cast.hpp>
#include <cstdio>
#include <fstream>
#include <algorithm> // count

#include <Eigen/Dense>
namespace clustering
{

template<typename T>
bool load_eigen_matrix( Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& mat, const std::string file ) {
  std::ifstream inFile(file);
  if (!inFile) {
    return false;
  }  
  unsigned nrows = std::count( std::istreambuf_iterator<char>(inFile), 
                               std::istreambuf_iterator<char>(), '\n' );
  inFile.seekg (0, std::ios::beg);
  std::string firstLine;
  getline(inFile, firstLine);
  unsigned ncols = std::count( firstLine.begin(), firstLine.end(), ',' ) + 1;
  inFile.seekg (0, std::ios::beg);
  
  mat.resize(nrows,ncols);
  utility::CSVIterator<T> matrixLine(inFile);  
  for ( size_t row = 0; row < nrows; ++ row ) {
    for ( size_t col = 0; col < ncols; ++ col ) {
      mat(row,col) = matrixLine->at(col);
    }
    ++matrixLine;
  } 
  inFile.close();

  return true;
}

template<typename T>
bool load_eigen_vector( Eigen::Matrix<T, Eigen::Dynamic, 1>& vec, const std::string file ) {
  std::ifstream inFile(file);
  if (!inFile) {
    return false;
  }  
  unsigned nrows = std::count( std::istreambuf_iterator<char>(inFile), 
                               std::istreambuf_iterator<char>(), '\n' );
  inFile.seekg (0, std::ios::beg);  
  vec.resize(nrows);
  utility::CSVIterator<T> vectorLine(inFile);  
  for ( size_t row = 0; row < nrows; ++ row ) {
    vec(row) = vectorLine->at(0);
    ++vectorLine;
  } 
  inFile.close();
}


} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_LOAD_EIGEN_HPP
