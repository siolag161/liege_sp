/****************************************************************************************
 * File: Diss.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 09/07/2014

 ***************************************************************************************/
#ifndef FLTM_DISS_HPP
#define FLTM_DISS_HPP

#include <vector>

#include "Distance.hpp"
namespace fltm
{

struct Comp {
  virtual double operator()( const size_t varA, const size_t varB ) { return compute(varA, varB); }
  virtual double compute( const size_t varA, const size_t varB ) = 0;
  virtual size_t size() const = 0;
  virtual void invalidCache() = 0;
  // virtual void reset( std::vector< std::vector<int> >& dm, std::vector<int>& pos ) = 0;
};

struct Distance: public Comp {};


struct MutInfoDiss: public Distance {
  MutInfoDiss( std::vector< std::vector<int> >& dm, std::vector<int>& pos, unsigned maxPos, double thres ):
      m_diss(dm, pos, maxPos, thres) {}
  virtual double compute( const size_t varA, const size_t varB ) {
    return m_diss(varA,varB);
  }

  virtual size_t size() const { return m_diss.size(); }
  virtual void invalidCache() { m_diss.invalidCache(); }
 
private:
  MutInfoDistance< std::vector< std::vector<int> > > m_diss;
};


} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_DISS_HPP
