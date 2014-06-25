/****************************************************************************************
 * File: statistics.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 22/06/2014

 ***************************************************************************************/
#ifndef STATS_STATISTICS_HPP
#define STATS_STATISTICS_HPP

#include <vector>
#include "test_g.hpp"
#include "fisher.hpp"
#include "chisq.hpp"

namespace stats
{

template <int I>
struct Int2Type
{
  enum { value = I };
};

enum TestType { G2 = 0, G2_YATES, CHISQ, CHISQ_YATES, FISHER };

template< unsigned int Test >
struct StatisticTest {
  static const int test = (Test == G2) ? G2 :
                          (Test == G2_YATES) ? G2_YATES :
                          (Test == CHISQ) ? CHISQ :
                          (Test == CHISQ_YATES) ? CHISQ_YATES : FISHER;

  template<class ContingencyTableT>
  double operator()( ContingencyTableT& mat ) const;

  /** @param
   *  @param 
   * requires that data.size() == which.size()
   */ 
  template<class VecT>
  double operator()( const VecT& geno,
                     const VecT& pheno,
                     const unsigned cardGenotype,
                     const unsigned cardPhenotype ) const {
    std::vector< std::vector<double> > contingencyTab( cardGenotype, std::vector<double>(cardPhenotype, 0.0) );
    for (unsigned i = 0; i < geno.size(); ++i) {
      unsigned row = geno[i];
      unsigned col = pheno[i];
      ++contingencyTab[row][col];
    };
    return pval( contingencyTab, Int2Type<test>() );    
  }
  
 private:  
  template<class ContingencyTableT>
  double pval( ContingencyTableT& mat, Int2Type<G2> ) const;

  template<class ContingencyTableT>
  double pval( ContingencyTableT& mat, Int2Type<G2_YATES> ) const;
  
  template<class ContingencyTableT>
  double pval( ContingencyTableT& mat, Int2Type<CHISQ> ) const;

  template<class ContingencyTableT>
  double pval( ContingencyTableT& mat, Int2Type<CHISQ_YATES> ) const;
  
  template<class ContingencyTableT>
  double pval( ContingencyTableT& mat, Int2Type<FISHER> ) const;

};

} // namespace statsends here. stats

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace stats
{

template< unsigned int Test >
template<class ContingencyTableT>
double StatisticTest<Test>::pval( ContingencyTableT& mat, Int2Type<G2> ) const {
  TestG2 g2;
  return g2.gtest(mat, false);
}


template< unsigned int Test >
template<class ContingencyTableT>
double StatisticTest<Test>::pval( ContingencyTableT& mat, Int2Type<G2_YATES> ) const {
  TestG2 g2;
  return g2.gtest(mat, true);
}

template< unsigned int Test >
template<class ContingencyTableT>
double StatisticTest<Test>::pval( ContingencyTableT& mat, Int2Type<CHISQ> ) const {
  TestChiSquared chisq;
  return chisq.chisqTest(mat, false);
}


template< unsigned int Test >
template<class ContingencyTableT>
double StatisticTest<Test>::pval( ContingencyTableT& mat, Int2Type<CHISQ_YATES> ) const {
  TestChiSquared chisq;
  return chisq.chisqTest(mat, true);
}

template< unsigned int Test >
template<class ContingencyTableT>
double StatisticTest<Test>::pval( ContingencyTableT& mat, Int2Type<FISHER> ) const {
  TestFisher fisher;
  return fisher.gtest(mat);
}


} // namespace statsends here. stats

/****************************************************************************************/
#endif // STATS_STATISTICS_HPP

