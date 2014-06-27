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
#include "maths.hpp"

using namespace clustering;
class Data  
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_Helpers, Data ) 

BOOST_AUTO_TEST_CASE( Test_Eigen_Scalar_Product )  {
  // Eigen::Vector3d x(1,2,3);
  // Eigen::Vector3d y(1,2,3);
  // Eigen::Matrix<double, -1, -1> z;  
  // maths::sigmoid sigmoid;
  // sigmoid(z,x);
  // BOOST_CHECK_EQUAL( z.rows(), x.rows() );
  // BOOST_CHECK_EQUAL( z.cols(), x.cols() );  
  //std::cout << z << std::endl;
}

using namespace Eigen;
BOOST_AUTO_TEST_CASE( Test_Cost_Function )  {
  const int N = 3, K = 2;
  Matrix<double,N,K> X; X << 1, 2, 3, 4, 5, 6;
  // std::cout << X << std::endl << std::endl;
  Matrix<double,N,1> y( 1,2,3 );
  // std::cout << y << std::endl << std::endl;
  Matrix<double,K,1> theta( 1,2 );
  // std::cout << theta << std::endl << std::endl;

  // maths::log log;
  // maths::sigmoid sigmoid;
  // sigmoid.compute< Matrix<double,N,K> >(X);
  // 
  // Matrix<double,N,K> X1;
 
  // std::cout << X1 << std::endl;
  //     // maths::matSigmoid< Matrix<
  // double,N,K> > (X);
  // maths::matSigmoid( const EigMatIn& in) {
  // double cost = 1.0/N*( prod(-y, log(sigmoid(X)) ) );

  // typedef  Matrix<double,N,K> MatNK;
  // auto left =  rod( -y, maths::log<MatNK>(maths::s gmoid<MatNK>(X) );
}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
