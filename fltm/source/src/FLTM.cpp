#include "FLTM.hpp"
#include "Graph.hpp"
#include "DataMatrix.hpp"
#include "LatentVarCriteria.hpp"

namespace fltm {

/**
 *
 */
void FLTM::operator()( FLTM_Result& result, FLTM_Data& input, FLTM_Options& opt ) {
  Matrix2GraphIndex mat2GraphIndex; Label2GraphIndex label2GraphIndex;
  const unsigned nbrObsVars = input.matrix.size();
  std::cout << "beginning FLTM..." << std::endl;
  setupVariables( input, result, mat2GraphIndex, label2GraphIndex, nbrObsVars, opt.cardinality );
  for ( int step = 0; step < opt.nbrSteps; ++step) {
    printf( "\nbeging step: %d of %d...\n", step, opt.nbrSteps );
    Matrix nextRoundMatrix; Matrix2GraphIndex nextRoundMat2GraphIndex;  
    clustAlgo.invalidCache();   
    printf( "start clustering of data of: %zu variables...\n", input.matrix.size() );
    auto clustering = clustAlgo().to_clustering();
    printf( "end clustering, obtained: %zu clusters...\n", clustering.size() );

    int singletonCount = 0;
    if ( containsOnlySingletons( singletonCount, clustering) ) {
      std::cout << "stops due to only singleton. " << std::endl;
      return;
    }
     
    int goodClusterCount = 0; nextRoundMatrix.reserve(clustering.size());
    int num_Clust = 0;
    for ( auto& clt: clustering ) {      
      if ( clt.size() > 1 ) {
        Matrix emMat; ResultEM resultEM;
        Variables emVars; 
        Variable latentVar = createLatentVar( boost::num_vertices(input.graph), cardFunc(clt) );        
        prepareEM( emMat, emVars, input, clt, mat2GraphIndex );        
        std::cout << "Cluster: " << num_Clust++ << " over " << clustering.size() - singletonCount << " of step: " << step << std::endl;
        std::cout << "executing EM on cluster of size: " << clt.size() << std::endl;
        emFunc( resultEM, latentVar, emVars, emMat, opt.emThres );
        std::cout << "done EM" << std::endl << std::endl;
        if ( goodLatentVariable( resultEM.imputedData, input.matrix, clt, opt.infoThres ) ) { // @todo: minASMI --> thresLatent                
          vertex_t vertex = addLatentNode( input.graph, latentVar, resultEM, label2GraphIndex ); // vertex is the index of the newly added node
          goodClusterCount++;
          nextRoundMatrix.push_back(resultEM.imputedData);
          nextRoundMat2GraphIndex.push_back( vertex );           
          result.addNode( input.graph[vertex] );
          result.imputedData.push_back( resultEM.imputedData );
        } else {
          updateNextRow( nextRoundMatrix, nextRoundMat2GraphIndex, mat2GraphIndex, input.matrix, clt );
        }
      } else {
        updateNextRow( nextRoundMatrix, nextRoundMat2GraphIndex, mat2GraphIndex, input.matrix, clt );
      }
    }
    
    if ( goodClusterCount == 0 ) {
       std::cout << "stops due to 0 good cluster. " << std::endl;
       return;
    }
    mat2GraphIndex = nextRoundMat2GraphIndex;
    input.matrix = nextRoundMatrix;
    input.positions = getLocalPositions( input.graph, mat2GraphIndex );
  }
}


///////////////////////////////////////////
void FLTM::setupVariables( FLTM_Data& input,
                           FLTM_Result& result,
                           Matrix2GraphIndex& mat2GraphIndex,
                           Label2GraphIndex& label2GraphIndex,
                           const size_t& nbrVars,
                           const size_t& cardinality) {
  for (size_t i = 0; i < input.labels.size(); ++i) {
    vertex_t vertex = createVertex( input.graph, cardinality, true, input.labels[i], input.positions[i], 0 );
    result.addNode( input.graph[vertex] );
    label2GraphIndex[ input.labels[i] ] = vertex;
    mat2GraphIndex.push_back(vertex) ;
  }
}
///////////////////////////////////////////////
std::vector<Position> FLTM::getLocalPositions( const Graph& graph, Matrix2GraphIndex& mat2GraphIndex ) {
  std::vector<Position> localPositions( mat2GraphIndex.size(), 0 );
  for ( size_t i = 0; i < mat2GraphIndex.size(); ++i ) {
    localPositions[i] = graph[ mat2GraphIndex.at(i) ].position;
  }
  return localPositions;
}

///////////////////////////////////////
bool FLTM::containsOnlySingletons( int& singletonCount, const Clustering& clustering ) {
  // unsigned singletonCount = 0;
  for ( auto clt: clustering ) {      
    if ( clt.size() <= 1) {
      ++singletonCount;
    }
  }
  printf("there are: %d singleton clusters\n", singletonCount);
  return (clustering.size() == singletonCount);
}

///////////////////////////////////////
Variable FLTM::createLatentVar( const int lab, const int cardinality ) {
  return Variable( boost::lexical_cast<std::string>(lab), plIntegerType(0, cardinality - 1) );
}

///////////////////////////////////////
void FLTM::prepareEM( Matrix& emMat,
                      Variables& vars,
                      const FLTM_Data& input,
                      const std::vector<int>& cluster,
                      const std::vector<int> mat2GraphIndex )  {
  
  Matrix *tEMMat = new Matrix();
  tEMMat->reserve(cluster.size());
  vars.clear();  
  tEMMat->push_back( std::vector<int>( ncols(input.matrix), -1) );  
  for ( auto& it: cluster ) {    
    vars ^= input.graph[mat2GraphIndex.at(it)].variable;
    tEMMat->push_back( input.matrix.at(it) );      
  }
  emMat = Transpose(*tEMMat);
  delete(tEMMat);
}

bool FLTM::goodLatentVariable( std::vector<int>& latentCol,
                               Matrix& mat,
                               std::vector<int>& cluster,
                               double goodLatentVarThres )
{
  AverageMutInfo averageMutInfo;
  double measuredQuantity = averageMutInfo( latentCol, mat, cluster );
  return ( measuredQuantity >= goodLatentVarThres );
}


vertex_t FLTM::addLatentNode( Graph& graph,
                              const Variable& latentVar,
                              ResultEM& resultEM,
                              Label2GraphIndex& label2GraphIndex ) {
  const vertex_t vertex = createVertex( graph,
                                        latentVar.cardinality(),
                                        false, // isLeaf
                                        latentVar.name() ); // position
  graph[vertex].setupProperties(&graph, resultEM.jointDistribution, label2GraphIndex);
  label2GraphIndex[latentVar.name()] = vertex;
  return vertex;
}

void FLTM::updateNextRow( Matrix& nextRoundMatrix, Matrix2GraphIndex& nextRoundMat2GraphIndex,
                          const Matrix2GraphIndex& mat2GraphIndex, const Matrix& matrix, const std::vector<int>& cluster ) {
  for ( auto& cit: cluster ) {
    nextRoundMat2GraphIndex.push_back( mat2GraphIndex[cit] );
    nextRoundMatrix.push_back( matrix[cit] );     
  }
}

}
