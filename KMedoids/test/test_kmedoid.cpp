#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include "kmedoids.hpp"
#include "chrono_measure.hpp"

using namespace clustering; 
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_KMedoids, Data ) 


struct Diss {
  Diss( const std::vector< std::vector<double> >& d ): data(d) {}
  double operator()( size_t a, size_t b ) const {
    double result = 0.0;
    for ( size_t i = 0; i < data.at(0).size(); ++ i ) {
      result += (data[a][i] - data[b][i] ) * ( data[a][i] - data[b][i] );
      // result += (data[a][i] - data[b][i] );
    }
    
    return result;
  }

  const std::vector< std::vector<double> >& data;
 
};

BOOST_AUTO_TEST_CASE( Test_Object_Most_Similar ) {
  size_t nrows = 2500, ncols = 1500;
  std::vector< std::vector<double> > data( nrows, std::vector<double>(ncols, 0.0) );  
  for ( size_t i = 0; i < nrows; ++i ) {
    for ( size_t j = 0; j < ncols; ++j )
      data[i][j] = i*1.0/1000;
  }

  PAM_Partition partition(nrows);
  partition.medoids[0] = 1250;
  Diss dist(data); 

  // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  partition.clear( data.size() );
  std::pair<ObjectId, double> rs =  object_most_similar( partition, data, dist );
  // std::cout << rs.first << ","
  //           << rs.second << std::endl;
  // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // std::cout << "Algo (sequential) took "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  //           << " (s).\n";

  // std::pair<ObjectId, double> bo =  object_most_similar( partition, data, dist );
  // partition.medoids[0] = bo.first;
  // for ( size_t i = 0; i < nrows; ++ i ) {
  //   partition.clusters[i] = 0;
  //   BOOST_CHECK_EQUAL( partition.medoid_of(1), bo.first );
  // }

  // BOOST_CHECK_EQUAL( dist );
  // for ( size_t i = 0; i < nrows; ++i ) {
  //   for ( size_t j = 0; j < ncols; ++j ) {
  //     //data[i][j] = i*1.0/1000;
  //     BOOST_CHECK_EQUAL( dist(i,j), dist(j,i) );
  //   }
  // }


  // std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  // // std::pair<ObjectId, double> bg = object_best_gain( partition, data, dist );
  // for ( size_t i = 0; i < 2000; ++i ) {
  //   partition.insert_medoid( i );
  // }
  // assign_objects_to_clusters( partition, data, dist );

  // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // std::cout << "Algo (sequential) took "
  //           << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  //           << " (ms).\n";
 //  std::cout << bg.first << ","
  //            << bg.second << std::endl;
}

BOOST_AUTO_TEST_CASE( Test_Assign ) {
  
  std::vector<double> a { 10.0, 10.0 }; // 0
  std::vector<double> a1 { 11.0, 11.0 };
  std::vector<double> a2 { 9.0, 11.0 };
  std::vector<double> a3 { 10.0, 12.0 };
  std::vector<double> a4 { 10.2, 11.0 };

  std::vector<double> b { 20.0, 20.0 }; // 5
  std::vector<double> b1 { 20.5, 20.0 };
  std::vector<double> b2 { 21.0, 22.0 };
  std::vector<double> b3 { 17.0, 21.0 };

  std::vector<double> c { 30.0, 30.0 }; // 9
  std::vector<double> c1 { 25.0, 31.0 };
  std::vector<double> c2 { 33.0, 32.0 };

  std::vector< std::vector<double> > data { a, a1, a2, a3, a4, b, b1, b2, b3, c, c1, c2 };
  Diss dist(data);

  PAM_Partition partition( data.size() );
  //partition.clear( data.size() );
  BOOST_CHECK_EQUAL( 1, partition.n_clusters() );

  partition.insert_medoid(5).insert_medoid(9);
  // for ( size_t cid = 0; cid < partition.n_clusters(); ++cid ) {
  //   // BOOST_CHECK_EQUAL( partition.cluster_of(0), 0);
  //   std::cout << cid << " has the medoid of: " <<  partition.medoid_of(cid) << std::endl;
  // }
  
  // BOOST_CHECK_EQUAL( data.size(), partition.n_objects() );
  // BOOST_CHECK_EQUAL( 3, partition.n_clusters() );
  // BOOST_CHECK_EQUAL( data.size(), partition.n_objects() );  
  
  assign_objects_to_clusters( partition, data, dist );

  for ( size_t oid = 0; oid < 5; ++ oid ) {
    ClusterId cid =  partition.cluster_of(oid);
    BOOST_CHECK_EQUAL( cid, 0 );  
    BOOST_CHECK_EQUAL( 0, partition.medoid_of(cid) );  
  }

  for ( size_t oid = 5; oid < 9; ++ oid ) {
    ClusterId cid =  partition.cluster_of(oid);
    BOOST_CHECK_EQUAL( cid, 1 );  
    BOOST_CHECK_EQUAL( 5, partition.medoid_of(cid) );
  }

   for ( size_t oid = 9; oid < 12; ++ oid ) {
    ClusterId cid = partition.cluster_of(oid);
     BOOST_CHECK_EQUAL( cid, 2 );  
     BOOST_CHECK_EQUAL( 9, partition.medoid_of(cid) );  
   }
  

 for ( size_t oid = 0; oid < partition.n_objects(); ++ oid ) {
     ClusterId cid =  partition.cluster_of(oid);
     // std::cout << oid << " belongs to cluster: " <<  cid << " of medoid: " << partition.medoid_of(cid) << std::endl;
  }

  // std::cout << dist( 6,  5 ) << ", " << dist( 6, 0 ) << std::endl;
}


