/****************************************************************************************
 * File: cluster.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 18/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_CLUSTER_HPP
#define CLUSTERING_CLUSTER_HPP

#include <cstddef>
#include <memory>
#include "cluster_iterator.hpp"
#include <iostream>
namespace clustering
{

struct Cluster {

  typedef int Id;
  typedef int Level;
  typedef int Size;
  typedef std::shared_ptr<Cluster> ClusterPtr;

  virtual ~Cluster() { // left.reset(); right.reset();
  }
  
  Cluster(): id(-1), level(-1), size(-1), left(nullptr), right(nullptr) {}
  Cluster( const Id id_ ): id(id_), level(0), size(-1), left(nullptr), right(nullptr) {}
  Cluster( const Id id_, ClusterPtr l, ClusterPtr r ): id(id_), left(l), right(r) {
    if (l) {
      assert(r);
      level = std::max(l->level, r->level) + 1;
    }
  }
  Cluster( const Id id_, const Level lv, ClusterPtr l, ClusterPtr r ): id(id_), level(lv), left(l), right(r) {} 
  
  
  Cluster( const Cluster& other ): id(other.id), level(other.level), left(other.left), right(other.right) {}
  

  bool isLeaf() const { return !left || ! right; }
  Cluster& setLeft( ClusterPtr l ) { left = l; return *this; }
  Cluster& setRight( ClusterPtr r) { right = r; return *this; }

  ClusterPtr getLeft() { return left; }
  ClusterPtr getRight() { return right; } 
  
  Id id;
  Level level; // level = id - nbr_leaves
  Size size;

  ClusterPtr left;
  ClusterPtr right;

  typedef ClusterIterator<Cluster, true, int> const_iterator;
  typedef ClusterIterator<Cluster, false, int> iterator;

  std::shared_ptr<Cluster> random() {
    return std::shared_ptr<Cluster>(new Cluster(6,nullptr,nullptr) );
  }

  std::shared_ptr<Cluster> randomize() {
    return std::shared_ptr<Cluster>(new Cluster(6,nullptr,nullptr) );
  }
  
  const_iterator begin() const {
    //return std::shared_ptr<Cluster>(new Cluster(this->id, this->left, this->right) );
    return std::shared_ptr<Cluster>( new Cluster(*this) );

  }

  void print() {
    printf("id: %d, lv: %d, sz: %d\n", id, level, size);
  }
  
  const_iterator end() const {
    return std::shared_ptr<Cluster>(nullptr);
  }

  // iterator begin() {
  //   return std::shared_ptr<Cluster>( this );
  // }

  // iterator end() {
  //   //return std::shared_ptr<Cluster>( new Cluster );
  //   return std::shared_ptr<Cluster>(nullptr);
  // }

  // const std::shared_ptr<Cluster> begin() const {
  //   return std::shared_ptr<Cluster>( new Cluster(*this) );
  // }
};


inline std::ostream& operator<<( std::ostream& os, const Cluster& cluster ) {
  os << "Items of cluster ("<< cluster.id << "): ";
  for ( auto& item: cluster ) {
    os << item << " ";
  }
  os << std::endl;  
  return os;
}

} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_CLUSTER_HPP
