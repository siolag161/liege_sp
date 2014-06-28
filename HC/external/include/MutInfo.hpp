/****************************************************************************************
 * File: MutInfotheo.hpp
 * Description: 
 * @author: Phan Duc Thanh (duc-thanh.phan@univ-nantes.fr) - Under supervision of Christine Sinoquet (christine.sinoquet@univ-nantes.fr)
 * @date: 29/12/2013

 ***************************************************************************************/
#ifndef INFOTHEO_MUTINFO_HPP
#define INFOTHEO_MUTINFO_HPP

#include <math.h> //log
#include <map>
#include <stdlib.h> // abs
#include <algorithm>    // std::min
#include <numeric> // std::accumulate
#include <boost/shared_ptr.hpp>

#include "TypeUtils.hpp" // for utility::Int2Type
#include "Entropy.hpp"

namespace clustering
{


template<int EstimateMethodT>
struct MutualInformation
{

  MutualInformation() {}

  /**
   * Takes two iterators as input and computes the mutual information
   * according to the method selected for estimating the entropy
   * @param maxValXY maximum value over all the possible values of random variables X and Y
   */
  template<typename XIterator, typename YIterator>
  double operator()(XIterator xBegin, XIterator xEnd, YIterator yBegin) {
    return compute(xBegin, xEnd, yBegin, utility::Int2Type<EstimateMethodT>());
    
  }

  template<typename VecT>
  double operator()(const VecT& xVec, const VecT& yVec) {    
    return compute(xVec.begin(), xVec.end(), yVec.begin(), utility::Int2Type<EstimateMethodT>());
  }


  template<template<class> class MatrixT, class T>
  boost::shared_ptr<MatrixT<double> > operator()(const MatrixT<T>& mat);
  
 protected:  
  template<typename XIterator, typename YIterator>
  double compute(XIterator xBegin, XIterator xEnd, YIterator yBegin, utility::Int2Type<EMP>);
  
  template<template<class> class MatrixT, class T>
  boost::shared_ptr<MatrixT<double> > compute(const MatrixT<T>& mat, utility::Int2Type<EMP>);

};

} // namespace clustering ends here.

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


template<int EstimateMethodT> 
template<template<class> class MatrixT, class T>
boost::shared_ptr<MatrixT<double> > MutualInformation<EstimateMethodT>::operator()(const MatrixT<T>& mat)
{
  return compute(mat, utility::Int2Type<EstimateMethodT>());
}


////////////////////////////////////// Implementation of COMPUTE functions /////////////////////////////////////////////


template<int EstimateMethodT> 
template<typename XIterator, typename YIterator>
double MutualInformation<EstimateMethodT>::compute( XIterator xBegin, XIterator xEnd,
                                                    YIterator yBegin, utility::Int2Type<EMP> )
{ 

  std::map<unsigned, unsigned> xCountMap;
  std::map<unsigned, unsigned> yCountMap;
  typedef std::pair<unsigned, unsigned> IntPair;
  std::map<IntPair, unsigned> jointCountMap;
  
  double vecLen = 0.0;
  for (; xBegin != xEnd; ++xBegin)  {
    const unsigned xVal = *xBegin; const unsigned yVal = *yBegin;
    updateCountMap(xCountMap, xVal);
    updateCountMap(yCountMap, yVal);

    const IntPair jointKey = std::make_pair(*xBegin, *yBegin); 
    updateCountMap(jointCountMap, jointKey);
    vecLen++; yBegin++;      
  }
    
  double xLogSum = accumulate(xCountMap.begin(), xCountMap.end(), 0.0, sumLogCount<unsigned>);
  double yLogSum = accumulate(yCountMap.begin(), yCountMap.end(), 0.0, sumLogCount<unsigned>);
  double jointLogSum = accumulate(jointCountMap.begin(), jointCountMap.end(),
                                  0.0, sumLogCount<IntPair>);

  // double entropyX = log(vecLen) - 1/vecLen*(xLogSum);
  // double entropyY = log(vecLen) - 1/vecLen*(yLogSum);
  // double jEntropy = log(vecLen) - 1/vecLen*(jointLogSum);
  return log(vecLen) + 1/vecLen*(jointLogSum - xLogSum - yLogSum);
}

/** MatrixT passed as parameter is a row-major Matrix in which each Row denotes a Variable.
 *  Implemented using the formula MI(X,Y) = E(X) + E(Y) - E(X,Y)
 */
template<int EstimateMethodT> 
template<template<class> class MatrixT, class T>
boost::shared_ptr<MatrixT<double> >
     MutualInformation<EstimateMethodT>::compute(const MatrixT<T>& mat,
                                                 utility::Int2Type<EMP>)
{
  boost::shared_ptr<MatrixT<double> > result(new MatrixT<double>(mat.nbrRows(), mat.nbrRows()));
  std::map<unsigned, double> entropyMap;

  Entropy<EMP> entropy;
  JointEntropy<EMP> mutualEntropy;
  for (unsigned i = 0; i < mat.nbrRows(); ++i) {    
    if (entropyMap.find(i) == entropyMap.end()) // if entropy of i-th variable is not yet computed
      entropyMap[i] = entropy(mat[i]); // computes and puts in the map

    (*result)[i][i] = entropy(mat[i]); // MI(X,X) = E(X)
    for (unsigned j = i+1; j < mat.nbrRows(); ++j) {
        entropyMap[j] = entropy(mat[j]); // entropy of j-th variable is not yet computed         
       double ijEntropy = mutualEntropy(mat[i], mat[j]);
       double mutInfo = entropyMap[i] + entropyMap[j] - ijEntropy;
       (*result)[i][j] = mutInfo; 
       (*result)[j][i] = (*result)[i][j];
    }
  }
  return result;
}

} // namespace infotheo ends here. 

/****************************************************************************************/
#endif // INFOTHEO_MUTINFO_HPP
