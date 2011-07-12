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
  
  void CCLocationManager::removeDelegate(CCLocationManagerDelegate* pDelegate)
  {
    [[LocationDispatcher sharedLocationDispatcher] removeDelegate:pDelegate];
  }
  
  void CCLocationManager::addDelegate(CCLocationManagerDelegate* pDelegate)
  {
    [[LocationDispatcher sharedLocationDispatcher] addDelegate:pDelegate];
  }
}
 