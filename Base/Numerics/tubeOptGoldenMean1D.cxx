/*=========================================================================

Library:   TubeTK/VTree3D

Authors: Stephen Aylward, Julien Jomier, and Elizabeth Bullitt

Original implementation:
Copyright University of North Carolina, Chapel Hill, NC, USA.

Revised implementation:
Copyright Kitware Inc., Carrboro, NC, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#include "tubeOptGoldenMean1D.h"
#include <cmath>
#include <iostream>

namespace tube
{

OptGoldenMean1D::OptGoldenMean1D( )
: Optimizer1D( )
{
}

OptGoldenMean1D::OptGoldenMean1D( UserFunc<double, double> *newFuncVal )
: Optimizer1D( newFuncVal, NULL )
{
}


OptGoldenMean1D::~OptGoldenMean1D( )
{
}


void OptGoldenMean1D::use( UserFunc<double, double> * newFuncVal )
{
  Optimizer1D::use( newFuncVal, NULL );
}


bool OptGoldenMean1D::m_Extreme( double *extX, double *extVal )
{
  double maxSign = 1;
  if( m_SearchForMin )
    {
    maxSign = -1;
    }

  double v = maxSign * m_FuncVal->value( *extX );
  double prevV = v;
  double xstep = m_XStep;
  int dir = 1;

  double v1 = v-std::abs(v*0.0001);
  if( *extX+dir*xstep <= m_XMax && *extX+dir*xstep >= m_XMin )
    {
    v1 = maxSign * m_FuncVal->value( *extX+dir*xstep );
    }

  double v2 = v1-std::abs(v1*0.0001);
  if( *extX-dir*xstep <= m_XMax && *extX-dir*xstep >= m_XMin )
    {
    v2 = maxSign * m_FuncVal->value( *extX-dir*xstep );
    }

  if( v2>v1 )
    {
    dir = -1;
    v = v2;
    }
  else
    {
    v = v1;
    }

  unsigned int iter = 0;
  int dirInit = dir;
  while( v < prevV && xstep > 2 * m_Tolerance && iter < m_MaxIterations )
    {
    dir *= -1;
    if( *extX+dir*xstep <= m_XMax && *extX+dir*xstep >= m_XMin )
      {
      v = maxSign * m_FuncVal->value( *extX+dir*xstep );
      }
    else
      {
      dir *= -1;
      }
    if( v<prevV && dir == dirInit )
      {
      xstep /= 1.618;
      }
    ++iter;
    }

  prevV = v;

  *extX += dir*xstep;
  if( *extX > m_XMax )
    {
    *extX = m_XMax;
    *extVal = m_FuncVal->value( *extX );
    return false;
    }
  if( *extX < m_XMin )
    {
    *extX = m_XMin;
    *extVal = m_FuncVal->value( *extX );
    return false;
    }

  dirInit = dir;
  while( xstep > m_Tolerance && iter < m_MaxIterations )
    {
    if( *extX+dir*xstep <= m_XMax && *extX+dir*xstep >= m_XMin )
      {
      v = maxSign * m_FuncVal->value( *extX+dir*xstep );
      }
    else
      {
      if( *extX > m_XMax )
        {
        *extX = m_XMax;
        *extVal = m_FuncVal->value( *extX );
        return false;
        }
      if( *extX < m_XMin )
        {
        *extX = m_XMin;
        *extVal = m_FuncVal->value( *extX );
        return false;
        }
      }
    while( v > prevV && iter < m_MaxIterations )
      {
      dirInit = dir;
      prevV = v;
      *extX += dir*xstep;
      if( *extX > m_XMax )
        {
        *extX = m_XMax;
        *extVal = m_FuncVal->value( *extX );
        return false;
        }
      if( *extX < m_XMin )
        {
        *extX = m_XMin;
        *extVal = m_FuncVal->value( *extX );
        return false;
        }
      v = maxSign * m_FuncVal->value( *extX+dir*xstep );
      ++iter;
      }
    if( dir == dirInit )
      {
      xstep /= 1.618;
      }
    dir *= -1;
    }

  *extVal = maxSign * prevV;
  return true;
}


} // namespace tube
