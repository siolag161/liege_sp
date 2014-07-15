/****************************************************************************************
 * File: Evaluator.hpp
 * Description: 
 * @author: Phan Duc Thanh (duc-thanh.phan@univ-nantes.fr) - Under supervision of Christine Sinoquet (christine.sinoquet@univ-nantes.fr)
 * @date: 06/01/2014

 ***************************************************************************************/
#ifndef LATENT_TREE_MODEL_EVALUATOR_HPP
#define LATENT_TREE_MODEL_EVALUATOR_HPP

/** This find contains different criteria for evaluating a latent variable found and imputed
 * by FLTM.
 */

#include "Entropy.hpp"
#include "MutInfo.hpp"

#include <fstream>
namespace fltm
{

/** This functor computes (estimates) the average mutual information between the latent variable
 * and its children. So it returns simply sum(mutInfo) / nbrOfChildren.
 *
 */
struct AverageMutInfo
{
/** it requries as paratemers:
 *  - a vector of the latent variable' sample values
 *  - a transposed matrix of the dat
 *  - a cluster containing the children's indices (w.r.t the dataset)
 */
  template<class VecT, class MatrixT, class ClusterT>
  double operator()( const VecT& latentVar,
                     const MatrixT& mat,
                     ClusterT& cluster );
};

} // namespace latent_tree_modelends here. latent_tree_model

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace fltm
{

/** @todo: bug in this class
 *
 */
template<class VecT, class MatrixT, class ClusterT>
double AverageMutInfo::operator()( const VecT& latentVar,
                                   const MatrixT& mat,
                                   ClusterT& cluster )
{
  if (cluster.size() == 0) return 0.0;

  Entropy<EMP> entropy;
  JointEntropy<EMP> jointEntropy;
  MutualInformation<EMP> mutualInfo;
  
  double totalScaledMutInfo = 0.0;
  double varEntropy = entropy(latentVar);
  
  // for (size_t i = 0; i < latentVar.size(); ++i) {
  //   std::cout << latentVar[i] << ", ";
  // }
  // std::cout << "\ncluster size: " << cluster.size() << std::endl;
  for ( auto& idx: cluster ) {   
    double idxEntropy = entropy( mat[idx] );     
    double minEntropy = std::min( idxEntropy, varEntropy );
    double jEntropy = jointEntropy( latentVar, mat[idx] );

    //printf("entropyA: %f, entropyB: %f\n", idxEntropy, varEntropy);
    if ( minEntropy == 0 ) 
      continue;
  
    double mutInfo = varEntropy + idxEntropy - jEntropy;
    double scaledMutualInfo = mutInfo / minEntropy;
    totalScaledMutInfo += scaledMutualInfo;
    
  }
  double result =  totalScaledMutInfo / cluster.size(); 
  return result;
}


} // namespace latent_tree_modelends here. latent_tree_model

/****************************************************************************************/
#endif // LATENT_TREE_MODEL_EVALUATOR_HPP
