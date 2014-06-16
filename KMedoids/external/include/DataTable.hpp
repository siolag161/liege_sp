/****************************************************************************************
 * File: DataTable.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 29/04/2014

 ***************************************************************************************/
#ifndef UTILITY_DATATABLE_HPP
#define UTILITY_DATATABLE_HPP

namespace utility
{

template<typename MatrixT>
unsigned nrow(const MatrixT& mat) {
  return mat.size();
}

template<typename MatrixT>
unsigned ncol(const MatrixT& mat) {
  return mat.empty() ? 0 : mat.at(0).size();
}


} // namespace Utilityends here. Utility



/****************************************************************************************/
#endif // UTILITY_DATATABLE_HPP
