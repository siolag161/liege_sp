/****************************************************************************************
 * File: signi.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 07/07/2014

 ***************************************************************************************/
#ifndef _SIGNI_HPP
#define _SIGNI_HPP

typedef int Position;
typedef std::string Label;

struct Options {
  std::string snpFile;
  std::string labelMapFile;  
  std::string partitionFile;
  std::string outPath;
};


struct SNP {
  std::string chr;
  int id;
  std::string afx_label;
  std::string rs_label;
  int position;

  std::string algo;
  int cluster;
  double p_value;

};

std::ostream& operator << ( std::ostream& os, const SNP& snp ) { 
  os << "chr: " << snp.chr << ", "
     << "id: " << snp.id << ", "
     << "rs_label: " << snp.rs_label << ", "
     << "p_value: " << std::setprecision(20) << snp.p_value << ", "
     << "position: " <<  snp.position << ", "
     << "cluster: " << snp.cluster << ", "
     << "algo: " << snp.algo;
  return os;
}


typedef std::vector<SNP> Partition;
typedef std::vector< std::vector<SNP> > Clustering;

Options getProgramOptions(int argc, char** argv);
void checkInputFiles( std::string& path, std::string filename );
//////////////////////////////////////////////////////////////////////

void read_lab_map( Partition& all_snps, std::map<int,int>& global2local_ids, const std::string fn );
void read_partition( Partition& partition, const std::vector<SNP>& all_snps, const std::string );
void read_signi_snps( std::vector<SNP>& signi_snps, const std::string fn );



struct NearestCluster {
  virtual std::string name() = 0;
  virtual int find( const SNP& snp, const Clustering& clustering) = 0;
};

struct Avg_NearestCluster: public NearestCluster {
  virtual std::string name() {
    return "Min: Average Nearest Cluster";
  }
  
  virtual int find( const SNP& snp, const Clustering& clustering) {
    int cid = 0;
    double min_dist = std::numeric_limits<double>::max();
    for ( size_t i = 0; i < clustering.size(); ++i ) {
      double d = 0.0;
      for ( auto& c: clustering[i] ) {
        d += std::abs( c.position - snp.position );
      }
      d = d / clustering[i].size();

      if ( d < min_dist ) {
        min_dist = d;
        cid = i;
      }
    }
    printf("distance (%s): %f\n", name().c_str(), min_dist);
    return cid;
  }
};


struct Max_NearestCluster: public NearestCluster {
  virtual std::string name() {
    return "Min: Max Nearest Cluster";
  }
  
  virtual int find( const SNP& snp, const Clustering& clustering) {
    int cid = 0;    
    int min_dist = std::numeric_limits<int>::max();    
    for ( size_t i = 0; i < clustering.size(); ++i ) {
      int d = -1;
      if ( clustering[i].size() == 0 ) {
        std::cout << "empty: " << i << std::endl;
      }
      for ( auto& c: clustering[i] ) {
        d = std::max( d, std::abs( c.position - snp.position ) );
      }
      // printf("d: %d\n", d );
      if ( d < min_dist ) {
        min_dist = d;
        cid = i;
      }
    }

    printf("distance (%s): %d\n", name().c_str(), min_dist);
    return cid;
  }
};


struct Min_NearestCluster: public NearestCluster {
  virtual std::string name() {
    return "Min: Min Nearest Cluster";
  }
  
  virtual int find( const SNP& snp, const Clustering& clustering) {
    int cid = 0;    
    int min_dist = std::numeric_limits<int>::max();    
    for ( size_t i = 0; i < clustering.size(); ++i ) {
      int d = std::numeric_limits<int>::max();
      for ( auto& c: clustering[i] ) {
        d = std::min( d, std::abs( c.position - snp.position ) );
      }

      if ( d < min_dist ) {
        min_dist = d;
        cid = i;
      }
    }
    printf("distance (%s): %d\n", name().c_str(), min_dist);
    return cid;
  }
};


Clustering partition2clustering( const Partition& partition);

void impute_signi_snps( std::vector<SNP>& signi_snps,
                        const Partition& partition,
                        const Clustering& clustering,
                        const std::map<int,int> global2local_ids,
                        const std::vector<NearestCluster*>& nearestClusters,
                        const std::string outFN );
/****************************************************************************************/
#endif // _SIGNI_HPP
