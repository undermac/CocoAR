//
//  LocationDelegateWrapper.m
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 01/07/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#import "LocationDelegateWrapper.h"


@implementation LocationDispatcher

static LocationDispatcher* s_pLocationDispatcher;

@synthesize delegateWrappers;

+ (id) sharedLocationDispatcher
{
  if (s_pLocationDispatcher == nil) {
    s_pLocationDispatcher = [[self alloc] init];
  }
  
  return s_pLocationDispatcher;
}

- (id)init
{
  if ([CLLocationManager locationServicesEnabled]){
    self.delegateWrappers = [NSMutableArray arrayWithCapacity:4];
    locationManager = [[CLLocationManager alloc] init];
    locationManager.delegate = self;
    locationManager.desiredAccuracy = kCLLocationAccuracyBestForNavigation;
    locationManager.distanceFilter = kCLDistanceFilterNone;
    [locationManager startUpdatingLocation];
  }
  if ([CLLocationManager headingAvailable]) {
    locationManager.headingFilter = 1;
    [locationManager startUpdatingHeading];
  }
  return self;
}

- (CLLocation *) getPosition{
  return locationManager.location;
}

- (void) dealloc
{
  [delegateWrappers release];
  [super dealloc];
}

- (id) findDelegateWrapperByDelegate: (cocos2d::CCLocationManagerDelegate *) delegate
{
  for (LocationDelegateWrapper *wrapper in delegateWrappers) {
    if (wrapper.delegate_ == delegate) {
      return wrapper;
    }
  }
  
  return nil;
}

- (void) addDelegate: (cocos2d::CCLocationManagerDelegate *) delegate
{
  [delegateWrappers addObject: [LocationDelegateWrapper delegateWrapperWithDelegate:delegate]];
}

- (void) removeDelegate: (cocos2d::CCLocationManagerDelegate *) delegate
{
  [delegateWrappers removeObject:[self findDelegateWrapperByDelegate:delegate]];
}

- (void)locationManager:(CLLocationManager *)manager
    didUpdateToLocation:(CLLocation *)newLocation
           fromLocation:(CLLocation *)oldLocation;
{
  cocos2d::CCLocation newLocationPoint;
  
  newLocationPoint.altitude = newLocation.altitude;
  newLocationPoint.longitude = newLocation.coordinate.longitude;
  newLocationPoint.latitude = newLocation.coordinate.latitude;
  
  newLocationPoint.horizontalAccuracy = newLocation.horizontalAccuracy;
  newLocationPoint.verticalAccuracy = newLocation.verticalAccuracy;
  
  for (LocationDelegateWrapper *wrapper in delegateWrappers) {
    [wrapper didUpdateLocation:&newLocationPoint];
  }
}

- (void)locationManager:(CLLocationManager *)manager didUpdateHeading:(CLHeading *)newHeading {
  cocos2d::CCHeading newCCHeading; 
  
  newCCHeading.headingAccuracy = newHeading.headingAccuracy;
  newCCHeading.magneticHeading = newHeading.magneticHeading;
  newCCHeading.trueHeading = newHeading.trueHeading;
  
  newCCHeading.rawX = newHeading.x;
  newCCHeading.rawY = newHeading.y;
  newCCHeading.rawZ = newHeading.z;
  
  for (LocationDelegateWrapper *wrapper in delegateWrappers) {
    [wrapper didUpdateHeading:&newCCHeading];
  }
}

@end


@implementation LocationDelegateWrapper

@synthesize delegate_;

+ (id) delegateWrapperWithDelegate:(cocos2d::CCLocationManagerDelegate *)delegate
{
  return [[self alloc] initWithDelegate: delegate];
}

- (id) initWithDelegate: (cocos2d::CCLocationManagerDelegate *)delegate
{
//  delegate->AccelerometerKeep();
  self.delegate_ = delegate;
  
  return self;
}

- (void) didUpdateLocation:(cocos2d::CCLocation *)newLocation
{
  self.delegate_->updateLocation(newLocation);
}

- (void) didUpdateHeading:(cocos2d::CCHeading *)newHeading
{
  self.delegate_->updateHeading(newHeading);
}

-  (void) dealloc
{
//  self.delegate_->AccelerometerDestroy();
  [super dealloc];
}

@end

