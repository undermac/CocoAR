//
//  ArScene.cpp
//  ARcocos
//
//  Created by Javier de la Peña Ojea on 27/06/11.
//  Copyright 2011 Artifact. All rights reserved.
//

//[DONE] Geolocalización de objetos -> 2 días
//[DONE] Desplegar texto -> 2 días
//[DONE] Crear cámera delegate -> 2 días

//[TODO]Crear elementos geo 
//[TODO]Crear elementos posicionados relativos

//[TODO] Solucionar BUG -Mirando al Sur- Que lo arrelé el Mac del futuro.
//[TODO] Click en objetos -> 3 días
//[TODO] Prueba fiesta -> 3 días
//[TODO] Sacar API -> 7 días
//[TODO] Enlazar API con Orama -> 7 días
//[TODO] Realizar pruebas rápidas -> 1 día
//[TODO] Documentar init -> 3 días
//[TODO] Documentar Análisis -> 9 días
//[TODO] Documentar Diseño -> 9 días
//[TODO] Documentar Implementación 9 días
//[TODO] Realizar documentación -> 15 días

#include "ArScene.h"
#include "CCLocationManager.h"
#include "bMath.h"
#include <math.h>
#include "MDLModel.h"
#include <vector>

#define RELEASE 0
#define DEBUG_GAME 1

#define LABEL_FONT_TYPE "Thonburi"
#define TAG_CLOSE_BUTTON 95


//3D Test
#define	TMDL_HELICOPTER	"apache.mdl"
#define	TMDL_OSPREY	"osprey.mdl"

#define	MESH_FLECHA "flecha.obj"

#define	MYMESH01 "barracones.obj"
#define	MYMESH02 "war3farm.obj"
#define	MYMESH03 "war3mill.obj"
#define	MYMESH04 "war3blacksmith.obj"
#define	MYMESH05 "war3barracks.obj"
#define	MYMESH06 "war3_buildingsmall.obj"

#define	MYMESH08 "poste.obj"

//Funtions
#define DEG_TO_RAD(X) (X*M_PI/180.0)
#define RAD_TO_DEG(X) (X*180.0/M_PI)


GLfloat XCam=0.0f,YCam=1.0f,ZCam=0.0f;
GLfloat XEye=0.0f,YEye=1.5f,ZEye=0.5f;
GLfloat xrot3=0.0f,yrot3=0.0f,zrot3=0.0f;
GLfloat xUp=0.0f, yUp =0.0f, zUp=0.1f;

GLfloat eulerAngles[3];

static cocos2d::CCLocation userLocation;
static cocos2d::CCHeading userHeading;

