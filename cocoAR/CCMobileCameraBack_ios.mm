//
//  CCMobileCameraBack_ios.cpp
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 08/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#include "CCMobileCameraBack_ios.h"
#include "CCMobileCameraBackWrapper.h"

namespace cocos2d {
  CCMobileCameraBackManager *CCMobileCameraBackManager:: s_spCCMobileManager=NULL;
  
  CCMobileCameraBackManager::CCMobileCameraBackManager(){
  }
  CCMobileCameraBackManager::~CCMobileCameraBackManager(){
  }
  
  CCMobileCameraBackManager* CCMobileCameraBackManager::sharedCCMobileCameraBackManager()
  {
    if (s_spCCMobileManager == NULL) {
      s_spCCMobileManager = new CCMobileCameraBackManager();
    }
    return s_spCCMobileManager;
  }
  
  void CCMobileCameraBackManager::removeDelegate(CCMobileCameraBackDelegate* pDelegate)
  {
    [[MobileCameraBackDispacher sharedCameraBackDispatcher] removeDelegate:pDelegate];
  }
  void CCMobileCameraBackManager::addDelegate(CCMobileCameraBackDelegate* pDelegate)
  {
    [[MobileCameraBackDispacher sharedCameraBackDispatcher] addDelegate:pDelegate];
  }

}