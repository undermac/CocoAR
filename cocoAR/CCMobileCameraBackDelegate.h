//
//  CCMobileCameraBackDelegate.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 08/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#ifndef cocoAR_CCMobileCameraBackDelegate_h
#define cocoAR_CCMobileCameraBackDelegate_h

namespace cocos2d {
  
  class CCMobileCameraBackDelegate
  {
  public:
    
    //! call the release() in child layer
    virtual void MobileCameraBackManagerDestroy(void) {}
    //! call the retain() in child layer
    virtual void MobileCameraBackManagerKeep(void) {}
  };
} // namespace cocos2d

#endif
