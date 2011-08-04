//
//  CCLocationManager_ios.m
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 01/07/11.
//  Copyright 2011 Artifact. All rights reserved.
//

//#import "CCLocationManager_ios.h"

#include "CCLocationManager.h"
#include "LocationDelegateWrapper.h"

namespace cocos2d {
  
  CCLocationManager* CCLocationManager::s_spCLLocationManager = NULL;
  
  CCLocationManager::CCLocationManager()
  {
  }
  
  CCLocationManager::~CCLocationManager()
  {
  }
  
  CCLocationManager* CCLocationManager::sharedCCLocationManager()
  {
    if (s_spCLLocationManager == NULL) {
      s_spCLLocationManager = new CCLocationManager();
    }
    return s_spCLLocationManager;
  }
  
  double CCLocationManager::distanceFromLocation (double latitude1, double longitude1, double latitude2, double lontitude2)
  {
    CLLocation *object1Location = [[CLLocation alloc] initWithLatitude:latitude1 longitude:longitude1];
    CLLocation *object2Location = [[CLLocation alloc] initWithLatitude:latitude2 longitude:lontitude2];
    
    double distance = [object1Location distanceFromLocation:object2Location];
                    
    return distance;
  }
    
  //http://shawnsbits.com/blog/2011/07/07/calculating-heading-with-corelocation/
//  double CCLocationManager::bearingBetweenStartLocation(double latitude1, double longitude1, double latitude2, double lontitude2)
//  {
//    CLLocation *northPoint = [[[CLLocation alloc] initWithLatitude:latitude1+.01 longitude:lontitude2] autorelease];
//    
//    CLLocation *object1Location = [[CLLocation alloc] initWithLatitude:latitude1 longitude:longitude1];
//    CLLocation *object2Location = [[CLLocation alloc] initWithLatitude:latitude2 longitude:lontitude2];
//    
//    float magA = [northPoint distanceFromLocation:object1Location];
//    float magB = [object2Location distanceFromLocation:object1Location];
//    
//    
//    CLLocation *startLat = [[[CLLocation alloc] initWithLatitude:latitude1 longitude:0] autorelease];
//    CLLocation *endLat = [[[CLLocation alloc] initWithLatitude:latitude2 longitude:0] autorelease];
//    float aDotB = magA*[endLat distanceFromLocation:startLat];
//    
//    return acosf(aDotB/(magA*magB))*(180/M_PI);
//  }
  
  //http://stackoverflow.com/questions/642555/how-do-i-calculate-the-azimuth-angle-to-north-between-two-wgs84-coordinates
  double CCLocationManager::bearingBetweenStartLocation(double latitude1, double longitude1, double latitude2, double lontitude2) 
  {
    double longitudinalDifference = lontitude2 - longitude1;
    double latitudinalDifference = latitude2 - latitude1;
    double possibleAzimuth = (M_PI * .5f) - atan(latitudinalDifference / longitudinalDifference);
    if (longitudinalDifference > 0) return possibleAzimuth;
    else if (longitudinalDifference < 0) return possibleAzimuth + M_PI;
    else if (latitudinalDifference < 0) return M_PI;
    
    return 0.0f; //Not angle
  }

  void CCLocationManager::removeDelegate(CCLocationManagerDelegate* pDelegate)
  {
    [[LocationDispatcher sharedLocationDispatcher] removeDelegate:pDelegate];
  }
  void CCLocationManager::addDelegate(CCLocationManagerDelegate* pDelegate)
  {
    [[LocationDispatcher sharedLocationDispatcher] addDelegate:pDelegate];
  }
}
 