BOOST_AUTO_TEST_CASE( Test_Best_Gain ) {
  std::vector<double> a1 { 11.0 }; // 0
  std::vector<double> a2 { 12.0 };
  std::vector<double> a3 { 13.0 };
  std::vector<double> a4 { 14.0 };
  std::vector<double> a5 { 15.1 };
  std::vector<double> a6 { 16.0 };
  std::vector<double> a7 { 17.0 };

  std::vector< std::vector<double> > data { a1, a2, a3, a4, a5, a6, a7 };
  Diss dist(data);

  PAM_Partition partition( data.size() );


  std::pair<ObjectId, double> bg = object_best_gain( partition, data, dist );
  BOOST_CHECK_EQUAL( bg.first, 4);
}


BOOST_AUTO_TEST_CASE( Test_Data_Leval_Tolerance ) {

  size_t nrows = 300, ncols = 350;
  std::vector< std::vector<double> > data( nrows, std::vector<double>(ncols, 0.0) );  
  for ( size_t i = 0; i < nrows; ++i ) {
    for ( size_t j = 0; j < ncols; ++j )
      data[i][j] = 1.0;
  }

  Diss dist(data);
  
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();  
  double tolerance = get_data_level_tolerance( 1, data, dist );
  //std::cout << "tolerance: " << tolerance << std::endl;
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();  

  std::cout << "Algo (sequential) took "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
            << " (ms).\n";
  
}

BOOST_AUTO_TEST_CASE( Test_Second_Medoid ) {
  std::vector<double> a { 10.0, 10.0 }; // 0
  std::vector<double> a1 { 11.0, 11.0 };  std::vector<double> a2 { 9.0, 11.0 };
  std::vector<double> a3 { 10.0, 12.0 };  std::vector<double> a4 { 10.2, 11.0 };

  std::vector<double> b { 20.0, 20.0 }; // 5
  std::vector<double> b1 { 19.5, 18.0 }; // 6
  std::vector<double> b2 { 21.0, 22.0 }; // 7
  std::vector<double> b3 { 23.0, 21.0 };

  std::vector<double> c { 30.0, 30.0 }; // 9
  std::vector<double> c1 { 27.0, 31.0 };
  std::vector<double> c2 { 33.0, 32.0 };

  std::vector< std::vector<double> > data { a, a1, a2, a3, a4, b, b1, b2, b3, c, c1, c2 };
  Diss dist(data);

  std::vector<MedoidId> medoids { 0, 5, 9 };

  PAM_Partition partition( data.size() );
  partition.medoids = medoids;

  auto rs_b1 = find_object_clusters( 6, partition, data, dist );
  BOOST_CHECK_EQUAL( rs_b1.first.first, 1);
  BOOST_CHECK_EQUAL( rs_b1.second.first, 0);

  auto rs_b2 = find_object_clusters( 7, partition, data, dist );
  BOOST_CHECK_EQUAL( rs_b2.first.first, 1);
  BOOST_CHECK_EQUAL( rs_b2.second.first, 2);

  auto rs_b3 = find_object_clusters( 10, partition, data, dist );
  BOOST_CHECK_EQUAL( rs_b3.first.first, 2);
  BOOST_CHECK_EQUAL( rs_b3.second.first, 1);

  auto rs_b4 = find_object_clusters( 11, partition, data, dist );
  BOOST_CHECK_EQUAL( rs_b4.first.first, 2);
  BOOST_CHECK_EQUAL( rs_b4.second.first, 1);

  // std::cout << dist( 6, 2) << ", " << dist(6,5) << std::endl;
  // std::cout << rs_b1.first.second << std::endl;
  
}

