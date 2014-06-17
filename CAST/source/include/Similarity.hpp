/****************************************************************************************
 * File: Similarity.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 17/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_SIMILARITY_HPP
#define CLUSTERING_SIMILARITY_HPP

#include <map>
#include <vector>
#include "MutInfo.hpp"

namespace fltm
{

typedef std::map< size_t, double > SimiCache;
typedef std::vector < std::vector <int> > Matrix;
typedef std::map< unsigned, std::map<unsigned, double> > SimiMatrix;

typedef int Position;
typedef std::vector< Position > Positions;
struct SimilarityCompute {
  const double MAX_SIMILARITY = 1.0, MIN_SIMILARITY = 0.0;
  explicit SimilarityCompute( const Positions& pos ,
                              const Matrix& mat,
                              unsigned maxDistance,                                      
                              double simiThres );
  virtual ~SimilarityCompute() {}
  virtual double operator()( unsigned, unsigned );
 protected:
  const Positions& positions;
  const Matrix& dataMat;
  unsigned maxPosition;  
  double m_simiThres;

  std::vector<double> entropyMap;

  unsigned nbrVars;
  SimiCache simiCache;

};


template<class DM>
double mutualInformationSimilarity( std::vector<double>& entropyMap,
                                    SimiCache& simiCache,
                                    const DM& dataMat,
                                    const size_t varA,
                                    const size_t varB ) {
  double result = 0.0;
  size_t nVars = entropyMap.size();
  size_t key = 2*nVars*varA + varB;
  Entropy<EMP> entropy; // empirical entropy (estimates the probability by frequency)
  JointEntropy<EMP> mutEntropy; // empirial mutual entropy;

  if ( entropyMap.at(varA) < 0.0 ) {
    entropyMap[varA] = entropy( dataMat.at(varA) ); // computes entropy of varA only if not already done
  }        
  if ( entropyMap.at(varB) < 0.0 ) { // computes entropy of varB only if not already done
    entropyMap[varB] = entropy( dataMat.at(varB)) ; // since this operation could be expensive
  }
    
  double minEntropyAB = std::min( entropyMap[varA], entropyMap[varB]); // takes the min
  if (minEntropyAB != 0) {
    double mutEntropyAB = mutEntropy( dataMat.at(varA), dataMat.at(varB) );
    double mutualInfoAB = entropyMap[varA] + entropyMap[varB] - mutEntropyAB; // classic formula
    double normalizedMutInfo = mutualInfoAB / minEntropyAB;        
    result = normalizedMutInfo;
  }
  return result;
}

} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace fltm
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_SIMILARITY_HPP
