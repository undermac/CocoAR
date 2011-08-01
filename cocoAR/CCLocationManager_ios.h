//
//  CCLocationManager_ios.h
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 01/07/11.
//  Copyright 2011 Artifact. All rights reserved.
//

//#ifndef __PLATFORM_IPHONE_CCACCELEROMETER_H__
//#define __PLATFORM_IPHONE_CCACCELEROMETER_H__

#include "CCLocationManagerDelegate.h"

namespace   cocos2d {
  
  class CCLocationManager
  {
  public:
    CCLocationManager();
    ~CCLocationManager();
    
    static CCLocationManager* sharedCCLocationManager();
    
    double distanceFromLocation (double latitude1, double longitude1, double latitude2, double lontitude2);
    
    void removeDelegate(CCLocationManagerDelegate* pDelegate);
    void addDelegate(CCLocationManagerDelegate* pDelegate);
    
  private:
    static CCLocationManager* s_spCLLocationManager;
    static CCLocation* s_spCCLocation;
  };
  
}//namespace   cocos2d 

//#endif
