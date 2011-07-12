//
//  LocationDelegateWrapper.h
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 01/07/11.
//  Copyright 2011 Artifact. All rights reserved.
//
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#endif
#import "CCLocationManagerDelegate.h"
#import <UIKit/UIKit.h>

@interface LocationDispatcher : NSObject<CLLocationManagerDelegate> 
{
  NSMutableArray *delegateWrappers;
  CLLocationManager *locationManager;
}

@property(readwrite, retain) NSMutableArray* delegateWrappers;

+ (id) sharedLocationDispatcher;
- (id) init;
- (void) addDelegate: (cocos2d::CCLocationManagerDelegate *) delegate;
- (void) removeDelegate: (cocos2d::CCLocationManagerDelegate *) delegate;

@end

@interface LocationDelegateWrapper : NSObject{
  cocos2d::CCLocationManagerDelegate *delegate_;
}

@property(readwrite) cocos2d::CCLocationManagerDelegate *delegate_;

+ (id) delegateWrapperWithDelegate:(cocos2d::CCLocationManagerDelegate *)delegate;
- (id) initWithDelegate: (cocos2d::CCLocationManagerDelegate *)delegate;
- (void) didUpdateLocation:(cocos2d::CCLocation *)newLocation;
- (void) didUpdateHeading:(cocos2d::CCHeading *)newHeading;

@end
