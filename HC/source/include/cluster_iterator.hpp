/****************************************************************************************
 * File: ClusterIterator.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 19/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_CLUSTERITERATOR_HPP
#define CLUSTERING_CLUSTERITERATOR_HPP

#include <iterator>
#include <memory>
#include <queue>
namespace clustering
{

template< class Cluster, bool is_const_iterator = true, class ValueType = int>
struct ClusterIterator : public std::iterator<std::bidirectional_iterator_tag, ValueType> {
  /**
   * For const_iterator:   define ClusterPtrType to be a   const Cluster*
   * For regular iterator: define ClusterPtrType to be a   Cluster*
   */
  typedef typename std::conditional<is_const_iterator, const std::shared_ptr<Cluster>, std::shared_ptr<Cluster> >::type ClusterPtrType;
 
  /**
   * For const_iterator:   define ValueReferenceType to be a   const ValueType&
   * For regular iterator: define ValueReferenceType to be a   ValueType&
   */
  typedef typename std::conditional<is_const_iterator, const ValueType&, ValueType&>::type ValueReferenceType;

  void addValues( ClusterPtrType clt ) {
    if (clt) {
      if (clt->isLeaf()) {
        vals.push(clt->id);
      } else {
        addValues(clt->left);
        addValues(clt->right);
      }
    }
  }

  ClusterIterator(): ClusterIterator(nullptr) {}
  // /**
  //  * Regular constructor: set up your iterator.
  //  */
  ClusterIterator(ClusterPtrType clt) : clusterPtr(clt) {
    // You might want to do something here, but not necessarily
    addValues(clusterPtr);    
  }
 
  // /**
  //  * Copy constructor. Allows for implicit conversion from a regular iterator to a const_iterator
  //  */
  ClusterIterator(const ClusterIterator<Cluster,false,ValueType>& other) : clusterPtr(other.clusterPtr) {
    addValues(clusterPtr);    
  }

  ClusterIterator(const ClusterIterator<Cluster,true,ValueType>& other) : clusterPtr(other.clusterPtr) {
    addValues(clusterPtr);    
  }
  
  /**
   * Equals comparison operator
   */
  bool operator== (const ClusterIterator& other) const {
    return other.vals.empty() ? vals.empty() : vals.front() == other.vals.front();
  }
 
  /**
   * Not-equals comparison operator
   * @see operator==(const ClusterIterator&) const
   */
  bool operator!= (const ClusterIterator& other) const {
    return !(*this == other);
  }
 
  /**
   * Dereference operator
   * @return the value of the element this iterator is currently pointing at
   */
  ValueReferenceType operator*() {
    // Up to you to define: get a reference to an element in your data structure
    return vals.front();
  }
 
  ClusterIterator &operator++(){
    vals.pop();
    return *this;
  } 
 
  /**
   * Postfix increment operator (e.g., it++)
   */
  ClusterIterator operator++(int){
    const ClusterIterator old(*this);
    ++(*this);
    return old;
  }

  ClusterPtrType clusterPtr;
  std::queue<ValueType> vals;
 
}; // end of nested class ClusterIterator

} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_CLUSTERITERATOR_HPP
