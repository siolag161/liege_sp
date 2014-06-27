/****************************************************************************************
 * File: logistic_regression.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_LOGISTIC_REGRESSION_HPP
#define CLUSTERING_LOGISTIC_REGRESSION_HPP 

#include <lbfgs.h>
#include <cstdio>
namespace clustering
{

struct LogisticRegression {


 public:
  lbfgsfloatval_t evaluate( const lbfgsfloatval_t *theta, lbfgsfloatval_t *grad, const int K, const lbfgsfloatval_t step );
  static lbfgsfloatval_t _evaluate( void *instance, const lbfgsfloatval_t *theta, lbfgsfloatval_t *grad, const int K, const lbfgsfloatval_t step ) {
    return reinterpret_cast<LogisticRegression*>(instance)->evaluate( theta, grad, K, step);

  }
  
  static int _progress(
      void *instance,
      const lbfgsfloatval_t *theta,
      const lbfgsfloatval_t *grad,
      const lbfgsfloatval_t fx,
      const lbfgsfloatval_t xnorm,
      const lbfgsfloatval_t gnorm,
      const lbfgsfloatval_t step,
      int K, int maxIter, int nEva )
  {
    return reinterpret_cast<LogisticRegression*>(instance)->progress( theta, grad, fx, xnorm, gnorm, step, K, maxIter, nEva);
  }

  int progress( const lbfgsfloatval_t *theta,
                const lbfgsfloatval_t *grad,
                const lbfgsfloatval_t fx,
                const lbfgsfloatval_t xnorm,
                const lbfgsfloatval_t gnorm,
                const lbfgsfloatval_t step,
                int N, int maxIter, int nEva );

  LogisticRegression( Eigen::MatrixXd& X,
                      Eigen::VectorXd& y ) : m_X(X), m_y(y), m_theta(NULL) {

    const unsigned K = X.cols();
    e_theta.resize(K,1); e_theta.setZero();   
    e_grad.resize(K,1); e_grad.setZero();    
  }
  
  virtual ~LogisticRegression()
  {
    if (m_theta != NULL) {
      lbfgs_free(m_theta);
      m_theta = NULL;
    }
  }

  int execute( const int maxIter );  

  void eigen2Lbfgs( lbfgsfloatval_t *lbgfs, const Eigen::Matrix<double, Eigen::Dynamic, 1>& eigen );
  void lbfgs2Eigen( Eigen::Matrix<double, Eigen::Dynamic, 1>& eigen, const lbfgsfloatval_t *lbfgs );
  
  lbfgsfloatval_t *m_theta;
  int m_iter;

  Eigen::MatrixXd& m_X;
  Eigen::VectorXd& m_y;
  
  Eigen::VectorXd e_theta;
  Eigen::VectorXd e_grad;
  
};

} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_LOGISTIC_REGRESSION_HPP
