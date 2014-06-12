/****************************************************************************************
 * File: Range.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 02/05/2014

 ***************************************************************************************/
#ifndef FLTM_RANGE_HPP
#define FLTM_RANGE_HPP

#include "TypeUtils.hpp"
namespace utility
{

const static int ITER_TYPE_ADD = 0;
const static int ITER_TYPE_MUL = 1;

template<class T, int TYPE>
class Range;

template<class T, int TYPE = 0>
struct RangeIterator {

  
  RangeIterator(const Range<T, TYPE> & range, const T curr, const T step):
      m_range(range), m_curr(curr), m_step(step) {}

  bool operator != (const RangeIterator<T,TYPE>& rhs) { return ( m_curr < rhs.m_curr ); }

  const RangeIterator<T, TYPE>& operator++() {
    increment(Int2Type<TYPE>());
    return *this;
  }
   
  const RangeIterator<T,TYPE> operator++() const {
    RangeIterator tmp(m_range, m_curr, m_step);
    //increment(TYPE);
    increment(Int2Type<TYPE>());
    return tmp;
  }

  T operator*() const { return m_curr; }
  
 private:
  void increment( Int2Type<ITER_TYPE_ADD> ) { m_curr += m_step;  }
  void increment( Int2Type<ITER_TYPE_MUL> ) { m_curr *= m_step;  }
  
  const Range<T, TYPE>& m_range;
  T m_curr;
  T m_step;

};

template<class T, int TYPE>
struct Range {
  typedef RangeIterator<T,TYPE> iterator;
    
  Range(const T min, const T max, const T step): m_min(min), m_max(max), m_step(step) {}

  iterator begin() const { return RangeIterator<T,TYPE>(*this, m_min, m_step); }
  iterator end() const { return RangeIterator<T,TYPE>(*this, m_max, m_step); } 

 private:
  T m_min;
  T m_max;
  T m_step;
};



} // namespace fltmends here. fltm

/***************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace utility
{


} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_RANGE_HPP
