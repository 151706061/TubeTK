/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <itkImage.h>
#include <itkFilterWatcher.h>
#include <itkExceptionObject.h>

#include "itkTubeMetaLDA.h"

int itkTubeMetaLDATest(int argc, char* argv [] )
{
  if( argc != 2 )
    {
    std::cout << "Usage: testname <tempfilename>" << std::endl;
    return EXIT_FAILURE;
    }

  vnl_vector< double > v(3);
  vnl_matrix< double > m(3, 3);
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;
  m(0,0) = 1;
  m(0,1) = 2;
  m(0,2) = 3;
  m(1,0) = 4;
  m(1,1) = 5;
  m(1,2) = 6;
  m(2,0) = 7;
  m(2,1) = 8;
  m(2,2) = 9;

  itk::tube::MetaLDA mlda1;
  mlda1.SetLDAValues( v );
  mlda1.SetLDAMatrix( m );
  if( mlda1.GetLDAValues() != v || mlda1.GetLDAMatrix() != m )
    {
    std::cout << "LDA values don't match after set"
      << std::endl;
    return EXIT_FAILURE;
    }


  itk::tube::MetaLDA mlda2( mlda1 );
  if( mlda2.GetLDAValues() != mlda1.GetLDAValues() ||
    mlda2.GetLDAMatrix() != mlda1.GetLDAMatrix() )
    {
    std::cout << "LDA values don't match after copy constructor"
      << std::endl;
    return EXIT_FAILURE;
    }

  itk::tube::MetaLDA mlda3( v, m );
  if( mlda3.GetLDAValues() != mlda1.GetLDAValues() ||
    mlda3.GetLDAMatrix() != mlda1.GetLDAMatrix() )
    {
    std::cout << "LDA values don't match after explicit constructor"
      << std::endl;
    return EXIT_FAILURE;
    }

  mlda1.Write( argv[1] );

  itk::tube::MetaLDA mlda4( argv[1] );
  if( mlda4.GetLDAValues() != mlda1.GetLDAValues() ||
    mlda4.GetLDAMatrix() != mlda1.GetLDAMatrix() )
    {
    std::cout << "LDA values don't match after write/read constructor"
      << std::endl;
    return EXIT_FAILURE;
    }

  itk::tube::MetaLDA mlda5;
  mlda5.InitializeEssential( v, m );
  if( mlda5.GetLDAValues() != mlda1.GetLDAValues() ||
    mlda5.GetLDAMatrix() != mlda1.GetLDAMatrix() )
    {
    std::cout << "LDA values don't match after InitializeEssential"
      << std::endl;
    return EXIT_FAILURE;
    }

  mlda5.Clear( );
  if( mlda5.GetLDAValues().size() != 0 )
    {
    std::cout << "LDA size not 0 after clear." << std::endl;
    return EXIT_FAILURE;
    }

  mlda5.Read( argv[1] );
  if( mlda5.GetLDAValues() != mlda1.GetLDAValues() ||
    mlda5.GetLDAMatrix() != mlda1.GetLDAMatrix() )
    {
    std::cout << "LDA values don't match after read"
      << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
