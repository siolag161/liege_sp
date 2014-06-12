 
/****************************************************************************************
 * File: Clustering.hpp
 * Description: 
 * @author: Phan Duc Thanh (duc-thanh.phan@univ-nantes.fr) - Under supervision of Christine Sinoquet (christine.sinoquet@univ-nantes.fr)
 * @date: 13/11/2013

 ***************************************************************************************/
#ifndef CLUSTERING_CLUSTERING_HPP
#define CLUSTERING_CLUSTERING_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace fltm {

template<typename ItemType>
class Cluster {
 public:
  typedef ItemType item_type;
  typedef typename std::vector<ItemType>::iterator iterator;
  typedef typename std::vector<ItemType>::const_iterator const_iterator;
  
  iterator begin() { return items.begin();}
  iterator end() { return items.end();}
  
  const_iterator begin() const { return items.begin(); }
  const_iterator end() const { return items.end(); }
  
  Cluster& append_item(const ItemType& item) { items.push_back(item); return *this; }
  Cluster& add(const ItemType& item) { return append_item(item); }
  Cluster& push_back(const ItemType& item) { return append_item(item); }
  
  ItemType& operator[](const unsigned& idx) { return items[idx]; }
  const ItemType& operator[](const unsigned& idx) const { return at(idx); }
  const ItemType& at(const unsigned& idx) const { return items.at(idx); }

  void set_name(const std::string& lab) { name = lab; }
  const std::string& get_name() const { return name; }

  unsigned size() const { return items.size(); }

  void remove(const unsigned& idx) { items.erase(items.begin()+idx) ;}
  
 private:
  std::vector<ItemType> items; // indices of item contained in this cluster
  std::string name;
};


template<typename ItemType>
class Partition {
 public:
  typedef Cluster<ItemType> cluster_type;
  typedef std::unique_ptr< Cluster<ItemType> > cluster_ptr;

  typedef std::map< std::string, cluster_ptr> Name2Cluster;

  const cluster_ptr operator[] (const std::string& idx) const { return at(idx); }
  cluster_ptr operator[] (const std::string& idx) { return name2Cluster[idx]; }  
  const cluster_ptr at(const std::string& idx) const { return name2Cluster.at(idx); }

  Partition( const Name2Cluster& name2Clust ): name2Cluster(name2Clust) {}  
  Partition() {}

  Partition& insert( cluster_ptr cluster)
  {
    //name2Cluster[cluster->get_name()] = cluster;
    name2Cluster.insert( std::make_pair(cluster->get_name(), std::move(cluster)) );
    return *this;
  }
  
  unsigned size() const { return name2Cluster.size(); }

  typedef typename Name2Cluster::iterator iterator; 
  typedef typename Name2Cluster::const_iterator const_iterator;

  iterator begin() { return name2Cluster.begin(); }
  iterator end() { return name2Cluster.end(); }

  const_iterator begin() const { return name2Cluster.begin(); }
  const_iterator end() const { return name2Cluster.end(); }
  
 protected:
  Name2Cluster name2Cluster;
};
  
} // namespace fltm ends here

/****************************************************************************************/
#endif // CLUSTERING_CLUSTERING_HPP
