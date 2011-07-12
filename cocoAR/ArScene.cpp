//
//  ArScene.cpp
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 27/06/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#include "ArScene.h"
#include "CCLocationManager.h"
#include "bMath.h"
#include "mesh.h"
#include "MDLModel.h"
#include <vector>

#define RELEASE 0
#define DEBUG_GAME 1

#define	MDLFILE01	"mechgibs.mdl"
#define	MDLFILE02	"Irys.mdl"
#define	MDLFILE03	"apache.mdl"
#define	MDLFILE04	"osprey.mdl"
#define	MDLFILE05	"osprey.mdl"

#define	MYMESH01 "barracones.obj"
#define	MYMESH02 "war3farm.obj"
#define	MYMESH03 "war3mill.obj"
#define	MYMESH04 "war3blacksmith.obj"
#define	MYMESH05 "war3barracks.obj"
#define	MYMESH06 "war3_buildingsmall.obj"
#define	MYMESH07 "flecha.obj"
#define	MYMESH08 "poste.obj"

TMDLModel	MdlModel[10];					// MDL Model
Mesh myMesh[10];

GLfloat XCam=0.0f,YCam=1.5f,ZCam=0.0f;
GLfloat XEye=0.0f,YEye=1.5f,ZEye=0.5f;
GLfloat xrot3=0.0f,yrot3=0.0f,zrot3=0.0f;
GLfloat xUp=0.0f, yUp =0.0f, zUp=0.1f;
#define ACELERATION_MATRIX_MAX 8
GLfloat XaccelerationMatrix [ACELERATION_MATRIX_MAX];
GLfloat YaccelerationMatrix [ACELERATION_MATRIX_MAX];
GLfloat ZaccelerationMatrix [ACELERATION_MATRIX_MAX];

USING_NS_CC;

CCScene* ArScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	ArScene *layer = ArScene::node();
  
	// add layer as a child to scene
	scene->addChild(layer);
  
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ArScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
  
	this->setIsTouchEnabled(true);
	this->setIsAccelerometerEnabled(true);
	this->setIsKeypadEnabled(true);
  CCLocationManager::sharedCCLocationManager()->addDelegate(this);
  
//  this->userLocation = CCLocationManager::sharedCCLocationManager()->getLocation();
  
  
//  CCSprite *pig = CCSprite::spriteWithFile("pigrate.png");
//  this->addChild(pig,0);
//  pig->setScale(0.25f);
//  pig->setPosition(ccp(160, 210));
  
	MdlModel[0].Init((char *)&MDLFILE01);
	MdlModel[0].SetSequence(0);
	MdlModel[1].Init((char *)&MDLFILE02);
	MdlModel[1].SetSequence(1);
	MdlModel[2].Init((char *)&MDLFILE03);
	MdlModel[2].SetSequence(1);
	MdlModel[3].Init((char *)&MDLFILE04);
	MdlModel[3].SetSequence(1);
  
	myMesh[0].LoadModel((char *)&MYMESH01);
	myMesh[1].LoadModel((char *)&MYMESH02);
	myMesh[2].LoadModel((char *)&MYMESH03);
	myMesh[3].LoadModel((char *)&MYMESH04);
	myMesh[4].LoadModel((char *)&MYMESH05);
  myMesh[5].LoadModel((char *)&MYMESH07);
  myMesh[6].LoadModel((char *)&MYMESH08);
  
    for (int j = 0; j < ACELERATION_MATRIX_MAX; j++) {
      XaccelerationMatrix[j] = 0.0f;
      YaccelerationMatrix[j] = 0.0f;
      ZaccelerationMatrix[j] = 0.0f;
    }
  
  this->schedule( schedule_selector(ArScene::arUpdate) );
  update(0);
  CCDirector::sharedDirector()->resume();
  CCDirector::sharedDirector()->setProjection(kCCDirectorProjection3D);
	CCDirector::sharedDirector()->setDepthTest(true);
	
	return true;
}

void ArScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void ArScene::arUpdate(ccTime dt)
{
  MdlModel[0].AdvanceFrame(dt*2);
	MdlModel[1].AdvanceFrame(dt*2);
	MdlModel[2].AdvanceFrame(dt*2);
	MdlModel[3].AdvanceFrame(dt*2);
  
//  userLocation = CCLocationManager::sharedCCLocationManager()->getLocation();

//  printf("\nLocalización del puto usuario %f %f %f\n", userLocation->longitude,userLocation->latitude,userLocation->altitude);
}

void ArScene::visit() 
{	
	CCLayer::visit();
  
	glColor4f(1.0f,1.0f,1.0f,1.0f);		
  
	CCDirector::sharedDirector()->setDepthTest(true);
	glPushMatrix();
  
	//-----------------------------------------------------------------------
  
  GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
  GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
  GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1.0 };
  GLfloat light_position[] = { 1.0, 1.0, 0.0, 0.0 };
  
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
  glEnable(GL_LIGHT0);
  
  CCDirector::sharedDirector()->setProjection(kCCDirectorProjection3D);	
  CCDirector::sharedDirector()->applyOrientation();
  
//    [TODO] ALPHA ...
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  
  glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();		
	
//  gluLookAt(0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 500.0f, 1, 0, 0); // WORKING
  
  gluLookAt(XCam, YCam, ZCam,	XEye, -zUp*50.0f, ZEye, xUp, yUp, 0);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);	
  
	//-----------------------------------------------
  if (yrot3 > 360) {
    yrot3 = 0;
//    printf("\nOPEN GL WORKING \n");
//    printf("\nx:%f y:%f z:%f\n", -xUp, -yUp, -zUp);
  }
  yrot3 += 1.0f;	
  
  glPushMatrix();
  glTranslatef(-100.0f, 0.0f, 1000.0f);
	glScalef( 0.5f, 0.5f, 0.5f );
  glRotatef(yrot3, 0, 1, 0);
  myMesh[3].Draw();
	glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 45.0f);
	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3, 0, 1, 0);
  myMesh[6].Draw();
	glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-200.0f, 0.0f, 500.0f);
	glScalef( 0.5f, 0.5f, 0.5f );
  glRotatef(yrot3, 0, 1, 0);
  myMesh[1].Draw();	
	glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.0f, 25.0f, 100.0f);
	glScalef( 0.5f, 0.5f, 0.5f );
  glRotatef(yrot3*3, 0, 1, 0);
  myMesh[5].Draw();	
	glPopMatrix();
  
  glPushMatrix();
  glTranslatef(200.0f, 0.0f, 1000.0f);
	glScalef( 0.5f, 0.5f, 0.5f );
  glRotatef(yrot3, 0, 1, 0);
  myMesh[1].Draw();
	glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.0f, 300.0f, 200.0f);
	glScalef( 0.5f, 0.5f, 0.5f );
  glRotatef(yrot3, 0, 1, 0);
  glRotatef(30, 0, 1, 0);
  glRotatef(-90.0f, 1, 0, 0);
  MdlModel[2].DrawModel();
	glPopMatrix();
  

  //-------------------------------------------------------------------------------------------------

	glDisable(GL_CULL_FACE);	
	glClearColor(0.0f,0.0f,0.0f,0.0f) ;		
	glPopMatrix();	
  
	CCDirector::sharedDirector()->setDepthTest(true);
	glEnableClientState(GL_COLOR_ARRAY);
  
}

#define KACCERERATION 0.15f

//static float xAccelerationFactor = 0.01f;
//static float yAccelerationFactor = 0.01f;
//static float zAccelerationFactor = 0.01f;
//
//static int auxCount = 0;
//
//static GLfloat xAceleration = KACCERERATION;
//static GLfloat yAceleration = KACCERERATION;
//static GLfloat zAceleration = KACCERERATION;


