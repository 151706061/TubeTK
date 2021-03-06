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
#define ITK_LEAN_AND_MEAN

#include "itkImage.h"

#include "itkTubeCVTImageFilter.h"
//#include "itkTubeLabelOverlapMeasuresImageFilter.h"
#include "itkTubeOtsuThresholdMaskedImageFilter.h"
#include "itkTubePDFSegmenter.h"
#include "itkTubeRadiusExtractor.h"
#include "itkTubeRidgeExtractor.h"
#include "itkTubeRidgeSeedGenerator.h"
#include "itkTubeTubeExtractor.h"
#include "itkTubeTubeNetworkExtractor.h"

int tubeBaseSegmentationPrintTest( int, char* [] )
{
  typedef itk::Image< float, 2 > ImageType;

  itk::tube::CVTImageFilter< ImageType >::Pointer
    cvtObject = itk::tube::CVTImageFilter< ImageType >::New();
  std::cout << "-------------itkTubeCVTImageFilter" << cvtObject
    << std::endl;

  //itk::tube::LabelOverlapMeasuresImageFilter< ImageType >::Pointer
    //loObject =
    //itk::tube::LabelOverlapMeasuresImageFilter< ImageType >::New();
  //std::cout << "-------------itkTubeLabelOverlapMeasuresImageFilter"
    //<< loObject << std::endl;

  itk::tube::OtsuThresholdMaskedImageFilter< ImageType, ImageType >::Pointer
    otsuObject = itk::tube::OtsuThresholdMaskedImageFilter< ImageType,
      ImageType >::New();
  std::cout << "-------------itkTubeOtsuThresholdMaskedImageFilter"
    << otsuObject << std::endl;

  itk::tube::PDFSegmenter< ImageType, 3, ImageType >::Pointer
    pdfObject = itk::tube::PDFSegmenter< ImageType, 3, ImageType >::New();
  std::cout << "-------------itkTubePDFImageFilter" << pdfObject
    << std::endl;

  itk::tube::RadiusExtractor< ImageType >::Pointer
    radiusObject = itk::tube::RadiusExtractor< ImageType >::New();
  std::cout << "-------------itkTubeRadiusExtractor" << radiusObject
    << std::endl;

  itk::tube::RidgeExtractor< ImageType >::Pointer
    ridgeObject = itk::tube::RidgeExtractor< ImageType >::New();
  std::cout << "-------------itkTubeRidgeExtractor" << ridgeObject
    << std::endl;

  itk::tube::RidgeSeedGenerator< ImageType, ImageType >::Pointer
    ridgeSeedObject =
    itk::tube::RidgeSeedGenerator< ImageType, ImageType >::New();
  std::cout << "-------------itkTubeRidgeSeedGenerator"
    << ridgeSeedObject << std::endl;

  itk::tube::TubeExtractor< ImageType >::Pointer
    tubeObject = itk::tube::TubeExtractor< ImageType >::New();
  std::cout << "-------------itkTubeTubeExtractor" << tubeObject
    << std::endl;

  itk::tube::TubeNetworkExtractor< ImageType, ImageType >::Pointer
    netObject =
    itk::tube::TubeNetworkExtractor< ImageType, ImageType >::New();
  std::cout << "-------------itkTubeTubeNetworkExtractor" << netObject
    << std::endl;

  return EXIT_SUCCESS;
}
