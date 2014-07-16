/****************************************************************************************
 * File: GraphIO.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 03/04/2014

 ***************************************************************************************/
#ifndef FLTM_GRAPH_GRAPHIO_HPP
#define FLTM_GRAPH_GRAPHIO_HPP

#include "Graph.hpp"
#include "CSVParser.hpp"
#include <map>
#include <string>
#include <pl.h>
#include <iomanip>
namespace fltm
{

static const char SEPARATOR = ',';

enum FULL { FULL_ID = 0, FULL_LABEL, FULL_LATENT, FULL_PARENT, FULL_LEVEL, FULL_POSITION, FULL_CARDINALITY };
enum LAB_POS { LP_ID = 0, LP_LABEL, LP_POSITION };

enum SINGLE { SINGLE_ID = 0, SINGLE_LATENT, SINGLE_PARENT, SINGLE_LEVEL, SINGLE_CARDINALITY };
enum TULIP_VERTICES { TULIP_ID = 0, TULIP_LATENT, TULIP_LEVEL, TULIP_CARDINALITY };
enum TULIP_EDGES { TULIP_PARENT_ID = 1 };
enum BN_VERTICES { BN_LATENT_ID = 0, NBR_CHILDREN };

static const std::string ID = "id"; static const std::string LABEL = "label";
static const std::string LATENT = "latent"; static const std::string PARENT = "parent";
static const std::string LEVEL = "level"; static const std::string POSITION = "position";
static const std::string CARDINALITY = "cardinality";
static const std::string PARENT_ID = "parent_id";

typedef std::map<int, std::pair<std::string, int> > LabPosMap;

/*
 *  
 */
struct FLTMGraphReader {
  /** Loads the graph's idendity, label and position a file and returns a
   * std::map that maps the identity to a std::pair of label and postion
   */
  inline LabPosMap readLabPos( const std::string labPosFileName ) const;  
};

/** 
 *
 */
struct SingleGraphLoad: public FLTMGraphReader {       
  inline void operator()( Graph& graph, const std::string labPosFileName, const std::string singleFileName ) const;
  Graph operator()( const std::string labPosFileName, const std::string singleFileName ) const {
    Graph graph; (*this)(graph, labPosFileName, singleFileName); return graph;
  }
};

/** Loads a graph which represents the topology of the FLTM Model from two separate files: one containing information regarding edges
 *  and the other concerning the vertices
 */
struct TulipGraphLoad: public FLTMGraphReader {  
  inline void operator()( Graph& graph, const std::string labPosFileName, const std::string vertexFileName, const std::string edgeFileName ) const;
  Graph operator()(  const std::string labPosFileName, const std::string vertexFileName, const std::string edgeFileName) const {
    Graph graph; (*this)( graph, labPosFileName, vertexFileName, edgeFileName ); return graph;
  }
};


/** Loads the Bayesian network corresponding to the FLTM model from two files:
 *  - vertexFileName: information regarding vertices of the following format: ID, LATENT, PARENT, LEVEL, CARDINALITY
 *  - distributionFileName:
 *     == example: Z has 2 children X1 and X2; card(Z) = 3; card(X1) = card(X2) = 2;
 *         Z_id 2
 *         p(Z=0) p(z=1) p(z=2)
 *         X1_id
 *         P(x1=0 | Z=0) p(X1=1 | Z=0)
 *         p(x1 | Z=1)
 *         p(x1 | Z=2)
 *         X2_id
 *         P(x2=0 | Z=0) p(X2=1 | Z=0)
 *         p(x2 | Z=1)
 *         p(x2 | Z=2)
 *
 *         Y_id 7
 *         etc.
 */
struct BayesGraphLoad: public FLTMGraphReader {  
  inline void operator()( Graph& graph, const std::string labPosFileName, const std::string vertexFileName, const std::string distributionFileName ) const;
  Graph operator()( const std::string labPosFileName, const std::string vertexFileName, const std::string distributionFileName ) const  {
    Graph graph;  (*this)( graph, labPosFileName, vertexFileName, distributionFileName );  return graph;
  }
};
///////////////////////////////////////////////////////////////

/**
 *
 */
struct FLTMGraphWriter {  
  inline void writeLabPos( const Graph& graph, const std::string labPosFileName );  
};


/** Save graph which represents the topology of the FLTM Model into two files: one containing the label-position
 *  information and the other containing: id, latent, id of the paren, level and the cardiality. 
 */
struct SingleGraphSave: public FLTMGraphReader {  
  inline void operator()( Graph& graph, const std::string singleFileName ) const;
};

/** Saves an FLTM Graph into two files:
  *    - one containing information regarding verices with the following format: ID, LATENT, LEVEL
  *    - one  concerning the edges: ID, ID_PARENT
  */
struct TulipGraphSave {  
  inline void operator()( const Graph& graph, const std::string vertexFileName, const std::string edgeFileName) const;
};


/** Saves the Bayesian network corresponding to the FLTM model to two files of the following format:
 *  - vertexOutputFilename: ID, LABEL, LATENT, PARENT, LEVEL, POSITION, CARDINALITY
 *  - distributionFileName: see above
 */
struct BayesGraphSave {  
  inline void operator()( const Graph& graph, const std::string vertexFileName, const std::string distributionFileName ) const;
};



} // namespace fltm_graph ends here. fltm_graph

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace fltm
{

LabPosMap FLTMGraphReader::readLabPos( const std::string labPosFileName ) const {
  LabPosMap lpMap;
  std::ifstream labPosFile(labPosFileName.c_str());    
  utility::CSVIterator<std::string> labPosLine(labPosFile); ++labPosLine;
  for( ; labPosLine != utility::CSVIterator<std::string>(); ++labPosLine ) {
    int id = boost::lexical_cast<int>( (*labPosLine)[LP_ID] );
    std::string label = (*labPosLine)[LP_LABEL] ;
    int position = boost::lexical_cast<int>( (*labPosLine)[LP_POSITION] );
    lpMap[id] = std::pair<std::string, int>(label, position);
  }
  return lpMap;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SingleGraphLoad::operator()( Graph& graph, const std::string labPosFileName, const std::string singleFileName ) const {
  
  std::ifstream singleFile(singleFileName.c_str());  
  if (!singleFile) {
    return ;
  }

  LabPosMap labPosMap = readLabPos(labPosFileName);
    
  typedef std::map<int,int> EdgeMap; 
  EdgeMap edgeMap;
  utility::CSVIterator<std::string> singleLine(singleFile); ++singleLine;
  for( ; singleLine != utility::CSVIterator<std::string>(); ++singleLine ) {
    int id = boost::lexical_cast<int>( (*singleLine)[SINGLE_ID] );   
    bool isLeaf = !(boost::lexical_cast<bool>( (*singleLine)[SINGLE_LATENT])) ;
    int parentId = boost::lexical_cast<int>( (*singleLine)[SINGLE_PARENT] );
    int level =  boost::lexical_cast<int>( (*singleLine)[SINGLE_LEVEL] );  
    int cardinality = boost::lexical_cast<int>( (*singleLine)[SINGLE_CARDINALITY] );      
    edgeMap[id] = parentId;
    
    std::string label = labPosMap[id].first;
    int position = labPosMap[id].second;

    id = createVertex( graph, cardinality, isLeaf, label, position, level );

  }
  
  for ( EdgeMap::const_iterator it = edgeMap.begin(); it != edgeMap.end(); ++it ) {
    if (it->second != -1) { // if the node is not root
      boost::add_edge( it->second, it->first, graph );
    }
  }

  singleFile.close();
}

///////////////////////////////////////////////////////////////////////////////////////
void TulipGraphLoad::operator()( Graph& graph,
                                 const std::string labPosFileName,
                                 const std::string vertexFileName,
                                 const std::string edgeFileName ) const {
  
  LabPosMap labPosMap = readLabPos(labPosFileName);
  
  std::ifstream vertexFile(vertexFileName.c_str());  
  std::ifstream edgeFile(edgeFileName.c_str());
  if (!edgeFile || !vertexFile) {
    return ;
  }

  utility::CSVIterator<std::string> vertexLine(vertexFile); ++vertexLine;
  for( ; vertexLine != utility::CSVIterator<std::string>(); ++vertexLine ) {         
    int id = boost::lexical_cast<int>( (*vertexLine)[TULIP_ID] );   
    bool isLeaf = !(boost::lexical_cast<bool>( (*vertexLine)[TULIP_LATENT])) ;
    int level =  boost::lexical_cast<int>( (*vertexLine)[TULIP_LEVEL] );  
    int cardinality = boost::lexical_cast<int>( (*vertexLine)[TULIP_CARDINALITY] );      
    
    std::string label = labPosMap[id].first;
    int position = labPosMap[id].second;

    id = createVertex( graph, cardinality, isLeaf, label, position, level );    
  }
  
  
 utility::CSVIterator<std::string> edgeLine(edgeFile); ++edgeLine; // skips the header
  for(; edgeLine != utility::CSVIterator<std::string>(); ++edgeLine ) {    
     
    int sourceId = boost::lexical_cast<int>( (*edgeLine)[TULIP_PARENT_ID] );
    int targetId = boost::lexical_cast<int>( (*edgeLine)[TULIP_ID] );
    boost::add_edge( sourceId, targetId, graph );
  }
  edgeFile.close();
}

/**
 *
 */
void BayesGraphLoad::operator()( Graph& graph,
                                 const std::string labPosFileName,
                                 const std::string vertexFileName,
                                 const std::string distributionFileName ) const {  

  LabPosMap labPosMap = readLabPos(labPosFileName);

  std::ifstream vertexFile(vertexFileName.c_str()), distributionFile(distributionFileName.c_str());  
  utility::CSVIterator<std::string> vertexLine(vertexFile); ++vertexLine;
  std::cout << "begin loading vertices\n";
  for( ; vertexLine != utility::CSVIterator<std::string>(); ++vertexLine ) {
    int id = boost::lexical_cast<int>( (*vertexLine)[0]);
    bool isLeaf = !(boost::lexical_cast<bool>( (*vertexLine)[1]));
    int level =  boost::lexical_cast<int>( (*vertexLine)[2] );
    int cardinality = boost::lexical_cast<int>( (*vertexLine)[3]);
    std::string label = labPosMap[id].first;
    int position = labPosMap[id].second;

    id = createVertex( graph, cardinality, isLeaf, label, position, level );

  }
  std::cout << "end loading vertices\n\n";
  std::cout << "begin loading bayes\n\n";

  utility::CSVIterator<std::string> distributionLine(distributionFile); // iterator over lines
  while ( distributionLine != utility::CSVIterator<std::string>() ) {
    plVariablesConjunction variables; // holds child variables
    plComputableObjectList dataCndTable;  
    int latentId =  boost::lexical_cast<int>( (*distributionLine)[BN_LATENT_ID] );
    int nbrChildren = boost::lexical_cast<int>( (*distributionLine)[NBR_CHILDREN] );
    Node& latentNode = graph[ latentId ]; ++distributionLine;

    std::vector< plProbValue > probValuesZ;
    for ( int latentVal = 0; latentVal < latentNode.variable.cardinality(); ++latentVal) { // loads probability table for the latent var
      probValuesZ.push_back( boost::lexical_cast<plProbValue>( (*distributionLine)[latentVal] ) );
    }
    const plProbTable probTabZ(latentNode.variable, probValuesZ); ++distributionLine;    
    for ( int child = 0; child < nbrChildren; ++child ) {
      int childId = boost::lexical_cast<int>( (*distributionLine)[BN_LATENT_ID] ); ++distributionLine;
      Node& childNode = graph[ childId ]; variables ^= childNode.variable;     
      plDistributionTable distTab_Xi_Z ( childNode.variable, latentNode.variable );
      for ( int latentVal = 0; latentVal < latentNode.variable.cardinality(); ++latentVal ) {
        std::vector< plProbValue > probValuesXiZ_vals;
        for ( int childVal = 0; childVal < childNode.variable.cardinality(); ++childVal ) {
          probValuesXiZ_vals.push_back( boost::lexical_cast<plProbValue>( (*distributionLine)[childVal] ) );
        }
        distTab_Xi_Z.push( plProbTable( childNode.variable, probValuesXiZ_vals), (int)latentVal );
        ++distributionLine;
      }
      dataCndTable *= distTab_Xi_Z; // adds the conditional table to result
      boost::add_edge( latentId, childId, graph );
    }
    ++distributionLine;
    latentNode.jointDistribution = plJointDistribution(latentNode.variable ^ variables, probTabZ * dataCndTable);
  }
  distributionFile.close(); vertexFile.close();  
}

////////////////////////////////// WRITER BELOW THIS //////////////////////////////////////

void FLTMGraphWriter::writeLabPos( const Graph& graph,
                                   const std::string labPosFileName ) {
  std::ofstream labPosFile(labPosFileName.c_str());  
  vertex_iterator vi, vi_end;
  labPosFile << ID << SEPARATOR << LABEL << SEPARATOR << POSITION << SEPARATOR << std::endl;  // writes header
  for ( boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi ) {
    int vertex = *vi;
    labPosFile << graph[vertex].index << SEPARATOR
               << graph[vertex].label << SEPARATOR
               << graph[vertex].position << std::endl; 
  }  
  labPosFile.close();
}

////////////////////////////////////////////////////////////////////////////////////////////
void SingleGraphSave::operator()( Graph& graph, const std::string singleFileName ) const {
  
  std::ofstream singleFile(singleFileName.c_str());
  
  std::map<int, int> edgeMap;
  edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei) {
    edgeMap[ boost::target(*ei, graph)  ] = boost::source(*ei, graph);
  }
   
  vertex_iterator vi, vi_end;
  singleFile << ID << SEPARATOR <<  LATENT << SEPARATOR << PARENT << SEPARATOR << LEVEL << SEPARATOR << CARDINALITY << "\n";  // writes header
  for ( boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi ) {
    int vertex = *vi;
    singleFile << graph[vertex].index << SEPARATOR
               << !graph[vertex].isLeaf << SEPARATOR;
    if (edgeMap.find( vertex ) == edgeMap.end()) 
      singleFile << -1 << SEPARATOR;
    else
      singleFile << edgeMap[vertex] << SEPARATOR;
    
    singleFile << graph[vertex].level << SEPARATOR
               << graph[vertex].variable.cardinality() << std::endl;
  }  
  
  singleFile.close();
}


/////////////////////////////////////////////////////////////////////////////////////////
void TulipGraphSave::operator()( const Graph& graph,
                                 const std::string vertexFileName,
                                 const std::string edgeFileName ) const {
 
  // writeVertices(graph, vertexFileName);
  std::ofstream vertexFile(vertexFileName.c_str());
  vertex_iterator vi, vi_end;
  vertexFile << ID << SEPARATOR << LATENT << SEPARATOR << LEVEL << SEPARATOR << CARDINALITY << "\n";  // writes header
  for ( boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi ) {
    int vertex = *vi;
    vertexFile << graph[vertex].index << SEPARATOR
               << !(graph[vertex].isLeaf) << SEPARATOR
               << graph[vertex].level << SEPARATOR
               << graph[vertex].variable.cardinality()
               << std::endl;
  }  
  vertexFile.close();

  std::ofstream edgeFile(edgeFileName.c_str());
  edgeFile << ID << SEPARATOR << PARENT_ID << "\n"; // writes header
  edge_iterator ei, ei_end;
  for( boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei ) {
    edgeFile << boost::target(*ei, graph) << SEPARATOR << boost::source(*ei, graph) << std::endl;
  }
  edgeFile.close(); 
 
}

////////////////////////////////////////////////////////////////////////////////
void BayesGraphSave::operator()( const Graph& graph,
                                 const std::string vertexFileName,
                                 const std::string distributionFileName ) const {
  
  std::ofstream distributionFile(distributionFileName.c_str()), vertexFile(vertexFileName.c_str());
  vertex_iterator vi, vi_end;
  Label2Index label2Index;
  vertexFile << ID << SEPARATOR << LATENT << SEPARATOR << LEVEL << SEPARATOR << CARDINALITY << "\n";  // writes header
  std::cout << "saving vertices...\n";
  for ( boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi ) {
    int vertex = *vi;
    vertexFile << graph[vertex].index << SEPARATOR << !(graph[vertex].isLeaf) << SEPARATOR
               << graph[vertex].level << SEPARATOR << graph[vertex].variable.cardinality() << std::endl;
    label2Index[ graph[*vi].label ] = graph[*vi].index;
  }
  vertexFile.close();
  std::cout << "saving joint distribution...\n";
   
  for ( boost::tie(vi, vi_end) = boost::vertices(graph); vi != vi_end; ++vi ) {
    const Node& node = graph[*vi];                 
    if ( !node.isLeaf ) {
      plJointDistribution distribution = node.jointDistribution;
      plVariablesConjunction all_variables = distribution.get_variables(); // all variables (latent variable and its children)

      plVariablesConjunction variables; // child variables
      for (int i = 1; i <  all_variables.size(); ++i)
        variables ^=  all_variables[i]; // initializes child conjunction

      plSymbol latentVar =  all_variables[0]; // latent variable

      distributionFile << node.index << SEPARATOR <<  variables.size() << std::endl;

      plComputableObjectList objLists = distribution.get_computable_object_list();

      plComputableObject probTableZ = objLists.get_distribution_over(latentVar); // distribution table for the latent variable
      int val;
      for ( val = 0; val < latentVar.cardinality() - 1 ; ++val ) {      
        distributionFile << std::fixed << std::setprecision(30) << probTableZ.compute( plValues().add(latentVar, val) ) << SEPARATOR; // P(latentVar = val)
      }

      distributionFile << std::fixed << std::setprecision(30) << probTableZ.compute( plValues().add(latentVar, val) )  << std::endl; // writes last probability value
      for ( int i = 0; i < variables.size(); ++i ) {
        plSymbol varX = variables[ i ]; // retrieves the child variable
        distributionFile << label2Index[ varX.name() ]  << std::endl; // writes child variable's id
        plComputableObject compTableXZ = objLists.get_distribution_over( varX ); // conditional distribution P(X_i | Z)
        plDistributionTable& distTableXZ = static_cast<plDistributionTable&>( compTableXZ ); // casting P(X_i | Z) to derived class
        for ( val = 0; val < latentVar.cardinality(); ++val ) {
          int childVal;
          for ( childVal = 0; childVal < varX.cardinality() - 1; ++childVal ) { // for each value x of the child variable
            distributionFile << std::fixed << std::setprecision(30) << distTableXZ.compute( plValues().add(latentVar, val).add(varX, childVal) ) << SEPARATOR; // p(X_i = childVal | Z = val)
          }
          distributionFile << std::fixed << std::setprecision(30) << distTableXZ.compute( plValues().add(latentVar, val).add(varX, childVal) ) << std::endl;
        }
      }
      distributionFile << std::endl; // breaks the line, moves to the next latent variable
    }
  }
 
  distributionFile.close();
}


} // namespace fltm_graphends here. fltm_graph

/****************************************************************************************/
#endif // FLTM_GRAPH_GRAPHIO_HPP
 
