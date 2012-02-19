//
//  CocoArTestAppController.h
//  CocoArTest
//
//  Created by Javier de la Peña Ojea on 08/08/11.
//  Copyright Artifact 2011. All rights reserved.
//

#import <CoreLocation/CoreLocation.h>

@class RootViewController;

@protocol MyCLControllerDelegate 
@required
- (void)locationUpdate:(CLLocation *)location;
- (void)locationError:(NSError *)error;
@end

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate,CLLocationManagerDelegate> {
  UIWindow *window;
  RootViewController	*viewController;
  CLLocationManager *locationManager;
  id delegate;
}
@property (nonatomic, retain) CLLocationManager *locationManager;
@property (nonatomic, assign) id  delegate;
@property (nonatomic, retain) UITabBarController *tab;

- (void)locationManager:(CLLocationManager *)manager
    didUpdateToLocation:(CLLocation *)newLocation
           fromLocation:(CLLocation *)oldLocation;

- (void)locationManager:(CLLocationManager *)manager
       didFailWithError:(NSError *)error;

@end

