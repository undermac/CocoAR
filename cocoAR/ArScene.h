//
//  ArScene.h
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 27/06/11.
//  Copyright 2011 Artifact. All rights reserved.
//



#ifndef ARcocos_ArScene_h
#define ARcocos_ArScene_h
#include "cocos2d.h"
#include "Geo3DObject.h"
#include "CCLocationManager.h"
#include "CCMobileCameraBack.h"
#include "bMath.h"
#include <math.h>
#include <vector>
#include "MDLModel.h"
#include "mesh.h"


void ccDraw3DLine(GLfloat lineWidth, cocos2d::ccColor4B color, GLfloat xOrigin, GLfloat yOrigin, GLfloat zOrigin, GLfloat xDestination, GLfloat yDestination, GLfloat zDestination);



class ArScene : public cocos2d::CCLayer, public cocos2d::CCLocationManagerDelegate, public cocos2d::CCMobileCameraBackDelegate
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
  
	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
	
  void visit();
  
  void arUpdate(cocos2d::ccTime dt);
  
  void didAccelerate(cocos2d::CCAcceleration* acceleration);
  
	void ccTouchesBegan(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchesMoved(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
  void menuObjectPress(CCObject* pSender);
  
  void menuCloseCallback(CCObject* pSender);
  
  void updateLocation(cocos2d::CCLocation* loc);
  void updateHeading(cocos2d::CCHeading* newHeading);

  
  void LocationManagerDestroy(void);
  void LocationManagerKeep(void);
  
  static CCARGeneric3DModel* loadModel(string filename, CCARModelType modelType);
  static CCARGeneric3DModel* findModel(string filename);
  static cocos2d::CCLocation getUserlocation(void);
  static double getUserHeading(void);
  
  CCARGeneric3DObject* addARObject(CCARGeneric3DObject* object);
  void deleteARObject(CCARGeneric3DObject* object);
  
  //test
  bool testEnable;
  void loadTest();
  void drawFloor();
  
  //Location
  void drawAr3DObjects();
  
  cocos2d::CCLabelTTF* testInfo1,*testInfo2,*testInfo3;
  cocos2d::CCLabelTTF* pLabelLocation, *pLabelHeading, *pLabelGPS;
  cocos2d::CCMenu* m_pMenu;
  
  static cocos2d::CCLocation userLocation;
  static cocos2d::CCHeading userHeading;
  static double adjustedUserHeading;
  
  static vector<CCARGeneric3DModel*> models3D;
  static vector<CCARGeneric3DObject*> objects3D;
  
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(ArScene);
};

#endif
