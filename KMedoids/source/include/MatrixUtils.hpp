/****************************************************************************************
 * File: MatrixUtils.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 13/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_MATRIXUTILS_HPP
#define CLUSTERING_MATRIXUTILS_HPP

namespace clustering
{

template< class DataMatrix>
size_t nrows( const DataMatrix& dm ) {
  return dm.size();
}

template< class DataMatrix>
size_t ncols( const DataMatrix& dm ) {
  return dm.empty() ? 0 : dm.at(0).size();
}

template< class DataMatrix>
size_t nitems( const DataMatrix& dm ) {
  return nrows(dm) * ncols(dm);
}


template <class DataMatrix>
typename DataMatrix::value_type sumMatrix( const DataMatrix& mat,
                                 size_t row_start = 0, size_t row_end = std::numeric_limits<size_t>::max(),
                                 size_t col_start = 0, size_t col_end = std::numeric_limits<size_t>::max() )
{
  if (row_end > mat.size1()) row_end = mat.size1();
  if (col_end > mat.size2()) col_end = mat.size2();

  typename DataMatrix::value_type total = 0;
  for (size_t i = row_start; i < row_end; i++) {
    for (size_t j = col_start; j < col_end; j++) { 
      total += mat(i,j);
    }
  }
  return total;
}


} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_MATRIXUTILS_HPP
