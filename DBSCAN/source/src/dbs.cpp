// #include <iostream>
// #include <boost/algorithm/minmax.hpp>
// #include <vector>

// #include "dbs.hpp"
// #include "DistCompute.hpp"
// namespace clustering
// {

// DBSCAN::DBSCAN()
// {
//   reset();
// }

// void DBSCAN::reset() {
//   m_labels.clear();
//   std::vector<uint32_t>().swap(m_labels);
// }

// void DBSCAN::prepare_labels( size_t s )
// {
//   m_labels.resize(s);

//   for( auto & l : m_labels)
//   {
//     l = -1;
//   }
// }

// //template< class DistCompute >
// DBSCAN::Neighbors DBSCAN::find_neighbors( const Index pid,
//                                           const ClusterData& mat,
//                                           const DistCompute& dist,
//                                           const double eps )
// {
//   Neighbors ne;
//   size_t nvars = mat.size();
//   for ( Index i = 0; i < nvars; ++i )
//   {
//     if 	( dist( i, pid ) <= eps )
//     {
//       ne.push_back(i);
//     }
//   }
//   return ne;
// }


// const DBSCAN::Labels& DBSCAN::operator()( const ClusterData& dm,
//                                           const DistCompute& dist,
//                                           const double eps,
//                                           const unsigned min_elems ) {
  
//   size_t nvars = dm.size();
//   prepare_labels( nvars );
    
//   std::vector<uint32_t> visited( nvars, 0 );
//   uint32_t cluster_id = 0;

//   for (uint32_t pid = 0; pid < nvars; ++pid)
//   {
//     if ( !visited[pid] ) {  
//       visited[pid] = 1;
//       Neighbors ne = find_neighbors( pid, dm, dist, eps );
//       if ( ne.size() >= min_elems ) {
//         m_labels[pid] = cluster_id;
//         for (uint32_t i = 0; i < ne.size(); ++i) {
//           uint32_t nPid = ne[i];
//           if ( !visited[nPid] ) {
//             visited[nPid] = 1;
//             Neighbors ne1 = find_neighbors( nPid, dm, dist, eps );
//             if ( ne1.size() >= min_elems ) {
//               for (const auto & n1 : ne1) {
//                 ne.push_back(n1);
//               }
//             }
//           }
//           if ( m_labels[nPid] == -1 ) {
//             m_labels[nPid] = cluster_id;
//           }
//         }
//         ++cluster_id;
//       }
//     }
//   }

//   m_nclusters = cluster_id;
//   return m_labels;
// }

// }
