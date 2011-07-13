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
#include "ArObject.h"

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

vector<Ar3DObjects> myAr3dObjects;

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
  
  test1Init();
  
  this->schedule( schedule_selector(ArScene::arUpdate) );
  update(0);
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
  
  gluLookAt(XCam, YCam, ZCam,	XEye, -zUp*50, ZEye, xUp, yUp, 0);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);	
  
	//-----------------------------------------------

  test1();
  
  //-------------------------------------------------------------------------------------------------

	glDisable(GL_CULL_FACE);	
	glClearColor(0.0f,0.0f,0.0f,0.0f) ;		
	glPopMatrix();	
  
	CCDirector::sharedDirector()->setDepthTest(true);
	glEnableClientState(GL_COLOR_ARRAY);
  
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
  
  //[TODO ARREGLAR]
  if (accelZ > 1.0f)
    accelZ = 1.0f;
  else if (accelZ < -1.0f)
    accelZ = -1.0f;
}

void ArScene::updateLocation(CCLocation* newLocation){
  printf("HELLO WORLD !!");
}

void ArScene::updateHeading(CCHeading* newHeading){
  
  float adjustedHeading;
	float adjustment = tilt;			//This needs to be calibrated properly.
	
	//Adjust the heading due to our tilt and way we hold the phone.
	float compassBearing = newHeading->trueHeading;
	adjustedHeading = (compassBearing - 90) - adjustment;  //90 is because North is now east.
	
	//Better to have things running from 0 to 360 degrees.
	if(adjustedHeading < 0)
	{
		adjustedHeading = adjustedHeading + 360;
	}
  
  XEye = (-sin(adjustedHeading*(_pi/180))*cos(zUp))*50;
  ZEye = (cos(adjustedHeading*(_pi/180))*cos(zUp))*50;
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
  
}

void test2(){
  
}
