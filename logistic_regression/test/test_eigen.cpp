#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE
#endif  
#include <boost/test/unit_test.hpp>

#include <omp.h>

#include <vector>
#include <lbfgs.h>

#include <Eigen/Dense>
#include "load_eigen.hpp"

using namespace clustering;
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_LR_Eigen, Data ) 

BOOST_AUTO_TEST_CASE( Test_Eigen_Load)  {
  Eigen::MatrixXd matrix;
  load_eigen_matrix( matrix, "data.txt");
  BOOST_CHECK_EQUAL( matrix.rows(), 100 );
  BOOST_CHECK_EQUAL( matrix.cols(), 2 );

  Eigen::VectorXi vec;
  load_eigen_vector( vec, "rs.txt");
  BOOST_CHECK_EQUAL( vec.rows(), 100 );
  BOOST_CHECK_EQUAL( vec.cols(), 1 );  
}


BOOST_AUTO_TEST_CASE( Test_Eigen_Scalar_Product )  {
  Eigen::Vector3d x(1,2,3);
  Eigen::Vector3d y(1,2,3);
  Eigen::Matrix<double, 1,3> z(1,2,3);
  
  double prod = x.dot(y);
  BOOST_CHECK_EQUAL( prod, 14 );

  prod = x.dot(z.transpose());
  BOOST_CHECK_EQUAL( prod, 14 );
}


BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
