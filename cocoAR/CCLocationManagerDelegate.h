//
//  CCLocationManagerDelegate.h
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 01/07/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#ifndef ARcocos_CCLocationManagerDelegate_h
#define ARcocos_CCLocationManagerDelegate_h

namespace cocos2d {
  /**
   @brief The device gps reports values for location
   */
  typedef struct 
  {
    double longitude;
    double altitude;
    double latitude;
    
    double horizontalAccuracy;
    double verticalAccuracy;
    
    double course;
    double speed;
    
    double timestamp;
  } CCLocation;
  
  typedef struct{
    double headingAccuracy;
    
    double magneticHeading;
    double trueHeading;
    
    double rawX;
    double rawY;
    double rawZ;
  } CCHeading;

  class CCLocationManagerDelegate
  {
  public:
    virtual void updateLocation(CCLocation* newLocation) {}
    virtual void updateHeading(CCHeading* newHeading) {}
    
    //! call the release() in child layer
    virtual void LocationManagerDestroy(void) {}
    //! call the retain() in child layer
    virtual void LocationManagerKeep(void) {}
  };
} // namespace cocos2d


#endif
