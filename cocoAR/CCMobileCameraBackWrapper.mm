//
//  CCMobileCameraBackWrapper.cpp
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 08/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#include "CCMobileCameraBackWrapper.h"
#include "CCDirector.h"

@implementation MobileCameraBackDispacher

static MobileCameraBackDispacher* s_pMobileCameraBackDispacher;

@synthesize delegateWrappers;

+ (id) sharedCameraBackDispatcher
{
  if (s_pMobileCameraBackDispacher == nil) {
    s_pMobileCameraBackDispacher = [[self alloc] init];
  }
  
  return s_pMobileCameraBackDispacher;
}

- (id)init{
  camera = [[UIImagePickerController alloc] init];
  camera.delegate = self;
  camera.navigationBarHidden = YES;
  camera.toolbarHidden = YES;
  camera.sourceType = UIImagePickerControllerSourceTypeCamera;
  camera.showsCameraControls = NO;

  
// [OMG CODE HARDCORE BUG ]
//  UIView *overView = [[[[UIApplication sharedApplication] keyWindow] subviews] lastObject];
//  camera.cameraOverlayView = overView;
//  overView.userInteractionEnabled = YES;
//  CGFloat camScaleup = 1.54;
//  camera.cameraViewTransform = CGAffineTransformScale(camera.cameraViewTransform, camScaleup, camScaleup);
  
  CGAffineTransform translate = CGAffineTransformMakeTranslation(0.0, 27.0);
  camera.cameraViewTransform = translate;
  CGAffineTransform scale = CGAffineTransformScale(translate, 1.54, 1.54);
  camera.cameraViewTransform = scale;
  
  [[[UIApplication sharedApplication] keyWindow] addSubview:camera.view];
  [[[UIApplication sharedApplication] keyWindow] sendSubviewToBack:camera.view];
  
  return self;
}

- (void) dealloc
{
  [delegateWrappers release];
  [super dealloc];
}

- (id) findDelegateWrapperByDelegate: (cocos2d::CCMobileCameraBackDelegate *) delegate
{
  for (MobileCameraBackDelegateWrapper *wrapper in delegateWrappers) {
    if (wrapper.delegate_ == delegate) {
      return wrapper;
    }
  }
  
  return nil;
}

- (void) addDelegate: (cocos2d::CCMobileCameraBackDelegate *) delegate
{
  [delegateWrappers addObject: [MobileCameraBackDelegateWrapper delegateWrapperWithDelegate:delegate]];
}

- (void) removeDelegate: (cocos2d::CCMobileCameraBackDelegate *) delegate
{
  [delegateWrappers removeObject:[self findDelegateWrapperByDelegate:delegate]];
}
@end


@implementation MobileCameraBackDelegateWrapper

@synthesize delegate_;

+ (id) delegateWrapperWithDelegate:(cocos2d::CCMobileCameraBackDelegate *)delegate
{
  return [[self alloc] initWithDelegate: delegate];
}

- (id) initWithDelegate: (cocos2d::CCMobileCameraBackDelegate *)delegate
{
  //  delegate->AccelerometerKeep();
  self.delegate_ = delegate;
  
  return self;
}

-  (void) dealloc
{
  //  self.delegate_->AccelerometerDestroy();
  [super dealloc];
}

@end