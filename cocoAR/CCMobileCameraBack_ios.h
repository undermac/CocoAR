//
//  CCMobileCameraBack_ios.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 08/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#ifndef cocoAR_CCMobileCameraBack_ios_h
#define cocoAR_CCMobileCameraBack_ios_h

#include "CCMobileCameraBackDelegate.h"

namespace   cocos2d {
  
  class CCMobileCameraBackManager
  {
  private:
    static CCMobileCameraBackManager* s_spCCMobileManager;
  public:
    CCMobileCameraBackManager();
    ~CCMobileCameraBackManager();
    void addDelegate(CCMobileCameraBackDelegate* pDelegate);
    void removeDelegate(CCMobileCameraBackDelegate* pDelegate);
    
    static CCMobileCameraBackManager* sharedCCMobileCameraBackManager();
  };
}

#endif
