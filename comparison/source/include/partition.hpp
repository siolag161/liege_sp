/****************************************************************************************
 * File: clustering.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 02/07/2014

 ***************************************************************************************/
#ifndef PARTITION_PARTITION_HPP
#define PARTITION_PARTITION_HPP

#include <vector>
#include <set>
#include <algorithm>

#include "counter.hpp"

namespace clustering
{

// struct Cluster {
//   typedef int Id;
// };
typedef int ObjectId;
typedef std::vector< std::set<ObjectId> > Clustering;

struct Partition {
  
  typedef int Label;  
  typedef std::vector<Label> Labels;
  
  std::set<Label> labelSet;
  Labels labels; // maps Id -> Label

  size_t nbr_clusters() const {
    return labelSet.size();
  }

  size_t nbr_objects() const {
    return labels.size();
  }
  
  Label cluster( const ObjectId objId ) const {
    return labels.at(objId);
  }

  Partition& cluster( const ObjectId obj_id, const Label clust_id ) {
    labels[obj_id] = clust_id;
    labelSet.insert(clust_id);
    return *this;
  }
   
  Partition( const size_t sz ) {
    labels.resize( sz, -1 );
  }

  Partition( const Clustering& clt ) {
    size_t sz = 0;
    for ( size_t c = 0; c < clt.size(); ++c ) {
      sz += clt.at(c).size();
    }    
    labels.resize( sz, -1 );

    for ( size_t c = 0; c < clt.size(); ++c ) {
      // for ( size_t i = 0; i < clt.at(c).size(); ++i ) {
      //   labels[ clt.at(c) ] = c;
      // }
      for ( auto& i: clt.at(c) ) {
        labels[i] = c;
      }
    }
  }

  Clustering to_clustering() const {
    Clustering clt;
    clt.resize( nbr_clusters(), std::set<ObjectId>() );

    for ( size_t obj = 0; obj < nbr_objects(); ++ obj ) {
      Label clustId = labels[obj];
      clt[clustId].insert(obj);
    }
    return clt;
  }

  // std::string dataset_name;
  // std::string algorithm_name;
  
};

// std::ostream& operator << ( std::ostream& os, const SNP& snp ) {
//   os << "chr: " << snp.chr << ", "
//      << "id: " << snp.id << ", "
//      << "label: " << snp.label << ", "
//      << "p_value: " << snp.p_value << ", "
//      << "cluster: " << snp.cluster << ", "
//      << "algo: " << snp.algo;
//   return os;
// }




} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{



} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // PARTITION_PARTITION_HPP
