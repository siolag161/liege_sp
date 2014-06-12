/****************************************************************************************
 * File: dbs.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 11/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_DBS_HPP
#define CLUSTERING_DBS_HPP

namespace clustering
{

class DBSCAN
{  

 public:
  // typedef std::vector< std::vector<double> > DistanceMatrix;
  // typedef std::vector< std::vector<double> > ClusterData;
  typedef int Index;
  typedef std::vector<Index> Neighbors;
  typedef std::vector<Index> Labels;

  DBSCAN() { reset(); }
  ~DBSCAN() {}

  const Labels& get_labels() const { return m_labels; }

  template< class ClusterData, class DistCompute >
  const Labels& operator()( const ClusterData& distMatrix,
                            DistCompute& distCompute,
                            const unsigned min_elems,
                            const double eps) ;
  
  void reset() {  m_labels.clear();  std::vector<Index>().swap(m_labels); }
  const int nclusters() const { return m_nclusters; }

 private:
  
  void prepare_labels( size_t s ) {
    m_labels.reserve(s);
    for ( size_t i = 0; i < s; ++ i ) {
      m_labels.push_back(-1);
    }
  }
  
  template< class ClusterData, class DistCompute >
  Neighbors find_neighbors( const Index index,
                            const ClusterData& distMatrix,
                            DistCompute& distCompute,
                            const double eps ) ;
  Labels m_labels;
  int m_nclusters;

  std::vector< std::vector<Index> > clustering;

};


} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{

template< class ClusterData, class DistCompute >
const DBSCAN::Labels& DBSCAN::operator()( const ClusterData& dm,
                                          DistCompute& dist,
                                          const unsigned min_elems,                                          
                                          const double eps ) {

  size_t nvars = dm.size();
  prepare_labels( nvars );
    
  std::vector<uint32_t> visited( nvars, 0 );
  uint32_t cluster_id = 0;

  for (uint32_t pid = 0; pid < nvars; ++pid)
  {
    if ( !visited[pid] ) {  
      visited[pid] = 1;
      Neighbors ne = find_neighbors( pid, dm, dist, eps );
      if ( ne.size() >= min_elems ) {
        m_labels[pid] = cluster_id;
        for (uint32_t i = 0; i < ne.size(); ++i) {
          uint32_t nPid = ne[i];
          if ( !visited[nPid] ) {
            visited[nPid] = 1;
            Neighbors ne1 = find_neighbors( nPid, dm, dist, eps );
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

  m_nclusters = cluster_id;
  return m_labels;  
}

template< class ClusterData, class DistCompute >
DBSCAN::Neighbors DBSCAN::find_neighbors( const Index pid,
                                          const ClusterData& mt,
                                          DistCompute& dist,
                                          const double eps ) {
  Neighbors ne;
  size_t nvars = mt.size();
  for ( Index i = 0; i < nvars; ++i ) {
    if 	( dist( i, pid ) <= eps ) {
      ne.push_back(i);
    }
  }
  return ne;
}

} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_DBS_HPP
