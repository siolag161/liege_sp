/****************************************************************************************
 * File: sigmoid.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/06/2014

 ***************************************************************************************/
#ifndef MATHS_SIGMOID_HPP
#define MATHS_SIGMOID_HPP

#include <cmath>
#include <Eigen/Dense>

namespace maths
{

struct Sigmoid {
  
  template<class T>
  static double compute( const T t ) {
    return 1.0 / ( 1.0 + std::exp(-t) );
  }

  template<class T>
  double operator()( const T t ) const {
    return compute(t);
  }

  template< class EigMatOut, class EigMatIn >
  static void compute( EigMatOut& out, const EigMatIn& in) {
    out.resize( in.rows(), in.cols()) ;
    for ( unsigned row = 0; row < in.rows(); ++row ) {
       for ( unsigned col = 0; col < in.cols(); ++col ) {
         out(row,col) = compute( in(row,col) );
       }
     }
  }
  
  template< class EigMatOut, class EigMatIn  >
  void operator()( EigMatOut& out, const EigMatIn& in) const {
    compute( out, in );
  }
};

template< class EigMatOut, class EigMatIn >
EigMatOut sigmoid( const EigMatIn& in) {
  EigMatOut out;
  Sigmoid::compute( out, in );
  return out;
}

struct Log {
  template< class EigMatOut, class EigMatIn >
  void operator()( EigMatOut& out, const EigMatIn& in) const {
    compute ( out, in );
  }

  template< class EigMatOut, class EigMatIn >
  static void compute( EigMatOut& out, const EigMatIn& in)  {
    out.resize( in.rows(), in.cols()) ;
    for ( unsigned row = 0; row < in.rows(); ++row ) {
      for ( unsigned col = 0; col < in.cols(); ++col ) {
        out(row,col) = std::log( in(row,col) );
      }
    }
  }

  // template< class EigMatIn, class EigMatOut >
  // static EigMatOut compute( const EigMatIn& in) {

  // }
};

template< class EigMatOut, class EigMatIn  >
EigMatOut log( const EigMatIn& in) {
  EigMatOut out;
  Log::compute ( out, in );
  return out;
}


/////////////////////////////////////////
template< typename T >
Eigen::Matrix<T, -1, 1> ones( const int N ) {
  Eigen::Matrix<T, -1, 1> o;
  o.resize(N);
  o.setOnes();
  return o;
}





} // namespace mathsends here. maths

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace maths
{


} // namespace mathsends here. maths

/****************************************************************************************/
#endif // MATHS_SIGMOID_HPP
