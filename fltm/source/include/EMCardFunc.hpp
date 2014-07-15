/****************************************************************************************
 * File: EMCardFunc.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 11/07/2014

 ***************************************************************************************/
#ifndef FLTM_EMCARDFUNC_HPP
#define FLTM_EMCARDFUNC_HPP

namespace fltm
{

struct CardFunc {
  // virtual size_t operator()(const ClusterT& variables)  {
  //   size_t nbrVariables = variables.size();
  //   return std::min(floor(alpha * nbrVariables + beta), maxCard);
  // }
  virtual int operator()(const std::vector<int>& cluster) = 0;
};

/** Computes the cardinality according to the following formula:
 * card = min(alpha * nbrChildren + beta, maxCardinality)
 * where alpha, beta and maxCard are given.
 */
struct LinearCardinality: public CardFunc
{
  LinearCardinality(const double a, const double b, const int card):
      alpha(a), beta(b), maxCard(card) {}

  // template<typename ClusterT>
    virtual int operator()(const std::vector<int>& cluster)  {
    int nbrVariables = cluster.size();
    return std::min( int(alpha * nbrVariables + beta), maxCard);
  }

 protected:
  double alpha;
  double beta;
  int maxCard;
};

} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_EMCARDFUNC_HPP