static vector<CCARGeneric3DModel*> models3D;
static vector<CCARGeneric3DObject*> objects3D;

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
  
  CCDirector::sharedDirector()->setDisplayFPS(false);
  this->testEnable = true;
  
	this->setIsAccelerometerEnabled(true);
  this->setIsTouchEnabled(true);
  CCLocationManager::sharedCCLocationManager()->addDelegate(this);
  CCMobileCameraBackManager::sharedCCMobileCameraBackManager()->addDelegate(this);
  
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
                                                                     "CloseNormal.png",
                                                                     "CloseSelected.png",
                                                                     this,
                                                                     menu_selector(ArScene::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, CCDirector::sharedDirector()->getWinSize().height - 20) );
  
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);
  
  testInfo1 = CCLabelTTF::labelWithString("00.0", LABEL_FONT_TYPE, 12);
  testInfo1->setPosition(ccp(160,450));
  this->addChild(testInfo1,1);
  testInfo2 = CCLabelTTF::labelWithString("00.0", LABEL_FONT_TYPE, 12);
  testInfo2->setPosition(ccp(160,440));
  this->addChild(testInfo2,1);
  testInfo3 = CCLabelTTF::labelWithString("00.0", LABEL_FONT_TYPE, 12);
  testInfo3->setPosition(ccp(160,430));
  this->addChild(testInfo3,1);
  
  
  CCSize pLabelSize = CCDirector::sharedDirector()->getWinSize();
  pLabelSize.height = 20;
  pLabelSize.width = pLabelSize.width - 10;
  
  pLabelLocation = CCLabelTTF::labelWithString("00.0", pLabelSize, CCTextAlignmentCenter ,LABEL_FONT_TYPE, 14);
  pLabelLocation->setPosition(ccp(160,25));
  
  pLabelGPS = CCLabelTTF::labelWithString("00.0", pLabelSize, CCTextAlignmentCenter ,LABEL_FONT_TYPE, 14);
  pLabelGPS->setPosition(ccp(160,10));
  
  pLabelHeading = CCLabelTTF::labelWithString("00.0" ,LABEL_FONT_TYPE, 18);
  pLabelHeading->setPosition(ccp(160,470));
  
  this->addChild(pLabelLocation,1);
  this->addChild(pLabelGPS,1);
  this->addChild(pLabelHeading,1);
  
  if (testEnable) {
    loadTest();
  }
  
  // Esto se tiene que hacer desde fuera...
  this->loadModel(TMDL_HELICOPTER, CCARType_TMDLModel);
  this->loadModel(TMDL_OSPREY, CCARType_TMDLModel);
  this->loadModel(MESH_FLECHA, CCARType_Mesh);
  this->loadModel(MYMESH01, CCARType_Mesh);
  this->loadModel(MYMESH02, CCARType_Mesh);
  this->loadModel(MYMESH03, CCARType_Mesh);
  this->loadModel(MYMESH04, CCARType_Mesh);
  this->loadModel(MYMESH05, CCARType_Mesh);
  this->loadModel(MYMESH08, CCARType_Mesh);
  
  
  
  this->schedule( schedule_selector(ArScene::arUpdate) );
  
  CCDirector::sharedDirector()->resume();
  CCDirector::sharedDirector()->setProjection(kCCDirectorProjection3D);
	CCDirector::sharedDirector()->setDepthTest(true);
  
	return true;
}

void ArScene::arUpdate(ccTime dt)
{
  for(unsigned int i=0; i< models3D.size(); i++){
    models3D[i]->AdvanceFrame(dt*2);
  }
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
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  
  glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();		
  
  if (eulerAngles[1] > 90.0f && eulerAngles[1] < 270.0f) {
    gluLookAt(XCam, YCam, ZCam,	XEye, -zUp*100, ZEye, -xUp*100, yUp*100, 0.0f);
  }else{
    gluLookAt(XCam, YCam, ZCam,	XEye, -zUp*100, ZEye, xUp*100, yUp*100, 0.0f);
  }
  
  
  //Y
  glColor4f(1.0f,0.0f,0.0f,0.8f);
  ccDraw3DLine(5.0f,0.0f, 0.0f, 0.0f, 1500.0f, 0.0f, 0.0f);
  
  //Este & X
  glColor4f(0.0f,1.0f,0.0f,0.8f);
  ccDraw3DLine(5.0f,0.0f, 0.0f, 0.0f, 0.0f, 1500.0f, 0.0f);
  
  //Norte & Z
  glColor4f(0.0f,0.0f,1.0f,0.8f);
  ccDraw3DLine(5.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1500.0f);
  
  //Sur
  glColor4f(0.0f,1.0f,1.0f,0.8f);
  ccDraw3DLine(5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1500.0f);
  
  //Oeste
  glColor4f(1.0f,0.0f,1.0f,0.8f);
  ccDraw3DLine(5.0f, 0.0f, 0.0f, 0.0f, -1500.0f, 0.0f, 0.0f);
  
  
  glColor4f(1.0f,1.0f,1.0f,0.95f);
  
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);	
  
  
  for(unsigned int i=0; i< objects3D.size(); i++){
    objects3D[i]->draw();
  }
  
  glColor4f(0.5f,0.5f,0.5f,0.5f);
  drawFloor();
  
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
  
  eulerAngles[0] = atan2(accelZ, -accelY)*(180/_pi);  
  eulerAngles[2] = atan2(accelX, -accelY)*(180/_pi);
  
  XEye = -sin(eulerAngles[1]*(_pi/180))*50;
  YEye = accelZ*50;
  ZEye = cos(eulerAngles[1]*(_pi/180))*50;
  
}

