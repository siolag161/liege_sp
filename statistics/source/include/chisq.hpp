/****************************************************************************************
 * File: chisq.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 24/06/2014

 ***************************************************************************************/
#ifndef STATS_CHISQ_HPP
#define STATS_CHISQ_HPP

#include <cmath>
namespace stats
{

struct TestChiSquared {

  /** @param obs Observed frequencies (or counts) in each category
   *  @param exp Expected frequencies (or counts) in each category
   */
  template<class VecT>
  double statistic( const VecT& obs, const VecT& exp, bool useYates = false  ) const {
    double statistic = 0.0;
    for (unsigned sz = 0; sz < obs.size(); ++sz) {
      if ( obs[sz]*exp[sz] != 0.0 ) {
        double t = 0.0;
        if (useYates)
          t = abs(obs[sz] - exp[sz]) - 0.5;
        else
          t = obs[sz] - exp[sz];
        statistic += t*t / exp[sz];
      }
    }
    return statistic;
  }

  /** @param obs Observed frequencies (or counts) in each category
   *  @param exp Expected frequencies (or counts) in each category
   */
  template<class VecT>
  double chisqTest( const VecT& obs, const VecT& exp, bool useYates = false ) const {
    double stat = statistic(obs, exp, useYates);
    const unsigned degreeFreedom = obs.size();
    return chisqTest(stat, degreeFreedom);
  }

  /**
   *
   */
  double chisqTest( const double statistic, const unsigned int degreeFreedom ) const {
    boost::math::chi_squared dist(degreeFreedom);
    double p_value = 1.0;
    try {
      p_value = 1- boost::math::cdf(dist, statistic);
    }
    catch ( std::exception& e) {
      p_value = 1.0;
    }
    return p_value;
  }


  /** @param
   *  @param 
   * requires that data.size() == which.size()
   */
  template<class ContigencyTabT>
  double chisqTest( const ContigencyTabT& contigencyTab, bool useYates = false ) const {
    assert(contigencyTab.size() > 0);
    const unsigned nbrRows = contigencyTab.size();
    const unsigned nbrColumns = contigencyTab[0].size();

    ContigencyTabT tab = contigencyTab;
  
    double tableSum = 0.0;
    std::vector<double> rowSums(nbrRows, 0.0), columnSums(nbrColumns, 0.0);

    for (unsigned row = 0; row < nbrRows; ++row) {
      for (unsigned col = 0; col < nbrColumns; ++col) {
        double caseVal = tab[row][col];
          columnSums[col] += caseVal;
          rowSums[row] += caseVal;
          tableSum += caseVal;        
      }
    }

    double statistic = 0.0;
    for (unsigned row = 0; row < nbrRows; ++row) {
      for (unsigned col = 0; col < nbrColumns; ++col) {
        double observed = tab[row][col];
        double expected = rowSums[row]*columnSums[col] / tableSum;
        if ( observed*expected != 0.0 ) {
          // statistic += 2 * observed * std::log( observed / expected );
          double t = 0.0;
          if (useYates)
            t = std::abs(observed - expected) - 0.5;          
          else
            t = observed - expected;
          statistic += t*t / expected;
        }
      }
    }    
    const unsigned degreeFreedom = (nbrRows-1)*(nbrColumns-1);
    return chisqTest( statistic, degreeFreedom );
  }  
};

} 

/****************************************************************************************/
#endif // STATS_CHISQ_HPP
