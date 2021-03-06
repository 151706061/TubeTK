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
#ifndef __itkDifferenceImageFilter2_h
#define __itkDifferenceImageFilter2_h

#include "itkImageToImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace itk
{

/** \class DifferenceImageFilter2
 * \brief Implements comparison between two images.
 *
 * This filter is used by the testing system to compute the difference between
 * a valid image and an image produced by the test. The comparison value is
 * computed by visiting all the pixels in the baseline images and comparing
 * their values with the pixel values in the neighborhood of the homologous
 * pixel in the other image.
 *
 * \ingroup IntensityImageFilters   Multithreaded
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT DifferenceImageFilter2 :
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef DifferenceImageFilter2                           Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>    Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DifferenceImageFilter2, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                                         InputImageType;
  typedef TOutputImage                                        OutputImageType;
  typedef typename OutputImageType::PixelType                 OutputPixelType;
  typedef typename OutputImageType::RegionType                OutputImageRegionType;
  typedef typename NumericTraits<OutputPixelType>::RealType   RealType;
  typedef typename NumericTraits<RealType>::AccumulateType    AccumulateType;

  /** Set the valid image input.  This will be input 0.  */
  virtual void SetValidInput(const InputImageType* validImage);

  /** Set the test image input.  This will be input 1.  */
  virtual void SetTestInput(const InputImageType* testImage);

  /** Set/Get the maximum distance away to look for a matching pixel.
      Default is 0. */
  itkSetMacro(ToleranceRadius, int);
  itkGetConstMacro(ToleranceRadius, int);

  /** Set/Get the minimum threshold for pixels to be different.
      Default is 0. */
  itkSetMacro(DifferenceThreshold, OutputPixelType);
  itkGetConstMacro(DifferenceThreshold, OutputPixelType);

  /** Set/Get ignore boundary pixels.  Useful when resampling may have
   *    introduced difference pixel values along the image edge
   *    Default = false */
  itkSetMacro(IgnoreBoundaryPixels, bool);
  itkGetConstMacro(IgnoreBoundaryPixels, bool);

  /** Get parameters of the difference image after execution.  */
  itkGetConstMacro(MeanDifference, RealType);
  itkGetConstMacro(TotalDifference, AccumulateType);
  itkGetConstMacro(NumberOfPixelsWithDifferences, unsigned long);

protected:
  DifferenceImageFilter2();
  virtual ~DifferenceImageFilter2() {}

  void PrintSelf(std::ostream& os, Indent indent) const;

  /** DifferenceImageFilter2 can be implemented as a multithreaded
   * filter.  Therefore, this implementation provides a
   * ThreadedGenerateData() routine which is called for each
   * processing thread. The output image data is allocated
   * automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to
   * the portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& threadRegion,
#if ITK_VERSION_MAJOR == 3
                              int threadId);
#else
                              ThreadIdType threadId);
#endif

  void BeforeThreadedGenerateData();
  void AfterThreadedGenerateData();

  OutputPixelType          m_DifferenceThreshold;
  RealType                 m_MeanDifference;
  AccumulateType           m_TotalDifference;
  unsigned long            m_NumberOfPixelsWithDifferences;
  int                      m_ToleranceRadius;

  Array<AccumulateType>    m_ThreadDifferenceSum;
  Array<unsigned long>     m_ThreadNumberOfPixelsWithDifferences;

private:
  DifferenceImageFilter2(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_IgnoreBoundaryPixels;
};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_DifferenceImageFilter2(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT DifferenceImageFilter2< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef DifferenceImageFilter2< ITK_TEMPLATE_2 x > \
                                            DifferenceImageFilter2##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkDifferenceImageFilter2+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkDifferenceImageFilter2.txx"
#endif

#endif
