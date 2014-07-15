
#include "DBSCAN.hpp"
#include "Clustering.hpp"

namespace fltm {

Partition DBSCAN::operator()() {
 
  size_t nvars = comp->size(); //  for ( size_t i = 0; i < nvars; ++i) partition.cluster(i,-1);
  std::vector<int> m_labels( nvars, -1);
  std::vector<int> visited( nvars, 0 );  
  int cluster_id = 0;
  // std::cout << "here-1\n";
  for (int pid = 0; pid < nvars; ++pid) {
    if ( !visited[pid] ) {
      // printf("here-2-%d\n", pid);
      visited[pid] = 1;
      Neighbors ne = find_neighbors(pid);
      // printf("here-3-%d\n", pid);
      if ( ne.size() >= min_elems ) {
        m_labels[pid] = cluster_id; // partition.cluster( pid, cluster_id );
        for ( int i = 0; i < ne.size(); ++i) {
          // printf("here-4-%d-%d\n", pid, i);
          int nPid = ne[i];
          if ( !visited[nPid] ) {
            visited[nPid] = 1;
            Neighbors ne1 = find_neighbors(nPid);
            if ( ne1.size() >= min_elems ) {
              for (const auto & n1 : ne1) {
                ne.push_back(n1);
              }
            }
          }
          if ( m_labels[nPid] == -1 ) {
            m_labels[nPid] = cluster_id;
          }
        }
        ++cluster_id;
      }
    }
  }

  return to_partition(m_labels);   
}


DBSCAN::Neighbors DBSCAN::find_neighbors( const Index pid ) const {
  Neighbors ne;
  size_t nvars = comp->size();
  for ( Index i = 0; i < nvars; ++i ) {
    // printf("find-%d-%d\n", pid,i);

    if ( comp->compute( i, pid ) <= epsilon ) {
      ne.push_back(i);
    }
  }

  return ne;
}

/////////////////////////////////////////

Partition DBSCAN::to_partition( const std::vector<int>& labels ) {
  Partition partition;

  std::set<int> unique_labs;
  std::vector<int> singletons;
  for ( size_t i = 0; i < labels.size(); ++i ) {
    if ( labels.at(i) != -1 ) {
      partition.cluster( i,labels.at(i) );
      unique_labs.insert( labels.at(i) );
    } else {
      singletons.push_back(i);
    }
  }

  for ( auto& i: singletons ) {
    size_t cur_cluster = unique_labs.size();
    partition.cluster(i, cur_cluster);
    unique_labs.insert(cur_cluster);
  }
  
  return partition;
}



}
