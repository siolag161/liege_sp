/****************************************************************************************
 * File: EM.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 11/07/2014

 ***************************************************************************************/
#ifndef FLTM_EM_HPP
#define FLTM_EM_HPP  

#include <boost/filesystem.hpp> // to obtain the program's name

#include <boost/program_options.hpp>
#include <pl.h>
#include <vector>

namespace fltm
{

typedef plVariablesConjunction Variables;
typedef plComputableObjectList DistTabList; 
typedef plComputableObject DistTab;
typedef plSymbol Variable;
typedef plJointDistribution JointDistribution;
// /** This structure represents a set of attributes that should be returned by 
//  *  any EM algorithm dedicated to FLTM construction.
//  *  Attributes:
//  *  - varConj Of type proPT variable conjnction, containing observed variables first
//  *       and the latent variable at the end
//  *  - jointDistribution of type proBT jointDistribution, containing parameters of model
//  *       to be learnt by EM (i.e. conditional distribution of observed given
//  the latent variable and the distribution of the latent variable itself.
// */
struct ResultEM
{
  ResultEM() { } 
  JointDistribution jointDistribution;
  std::vector<int> imputedData; 
};

struct EMFunc {
  enum ImputType { ARGMAX = 0, DRAW }; // imputation type, an algorithm
  typedef plVariablesConjunction Variables;
  typedef plSymbol Variable ;
  typedef size_t Cardinality;
  typedef size_t Size; 
  typedef plEMLearner EMLearner;
  typedef std::vector<plLearnObject*> LearnObjectPtrs;
  typedef std::vector<EMLearner> CandidateModels;
  
  typedef std::vector< std::vector<int> > Matrix;  
  virtual void operator()( ResultEM& result,
                           const Variable& latentVar,
                           const Variables& variables,
                           const Matrix& dataTable,                   
                           const double threshold );

  void impute( ResultEM& result,                 
               const plSymbol& latentVar,
               const Matrix& dataTable,
               EMLearner& bestModel,
               plMatrixDataDescriptor<int>& dataDesc );
  
  virtual void run( ResultEM& result,
                    const Variable& latentVar,
                    const Variables& variables,
                    const Matrix& dataTable,
                    const std::vector< std::vector<bool> > & defTable,
                    const double threshold ) = 0;  

  virtual LearnObjectPtrs createLearnObjects( const Variable& latentVar, const Variables& variables );
  virtual plComputableObjectList createComputableObjects( const Variable& latentVar, const Variables& variables );
  virtual double logLikelihood( EMLearner& learner, plMatrixDataDescriptor<int>& dataDesc );
  virtual EMLearner getBestModel( CandidateModels& learners,
                                  plMatrixDataDescriptor<int>& dataDesc );
  virtual std::vector< std::vector<bool> > createDefinitionTable( const Matrix& dataMat );
  
};

struct MultiEM: public EMFunc {
  typedef std::vector< std::vector<int> > Matrix;  
  
 public:
  MultiEM( int nbrRts, int imMode ): nbrRestarts(nbrRts), imputMode(imMode) {}
  
  virtual void run( ResultEM& result,
                    const Variable& latentVar,
                    const Variables& variables,
                    const Matrix& dataTable,
                    const std::vector< std::vector<bool> > & defTable,
                    const double threshold );


  virtual void impute( ResultEM& result,                 
                       const plSymbol& latentVar,
                       const Matrix& dataTable,
                       EMLearner& bestModel,
                       plMatrixDataDescriptor<int>& dataDesc );

  int nbrRestarts;
  int imputMode; // methods for imputing missing values  (ARGMAX or DRAW)
  
};
  


} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_EM_HPP
