/****************************************************************************************
 * File: bic.hpp
 * Description: 
 * @author: siolag161 (thanh.phan@outlook.com)
 * @date: 13/06/2014
 * Based on the code of the Muster Project, retrived http://github.com/tgamblin/muster whose license
 * given right below:
//////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010, Lawrence Livermore National Security, LLC.  
// Produced at the Lawrence Livermore National Laboratory  
// LLNL-CODE-433662
// All rights reserved.  
//
// This file is part of Muster. For details, see http://github.com/tgamblin/muster. 
// Please also read the LICENSE file for further information.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the disclaimer below.
//  * Redistributions in binary form must reproduce the above copyright notice, this list of
//    conditions and the disclaimer (as noted below) in the documentation and/or other materials
//    provided with the distribution.
//  * Neither the name of the LLNS/LLNL nor the names of its contributors may be used to endorse
//    or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// LAWRENCE LIVERMORE NATIONAL SECURITY, LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////////////////////

///
/// @file bic.h
/// @author Todd Gamblin tgamblin@llnl.gov
/// @brief Template function implementations of the Bayesian Information Criterion.
///
/// The Bayesian Information Criterion (BIC) is a criterion for model selection
/// that balances a maximum likelihood estimator and a parameter count.  This
/// implementation is designed for clustering algorithms, in particular K-means and
/// K-medoids clustering algorithms where we expect clusters with  spherical gaussian
/// distributions.  
///
/// Here, we want to test whether a clustering's centroids or medoids are good predictors
/// of the points in a data set, so these are our parameters, and we try to find the best
/// clustering without too many clusters.  For more on this technique and the approach
/// we've based this implementation on, see this paper:
/// @par
/// Dan Pelleg and Andrew Moore.  <a href="http://www.cs.cmu.edu/~dpelleg/download/xmeans.pdf">
/// <b>X-Means: Extending K-Means with Efficient Estimation of the Number of Clusters</b></a>.
/// <i>Proceedings of the Seventeenth International Conference on Machine Learning</i>, 
/// San Francisco, CA. June 29-July 2, 2000.  pp 727-734.
/// 


 ***************************************************************************************/
#ifndef CLUSTERING_BIC_HPP
#define CLUSTERING_BIC_HPP

namespace clustering
{


} // namespace clusteringends here. clustering

/****************************** IMLEMENTATION BELOW THIS POINT **************************/
namespace clustering
{


} // namespace clusteringends here. clustering

/****************************************************************************************/
#endif // CLUSTERING_BIC_HPP
