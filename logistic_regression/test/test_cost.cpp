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
#include "cost_function.hpp"

using namespace clustering;
class Data   
{ 
};

// enum {
//     /** L-BFGS reaches convergence. */
//     LBFGS_SUCCESS = 0,
//     LBFGS_CONVERGENCE = 0,
//     LBFGS_STOP,
//     /** The initial variables already minimize the objective function. */
//     LBFGS_ALREADY_MINIMIZED,

//     /** Unknown error. */
//     LBFGSERR_UNKNOWNERROR = -1024,
//     /** Logic error. */
//     LBFGSERR_LOGICERROR,
//     /** Insufficient memory. */
//     LBFGSERR_OUTOFMEMORY,
//     /** The minimization process has been canceled. */
//     LBFGSERR_CANCELED,
//     /** Invalid number of variables specified. */
//     LBFGSERR_INVALID_N,
//     /** Invalid number of variables (for SSE) specified. */
//     LBFGSERR_INVALID_N_SSE,
//     /** The array x must be aligned to 16 (for SSE). */
//     LBFGSERR_INVALID_X_SSE,
//     /** Invalid parameter lbfgs_parameter_t::epsilon specified. */
//     LBFGSERR_INVALID_EPSILON,
//     /** Invalid parameter lbfgs_parameter_t::past specified. */
//     LBFGSERR_INVALID_TESTPERIOD,
//     /** Invalid parameter lbfgs_parameter_t::delta specified. */
//     LBFGSERR_INVALID_DELTA,
//     /** Invalid parameter lbfgs_parameter_t::linesearch specified. */
//     LBFGSERR_INVALID_LINESEARCH,
//     /** Invalid parameter lbfgs_parameter_t::max_step specified. */
//     LBFGSERR_INVALID_MINSTEP,
//     /** Invalid parameter lbfgs_parameter_t::max_step specified. */
//     LBFGSERR_INVALID_MAXSTEP,
//     /** Invalid parameter lbfgs_parameter_t::ftol specified. */
//     LBFGSERR_INVALID_FTOL,
//     /** Invalid parameter lbfgs_parameter_t::wolfe specified. */
//     LBFGSERR_INVALID_WOLFE,
//     /** Invalid parameter lbfgs_parameter_t::gtol specified. */
//     LBFGSERR_INVALID_GTOL,
//     /** Invalid parameter lbfgs_parameter_t::xtol specified. */
//     LBFGSERR_INVALID_XTOL,
//     /** Invalid parameter lbfgs_parameter_t::max_linesearch specified. */
//     LBFGSERR_INVALID_MAXLINESEARCH,
//     /** Invalid parameter lbfgs_parameter_t::orthantwise_c specified. */
//     LBFGSERR_INVALID_ORTHANTWISE,
//     /** Invalid parameter lbfgs_parameter_t::orthantwise_start specified. */
//     LBFGSERR_INVALID_ORTHANTWISE_START,
//     /** Invalid parameter lbfgs_parameter_t::orthantwise_end specified. */
//     LBFGSERR_INVALID_ORTHANTWISE_END,
//     /** The line-search step went out of the interval of uncertainty. */
//     LBFGSERR_OUTOFINTERVAL,
//     /** A logic error occurred; alternatively, the interval of uncertainty
//         became too small. */
//     LBFGSERR_INCORRECT_TMINMAX,
//     /** A rounding error occurred; alternatively, no line-search step
//         satisfies the sufficient decrease and curvature conditions. */
//     LBFGSERR_ROUNDING_ERROR,
//     /** The line-search step became smaller than lbfgs_parameter_t::min_step. */
//     LBFGSERR_MINIMUMSTEP,
//     /** The line-search step became larger than lbfgs_parameter_t::max_step. */
//     LBFGSERR_MAXIMUMSTEP,
//     /** The line-search routine reaches the maximum number of evaluations. */
//     LBFGSERR_MAXIMUMLINESEARCH,
//     /** The algorithm routine reaches the maximum number of iterations. */
//     LBFGSERR_MAXIMUMITERATION,
//     /** Relative width of the interval of uncertainty is at most
//         lbfgs_parameter_t::xtol. */
//     LBFGSERR_WIDTHTOOSMALL,
//     /** A logic error (negative line-search step) occurred. */
//     LBFGSERR_INVALIDPARAMETERS,
//     /** The current search direction increases the objective function value. */
//     LBFGSERR_INCREASEGRADIENT,
// };
 
BOOST_FIXTURE_TEST_SUITE( Test_COST, Data ) 

using namespace Eigen;
BOOST_AUTO_TEST_CASE( Test_Cost_Function )  {
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

  // std::cout <<  "code? : " << LBFGS_ALREADY_MINIMIZED << std::endl;
  
  Eigen::VectorXd y;
  load_eigen_vector( y, "rs.txt");
  BOOST_CHECK_EQUAL( y.rows(), 100 );
  BOOST_CHECK_EQUAL( y.cols(), 1 );

  Matrix<double, -1, 1> theta;
  theta.resize(K,1); theta.setZero();   
  Matrix<double, -1, 1> grad;
  grad.resize(K,1);

  CostFunction costFunction;
  Matrix<double, -1, 1> h_theta;

 
  VectorXd onesN = maths::ones<double>(N);  
  VectorXd quyt = maths::log<VectorXd>(h_theta);  
  // printf("(%d, %d)\n", quyt.rows(), quyt.cols());
  //
  // std::cout << quyt << std::endl;
  // std::cout << y << std::endl;
  // std::cout << yt * quyt << std::endl;
  // double v  = 0.0;
  // for ( size_t i = 0; i < N; ++i ) {
  //   v += y(i,0)*quyt(i,0);
  //   printf("%.3f, %.3f, %\n", y(i,0), quyt(i,0) );
  //   // std::cout << v << std::endl;
  // }

  // std::cout << y.dot( maths::log<VectorXd>(h_theta) );
  
  double cost = costFunction.cost( h_theta, theta, X, y );
  // std::cout << "init cost: " << cost << std::endl;

  cost = costFunction( grad, theta, X, y );
  // std::cout << "init cost: " << cost << std::endl;

  // std::cout << theta <<  std::endl;

}



BOOST_AUTO_TEST_SUITE_END()  /// Test InfoTheo ends here
