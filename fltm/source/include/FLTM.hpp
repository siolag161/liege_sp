/****************************************************************************************
 * File: FLTM.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 09/07/2014

 ***************************************************************************************/
#ifndef FLTM_FLTM_HPP
#define FLTM_FLTM_HPP  

#include <vector>
#include "Clustering.hpp"
#include "EM.hpp"
#include "DataMatrix.hpp"
#include "Graph.hpp"
#include "EMCardFunc.hpp"
namespace fltm
{
 
typedef std::vector< std::vector<int> > Matrix;
typedef plSymbol Variable;
typedef int Index;  
typedef std::string Label;
typedef int Position;
typedef std::map<Label, Index> Label2GraphIndex; // for a variable, label to global index
typedef std::map<Label, Position> Label2Pos;
typedef std::vector<Index> Matrix2GraphIndex;

struct FLTM_Result {
  FLTM_Result(): nbrLatentVariables(0) {}  
  void addNode(const Node& node) {
    while( (node.level) >= level2LatentVars.size()) {
      level2LatentVars.push_back(std::vector<vertex_t>());
    }
    level2LatentVars[node.level].push_back(node.index);
    ++nbrLatentVariables;
  }
  int nbrLatentVariables;
  std::vector< std::vector<vertex_t> > level2LatentVars; 
  Matrix imputedData;
};


struct FLTM_Data {  
  std::vector<Label> labels;
  std::vector<Position> positions;
  std::vector<unsigned> ids;
  std::vector< std::vector<int> > matrix;
  Graph graph;
  int cardinality;
};

struct FLTM_Options {
  int cardinality;
  int nbrSteps;
  // int maxDist;
  // double simiThres;
  double emThres;
  double infoThres;
};

struct FLTM {
  FLTM( AlgoClust& clustA, CardFunc& cardF, EMFunc& emF): clustAlgo(clustA), cardFunc(cardF), emFunc(emF) { }  

  void operator()( FLTM_Result& result, FLTM_Data& data, FLTM_Options& opt );  

 protected:
  void setupVariables( FLTM_Data& input,
                       FLTM_Result& result,
                       Matrix2GraphIndex& mat2GraphIndex,
                       Label2GraphIndex& label2GraphIndex,
                       const size_t& nbrVars,
                       const size_t& cardinality);

  std::vector<Position> getLocalPositions( const Graph& graph, Matrix2GraphIndex& mat2GraphIndex  );

  bool containsOnlySingletons( int& singleton,
                               const Clustering& clustering );
  Variable createLatentVar( const int lab, const int cardinality );
  ///////////////////////////////////////
  void prepareEM( Matrix& emMat,
                  Variables& vars,
                  const FLTM_Data& input,
                  const std::vector<int>& cluster,
                  const std::vector<int> local2Global );

  bool goodLatentVariable( std::vector<int>& latentCol,
                           Matrix& transposedMat,
                           std::vector<int>& cluster,
                           double goodLatentVarThres );

  vertex_t addLatentNode( Graph& graph,
                          const Variable& latentVar,
                          ResultEM& resultEM,
                          Label2GraphIndex& label2GraphIndex );

  void updateNextRow( Matrix& nextRowMatrix, Matrix2GraphIndex& nextRoundMat2GraphIndex,
                      const Matrix2GraphIndex& mat2GraphIndex, const Matrix& matrix, const std::vector<int>& cluster );
 protected:
  AlgoClust& clustAlgo;
  CardFunc& cardFunc;
  EMFunc& emFunc;
};

} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_FLTM_HPP
