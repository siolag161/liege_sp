/****************************************************************************************
 * File: cost_function.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 26/06/2014

 ***************************************************************************************/
#ifndef CLUSTERING_COST_FUNCTION_HPP
#define CLUSTERING_COST_FUNCTION_HPP

#include <Eigen/Dense>
#include "maths.hpp"
namespace clustering
{

struct CostFunction {

  template<typename T>
  double cost( Eigen::Matrix<T, Eigen::Dynamic, 1>& h_theta,
               const Eigen::Matrix<T, Eigen::Dynamic, 1>& theta,
               const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& X,
               const Eigen::Matrix<T, Eigen::Dynamic, 1>& y) const {

    const int N = X.rows(); // number of training examples
    typedef Eigen::VectorXd Vec; 
    Vec onesN = maths::ones<T>(N);
    Vec xtheta = X*theta;
    maths::Sigmoid()( h_theta, xtheta );
    double firstTerm = -y.dot( maths::log<Vec>(h_theta) );
    double secondTerm = -(onesN - y).dot( maths::log<Vec>( onesN - h_theta) );
    double j = ( firstTerm + secondTerm );
    return j / N;
  }
  
template<typename T>
  double operator()( Eigen::Matrix<T, Eigen::Dynamic, 1>& grad,
                     const Eigen::Matrix<T, Eigen::Dynamic, 1>& theta,
                     const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& X,
                     const Eigen::Matrix<T, Eigen::Dynamic, 1>& y) const {

    const int N = X.rows(); // number of training examples
    const int K = X.cols(); // number of features
    typedef Eigen::VectorXd Vec; 
    grad.resize(theta.rows(), theta.cols());
    grad.setZero();
    Vec h_theta;    
    double c = cost( h_theta, theta, X, y );    
    for ( size_t i = 0; i < N; ++i ) {
      double t = h_theta(i) - y(i);
      for ( size_t j = 0; j < K; ++j ) {
        grad(j) +=  1./N * t * X(i,j);
      }
    }
    return c;
  }
};


} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_COST_FUNCTION_HPP
