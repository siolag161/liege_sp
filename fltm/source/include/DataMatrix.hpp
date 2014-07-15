/****************************************************************************************
 * File: DataMatrix.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 11/07/2014

 ***************************************************************************************/
#ifndef CLUSTERING_DATAMATRIX_HPP
#define CLUSTERING_DATAMATRIX_HPP

#include <vector>
namespace fltm
{

typedef std::vector< std::vector<int> > Matrix;

template< class DataM >
size_t nrows( const DataM& mat ) {
  return mat.size();
}

template< class DataM >
size_t ncols( const DataM& mat ) {
  return mat.empty() ? 0 : mat.at(0).size();
}

template<class T>
std::vector< std::vector<T> > Transpose( const std::vector< std::vector<T> >& mat ) {
  std::vector< std::vector<T> > result( ncols(mat), std::vector<T>( nrows(mat), 0) );

  for (unsigned row = 0; row < nrows(mat); row++)
  {    
    for (unsigned col = 0; col < ncols(mat); col++)
    {
      result[col][row] = mat.at(row).at(col);
    }
  }  
  return result;
}

} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_DATAMATRIX_HPP
