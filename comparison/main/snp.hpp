/****************************************************************************************
 * File: snp.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 03/07/2014

 ***************************************************************************************/
#ifndef CLUSTERING_SNP_HPP
#define CLUSTERING_SNP_HPP

#include <map>
#include <vector>
#include <fstream>
#include <string>
#include "CSVParser.hpp"
#include <iostream>
#include "partition.hpp"

namespace clustering
{

struct SNP {
  std::string chr;
  
  std::string get_chr() const { return chr; }
  std::string get_label() const { return label; }
  int get_id() const { return id; }
  double get_pvalue() const { return p_value; }
  int get_cluster() const { return cluster; }
  std::string get_algo() const { return algo; }

  
  int id;
  std::string label;
  // int position;
  double p_value;
  // std::string test;
  int cluster;
  std::string algo;

  bool operator < (const SNP& o) const {
    return this->p_value < o.p_value;
  }

  
};

struct SNP_Partition {
  ~SNP_Partition() { }
  Partition partition;
  std::map<int,int> snp2par; // maps the 135 (genomcic) -> 0
  std::vector<int> par2snp; // maps the 135 (genomcic) -> 0

  std::map<int, int> algo2par; // maps the algo's clustering id (genomcic) -> 0  
  std::vector<int> par2algo; // maps the algo's clustering id (genomcic) -> 0

  SNP_Partition( const std::vector<SNP>& snps): partition(snps.size()) {
    int max_snp = -1;
    std::set<int> cluster_id_set;
    for ( const auto& snp: snps )  {
      if ( snp.get_id() > max_snp ) max_snp = snp.get_id();
      cluster_id_set.insert( snp.cluster );
    }
    par2algo.resize( cluster_id_set.size(), -1 );
    int i = 0;
    for ( const auto& id: cluster_id_set ) {
      par2algo[i] = id;
      algo2par[id] = i;
      ++i;
    }

    par2snp.resize( snps.size(), -1 );
    // snp2par.resize( max_snp, -1 );
    for (size_t i = 0; i < snps.size(); ++i) {
      const SNP snp = snps.at(i);
      par2snp[i] = snp.get_id();
      snp2par[snp.id] = i;
      partition.cluster( i, algo2par[snp.cluster] );
    }
  }
};

std::ostream& operator << ( std::ostream& os, const SNP_Partition& sp ) { 
  for ( int i = 0; i < sp.partition.labels.size(); ++i ) {
    auto& label = sp.partition.labels[i];
    os << i << "(" << sp.par2snp[i] << "): "
       << label << "(" << sp.par2algo[label] << ")" << std::endl;
  }
  os << std::endl;
  return os;
}

std::ostream& operator << ( std::ostream& os, const SNP& snp ) {
  os << "chr: " << snp.chr << ", "
     << "id: " << snp.id << ", "
     << "label: " << snp.label << ", "
     << "p_value: " << snp.p_value << ", "
     << "cluster: " << snp.cluster << ", "
     << "algo: " << snp.algo;
  return os;
}

std::ostream& operator << ( std::ostream& os, const std::vector<SNP>& snps ) {
 
  for ( const auto& snp: snps ) {
    os << snp << std::endl;
  }
  // std::cout << "size: " << snps.size() << std::endl;
  return os;
}



namespace clustering_file {
enum HEADER { CHR, ID, LABEL, POSITION, P_VALUE, TEST, CLUSTER, ALGO };
} 



} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{

// struct SNP_Partition {
//   Partition partition;
//   std::vector<int> snp2parId; // maps the 135 (genomcic) -> 0
//   std::vector<int> algo2parId; // maps the algo's clustering id (genomcic) -> 0  
//   std::vector<int> par2snpId; // maps the 135 (genomcic) -> 0
//   std::vector<int> par2algoId; // maps the algo's clustering id (genomcic) -> 0

//   SNP_Partition(std::vector<SNP>& snps);

// };



} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_SNP_HPP
