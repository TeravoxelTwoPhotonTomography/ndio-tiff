/** \file
 *  Helper functions for test suite.
 *
 *  @cond TEST
 */
#pragma once

#include "nd.h"

#define TOL_F32 1e-5
#include <cmath>

#define countof(e) (sizeof(e)/sizeof(*e))

template<class T> nd_t cast(nd_t a);

template<class T>
T RMSE(size_t n, T* a, T* b)
{ T ssq=0.0,t;
  for(size_t i=0;i<n;++i)
  { t = b[i]-a[i];
    ssq+=t*t;
  }
  return sqrt(ssq/n);
}

template<class T>
int firstdiff(size_t n, const T* a, const T* b)
{ for(size_t i=0;i<n;++i)
    if(a[i]!=b[i])
      return i;
  return -1;
}

template<class T>
T* zeros(size_t ndim, size_t* shape)
{ size_t i,nelem;
  nelem = shape[0];
  for(i=1;i<ndim;++i)
    nelem*=shape[i];
  T* v = new T[nelem];
  memset(v,0,nelem*sizeof(T));
}
/// @endcond
