// /****************************************************************************************
//  * File: DataTable.hpp
//  * Description: 
//  * @author: Phan Duc Thanh (duc-thanh.phan@univ-nantes.fr) - Under supervision of Christine Sinoquet (christine.sinoquet@univ-nantes.fr)
//  * @date: 02/12/2013

//  ***************************************************************************************/
// #ifndef DATA_TABLE_DATATABLE_HPP
// #define DATA_TABLE_DATATABLE_HPP

// #define NBR_ROWS_RESERVE 100000

// /** A pair of classes: Matrix and Vector which encode a matrix and a vector, respectively.
//  * The Matrix class is implemented following a row-major scheme.
//  * Some mathematical operators are supported, such as Transpose.
//  * Both Matrix and Vector derived from std::vector.
//  */

// #include <vector> 
// #include <cstdlib> // size_t
// #include <boost/shared_ptr.hpp> 
// #include <iostream>

// namespace fltm
// {

// /** Row encodes a row in the Matrix. It is derived from std::vector (to be compatible with
//  * proBT library). 
//  *
//  */
// template<typename ElemType = double>
// class Row: public std::vector<ElemType>
// {
//  public:

//   typedef typename std::vector<ElemType>::size_type SizeT;

//   Row(const SizeT& nbrElements = 0) { this->resize(nbrElements); }

//   template<typename InputIterator>
//   Row(InputIterator rangeBegin, InputIterator rangeEnd)
//   {
//     for( ; rangeBegin != rangeEnd; ++rangeBegin) {
//       this->push_back(*rangeBegin);
//     }
//   }
  
//   virtual Row& add(const ElemType& e) { this->push_back(e); return *this; }
//   virtual Row& remove(const SizeT& index) {this->erase(this->begin()+index); return *this;}
//   virtual Row& add(const SizeT& index, const ElemType& e) { this->insert(this->begin()+index, e); return *this; }
  
// };


// /** Matrix is a std::vector of std::vector to be compatible with ProBT library.
//  *
//  */
// template<typename ElemType = double>
// class Matrix: public std::vector<std::vector<ElemType> >
// {
//  public:
//   typedef typename  std::vector<std::vector<ElemType> >::size_type SizeT;

//   Matrix(const SizeT &nbrRows = 0, const SizeT &nbrColumns = 0);
  
//   template<typename InputIterator>
//   Matrix(const SizeT& nbrRows, const SizeT& nbrColumns, InputIterator rBegin, InputIterator rEnd);
  
//   virtual SizeT nbrRows() const { // nbr of rows in this matrix {
//     return this->size();
//   }
      
//   virtual SizeT nbrColumns() const { // nbr of columns (should be constant for all rows)
//     return nbrRows() > 0 ? (this->begin()->size()) : 0;
//   }

//   virtual SizeT nbrElements() const {
//     return nbrRows()*nbrColumns();
//   } // under the above assumption

//   // @todo: add --> addRow
//   Matrix& add(const std::vector<ElemType>& row) { this->push_back(row); return *this; } 

//   // @todo: pos --> colIndex; add --> insert
//   // 
//   Matrix& add(const SizeT& row, const SizeT& pos, const ElemType& e) {    
//     this->at(row).insert(this->at(row).begin()+pos, e);
//     return *this;
//   }

  
//   template<class VecT>
//   Matrix& insertColumn(const SizeT& colIndex, const VecT& column);
  
//   template<class VecT>
//   Matrix& addColumn(const VecT& column);
//   Matrix& removeColumn(const SizeT& colIndex);

//   const Row<ElemType> clone_row (const size_t& row) const {
//     return Row<ElemType>(this->at(row).begin(), this->at(row).end());
//   }
                                  
//   boost::shared_ptr<Matrix<ElemType> > Transpose() const; // returns a copy of the Matrix, transposed
// };

// } // namespace data_table ends here. 

// /****************************** IMLEMENTATION BELOW THIS POINT **************************/
// namespace fltm
// {

// //////////////////////////////////////////////////////////////////////////////////////////
// template<typename ElemType>
// Matrix<ElemType>::Matrix(const SizeT& nbrRows, const SizeT &nbrColumns)
// {
//   this->reserve(NBR_ROWS_RESERVE); // to anticipate further row addition; assumption: nbrRows < NBR_ROWS_RESERVE
//   if(nbrRows > 0)
//     this->resize(nbrRows, Row<ElemType>(nbrColumns)); // nbrRows rows of length nbrColumns
// }

// /** Constructs a Matrix and initializes from a external data source (example: a file), delimited by
//  * rBegin and rEnd
//  */
// template<typename ElemType>
// template<typename InputIterator>
// Matrix<ElemType>::Matrix(const SizeT &nbrRows, const SizeT &nbrColumns,
//                   InputIterator rBegin, InputIterator rEnd)
// {
//   this->reserve(NBR_ROWS_RESERVE);
//   if(nbrRows > 0) {
//     this->resize(nbrRows, Row<ElemType>(nbrColumns)); // nbrRows rows of length nbrColumns