void ArScene::didAccelerate(cocos2d::CCAcceleration* acceleration){

  xUp = - acceleration->x;
  yUp = - acceleration->y;
  zUp = - acceleration->z;
  
  
//  GLfloat xAux=acceleration->x;
//  GLfloat yAux=acceleration->y;
//  GLfloat zAux=acceleration->z;  
//  XaccelerationMatrix[auxCount] = acceleration->x;
//  YaccelerationMatrix[auxCount] = acceleration->y;
//  ZaccelerationMatrix[auxCount] = acceleration->z;
//  
//  for (unsigned int i = 0; i < ACELERATION_MATRIX_MAX; i++) {
//    xAux = XaccelerationMatrix[i] + xAux;
//    yAux = YaccelerationMatrix[i] + yAux;
//    zAux = ZaccelerationMatrix[i] + zAux;
//  }
//  
//  GLfloat xAverage = - xAux/ACELERATION_MATRIX_MAX;
//  GLfloat yAverage = - yAux/ACELERATION_MATRIX_MAX;
//  GLfloat zAverage = - zAux/ACELERATION_MATRIX_MAX;
//  
//  if (xAverage == xUp) {
//    xAceleration = 0;
//  }else if (xAverage > xUp) {
//    if (xAceleration > 0) {
//      xAceleration = xAceleration + (xAverage - xUp)*xAccelerationFactor;
//      xAccelerationFactor *= 2;
//    }else{
//      xAceleration = (xAverage - xUp)*xAccelerationFactor;
//      xAccelerationFactor = KACCERERATION;
//    }
//  }else{
//    if (xAceleration < 0) {
//      xAceleration = xAceleration + (xAverage - xUp)*xAccelerationFactor;
//      xAccelerationFactor *= 2;
//    }else{
//      xAceleration = (xAverage - xUp)*xAccelerationFactor;
//      xAccelerationFactor = KACCERERATION;
//    }
//  }
//  xUp += xAceleration;
//  
//  if (yAverage == yUp) {
//    yAceleration = 0;
//  } else if (yAverage > yUp) {
//    if (yAceleration > 0) {
//      yAceleration = yAceleration + (yAverage - yUp)*yAccelerationFactor;
//      yAccelerationFactor *= 2;
//    }else{
//      yAceleration = (yAverage - yUp)*yAccelerationFactor;
//      yAccelerationFactor = KACCERERATION;
//    }
//  }else{
//    if (yAceleration < 0) {
//      yAceleration = yAceleration + (yAverage - yUp)*yAccelerationFactor;
//      yAccelerationFactor *= 2;
//    }else{
//      yAceleration = (yAverage - yUp)*yAccelerationFactor;
//      yAccelerationFactor = KACCERERATION;
//    }
//  }
//  yUp += yAceleration;
//  if (zAverage == zUp) {
//    zAceleration = 0;
//  } else if (zAverage > zUp) {
//    if (zAceleration > 0) {
//      zAceleration = zAceleration + (zAverage - zUp)*zAccelerationFactor;
//      zAccelerationFactor *= 2;
//    }else{
//      zAceleration = (zAverage - zUp)*zAccelerationFactor;
//      zAccelerationFactor = KACCERERATION;
//    }
//  }else{
//    if (zAceleration < 0) {
//      zAceleration = zAceleration + (zAverage - zUp)*zAccelerationFactor;
//      zAccelerationFactor *= 2;
//    }else{
//      zAceleration = (zAverage - zUp)*zAccelerationFactor;
//      zAccelerationFactor = KACCERERATION;
//    }
//  }
//  zUp += zAceleration;
//  
//  if (auxCount >= ACELERATION_MATRIX_MAX-1)
//    auxCount = 0;
//  else
//    auxCount++;
}

void ArScene::updateLocation(CCLocation* newLocation){
  printf("HELLO WORLD !!");
}
void ArScene::updateHeading(CCHeading* newHeading){
  printf("\ntrueHeading:%f XEye =%f YEye = %f", newHeading->trueHeading,sin(newHeading->trueHeading*(_pi*180)) ,cos(newHeading->trueHeading*(_pi*180)));
  XEye = -sin(newHeading->trueHeading*(_pi/180))*50;
  ZEye = cos(newHeading->trueHeading*(_pi/180))*50;
}
void ArScene::LocationManagerDestroy(void){

}
void ArScene::LocationManagerKeep(void){

}
