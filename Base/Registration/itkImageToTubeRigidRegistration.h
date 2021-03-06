/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 ( the "License" );
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#ifndef __itkImageToTubeRigidRegistration_h
#define __itkImageToTubeRigidRegistration_h

#include "itkTransform.h"
#include "itkImageToSpatialObjectRegistrationMethod.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageToTubeRigidMetric.h"
#include "itkGradientDescentOptimizer.h"
#include "itkImage.h"
#include "itkLevenbergMarquardtOptimizer.h"
#include "itkConjugateGradientOptimizer.h"
#include "itkOnePlusOneEvolutionaryOptimizer.h"
#include "itkEuler3DTransform.h"
#include "itkImageRegionIterator.h"
#include "itkVectorContainer.h"
#include "itkTubeSpatialObject.h"
#include "itkCommand.h"

namespace itk
{

/** \class ImageToTubeRigidRegistration
 * \brief Base class for registration methods
 *
 * This Class define the generic interface for a registration method.
 * The basic elements of a registration method are:
 *   - Metric to compare the FixedImage and the TMovingTube
 *   - Transformation used to register the FixedImage against the TMovingTube
 *   - Optimization method used to search for the best transformation
 *
 * Registration is not limited to Images, and for this reason
 * this class is templated over the type of the FixedImage object,
 * the TMovingTube object and the transformation. This types are obtained
 * from the Metric type, to reduce the number of redundant
 * template parameters
 *
 *  \ingroup AffineImageRegistration
 */

template <class TFixedImage, class TMovingTube>
class ITK_EXPORT ImageToTubeRigidRegistration
: public ImageToSpatialObjectRegistrationMethod<TFixedImage, TMovingTube>
{
public:
  typedef ImageToTubeRigidRegistration                  Self;
  typedef ImageToSpatialObjectRegistrationMethod<TFixedImage, TMovingTube>
                                                        Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  typedef typename Superclass::FixedImageType           FixedImageType;

  /** Typedef of the mask image */
  typedef Image<unsigned char, 3>                       MaskImageType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( ImageToTubeRigidRegistration,
    ImageToSpatialObjectRegistrationMethod );

  typedef ImageToTubeRigidMetric<FixedImageType, TMovingTube>   MetricType;

  typedef typename MetricType::TransformParametersType     ParametersType;
  typedef typename MetricType::TransformType               TransformType;

  /**  Dimension of the images.  */
  enum {ImageDimension = FixedImageType::ImageDimension,
    ParametersDimension = TransformType::ParametersDimension};


  typedef typename Superclass::InterpolatorType   InterpolatorType;

  //typedef GradientDescentVariableStepOptimizer  OptimizerType;
  typedef GradientDescentOptimizer                OptimizerType;
  //typedef OnePlusOneEvolutionaryOptimizer       OptimizerType;

  /** Typedef for the optimizer observer */
  typedef typename itk::SimpleMemberCommand<Self> CommandIterationType;

  /** Get Center of Rotation */
  itk::Vector<double, 3> GetCenterOfRotation( void )
    {
    itk::Vector<double, 3> centerOfRotation;
    typename MetricType::Pointer metric = dynamic_cast<MetricType*>(
      this->GetMetric() );
    for( unsigned int i=0;i<3;i++ )
      {
      centerOfRotation[i]=metric->GetCenterOfRotation()( i );
      }
    return centerOfRotation;
    };

  /** Method that initiates the registration. */
  void StartRegistration( void );

  /** Start the sparse registration */
  void SparseRegistration( ParametersType & parameters );

  /** Set the number of iteration */
  itkSetMacro( NumberOfIteration, unsigned int );

  /** Set the learning rate */
  itkSetMacro( LearningRate, double );

  /** Set the initial position */
  void SetInitialPosition( double position[6] );

  /** Set the parameters scales */
  void SetParametersScale( double scales[6] );

  /** Set the iteration observer */
  itkSetObjectMacro( IterationCommand, CommandIterationType );

  /** Initialize the registration */
  void Initialize() throw ( ExceptionObject );

  /** Set the mask image */
  itkSetObjectMacro( MaskImage, MaskImageType );

  /** Write a matlab file to display the metric */
  void CreateMatlabMetric( const char* filename );

  void SetExtent( float extent ) {m_Extent = extent;}
  void SetVerbose( bool verbose ) {m_Verbose = verbose;}
  void SetKappa( float kappa ) {m_Kappa = kappa;}
  void SetSampling( unsigned int sampling ) {m_Sampling = sampling;}

protected:

  ImageToTubeRigidRegistration();
  virtual ~ImageToTubeRigidRegistration() {};

private:

  ImageToTubeRigidRegistration( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  typename MaskImageType::Pointer          m_MaskImage;
  unsigned int                             m_NumberOfIteration;
  typename CommandIterationType::Pointer   m_IterationCommand;
  bool                                     m_IsInitialized;
  double                                   m_LearningRate;
  ParametersType                           m_InitialPosition;
  ParametersType                           m_ParametersScale;
  float                                    m_Extent;
  float                                    m_Kappa;
  bool                                     m_Verbose;
  unsigned int                             m_Sampling;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToTubeRigidRegistration.txx"
#endif

#endif
