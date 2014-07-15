#include "Clustering.hpp"

namespace fltm {

Clustering Partition::to_clustering() const {
  Clustering clustering( nbrClusters(), std::vector<int>() );
  for ( const auto& i: m_labels ) {
    clustering[ i.second ].push_back( i.first );
  }  
  return clustering;
}

std::ostream& operator<<( std::ostream& os, const Clustering& clt ) {
  os << "there are: " << clt.size() << " clusters\n";  
  for ( size_t c = 0; c < clt.size(); ++c ) {
    os << "cluster: " << c << std::endl;
    for ( size_t i = 0; i < clt.at(c).size() - 1; ++i ) {
      os << clt.at(c).at(i) << ", ";
    }
    os << clt.at(c).at(clt.at(c).size() - 1) << std::endl;
  }
  return os;
}

std::ostream& operator<<( std::ostream& os, const Partition& p ) {  
  return os << p.to_clustering();
}
 

}
///////////////