void ArScene::updateLocation(CCLocation* newLocation){
  userLocation = *newLocation;
  
  char buffer[256];
  sprintf(buffer,"Longitude: %3.2f, Latitude: %3.2f, altitude: %4.0fm",userLocation.longitude, userLocation.latitude, userLocation.altitude);
  this->pLabelLocation->setString(buffer);
  
  sprintf(buffer,"Horizontal acc.: %3.2fm, Vertical Acc.: %3.2fm",userLocation.horizontalAccuracy, userLocation.verticalAccuracy);
  this->pLabelGPS->setString(buffer);
}

void ArScene::updateHeading(CCHeading* newHeading){
  
  userHeading = *newHeading;
  
  float adjustedHeading;
	float adjustment = tilt;			//This needs to be calibrated properly.
	
	//Adjust the heading due to our tilt and way we hold the phone.
  float compassBearing = newHeading->magneticHeading;
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
  
  char buffer[256];
  sprintf(buffer,"%3.0fº", adjustedHeading);
  this->pLabelHeading->setString(buffer);
  
  eulerAngles[1] = adjustedHeading;
  
  XEye = (-sin(eulerAngles[1]*(_pi/180))*cos(zUp))*50;
  ZEye = (cos(eulerAngles[1]*(_pi/180))*cos(zUp))*50;
}

void ArScene::LocationManagerDestroy(void){
  
}
void ArScene::LocationManagerKeep(void){
}

void ArScene::ccTouchesBegan(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent){
  printf("\nccTouchesBegan");
}
void ArScene::ccTouchesMoved(cocos2d::CCSet *pTouch, cocos2d::CCEvent *pEvent){
  printf("\nccTouchesMoved");
}

void ArScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
  printf("\nTouch Ended");
  if (testEnable)
    testEnable = 0;
  else testEnable = 1;
  printf("%i", testEnable);
}

void ArScene::menuCloseCallback(CCObject* pSender)
{
  printf("\nmenuCloseCallback");
}


void ArScene::loadTest()
{
  objects3D.push_back(new CCARObject3D(MESH_FLECHA, CCARType_Mesh, 1.5f, 0.0f, 0.0f, 500.0f));
  objects3D.push_back(new CCARGeo3DObject(MYMESH08, CCARType_Mesh, 0.1f ,-7.855653762817383f ,  42.344466179278236f));//JJ
  objects3D.push_back(new CCARGeo3DObject(MYMESH02, CCARType_Mesh, 0.1f ,-7.856292128562927f ,  42.34511443500709f));//Universidad
  objects3D.push_back(new CCARGeo3DObject(MYMESH03, CCARType_Mesh, 0.1f ,-7.855787873268127 ,  42.34516201314594f));// Universidad Norte 
  objects3D.push_back(new CCARGeo3DObject(TMDL_HELICOPTER, CCARType_TMDLModel, 0.25f ,-7.863861322402954f ,  42.34146865303537f));// Fuente san lazaro
  objects3D[4]->yTranslate = 200;
  objects3D[4]->xRotate = 270.0f;
  objects3D.push_back(new CCARGeo3DObject(MYMESH04, CCARType_Mesh, 0.1f ,-7.36083984375f ,  42.3037216984154f));// Orense oeste
  objects3D.push_back(new CCARGeo3DObject(MYMESH05, CCARType_Mesh, 0.1f ,-7.862091064453125f ,  42.348395259793f));// Orense rio
}


