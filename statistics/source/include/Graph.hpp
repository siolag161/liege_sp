/****************************************************************************************
 * File: Graph.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 13/07/2014

 ***************************************************************************************/
#ifndef FLTM_GRAPH_HPP
#define FLTM_GRAPH_HPP

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "pl.h"
namespace fltm
{

typedef plJointDistribution JointDistribution;
typedef int IndexType;
typedef std::string LabelType;
typedef plSymbol RandomVariable;
typedef std::map<IndexType, LabelType> Index2Label;
typedef std::map<LabelType, IndexType> Label2Position;
typedef std::map<LabelType, IndexType> Label2Index;

/** Forward declaration for Node class as it is part of the Graph declaration
 *
 */
struct Node;

/* 
 *
 */
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                Node, boost::no_property> Graph;



/**
 * Node 
 */
struct Node {

  ~Node() { }
  
  IndexType index; 
  LabelType label; // cache for variable.name()
  
  bool isLeaf;
  int position; // physical position on the genome
  int level; // indicates which level to which this node belongs

  Graph* graph; // reference to its graph

  RandomVariable variable; // represents the underlying random variable
  JointDistribution jointDistribution; // joint distribution of the Naive Bayes Model rooted in the variable
  
  Node(): isLeaf(true), position(-1), level(0), graph(NULL) { 
    //jointDistribution = new JointDistribution();
  }

  inline Node& setDistribution(JointDistribution& jointDist);
  inline Node& setPosition(Label2Index& label2Index, plComputableObjectList& objectList);  
  inline Node& setLevel(Label2Index& label2Index, plComputableObjectList& objectList);
  inline Node& setVariable(plComputableObjectList& objectList);
  
  /** Sets the following properties:
   *
   *    - parent graph (from parameter)
   *    - joint distribution (directly from parameter)
   *    - variable (deduced from parameter)
   *    - position (deduced from parameter)
   *    - level (deduced from parameter)
   *
   * Requires Label2Index which maps the label of a variable to its index
   * Requires that the node is a latent variable
   */
  Node& setupProperties(Graph* graph,
                        JointDistribution& jointDist,
                        Label2Index& label2Index ) {
    plComputableObjectList objectList = jointDist.get_computable_object_list();
    return setGraph(graph)
              .setDistribution(jointDist)
              .setVariable(objectList)
              .setLabel(label2Index)
              .setPosition(label2Index, objectList)
              .setLevel(label2Index, objectList);
  }
  

  Node& setGraph(Graph* graph) { this->graph = graph; return *this; }

  Node& setLabel(Label2Index& label2Index) {
    label = variable.name();
    label2Index[label] = this->index;
    return *this;
  }

  unsigned getIndex() const {
    return index;
  }


};

/**
 *
 */
typedef Graph::vertex_descriptor vertex_t; 

/**
 *
 */
typedef Graph::edge_descriptor edge_t;

typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;

typedef boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;


typedef std::pair<vertex_iterator, vertex_iterator> vertex_iter_pair;
typedef std::pair<edge_iterator, edge_iterator> edge_iter_pair;

/**
 *
 */
inline vertex_t createVertex( Graph& graph,
                              const unsigned& cardinality,
                              const bool isLeaf,                              
                              const std::string& label = "",
                              const unsigned& position = -1,
                              const unsigned& level = -1) {
  
  vertex_t vertexId = boost::add_vertex(graph);
  graph[vertexId].variable = RandomVariable(label, plIntegerType(0, cardinality - 1));
  graph[vertexId].isLeaf = isLeaf;
  graph[vertexId].position = position; // physical position on the genome
  graph[vertexId].label = label;
  graph[vertexId].index = vertexId;
  graph[vertexId].graph = &graph;
  graph[vertexId].level = level;
  return vertexId;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
Node& Node::setDistribution(JointDistribution& jointDist) {
  jointDistribution = jointDist;
  return *this;
}

/**
 *
 */
Node& Node::setVariable(plComputableObjectList& objectList) {
  unsigned latentVarIdx = 0;   
  variable = objectList[latentVarIdx].get_variables()[0]; // objectList is the collection of distributions for the Naive Bayes Model, augmented with annotations.
  // Example: P(X1,X2,Z) = P(Z)*P(X1|Z)*P(X2|Z). ObjectList[0] = P(Z), ObjectList[1] = P(X1|Z), ObjectList[2] = P(X2|Z). P(Z).get_variables() = {Z},
  // P(X1|Z).get_variables() = {Z,X1}. 
  
  return *this;
}

Node& Node::setPosition(Label2Index& label2Index, plComputableObjectList& objectList) {
 
  unsigned nbrChildren = objectList.size() - 1;
  
  if (nbrChildren > 0) {    
    unsigned totalPos = 0;
    for (unsigned child = 0; child < nbrChildren; ++ child) {
      LabelType label = objectList[child].get_variables()[0].name();
      IndexType index = label2Index[label];
      totalPos += (*graph)[index].position; 
    }
    
    this->position = totalPos / nbrChildren;
    // label2Pos[variable.name()] = position;  
  }
  return *this;
  
}

Node& Node::setLevel(Label2Index& label2Index, plComputableObjectList& objectList) {
  unsigned nbrChildren = objectList.size() - 1;

  int maxChildrenLevel = 0;
  for (unsigned child = 0; child < nbrChildren; ++ child) {      
    LabelType label = objectList[child].get_variables()[0].name();
    IndexType index = label2Index[label];

    if (maxChildrenLevel < (*graph)[index].level) {
      maxChildrenLevel = (*graph)[index].level;
    }
  }
  this->level = maxChildrenLevel + 1;

  
  return *this;  
}


inline int graph_height( const Graph& g ) {
  int h = -1;
  vertex_iterator vi, vi_end;
  for ( boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi ) {
    int vertex = *vi;
    h = std::max( g[vertex].level, h );
  }

  return h;
}



} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_GRAPH_HPP
