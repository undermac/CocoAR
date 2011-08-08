//
//  CCMobileCameraBackWrapper.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 08/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "CCMobileCameraBackDelegate.h"


@interface MobileCameraBackDispacher : NSObject <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{
    NSMutableArray *delegateWrappers;
    UIImagePickerController *camera;
}

@property(readwrite, retain) NSMutableArray* delegateWrappers;

+ (id) sharedCameraBackDispatcher;
- (id) init;
- (void) addDelegate: (cocos2d::CCMobileCameraBackDelegate *) delegate;
- (void) removeDelegate: (cocos2d::CCMobileCameraBackDelegate *) delegate;

@end


@interface MobileCameraBackDelegateWrapper : NSObject{
  cocos2d::CCMobileCameraBackDelegate *delegate_;
}

@property(readwrite) cocos2d::CCMobileCameraBackDelegate *delegate_;

+ (id) delegateWrapperWithDelegate:(cocos2d::CCMobileCameraBackDelegate *)delegate;
- (id) initWithDelegate: (cocos2d::CCMobileCameraBackDelegate *)delegate;

@end