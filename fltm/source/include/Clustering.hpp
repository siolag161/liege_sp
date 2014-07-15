/****************************************************************************************
 * File: Clustering.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 09/07/2014

 ***************************************************************************************/
#ifndef FLTM_CLUSTERING_HPP
#define FLTM_CLUSTERING_HPP

#include "Diss.hpp"
#include "Simi.hpp"
#include <set>
#include <vector>
#include <map>

#include <iostream>
namespace fltm
{

typedef std::vector< std::vector<int> > Clustering;

struct Partition {
  
  typedef std::map<int,int> Labels;
  
  size_t nbrClusters() const { return m_clusterSet.size(); }
  size_t nbrItems() const { return m_labels.size(); }

  Clustering to_clustering() const ;// {
  //   Clustering clustering( nbrClusters(), std::vector<int>() );
  //   for ( auto& i: m_labels ) {
  //     clustering[i.second].push_back(i.first);
  //     return clustering;
  //   }
  // }
 

  int cluster( int item ) const { return m_labels.at(item); }
  void cluster( int item, int cluster ) {
    m_labels[item] = cluster;
    m_clusterSet.insert(cluster);
  }

 private:
  std::set<int> m_clusterSet;
  Labels m_labels;     
};

std::ostream& operator<<( std::ostream& os, const Clustering& clt );
std::ostream& operator<<( std::ostream& os, const Partition& p );

/////////////////////////////////////////////////////////////////////////

struct AlgoClust {
  virtual Partition operator()() = 0;
  virtual Comp* getComp() { return comp; }
  virtual void setComp( Comp* c ) { comp = c; }
  virtual char* name() const = 0;
  virtual void invalidCache() { comp->invalidCache(); }

  AlgoClust( Comp* c): comp(c) {}
  virtual ~AlgoClust() { delete comp; }
 protected:
  Comp* comp;
};

} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_CLUSTERING_HPP
