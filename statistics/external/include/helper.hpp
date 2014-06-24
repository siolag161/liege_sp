

//////////////////////////////////////////////////////////////////
//                                                              //
//           PLINK (c) 2005-2009 Shaun Purcell                  //
//                                                              //
// This file is distributed under the GNU General Public        //
// License, Version 2.  Please see the file COPYING for more    //
// details                                                      //
//                                                              //
//////////////////////////////////////////////////////////////////


#ifndef __HELPER_H__
#define __HELPER_H__

#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <sstream>

//#include "plink.h"
//#include "options.h"

template<class T>
inline const T SQR(const T a) {return a*a;}
 
template<class T>
inline const T MAX(const T &a, const T &b)
        {return b > a ? (b) : (a);}
 
template<class T>
inline const T MIN(const T &a, const T &b)
        {return b < a ? (b) : (a);}
 
template<class T>
inline const T SIGN(const T &a, const T &b)
        {return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);}
 
template<class T>
inline void SWAP(T &a, T &b)
        {T dum=a; a=b; b=dum;}


void error(std::string msg);

std::string int2str(int);
std::string dbl2str(double,int prc = -1);
std::string dbl2str_fixed(double, int prc = -1);
std::string longint2str(long int);
std::string sw(std::string s , int n);
std::string sw(double d , int n);
std::string sw(double d , int f, int n);
std::string sw(int i , int n);

std::string itoa(int, int);

bool realnum(double d);


#endif
