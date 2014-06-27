#include <cstdio>
#include "maths.hpp"
#include "cost_function.hpp"
#include "logistic_regression.hpp"
#include <iostream>
#include <cstdlib>

namespace clustering{

lbfgsfloatval_t LogisticRegression::evaluate( const lbfgsfloatval_t *theta,
                                              lbfgsfloatval_t *grad,
                                              const int K,
                                              const lbfgsfloatval_t step )
{
  if( !grad ) {
     grad = lbfgs_malloc(K);
     for ( int k = 0; k < K; ++k ) grad[k] = 1;

  }
  lbfgs2Eigen( e_theta, theta ); //copy from theta -> e_theta
  
  CostFunction costFunction;  
  double cost = costFunction( e_grad, e_theta, m_X, m_y );  
  eigen2Lbfgs( grad, e_grad );  // copy from e_grad -> grad

  return cost;
}


int LogisticRegression::progress( const lbfgsfloatval_t *theta,
                                  const lbfgsfloatval_t *grad,
                                  const lbfgsfloatval_t fx,
                                  const lbfgsfloatval_t xnorm,
                                  const lbfgsfloatval_t gnorm,
                                  const lbfgsfloatval_t step,
                                  int K, int iter, int nEva )
{
  // std::cout << "iter: " << iter << Kstd::endl;
  // printf("Iteration %d:\n", iter);
  // printf("  fx = %f\n", fx);
  // // printf("  xnorm = %f, gnorm = %f, step = %f\n", xnorm, gnorm, step);
  // printf("\n");
  return 0;
}

int LogisticRegression::execute( const int maxIter )
{
  const unsigned K = m_X.cols();
  m_theta = lbfgs_malloc(K);
  // memset( m_theta, 1, K );
  for ( int k = 0; k < K; ++k ) m_theta[k] = 0;
  lbfgsfloatval_t fx;
  int code =  lbfgs( K, m_theta, &fx, _evaluate, _progress, this, NULL);
  std::cout << "finished: " << fx << std::endl;
  return code;
}

void LogisticRegression::eigen2Lbfgs( lbfgsfloatval_t *lbgfs, const Eigen::Matrix<double, Eigen::Dynamic, 1>& eigen ) {
  for ( size_t i = 0; i < eigen.rows(); ++i ) {
    lbgfs[i] = eigen(i,0);
  }
}

void LogisticRegression::lbfgs2Eigen( Eigen::Matrix<double, Eigen::Dynamic, 1>& eigen, const lbfgsfloatval_t *lbfgs ) {
  for ( size_t i = 0; i < eigen.rows(); ++i ) {
    eigen(i,0) = lbfgs[i];
  }
}

}
