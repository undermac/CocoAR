//
//  CCLocationManager_platform.h
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 01/07/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#ifndef ARcocos_CCLocationManager_platform_h
#define ARcocos_CCLocationManager_platform_h

#include "CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)
//#include "wophone/CCAccelerometer_wophone.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#include "win32/CCAccelerometer_win32.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "android/CCAccelerometer_android.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "CCLocationManager_ios.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_AIRPLAY)
//#include "airplay/CCAccelerometer_airplay.h"
#else
#error
#endif

#endif
