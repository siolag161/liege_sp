/****************************************************************************************
 * File: association_test.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 25/06/2014

 ***************************************************************************************/
#ifndef STATS_ASSOCIATION_TEST_HPP
#define STATS_ASSOCIATION_TEST_HPP

#include "statistics.hpp"

namespace stats
{

struct StatTest {
  typedef std::vector<int> PhenoVec;
  typedef std::vector<int> GenoVec;
  typedef std::vector< GenoVec > GenoMat;
  ~StatTest(){}
  StatTest(){}
  // StatisticTest( const std::string n ): name(n) {}
  virtual double execute( const GenoVec& geno,
                             const PhenoVec& pheno,
                             const unsigned cardGenotype,
                            const unsigned cardPhenotype ) const = 0;
  
  virtual double operator()( const GenoVec& geno,
                             const PhenoVec& pheno,
                             const unsigned cardGenotype,
                             const unsigned cardPhenotype ) const { return execute(geno, pheno, cardGenotype, cardPhenotype); }
  std::string name;
};

struct ChiSq: public StatTest  {
  ChiSq() { name = "ChiSq"; }
  
  virtual double execute( const GenoVec& geno,
                             const PhenoVec& pheno,
                             const unsigned cardGenotype,
                             const unsigned cardPhenotype ) const { return chisq(geno, pheno, cardGenotype, cardPhenotype); }
  
  // virtual double operator()( const GenoVec& geno,
  //                            const PhenoVec& pheno,
  //                            const unsigned cardGenotype,
  //                            const unsigned cardPhenotype ) { execute(geno, pheno, cardGenotype, cardPhenotype); }

  
  stats::StatisticTest<stats::CHISQ> chisq;
};

struct ChiSqCor: public StatTest  {
  
  ChiSqCor() { name = "ChiSq-Yates"; }  
  
  virtual double execute( const GenoVec& geno,
                             const PhenoVec& pheno,
                             const unsigned cardGenotype,
                             const unsigned cardPhenotype ) const { return chisq(geno, pheno, cardGenotype, cardPhenotype); }
  
  // virtual double operator()( const GenoVec& geno,
  //                            const PhenoVec& pheno,
  //                            const unsigned cardGenotype,
  //                            const unsigned cardPhenotype ) { execute(geno, pheno, cardGenotype, cardPhenotype); }
  
  stats::StatisticTest<stats::CHISQ_YATES> chisq;
};

struct Fisher: public StatTest  {
  Fisher() { name = "Fisher"; }

  virtual double execute( const GenoVec& geno,
                          const PhenoVec& pheno,
                          const unsigned cardGenotype,
                          const unsigned cardPhenotype ) const { return fisher(geno, pheno, cardGenotype, cardPhenotype); }
  
  // virtual double operator()( const GenoVec& geno,
  //                    const PhenoVec& pheno,
  //                    const unsigned cardGenotype,
  //                    const unsigned cardPhenotype ) {
  //   return fisher( geno, pheno, cardGenotype, cardPhenotype );
  // }
  stats::StatisticTest<stats::FISHER> fisher;
};

struct G: public StatTest  {
  G() { name = "G2"; }
  virtual double execute( const GenoVec& geno,
                          const PhenoVec& pheno,
                          const unsigned cardGenotype,
                          const unsigned cardPhenotype ) const { return g2(geno, pheno, cardGenotype, cardPhenotype); }
  
  stats::StatisticTest<stats::G2> g2;
};

struct G2Cor: public StatTest  {
  
  G2Cor() { name = "G2-Yates"; }
  
  virtual double execute( const GenoVec& geno,
                     const PhenoVec& pheno,
                     const unsigned cardGenotype,
                     const unsigned cardPhenotype ) {
    return g2( geno, pheno, cardGenotype, cardPhenotype );
  }
  stats::StatisticTest<stats::G2_YATES> g2;
};

} // namespace statsends here. stats

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace stats
{


} // namespace statsends here. stats

/****************************************************************************************/
#endif // STATS_ASSOCIATION_TEST_HPP
