/****************************************************************************************
 * File: Simi.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 11/07/2014

 ***************************************************************************************/
#ifndef FLTM_SIMI_HPP
#define FLTM_SIMI_HPP

#include "Diss.hpp"
namespace fltm
{

struct Simi: public Comp {};

struct MutInfoSimi: public Simi {
  MutInfoSimi( std::vector< std::vector<int> >& dm, std::vector<int>& pos, unsigned maxPos, double thres ):
      m_miDiss( dm, pos, maxPos, thres) {}

  virtual double compute( const size_t varA, const size_t varB ) {
    return 1.0 - m_miDiss(varA,varB);
  }

  virtual size_t size() const { return m_miDiss.size(); }
  virtual void invalidCache() { m_miDiss.invalidCache(); }

 private:
  MutInfoDiss m_miDiss;
};

} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_SIMI_HPP
