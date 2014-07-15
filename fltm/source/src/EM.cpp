
#define NOT_MISSING 1
#define MISSING 0
#define DEFAULT_VALUE = -1

#include "EM.hpp"
#include "DataMatrix.hpp"
namespace fltm {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EMFunc::operator()( ResultEM& result,
                         const Variable& latentVar,
                         const Variables& variables,
                         const Matrix& dataTable,                   
                         const double threshold ) {
  std::vector< std::vector<bool> > defTable = createDefinitionTable( dataTable ); 
  run( result, latentVar, variables, dataTable, defTable, threshold );
}
/////////////////////////////////////////////////////////////////////////////////////
EMFunc::LearnObjectPtrs EMFunc::createLearnObjects( const Variable& latentVar,
                                                    const Variables& variables )
{
  LearnObjectPtrs learnObjects;

  plLearnObject* learnLatent = new plLearnHistogram(latentVar);
  learnObjects.push_back(learnLatent);
  
  for (size_t var = 0; var < variables.size(); ++var) {
    learnObjects.push_back(new plCndLearnObject <plLearnHistogram> (variables[var], latentVar));
  }

  return learnObjects;
}

////////////////////////////////////////////////////////////////////
plComputableObjectList EMFunc::createComputableObjects( const Variable& latentVar,
                                                        const Variables& variables )
{
  plComputableObjectList dataCndTable; 

  const plProbTable latentProbInit(latentVar, true);
  dataCndTable *= latentProbInit;
  
  for (size_t x = 0; x < variables.size(); ++x) {
    plDistributionTable distTab_Xi_Z(variables[x], latentVar); 
    for (size_t h = 0; h < latentVar.cardinality(); ++h) {
      distTab_Xi_Z.push( plProbTable(variables[x], true), (int)h );
    }
    dataCndTable *= distTab_Xi_Z; // adds the conditional table to result     

  }  
  
  return dataCndTable;
}
////////////////////////////////////////////////////////////////////

double EMFunc::logLikelihood( EMLearner& learner, plMatrixDataDescriptor<int>& dataDesc )
{
  double result = 0.0;
  try {
    result = learner.get_last_computed_loglikelihood();
  } catch (plError& e) {
    result = (double)learner.compute_loglikelihood(dataDesc);
  }
  return result;
}

/////////////////////////////////////////////////////////////////////
EMFunc::EMLearner EMFunc::getBestModel( CandidateModels& learners,
                        plMatrixDataDescriptor<int>& dataDesc )
{
  EMLearner bestModel = learners[0];  
  for (size_t i = 0; i < learners.size(); ++i) {
    if ( logLikelihood( learners[i], dataDesc) > logLikelihood(bestModel, dataDesc)) {
      bestModel = learners[i];
    }
  }  
  return bestModel;
}

std::vector< std::vector<bool> > EMFunc::createDefinitionTable( const Matrix& dataMat ) {
    
  const Size nbrInds = nrows(dataMat);
  const Size nbrVars = ncols(dataMat);
  
  std::vector< std::vector<bool> > defTable;
  defTable.reserve( nbrInds );
  for (size_t ind = 0; ind < nbrInds; ++ind) {
    defTable.push_back(std::vector<bool>(nbrVars, NOT_MISSING));
    defTable[ind][0] = false; // the first columns = FALSE
  }

  return defTable;
} 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
void MultiEM::run( ResultEM& result,
                   const Variable& latentVar,
                   const Variables& variables,
                   const Matrix& dataTable,
                   const std::vector< std::vector<bool> > & defTable,
                   const double threshold ) {

  LearnObjectPtrs learnObjs = createLearnObjects(latentVar, variables);
  plMatrixDataDescriptor<int> dataDesc(latentVar ^ variables, &dataTable, &defTable);

  CandidateModels candidateModels;  // vector to store different learners

  for (size_t run = 0; run < nbrRestarts; ++run) {
    // std::cout << "run: " << run << std::endl;
    plComputableObjectList compObjList = createComputableObjects(latentVar, variables); // randomly initialized
    EMLearner learner(compObjList, learnObjs);
    learner.run(dataDesc, threshold); // executes the EM learning process    
    candidateModels.push_back(learner); // puts the learner into the collection
  }

  plEMLearner bestModel = getBestModel( candidateModels, dataDesc);
  impute( result, latentVar, dataTable, bestModel, dataDesc );
  result.jointDistribution = bestModel.get_joint_distribution();

}

void MultiEM::impute( ResultEM& result,                 
                      const plSymbol& latentVar,
                      const Matrix& dataTable,
                      EMLearner& bestModel,
                      plMatrixDataDescriptor<int>& dataDesc )
{
  std::vector<plValues> missingMostProbVals;
  std::vector<std::vector<plProbValue> > missingProbTable;
  bestModel.compute_missing_values_infos(dataDesc, missingMostProbVals, missingProbTable);

  const size_t nbrInds = missingMostProbVals.size(); 
  result.imputedData = std::vector<int> ( missingMostProbVals.size(), 0);

  if (imputMode == ARGMAX) { // argmax imputation
    for (size_t ind = 0; ind < nbrInds; ++ind) {
      result.imputedData[ind] = missingMostProbVals[ind][0];
    }
  } else { // random drawing from the distribution
    for (size_t ind = 0; ind < nbrInds; ++ind) {
      plProbTable probTab(latentVar, missingProbTable[ind], true);
      result.imputedData[ind] = probTab.draw()[0];
    } 
  }
}


}
