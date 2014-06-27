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
#include "logistic_regression.hpp"
#include "maths.hpp"
using namespace clustering;
class Data 
{ 
};
 
BOOST_FIXTURE_TEST_SUITE( Test_LR, Data ) 

double accuracy( const Eigen::MatrixXd& X, const Eigen::VectorXd& y, const Eigen::VectorXd& theta ) {
  const unsigned N = X.rows(), K = X.cols();
  // Eigen:::VectorXd p;
  // p.resize(N, 0); p.setZero();

  double correct = 0.0;
  Eigen::VectorXd xtheta = X*theta;
  Eigen::VectorXd predicted;
  maths::Sigmoid()( predicted, xtheta );

      
  std::cout << predicted << std::endl;
  for ( size_t i = 0; i < N; ++i ) {
    int p = predicted[i]> 0.5 ? 1 : 0;
    // printf("%d, %d, %f\n", )
    if ( p == y(i,0) ) ++correct;
  }

  return correct / N;
  
}

BOOST_AUTO_TEST_CASE( Test_Eigen_Load )  {
  Eigen::MatrixXd X_tmp;
  load_eigen_matrix( X_tmp, "data.txt");  
  const int N = X_tmp.rows();
  const int K = X_tmp.cols() + 1;  
  BOOST_CHECK_EQUAL( N, 100 );
  BOOST_CHECK_EQUAL( K, 3 );
  
  Eigen::MatrixXd X;
  X.resize(N,K);
  for ( size_t r = 0; r < N; ++r ) {
    for ( size_t c = 0; c < K-1; ++c ) {
      X(r,c+1) = X_tmp(r,c);
    }
    X(r,0) = 1;
  }
  
  Eigen::VectorXd y;
  load_eigen_vector( y, "rs.txt");
  BOOST_CHECK_EQUAL( y.rows(), 100 );
  BOOST_CHECK_EQUAL( y.cols(), 1 );

  //////////////
  std::cout << "BEGIN..............\n\n";
  LogisticRegression lg( X, y);
  int ret = lg.execute(5);

  /* Report the result. */
  printf("L-BFGS optimization terminated with status code = %d\n", ret);
  // printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, m_x[0], m_x[1]);
  // std::cout << "alibaba: " << LBFGSERR_LOGICERROR << std::endl;

  Eigen::VectorXd theta = lg.e_theta;
  // Eigen::Matrix<double, Eigen::Dynamic, 1> e_theta;
  std::cout << lg.e_theta << std::endl;
  printf("accuracy: %f\n", accuracy( X, y, theta) );
}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