//     for(SizeT idx = 0; rBegin != rEnd; rBegin++)
//     {
//       int row = idx / this->nbrColumns();
//       int col = idx % this->nbrColumns();
//       this->at(row)[col] = *rBegin;
//       idx++;
//     }
//   }
// }



// /** @todo: check the length
//  *
//  */
// template<typename ElemType>
// template<class ColumnT>
// Matrix<ElemType>& Matrix<ElemType>::insertColumn(const SizeT& colIndex, const ColumnT& column)
// {
//   for (SizeT row = 0; row < nbrRows(); ++row)
//   {
//     this->add(row, colIndex, column[row]); // @todo: check the name of the method
//   }

//   return *this;
// } 

// // @todo: verify if insert(lastColumn) = push_back
// template<typename ElemType>
// template<class ColumnT>
// Matrix<ElemType>& Matrix<ElemType>::addColumn(const ColumnT& column)
// {
//   return insertColumn(nbrColumns(), column);
// }

// template<typename ElemType>
// boost::shared_ptr<Matrix<ElemType> > Matrix<ElemType>::Transpose() const
// {
//   boost::shared_ptr<Matrix<ElemType> > result(new Matrix<ElemType>(nbrColumns(), nbrRows()));
//   for (SizeT row = 0; row < nbrRows(); row++)
//   {    
//     for (SizeT col = 0; col < nbrColumns(); col++)
//     {
//       result->at(col).at(row) = this->at(row).at(col);
//     }
//   }  
//   return result;
// }

// ////////////////////////////////////////// Output of the Matrix  ////////////////////////////////////////////

// /** Outputs a Row into an Out object under a specific format.
//  * Example: Row contains 3 elements a, b and c, then out = [a,b,c]
//  * @todo: check the final ','
//  */
// template <typename ElemType, typename charT, typename traits>
// std::basic_ostream<charT, traits>& operator<< (std::basic_ostream<charT, traits>& out,
//                                                const Row<ElemType>& v)
// {

//   /* Builds the stringstream to hold the result. */
//   if (v.empty()) {
//     return out;
//   }
  
//   std::basic_stringstream<charT, traits> buffer;

//   /* Copies formatting information */
//   buffer.imbue(out.getloc());
//   buffer.flags(out.flags());
//   buffer.precision(out.precision());

//   /* Gets a copy of the code conversion facet. */
//   const std::ctype<charT>& ccvt = std::use_facet< std::ctype<charT> >(out.getloc());

//   buffer << "[ ";
//   for(size_t idx = 0; idx < v.size() - 1; idx++)
//   {
//     buffer << v[idx] << ccvt.widen(',');       
//   }
  
//   buffer << v[v.size() - 1] <<" ]";

//   // /* Writes the buffer to the stream and returns the result. */
//   return out << buffer.str();
// }


// /** Outputs a Matrix
//  *  @todo: check whether possible to call buffer << row 
//  */
// template <typename ElemType, typename charT, typename traits>
// std::basic_ostream<charT, traits>& operator<< (std::basic_ostream<charT, traits>& out, const Matrix<ElemType>& mt)
// {

//   /* Builds the stringstream to hold the result. */
//   std::basic_stringstream<charT, traits> buffer;

//   /* Copies formatting info. */
//   buffer.imbue(out.getloc());
//   buffer.flags(out.flags());
//   buffer.precision(out.precision());

//   /* Gets a copy of the code conversion facet. */
//   const std::ctype<charT>& ccvt = std::use_facet< std::ctype<charT> >(out.getloc());
//   //buffer<<"[";
//   for(size_t row = 0; row < mt.nbrRows(); row++)
//   {
//     for(size_t col = 0; col < mt.nbrColumns() - 1; col++)
//     {
//       buffer << mt[row][col] << ccvt.widen(',');
//     }
//     if (mt.nbrColumns() > 0)
//     {
//       buffer << mt[row][mt.nbrColumns() - 1];
//     }
//     if (row < mt.nbrRows()-1 )
//     {
//       buffer << std::endl;
//     }
//   }
//   buffer<<"\n";

//   /* Writes the buffer to the stream and returns. */
//   return out << buffer.str();
// }


// /** This method provides a quick way for printing to standard output a std::vector of data.
//  *  As std::vector (and several other classes in sdt::collection) is parameterized by 2 types
//  * (in this case 2 classes), we need to write template<class, class> class V
//  */

// template<template<class, class> class V, class ElemType>
// void printVector(const V<ElemType, std::allocator<ElemType> >& v)
// {

//   std::cout<<"[ ";
//   for(size_t idx = 0; idx < v.size() - 1; idx++)
//   {
//     std::cout << v[idx] << ", ";   
  
//   }
//   if (v.size() > 0)
//     std::cout<<v[v.size()-1];
//   std::cout<<" ]\n";

//   /* Write the buffer to the stream and return. */
//   //return out << buffer.str();
// }


 
// } // namespace data_table ends here.

// /****************************************************************************************/
// #endif // DATA_TABLE_DATATABLE_HPP