BOOST_AUTO_TEST_CASE( Test_Swap_Gain ) {
  std::vector<double> a { 10.0, 10.0 }; // 0
  std::vector<double> a1 { 11.0, 10.0 }; // 4+1=5
  std::vector<double> a2 { 9.0, 11.0 }; // 1+1=2
  std::vector<double> a3 { -1.0, -1.0 };
  std::vector<double> a4 { 10.2, 11.0 };

  std::vector<double> b { 20.0, 20.0 }; // 5
  std::vector<double> b1 { 19.5, 18.0 }; // 6
  std::vector<double> b2 { 21.0, 22.0 }; // 7
  std::vector<double> b3 { 23.0, 21.0 };

  std::vector<double> c { 30.0, 30.0 }; // 9
  std::vector<double> c1 { 27.0, 31.0 };
  std::vector<double> c2 { 33.0, 32.0 };

  std::vector< std::vector<double> > data { a, a1, a2, a3, a4, b, b1, b2, b3, c, c1, c2 };
  Diss dist(data);

  std::vector<MedoidId> medoids { 0, 5, 9 };
  PAM_Partition partition( data.size() );
  partition.medoids = medoids;
  assign_objects_to_clusters( partition, data, dist );
  
  ClusterId cluster = 0;
  ObjectId candidate = 1;
  ObjectId obj = 0;
  
  // NO IMPACT: MEDOID
  cluster = 0; candidate = 1; obj = 0;
  BOOST_CHECK_EQUAL( swap_gain( partition, cluster, candidate, obj, data, dist ), 0.0 ); 
  
  // NO IMPACT: j == h
  cluster = 0; candidate = 1; obj = 1;
  BOOST_CHECK_EQUAL( swap_gain( partition, cluster, candidate, obj, data, dist ), 0.0 );

  // CLUSTER 0: SAME CLUSTER, CANDIDATE BECOMES MEDOID
  cluster = 0; candidate = 1; obj = 2;  
  BOOST_CHECK_EQUAL( swap_gain( partition, cluster, candidate, obj, data, dist ), -3.0 );

  // CLUSTER 0: SAME CLUSTER, SECOND BECOMES NEDOID
  cluster = 0; candidate = 3; obj = 1;
  // std::cout << dist(0,1) - dist(5,1) <<  std::endl;
  BOOST_CHECK_EQUAL( swap_gain( partition, cluster, candidate, obj, data, dist ), -180.0 );
  
}

BOOST_AUTO_TEST_CASE( Test_Clustering ) {
  std::cout << "TESTING...." << std::endl;
  std::vector<double> a { 10.0, 10.0 }; // 0
  std::vector<double> a1 { 11.0, 10.0 }; // 4+1=5
  std::vector<double> a2 { 9.0, 11.0 }; // 1+1=2
  std::vector<double> a3 { -1.0, -1.0 }; 
  std::vector<double> a4 { 10.2, 11.0 };

  std::vector<double> b { 20.0, 20.0 }; // 5
  std::vector<double> b1 { 19.5, 18.0 }; // 6
  std::vector<double> b2 { 21.0, 22.0 }; // 7
  std::vector<double> b3 { 23.0, 21.0 };

  std::vector<double> c { 30.0, 30.0 }; // 9
  std::vector<double> c1 { 27.0, 31.0 }; 
  std::vector<double> c2 { 33.0, 32.0 };

  std::vector< std::vector<double> > data { a, a1, a2, a3, a4, b, b1, b2, b3, c, c1, c2 };
  Diss dist(data);

  // std::vector<MedoidId> medoids { 0, 5, 9 };
  // PAM_Partition partition( data.size() );
  // partition.medoids = medoids;

  PAM pam(0.0);
  PAM_Partition partition = pam( data, dist, 3, 3 );
  // std::cout << partition << std::endl;  
}

BOOST_AUTO_TEST_CASE( Test_Clustering_1 ) {
 
}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
