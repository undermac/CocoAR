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
#include <math.h>

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

vector<Ar3DObject> myAr3dObjects;

GLfloat XCam=0.0f,YCam=1.5f,ZCam=0.0f;
GLfloat XEye=0.0f,YEye=1.5f,ZEye=0.5f;
GLfloat xrot3=0.0f,yrot3=0.0f,zrot3=0.0f;
GLfloat xUp=0.0f, yUp =0.0f, zUp=0.1f;
GLfloat eulerAngles[3];

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
  CCLocationManager::sharedCCLocationManager()->addDelegate(this);
  
  CCSprite *pig = CCSprite::spriteWithFile("pigrate.png");
  this->addChild(pig,1);
  pig->setScale(0.25f);
  pig->setPosition(ccp(160, 210));
  
  test1Init();

  testInit();

//  test2Init();
  
  this->schedule( schedule_selector(ArScene::arUpdate) );
  
  CCDirector::sharedDirector()->resume();
  CCDirector::sharedDirector()->setProjection(kCCDirectorProjection3D);
	CCDirector::sharedDirector()->setDepthTest(true);
	
	return true;
}

void ArScene::arUpdate(ccTime dt)
{
  MdlModel[0].AdvanceFrame(dt*2);
	MdlModel[1].AdvanceFrame(dt*2);
	MdlModel[2].AdvanceFrame(dt*2);
	MdlModel[3].AdvanceFrame(dt*2);
}

void ArScene::visit() 
{	
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
	
  //  gluLookAt(XCam, YCam, ZCam,	XEye, -zUp*50, ZEye, xUp, yUp, zUp);
  
  gluLookAt(XCam, YCam, ZCam,	XEye, -zUp*50, ZEye, xUp, yUp, zUp);
  
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);	
  
	//-----------------------------------------------
  
  test1();
  
//  test();
  
//    test2();
  //-------------------------------------------------------------------------------------------------
  
	glDisable(GL_CULL_FACE);	
	glClearColor(0.0f,0.0f,0.0f,0.0f) ;		
	glPopMatrix();	
  
	CCDirector::sharedDirector()->setDepthTest(true);
	glEnableClientState(GL_COLOR_ARRAY);
  
  CC_ENABLE_DEFAULT_GL_STATES();
	glClear(GL_DEPTH_BUFFER_BIT);
  
  CCLayer::visit();
}

#define KFilteringFactor 0.20f
float tilt;

void ArScene::didAccelerate(cocos2d::CCAcceleration* acceleration){
	static float accelX;
	static float accelY;
	static float accelZ;
	
	// Use a basic low-pass gravity filter to keep only the gravity component of each axis.
  accelX = (acceleration->x * KFilteringFactor) + (accelX * (1.0 - KFilteringFactor));
  accelY = (acceleration->y * KFilteringFactor) + (accelY * (1.0 - KFilteringFactor));
	accelZ = (acceleration->z * KFilteringFactor) + (accelZ * (1.0 - KFilteringFactor));
	
  //Get the tilt of the x and y
	tilt =  atan2(accelY, accelX)*(180/_pi);
  
  xUp = - accelX;
  yUp = - accelY;
  zUp = - accelZ;
  
//  eulerAngles[0] = atan2(accelZ, -accelY)*(180/_pi);  
//  eulerAngles[2] = atan2(accelX, -accelY)*(180/_pi);

  eulerAngles[0] = atan2(accelZ, -accelY)*(180/_pi);  
  eulerAngles[2] = atan2(accelX, -accelY)*(180/_pi);
  
//  YEye = (sin(eulerAngles[2]*(_pi/180)))*50;

  XEye = -sin(eulerAngles[1]*(_pi/180))*50;
  YEye = accelZ*50;
  ZEye = cos(eulerAngles[1]*(_pi/180))*50;
  
//  printf("\nRoll: %f, pitch: %f, Yaw: %f, tilt: %f", atan2(accelY, accelX)*(180/_pi), atan2(accelY, accelZ)*(180/_pi), eulerAngles[1], tilt);
//  printf("\nAccelX: %f, AccelY: %f, AccelZ: %f ",accelX,accelY,accelZ);
}

void ArScene::updateLocation(CCLocation* newLocation){
  this->userLocation = *newLocation;
}

