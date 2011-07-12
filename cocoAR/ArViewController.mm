//
//  ArViewController.m
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 29/06/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#import "ArViewController.h"

#import "cocos2d.h"
#import "EAGLView.h"
#import "CCEGLView.h"
#import <UIKit/UIKit.h>
#import "CCDirector.h"
#import "ARView.h"
#import "CCDirector.h"
@implementation ArViewController
+ (Class) layerClass
{
  return [CAEAGLLayer class];
}

- (id)init
{
    self = [super init];
    if (self) {
    }
    
    return self;
}


- (void)viewDidAppear:(BOOL)animated{
  //--------------------------------------------------------
  // [TODO] Prueba ...
  
  ARView *overlay = [[ARView alloc]
                          initWithFrame:CGRectMake(0, 0, 320, 480)];
  
  
  //-------------------------------------------------------
  
  self.wantsFullScreenLayout = YES;
  
  [[UIApplication sharedApplication] setStatusBarHidden: YES];
  
  arView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 320, 480)];
  UIImagePickerController* cameraView = [[UIImagePickerController alloc] init];
  cameraView.delegate = self;
  cameraView.navigationBarHidden = YES;
  cameraView.toolbarHidden = YES;
  cameraView.sourceType = UIImagePickerControllerSourceTypeCamera;
  cameraView.showsCameraControls = NO;
  cameraView.cameraOverlayView = overlay;
  CGFloat camScaleup = 1.54;
  cameraView.cameraViewTransform = CGAffineTransformScale(cameraView.cameraViewTransform, camScaleup, camScaleup);
  
  [self.view addSubview:arView];
  
  [self.view insertSubview:cameraView.view belowSubview:arView];
  
//  UIView *plView = [[[[[[self.view subviews] lastObject] subviews] lastObject] subviews] lastObject];
//	[[[plView subviews] objectAtIndex:3] removeFromSuperview];
  
  cocos2d::CCApplication::sharedApplication().run();
}

@end
