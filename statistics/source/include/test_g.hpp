/****************************************************************************************
 * File: test_g.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 22/06/2014

 ***************************************************************************************/
#ifndef STATS_TEST_G_HPP
#define STATS_TEST_G_HPP

#include <cmath>
#include <boost/math/distributions/chi_squared.hpp>
#include <numeric> // accumulate
#include <omp.h>
namespace stats
{

/** A statistic used in an alternative to the chi-squared goodness-of-fit test.
 *  The two test statistics usually have similar values and both have approximate
 *  chi-squared distributions when the model under test is a correct description
 *  of the data. The G2 test is usually preferred when comparing hierarchical models.
 *  If the observed cell frequencies are denoted by O1, O2,..., Ok and the
 *  corresponding expected cell frequencies by E1, E2,..., Ek, respectively,
 *  G2 is given by:  G2 = 2*sum( O_j*ln(O_j/E_j) )                  
 *  where ln is the natural logarithm. The test results from the work of Neyman and
 *  Egon Pearson, who first used the term 'likelihood ratio' in a 1931 paper.
 *
 *
 *  Returns:
 *  -------
 *  chisquare statistic: double
 *      The chisquare test statistic
 *  p: double
 *      The p-value of the test.
 *
 *  References:
 *  -----------
 *  http://en.wikipedia.org/wiki/G-test
 *
 */
struct TestG2 {

  /** @param obs Observed frequencies (or counts) in each category
   *  @param exp Expected frequencies (or counts) in each category
   */
  template<class VecT>
  double statistic( const VecT& obs, const VecT& exp )  {
    double statistic = 0.0;
    for (unsigned sz = 0; sz < obs.size(); ++sz) {
      if ( obs[sz]*exp[sz] != 0.0 ) {
        statistic += 2*obs[sz] * std::log( obs[sz] / exp[sz] );
      }
    }
    return statistic;
  }

  /** @param obs Observed frequencies (or counts) in each category
   *  @param exp Expected frequencies (or counts) in each category
   */
  template<class VecT>
  double gtest( const VecT& obs, const VecT& exp ) {
    double stat = statistic(obs, exp);
    const unsigned degreeFreedom = obs.size();
    return gtest(stat, degreeFreedom);
  }

  /**
   *
   */
  double gtest( const double statistic, const unsigned int degreeFreedom )  {
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
  double gtest( const ContigencyTabT& contigencyTab, bool useYates = false ) {
    assert(contigencyTab.size() > 0);
    const unsigned nbrRows = contigencyTab.size();
    const unsigned nbrColumns = contigencyTab[0].size();

    ContigencyTabT tab = contigencyTab;
    if ( useYates ) {
      assert( nbrRows == 2 ); assert( nbrColumns == 2);
      if ( tab[0][0]*tab[1][1] > tab[0][1]*tab[1][0] ) {
        tab[0][0] -= 0.5; tab[1][1] -= 0.5;
        tab[0][1] += 0.5; tab[1][0] += 0.5;
      } else {
        tab[0][0] += 0.5; tab[1][1] += 0.5;
        tab[0][1] -= 0.5; tab[1][0] -= 0.5;
      }
    }
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
          statistic += 2 * observed * std::log( observed / expected );
        }
      }
    }    
    const unsigned degreeFreedom = (nbrRows-1)*(nbrColumns-1);
    return gtest(statistic, degreeFreedom);
  }  
};

} // namespace stats ends here. 

/****************************************************************************************/
#endif // STATS_TEST_G_HPP

