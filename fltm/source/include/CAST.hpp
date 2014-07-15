/****************************************************************************************
 * File: CAST.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 11/07/2014

 ***************************************************************************************/
#ifndef CLUSTERING_CAST_HPP
#define CLUSTERING_CAST_HPP

#include "Clustering.hpp"
#include "Simi.hpp"

namespace fltm
{

struct CAST_Item {  
  // int matrixIndex;
  int index;
  double affinity;
  CAST_Item(const int matIdx, // const int glbIdx,
            const double aff):
      index(matIdx), // globalIndex(glbIdx)
      affinity(aff) {}
};
////////////////////////////////////////////////////////////////////////////
struct CAST: public AlgoClust {
  CAST ( Simi* sim, const double& thres): AlgoClust(sim), thresCAST(thres) {}  
  virtual Partition operator()();
  virtual char* name() const {
    char* name = new char[80];
    sprintf( name, "CAST_%.3f", thresCAST);
    return name;
  }
 protected: 
  Partition run( std::vector<CAST_Item>& unassignedCluster );

  void resetAffinity( std::vector<CAST_Item>& unAssignedCluster );

 protected:
  // Comp& simCompute;
  double thresCAST;     
};

//////////////////////////////////////////////////////////////////////////////
struct AffinityCompute {
  template<typename Compare>
  const int operator()(const std::vector<CAST_Item>& clust, Compare comp) const;    
};

inline void resetAffinity( std::vector<CAST_Item>& cluster);


void addGoodItem( std::vector<CAST_Item>& unassignedCluster, std::vector<CAST_Item>& openCluster, 
                  Comp& simMatrix, const int clusterIdx );

void removeBadItem( std::vector<CAST_Item>& unassignedCluster, std::vector<CAST_Item>& openCluster, 
                    Comp& simMatrix, const int clusterIdx );

void updateClustersAffinity( std::vector<CAST_Item>& sourceCluster, std::vector<CAST_Item>& targetCluster, 
                             Comp& simMatrix,
                             const int clusterIndex );

void moveItemBetweenClusters( std::vector<CAST_Item>& source, std::vector<CAST_Item>& target, const int clusterIndex );

////////////////////////////////////////////////////////////////////
// struct AffinityCompute {
//   template<typename Compare>
//   const int operator()(const CAST_Cluster& clust, Compare comp);
// };

// inline void resetAffinity( CAST_Cluster& cluster);

// void addGoodItem( CAST_Cluster& unassignedCluster, CAST_Cluster& openCluster, 
//                   SimilarityCompute& simMatrix, const int clusterIdx );

// void removeBadItem( CAST_Cluster& unassignedCluster, CAST_Cluster& openCluster, 
//                     SimilarityCompute& simMatrix, const int clusterIdx );

// void updateClustersAffinity( CAST_Cluster& sourceCluster, CAST_Cluster& targetCluster, 
//                              SimilarityCompute& simMatrix,
//                              const int clusterIndex );

// void moveItemBetweenClusters( CAST_Cluster& source, CAST_Cluster& target, const int clusterIndex );

} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_CAST_HPP