void ArScene::execTest()
{
  
//  if (yrot3 > 360) {
//    yrot3 = 0;
//  }
//  yrot3 += 1.0f;	
//  
//  glPushMatrix();
//  glTranslatef(300.0f, 300.0f, 200.0f);
//	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3, 0, 1, 0);
//  glRotatef(30, 0, 1, 0);
//  glRotatef(-90.0f, 1, 0, 0);
//  models3D[0]->Draw();
//	glPopMatrix();
//  
//  glPushMatrix();
//  glTranslatef(-300.0f, 300.0f, 200.0f);
//	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3, 0, 1, 0);
//  glRotatef(30, 0, 1, 0);
//  glRotatef(-90.0f, 1, 0, 0);
//  models3D[0]->Draw();
//	glPopMatrix();
//  
//  glPushMatrix();
//  glTranslatef(-300.0f, 300.0f, -200.0f);
//	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3, 0, 1, 0);
//  glRotatef(30, 0, 1, 0);
//  glRotatef(-90.0f, 1, 0, 0);
//  models3D[0]->Draw();
//	glPopMatrix();
//  
//  glPushMatrix();
//  glTranslatef(-100.0f, 0.0f, 1000.0f);
//	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3, 0, 1, 0);
//  models3D[0]->Draw();
//	glPopMatrix();
//  
//  glPushMatrix();
//  glTranslatef(0.0f, 0.0f, 45.0f);
//	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3, 0, 1, 0);
//  models3D[6]->Draw();
//	glPopMatrix();
//  
//  glPushMatrix();
//  glTranslatef(-200.0f, 0.0f, 500.0f);
//	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3, 0, 1, 0);
//  models3D[1]->Draw();	
//	glPopMatrix();
//  
//  glPushMatrix();
//  glTranslatef(0.0f, 25.0f, 100.0f);
//	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3*3, 0, 1, 0);
//  models3D[5]->Draw();	
//	glPopMatrix();
//  
//  glPushMatrix();
//  glTranslatef(200.0f, 0.0f, -1000.0f);
//	glScalef( 0.5f, 0.5f, 0.5f );
//  glRotatef(yrot3, 0, 1, 0);
//  models3D[3]->Draw();
//	glPopMatrix();

}

void ccDraw3DLine(GLfloat lineWidth,GLfloat xOrigin, GLfloat yOrigin, GLfloat zOrigin, GLfloat xDestination, GLfloat yDestination, GLfloat zDestination)
{
	ccVertex3F vertices[2] = 
  {
    {xOrigin * CC_CONTENT_SCALE_FACTOR(), yOrigin * CC_CONTENT_SCALE_FACTOR(), zOrigin * CC_CONTENT_SCALE_FACTOR()},
    {xDestination * CC_CONTENT_SCALE_FACTOR(), yDestination * CC_CONTENT_SCALE_FACTOR(), zDestination * CC_CONTENT_SCALE_FACTOR() },
  };
  
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
  glLineWidth (lineWidth);
	glDrawArrays(GL_LINES, 0, 2);
	
	// restore default state
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);	
}

void drawFloor(){
  ccVertex3F vertices[4] = 
  {
    { 1500.0f* CC_CONTENT_SCALE_FACTOR(), 0.0f* CC_CONTENT_SCALE_FACTOR(),  1500.0f* CC_CONTENT_SCALE_FACTOR()},
    {-1500.0f* CC_CONTENT_SCALE_FACTOR(), 0.0f* CC_CONTENT_SCALE_FACTOR(),  1500.0f* CC_CONTENT_SCALE_FACTOR()},
    { 1500.0f* CC_CONTENT_SCALE_FACTOR(), 0.0f* CC_CONTENT_SCALE_FACTOR(), -1500.0f* CC_CONTENT_SCALE_FACTOR()},
    {-1500.0f* CC_CONTENT_SCALE_FACTOR(), 0.0f* CC_CONTENT_SCALE_FACTOR(), -1500.0f* CC_CONTENT_SCALE_FACTOR()},
  };
  
  glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void ArScene::loadAr3DObjects(){
  
}


CCARGeneric3DModel* ArScene::loadModel(string filename, CCARModelType modelType)
{
  CCARGeneric3DModel* toret = findModel(filename);
  
  if (!toret) {
    switch (modelType){
      case CCARType_Mesh: {
        Mesh *myMesh = new Mesh();
        myMesh->LoadModel(const_cast<char*> ( filename.c_str() ));
        models3D.push_back(myMesh);
        return myMesh;
        break; 
      }
      case CCARType_TMDLModel: 
      {
        TMDLModel *myTMDL = new TMDLModel();
        myTMDL->Init(const_cast<char*> ( filename.c_str()));
        myTMDL->SetSequence(0);
        models3D.push_back(myTMDL);
        return myTMDL;
      }
        break; 
      default:  
        break; 
    } 
  }
  return toret;
}

CCARGeneric3DModel* ArScene::findModel(string filename){
  CCARGeneric3DModel* toret=NULL;
  
  for(unsigned int i=0; i< models3D.size(); i++){
    if (models3D[i]->modelName == filename) {
      return models3D[i];
    }
  }
  return toret;
}

CCLocation ArScene::getUserlocation(void){
  return userLocation;
}
CCHeading ArScene::getUserHeading(void){
  return userHeading;
}
