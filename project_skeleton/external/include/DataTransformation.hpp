/****************************************************************************************
 * File: MutInfoTransformation.hpp
 * Description: 
 * @author: Phan Duc Thanh (duc-thanh.phan@univ-nantes.fr) - Under supervision of Christine Sinoquet (christine.sinoquet@univ-nantes.fr)
 * @date: 27/01/2014

 ***************************************************************************************/
#ifndef DATA_TRANSFORMATION_MUTINFOTRANSFORMATION_HPP
#define DATA_TRANSFORMATION_MUTINFOTRANSFORMATION_HPP

#include <map> // map
#include <limits> // std::numeric_limits
#include <algorithm>  // std::min

#include "MutInfo.hpp"


namespace fltm
{

typedef unsigned Index;
typedef float Value;
//typedef std::map<Index, Value> IndexValueMap;
typedef std::map<Index, std::map<Index, float> > DistMap;

/** Functor which transforms a data matrix into a square matrix
 * of pair-wise mutual information values between variables. This functor only considers
 * pairs of variables whose distance (given as parameter) is less then a given global
 * threshold. 
 */
struct MutInfoTransformer
{
  /** Computes pair-wise normalized mutual information values between variables and returns the result.
   * Only considers variables whose distance is less then a given global threshold
   * Formula for normalized mutual information is given as follows:
   * NMI(X,Y) = MI(X, Y) / min( E(X), E(Y) ) <= 1
   * @param[in] dataMat Original row-major data matrix. Each row represents a variable
   * @param[in] consMap Constraint map, defining the distance between pairs of variables
   * @param[in] thres Threshold, by default equals +infinity (meaning no constraint)
   */
  template< class MatrixT, typename ConstraintMapT>
  DistMap operator()( const MatrixT& dataMat,
                      ConstraintMapT& consMap,
                      const double simiThres = 1,
                      const unsigned thres = std::numeric_limits<unsigned>::max() );

  /**  Computes pair-wise normalized mutual information values between variables
   * and puts the result in an output parameter.
   * Only considers variables whose distance is less then a given global threshold
   * @param[in] dataMat Original row-major data matrix. Each row represents a variable
   * @param[in] consMap Constraint map defines the distance between pairs of variables
   * @param[in] thres Threshold (no)
   * @param[out] transformedData Holds the result
   */
  template< class MatrixT, typename ConstraintMapT>
  void operator()( DistMap& targetMat,
                   const MatrixT& sourceMat,
                   ConstraintMapT& consMap,
                   const double simiThres = 1,
                   const unsigned thres = std::numeric_limits<unsigned>::max() );

};

} // namespace specific_matrix_transformation ends here.


/****************************** IMLEMENTATION BELOW THIS POINT **************************/

//#include "InformationTheory/MutInfo.hpp" // for MutualInformation calculation
#include "Entropy.hpp" // for Entropy calculation
#include <algorithm> // for std::min
#include <iostream>
namespace fltm
{

/** Creates a DistMap object and uses it as parameter in calling
 *  operator()(DistMap& transMat,
 *   const MatrixT<T>& dataMat,
 *   const ConstraintMapT& consMap,
 *   const double thres)
 */
template< class MatrixT, typename ConstraintMapT>
DistMap MutInfoTransformer::operator()( const MatrixT& dataMat,
                                        ConstraintMapT& consMap,
                                        const double simiThres,
                                        const unsigned maxDistance)
{
  DistMap result;
  (*this)( result, dataMat, consMap, simiThres, maxDistance );
  return result;
}

/**
 *
 */
template<class MatrixT, typename ConstraintMapT>
void MutInfoTransformer::operator()( DistMap& transMat,
                                     const MatrixT& dataMat,
                                     ConstraintMapT& consMap,
                                     const double simiThres,
                                     const unsigned maxDistance ) 
{
  unsigned nbrVars = dataMat.size(); // as the variables are stacked as rows
  //IndexValueMap entropy;
  std::vector<double> entropyMap( nbrVars, -1.0);
  Entropy<EMP> entropy; // empirical entropy (estimates the probability by frequency)
  JointEntropy<EMP> mutEntropy; // empirial mutual entropy;
  for (unsigned varA = 0; varA < nbrVars; ++varA) { 
    for (unsigned varB = varA + 1; varB < nbrVars; ++varB) {
      if (consMap(varA, varB) < maxDistance) { // only considers pairs within a certain range

        if ( entropyMap.at(varA) < 0.0 ) { 
          entropyMap[varA] = entropy(dataMat[varA]); // computes entropy of varA only if not already done
        }
        
        if ( entropyMap.at(varB) < 0.0 ) { // computes entropy of varB only if not already done
          entropyMap[varB] = entropy(dataMat[varB]); // since this operation could be expensive
        }

        double minEntropyAB = std::min(entropyMap[varA], entropyMap[varB]); // takes the min 
      
        if (minEntropyAB == 0) {
          transMat[varA][varB] = 0; // by convention 0 if minEntropyAB = 0
        } else {
          double mutEntropyAB = mutEntropy( dataMat[varA], dataMat[varB] );
          double mutualInfoAB = entropyMap[varA] + entropyMap[varB] - mutEntropyAB; // classic formula
          double normalizedMutInfo = mutualInfoAB / minEntropyAB;
          transMat[varA][varB] = (normalizedMutInfo) ;// > simiThres) ? 1.0 : 0.0;
        }
      }
    }
  }
}

// /**
//  *
//  */
// template<template<class> class MatrixT, typename ConstraintMapT, typename T>
// void MutInfoTransformer::operator()( DistMap& transMat,
//                                      const MatrixT<T>& dataMat )
// {
//   unsigned nbrVars = dataMat.size(); // as the variables are stacked as rows
//   IndexValueMap entropyMap;
//   info_theo::Entropy<info_theo::EMP> entropy; // empirical entropy (estimates the probability by frequency)
//   info_theo::JointEntropy<info_theo::EMP> mutEntropy; // empirial mutual entropy;
//   for (unsigned varA = 0; varA < nbrVars; ++varA) {    
//     for (unsigned varB = varA + 1; varB < nbrVars; ++varB) {
//       if (entropyMap.find(varA) == entropyMap.end()) { 
//         entropyMap[varA] = entropy(dataMat[varA]); // computes entropy of varA only if not already done
//       }
        
//       if (entropyMap.find(varB) == entropyMap.end()) { // computes entropy of varB only if not already done
//         entropyMap[varB] = entropy(dataMat[varB]); // since this operation could be expensive
//       }

//       double minEntropyAB = std::min(entropyMap[varA], entropyMap[varB]); // takes the min 
      
//       if (minEntropyAB == 0) {
//         transMat[varA][varB] = 0; // by convention 0 if minEntropyAB = 0
//       } else {
//         double mutEntropyAB = mutEntropy( dataMat[varA], dataMat[varB] );
//         double mutualInfoAB = entropyMap[varA] + entropyMap[varB] - mutEntropyAB; // classic formula
//         transMat[varA][varB] = mutualInfoAB / minEntropyAB; 
//       }      
//     }
//   }
// }

} // namespace specific_matrix_transformation ends here. 

/****************************************************************************************/
#endif // SPECIFIC_MATRIX_TRANSFORMATION_MUTINFOTRANSFORMATION_HPP
