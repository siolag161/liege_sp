/****************************************************************************************
 * File: DBSCAN.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 12/07/2014

 ***************************************************************************************/
#ifndef FLTM_DBSCAN_HPP
#define FLTM_DBSCAN_HPP

#include "Clustering.hpp"
#include "Distance.hpp"
namespace fltm
{

struct DBSCAN: public AlgoClust {
  typedef int Index;
  typedef std::vector<Index> Neighbors;
  typedef std::vector<Index> Labels;
  
  DBSCAN( Distance* d, const int eles, const double eps ): AlgoClust(d), min_elems(eles), epsilon(eps) {   
  }
 
  virtual Partition operator()(); 
  virtual char* name() const {
    char* name = new char[80];
    sprintf( name, "DBSCAN_%d_%.3f", min_elems, epsilon);
    return name;
  }

 protected:
  Neighbors find_neighbors( const Index pid ) const;
  static Partition to_partition( const std::vector<int>& labels );
   
 protected:
  int min_elems;
  double epsilon;
};

} // namespace fltmends here. fltm

/****************************************************************************************/
#endif // FLTM_DBSCAN_HPP