void ArScene::updateHeading(CCHeading* newHeading){
  
  this->userHeading = *newHeading;
  
  float adjustedHeading;
	float adjustment = tilt;			//This needs to be calibrated properly.
	
	//Adjust the heading due to our tilt and way we hold the phone.
  float compassBearing = newHeading->trueHeading;
  
  if (compassBearing == -1.0f) {
    compassBearing = newHeading->magneticHeading;
  }
  switch (CCDirector::sharedDirector()->getDeviceOrientation()){
    case CCDeviceOrientationPortrait:
      adjustedHeading = (compassBearing - 90.0f) - adjustment;  
      break; 
    case CCDeviceOrientationPortraitUpsideDown:  
      adjustedHeading = (compassBearing +  90.0f) - adjustment;
      break; 
    case CCDeviceOrientationLandscapeLeft:  
      adjustedHeading = (compassBearing) - adjustment;
      break; 
    case CCDeviceOrientationLandscapeRight:  
      adjustedHeading = (compassBearing - 180.0f) - adjustment;
      break; 
    default:  
      break; 
  }  

  adjustedHeading = (compassBearing - 90.0f) - adjustment; 
  
	//Better to have things running from 0 to 360 degrees.
	if(adjustedHeading < 0)
	{
		adjustedHeading = adjustedHeading + 360;
	}
  
  eulerAngles[1] = adjustedHeading;
  
  printf("\nUserHeading: %f",adjustedHeading);
  
//  XEye = (-sin(eulerAngles[1]*(_pi/180))*cos(eulerAngles[2]*(_pi/180)))*50;
//  ZEye = (cos(eulerAngles[1]*(_pi/180))*cos(eulerAngles[2]*(_pi/180)))*50;
  
  XEye = (-sin(eulerAngles[1]*(_pi/180))*cos(zUp))*50;
  ZEye = (cos(eulerAngles[1]*(_pi/180))*cos(zUp))*50;
  
//  YEye = (sin(eulerAngles[0]*(_pi/180)))*50;
}
void ArScene::LocationManagerDestroy(void){
  
}
void ArScene::LocationManagerKeep(void){
}

void test1Init(){
  
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
}

void test1(){
  if (yrot3 > 360) {
    yrot3 = 0;
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
  
}


void test2Init(){
  
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
}

void test2(){
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 1500.0f);
	glScalef( 5.5f, 5.5f, 5.5f );
  glRotatef(yrot3*3, 0, 1, 0);
  myMesh[1].Draw();	
	glPopMatrix();
  
  glPushMatrix();
  glTranslatef(-1500.0f, 0.0f, 0.0f);
	glScalef( 5.5f, 5.5f, 5.5f );
  glRotatef(yrot3*3, 0, 1, 0);
  myMesh[2].Draw();	
	glPopMatrix();
  
  glPushMatrix();
  glTranslatef(1500.0f, 0.0f, 0.0f);
	glScalef( 5.5f, 5.5f, 5.5f );
  glRotatef(yrot3*3, 0, 1, 0);
  myMesh[5].Draw();	
	glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -1500.0f);
	glScalef( 5.5f, 5.5f, 5.5f );
  glRotatef(yrot3*3, 0, 1, 0);
  myMesh[3].Draw();	
	glPopMatrix();
}

void ArScene::testInit()
{
  Ar3DObject *an3dObject = new Ar3DObject();
  an3dObject->LoadModel((char *)&MYMESH07);
  
  an3dObject->latitude = 43.49019867977202f;
  an3dObject->longitude = -8.246827125549316f;
  
//  43.49019867977202º, -8.246827125549316º
  

  an3dObject->altitude = 0.0f;
  an3dObject->modelType = 0;
  
  an3dObject->scale = 2.5f;
  
  an3dObject->xTranslate = 0.0f;
  an3dObject->yTranslate = 0.0f;
  an3dObject->zTranslate = 100.0f;
  
  an3dObject->xRotate = 0.0f;
  an3dObject->yRotate = 0.0f;
  an3dObject->zRotate = 0.0f;
  
  myAr3dObjects.push_back(*an3dObject);
}

void ArScene::test()
{
  for(unsigned int i=0; i< myAr3dObjects.size(); i++){
    
    double value = CCLocationManager::sharedCCLocationManager()->distanceFromLocation( userLocation.latitude,userLocation.longitude , myAr3dObjects[i].latitude, myAr3dObjects[i].longitude);
    
    printf("\nUserlocation: %f, %f",this->userLocation.latitude, this->userLocation.longitude);
    printf("\nObjectlocation: %f, %f",myAr3dObjects[i].latitude, myAr3dObjects[i].longitude);
    printf("\ndistance: %f",value);
    printf("\nUserHeading: %f",eulerAngles[1]);
    
    glPushMatrix();
    glTranslatef(myAr3dObjects[i].xTranslate, myAr3dObjects[i].yTranslate, myAr3dObjects[i].zTranslate);
    glScalef( myAr3dObjects[i].scale, myAr3dObjects[i].scale, myAr3dObjects[i].scale );
    glRotatef(myAr3dObjects[i].xRotate, 1, 0, 0);
    glRotatef(myAr3dObjects[i].yRotate, 0, 1, 0);
    glRotatef(myAr3dObjects[i].zRotate, 0, 0, 1);
    myAr3dObjects[i].Draw();	
    glPopMatrix();
  }
}





