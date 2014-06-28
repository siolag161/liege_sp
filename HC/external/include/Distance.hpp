/****************************************************************************************
 * File: Distance.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 12/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_DISTANCE_HPP
#define CLUSTERING_DISTANCE_HPP

#include <stdlib.h> /* abs */
#include "MutInfo.hpp"
#include <map>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>

namespace clustering
{

template<class DataMatrix>
struct MutInfoDistance {
  
  const double MAX_DISTANCE = 1.0, MIN_DISTANCE = 0.0;
  
  double operator()( const size_t varA, const size_t varB );

  MutInfoDistance( DataMatrix& dm, std::vector<int>& pos, unsigned maxPos, double thres );

  DataMatrix& dataMat;
  std::vector<int>& positions;
  unsigned maxPosition;
  double m_thres;
  
  std::vector<double> entropyMap;
  std::map< size_t, double > distCache;

  double m_median;
};

  
template<class DM>
double mutualInformationDistance( std::vector<double>& entropyMap,
                                  std::map<size_t,double>& distCache,
                                  const DM& dataMat,
                                  const size_t varA,
                                  const size_t varB );
template<class DM>
MutInfoDistance<DM>::MutInfoDistance( DM& dm, std::vector<int>& pos, unsigned maxPos, double thres ):
    dataMat(dm), positions(pos),
    maxPosition(maxPos), entropyMap(std::vector<double> (pos.size(), -1.0)),
    m_thres(thres)
{
  size_t nbrVars = positions.size();
  if ( thres > 0 ) { // binary-->median, have to pre-compute all    
    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::median(boost::accumulators::with_p_square_quantile) > > acc;
    
    #pragma omp parallel for schedule(dynamic) 
    for ( size_t varA = 0; varA < nbrVars; ++varA) {
      for ( size_t varB = varA+1; varB < nbrVars; ++varB ) {
        if ( abs( positions[varA] - positions[varB] ) > maxPos ) continue; 
        double result = mutualInformationDistance( entropyMap, distCache, dataMat, varA, varB );
        #pragma omp critical
        acc(result);
      }
    }
    #pragma omp critical
    m_thres = boost::accumulators::median(acc);
  }
}

template<class DM>
double MutInfoDistance<DM>::operator()( size_t varA, size_t varB ) {
  if ( varA == varB ) return MIN_DISTANCE;
  if ( varA > varB ) return (*this)( varB, varA );
  if ( abs( positions[varA] - positions[varB] ) >= maxPosition ) return MAX_DISTANCE;

  size_t nbrVars = dataMat.size();
  size_t key = 2*nbrVars*varA + varB;   
  double result = MAX_DISTANCE, rs = MAX_DISTANCE;
  std::map<size_t, double>::iterator iter;
      
  #pragma omp critical
  iter = distCache.find(key);

  if ( iter == distCache.end() ) {
    rs =  mutualInformationDistance( entropyMap, distCache, dataMat, varA, varB );
  }

  #pragma omp critical
  distCache[key] = rs;
    
  if ( m_thres > 0 ) { // binary-->median, have to pre-compute all
    result = ( rs > m_thres ) ? MAX_DISTANCE : MIN_DISTANCE;  
  } else { // continue
    result = rs;   
  }

  return result;
}

  
template<class DM>
double mutualInformationDistance( std::vector<double>& entropyMap,
                                  std::map<size_t,double>& distCache,
                                  const DM& dataMat,
                                  const size_t varA,
                                  const size_t varB )
{  
  double result = 1.0;
  size_t nVars = entropyMap.size();
  size_t key = 2*nVars*varA + varB;
  Entropy<EMP> entropy; // empirical entropy (estimates the probability by frequency)
  JointEntropy<EMP> mutEntropy; // empirial mutual entropy;

  double enA, enB;
  #pragma omp critical
  {  
    if ( entropyMap.at(varA) < 0.0 ) {
      entropyMap[varA] = entropy( dataMat.at(varA) ); // computes entropy of varA only if not already done
    }
    enA = entropyMap[varA];
  }

  #pragma omp critical
  { 
    if ( entropyMap.at(varB) < 0.0 ) { // computes entropy of varB only if not already done
      entropyMap[varB] = entropy( dataMat.at(varB) ) ; // since this operation could be expensive
    }
    enB = entropyMap[varB];
  }

  double minEntropyAB = std::min(enA, enB); // takes the min
  if (minEntropyAB != 0) {
    double mutEntropyAB = mutEntropy( dataMat.at(varA), dataMat.at(varB) );
    double mutualInfoAB = enA + enB - mutEntropyAB; // classic formula
    double normalizedMutInfo = mutualInfoAB / minEntropyAB;        
    result = 1.0 - normalizedMutInfo;
  }
  
  return result;
}



} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_DISTANCE_HPP
