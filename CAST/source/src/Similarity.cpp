
#include "Similarity.hpp"

#include "MutInfo.hpp"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/median.hpp>

namespace fltm {

SimilarityCompute::SimilarityCompute( const Positions& pos ,
                                      const Matrix& mat,
                                      unsigned maxPos,                                      
                                      double simiThres ):
    positions(pos),
    dataMat(mat),
    maxPosition(maxPos),
    m_simiThres(simiThres),
    entropyMap(std::vector<double> (pos.size(), -1.0))
{
  size_t nbrVars = positions.size();
  if ( simiThres > 0 ) { // binary-->median, have to pre-compute all    
    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::median(boost::accumulators::with_p_square_quantile) > > acc;
    for ( size_t varA = 0; varA < nbrVars; ++varA) {
      for ( size_t varB = varA+1; varB < nbrVars; ++varB ) {
        if ( abs( positions[varA] - positions[varB] ) > maxPos ) continue; //result =  MAX_DISTANCE; // 1.0 = max distance possible      
        double result = mutualInformationSimilarity( entropyMap, simiCache, dataMat, varA, varB );
        acc(result);
      }
    }
    m_simiThres = boost::accumulators::median(acc);
  }
}


double SimilarityCompute::operator()( unsigned varA,
                                      unsigned varB )
{
  if ( varA == varB ) return MAX_SIMILARITY;
  if ( varA > varB ) return (*this)( varB, varA );
  if ( abs( positions[varA] - positions[varB] ) >= maxPosition ) return MIN_SIMILARITY;

  size_t nbrVars = dataMat.size();
  size_t key = 2*nbrVars*varA + varB;   
  double result = MIN_SIMILARITY;

  std::map<size_t, double>::iterator iter = simiCache.find(key);  
  if ( iter == simiCache.end() ) {
    result = mutualInformationSimilarity( entropyMap, simiCache, dataMat, varA, varB );
    simiCache[key] = result;
  }
    
  if ( m_simiThres > 0 ) { // binary-->median, have to pre-compute all
    result = ( simiCache.at(key) > m_simiThres ) ? MAX_SIMILARITY : MIN_SIMILARITY;  
  } else { // continue
    result =  simiCache.at(key);   

  }

  return result;
}

}
