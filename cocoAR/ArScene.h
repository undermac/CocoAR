//
//  ArScene.h
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 27/06/11.
//  Copyright 2011 Artifact. All rights reserved.
//
#include "cocos2d.h"
#include "CCLocationManagerDelegate.h"
#include "CCLocationManager.h"

#ifndef ARcocos_ArScene_h
#define ARcocos_ArScene_h

class ArScene : public cocos2d::CCLayer, public cocos2d::CCLocationManagerDelegate
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
  
  cocos2d::CCLocation* userLocation;
  
	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
	// a selector callback
	virtual void menuCloseCallback(CCObject* pSender);
  
  void visit();
  
  void arUpdate(cocos2d::ccTime dt);
  
  void didAccelerate(cocos2d::CCAcceleration* acceleration);
  
  void updateLocation(cocos2d::CCLocation* loc);
  void updateHeading(cocos2d::CCHeading* newHeading);
  void LocationManagerDestroy(void);
  void LocationManagerKeep(void);
  
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(ArScene);
};

#endif
