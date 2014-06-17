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

namespace clustering
{


struct EuclideanDistance {
  
  typedef std::vector< std::vector<double> > DataMatrix;
  double operator()( const uint32_t varA, const uint32_t varB ) const {
    double rs = 0;
    size_t len = dataMat[varA].size();
    for ( size_t i = 0; i < len; ++i ) {
      rs += (dataMat[varA][i] - dataMat[varB][i]) * (dataMat[varA][i] - dataMat[varB][i]);
    }
    
    return std::sqrt(rs);
  }

  EuclideanDistance( const DataMatrix& dm ): dataMat(dm) {
    
  }

  const DataMatrix& dataMat;
};


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


} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/


#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>

namespace clustering
{

template<class DM>
double mutualInformationDistance( std::vector<double>& entropyMap,
                                 std::map<size_t,double>& distCache,
                                 const DM& dataMat,
                                 const size_t varA,
                                 const size_t varB ) {
  double result = 1.0;
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
    result = 1.0 - normalizedMutInfo;
  }
  // distCache[key] = result;
  return result;
}

template<class DM>
MutInfoDistance<DM>::MutInfoDistance( DM& dm, std::vector<int>& pos, unsigned maxPos, double thres ):
    dataMat(dm), positions(pos),
    maxPosition(maxPos), entropyMap(std::vector<double> (pos.size(), -1.0)),
    m_thres(thres)
{
  size_t nbrVars = positions.size();
  if ( thres > 0 ) { // binary-->median, have to pre-compute all    
    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::median(boost::accumulators::with_p_square_quantile) > > acc;
    for ( size_t varA = 0; varA < nbrVars; ++varA) {
      for ( size_t varB = varA+1; varB < nbrVars; ++varB ) {
        if ( abs( positions[varA] - positions[varB] ) > maxPos ) continue; //result =  MAX_DISTANCE; // 1.0 = max distance possible      
        double result =  mutualInformationDistance( entropyMap, distCache, dataMat, varA, varB );
        acc(result);
      }
    }
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
  double result = MAX_DISTANCE;

  std::map<size_t, double>::iterator iter = distCache.find(key);  
  if ( iter == distCache.end() ) {
    result =  mutualInformationDistance( entropyMap, distCache, dataMat, varA, varB );
    distCache[key] = result;
  }
    
  if ( m_thres > 0 ) { // binary-->median, have to pre-compute all

    result = ( distCache.at(key) > m_thres ) ? MAX_DISTANCE : MIN_DISTANCE;  
  } else { // continue
   result =  distCache.at(key);   

  }

  return result;
}

} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_DISTANCE_HPP
