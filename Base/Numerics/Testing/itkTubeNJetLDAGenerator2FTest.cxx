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
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "itkRecursiveGaussianImageFilter.h"

#include "itkTubeNJetLDAGenerator2F.h"

int itkTubeNJetLDAGenerator2FTest(int argc, char* argv [] )
{
  if( argc != 5 )
    {
    std::cerr << "Missing arguments." << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0]
      << " inputImage inputMask outputLDA0Image outputLDA1Image"
      << std::endl;
    return EXIT_FAILURE;
    }

  // Define the dimension of the images
  const unsigned int Dimension = 2;

  // Define the pixel type
  typedef float         PixelType;
  typedef unsigned char MaskPixelType;

  // Declare the types of the images
  typedef itk::Image<PixelType, Dimension>      ImageType;
  typedef itk::Image<MaskPixelType, Dimension>  MaskImageType;

  // Declare the reader and writer
  typedef itk::ImageFileReader< ImageType > ReaderType;
  typedef itk::ImageFileReader< MaskImageType > MaskReaderType;
  typedef itk::ImageFileWriter< ImageType > WriterType;


  // Declare the type for the Filter
  typedef itk::tube::NJetLDAGenerator2F FilterType;

  // Create the reader and writer
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception caught during input read:\n"  << e;
    return EXIT_FAILURE;
    }
  ImageType::Pointer inputImage = reader->GetOutput();

  // Create the reader and writer
  MaskReaderType::Pointer maskReader = MaskReaderType::New();
  maskReader->SetFileName( argv[2] );
  try
    {
    maskReader->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception caught during input read:\n"  << e;
    return EXIT_FAILURE;
    }
  MaskImageType::Pointer maskImage = maskReader->GetOutput();

  FilterType::NJetScalesType scales(3);
  scales[0] = 0.5;
  scales[1] = 1;
  scales[2] = 4;
  FilterType::NJetScalesType scales2(1);
  scales2[0] = 3;
  FilterType::Pointer filter = FilterType::New();
  filter->SetNJetImage( inputImage );
  filter->SetLabelmap( maskImage );
  filter->SetObjectId( 255 );
  filter->AddObjectId( 127 );
  filter->SetZeroScales( scales );
  filter->SetFirstScales( scales );
  filter->SetSecondScales( scales2 );
  filter->SetRidgeScales( scales2 );
  std::cout << filter << std::endl;
  filter->Update();

  filter->SetLabelmap( NULL );
  filter->UpdateLDAImages();

  std::cout << "Number of LDA = " << filter->GetNumberOfLDA() << std::endl;
  for( unsigned int i=0; i<filter->GetNumberOfLDA(); i++ )
    {
    std::cout << "LDA" << i << " = " << filter->GetLDAValue(i) << " : "
      << filter->GetLDAVector(i) << std::endl;
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetUseCompression( true );
  writer->SetInput( filter->GetLDAImage(0) );
  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception caught during write:\n"  << e;
    return EXIT_FAILURE;
    }

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName( argv[4] );
  writer2->SetUseCompression( true );
  writer2->SetInput( filter->GetLDAImage(1) );
  try
    {
    writer2->Update();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception caught during write:\n"  << e;
    return EXIT_FAILURE;
    }

  // All objects should be automatically destroyed at this point
  return EXIT_SUCCESS;
}
