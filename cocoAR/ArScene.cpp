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

//[DONE]Crear elementos geo 
//[DONE]Crear elementos posicionados relativos

//[DONE] Solucionar BUG -Mirando al Sur- Que lo arrelé el Mac del futuro.
//[DONE] Click en objetos -> 3 días
//[DONE] Prueba fiesta -> 3 días

//[TODO]Al eliminar un objeto, comprobar para eliminar el modelo.
//[TODO]A más de 1500m, cambiar el escalado del objeto y situarlo en el punto 1500.

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
#define	MYMESH04 "war3blacksmith.obj" //ERROR
#define	MYMESH05 "war3barracks.obj"
#define	MYMESH06 "war3_buildingsmall.obj"
#define	MYMESH08 "poste.obj"

//Funtions
#define DEG_TO_RAD(X) (X*M_PI/180.0)
#define RAD_TO_DEG(X) (X*180.0/M_PI)

cocos2d::CCLabelTTF* testInfo1,*testInfo2,*testInfo3;


GLfloat XCam=0.0f,YCam=1.0f,ZCam=0.0f;
GLfloat XEye=0.0f,YEye=1.5f,ZEye=0.5f;
GLfloat xrot3=0.0f,yrot3=0.0f,zrot3=0.0f;
GLfloat xUp=0.0f, yUp =0.0f, zUp=0.1f;

GLfloat eulerAngles[3];

static cocos2d::CCLocation userLocation;
static cocos2d::CCHeading userHeading;
static double adjustedUserHeading;

static vector<CCARGeneric3DModel*> models3D;
static vector<CCARGeneric3DObject*> objects3D;
static vector<CCARGeneric3DObject*> prueba3DObjects;

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

  CCDirector::sharedDirector()->setDisplayFPS(true);
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
  
  m_pMenu = CCMenu::menuWithItems(NULL);
	m_pMenu->setPosition( CCPointZero );
	this->addChild(m_pMenu, 1);
  
  testInfo1 = CCLabelTTF::labelWithString("", LABEL_FONT_TYPE, 12);
  testInfo1->setPosition(ccp(160,450));
  this->addChild(testInfo1,1);
  testInfo2 = CCLabelTTF::labelWithString("", LABEL_FONT_TYPE, 12);
  testInfo2->setPosition(ccp(160,440));
  this->addChild(testInfo2,1);
  testInfo3 = CCLabelTTF::labelWithString("", LABEL_FONT_TYPE, 12);
  testInfo3->setPosition(ccp(160,430));
  this->addChild(testInfo3,1);
  
  
  pLabelLocation = CCLabelTTF::labelWithString("", CCSize( CCDirector::sharedDirector()->getWinSize().width - 10, 20), CCTextAlignmentCenter ,LABEL_FONT_TYPE, 14);
  pLabelLocation->setPosition(ccp(160,25));
  
  pLabelGPS = CCLabelTTF::labelWithString("", CCSize(CCDirector::sharedDirector()->getWinSize().width - 10,20 ), CCTextAlignmentCenter ,LABEL_FONT_TYPE, 14);
  pLabelGPS->setPosition(ccp(160,10));
  
  pLabelHeading = CCLabelTTF::labelWithString("" ,LABEL_FONT_TYPE, 18);
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
  
  //  ccColor4F r = ccc4FFromccc4B(ccc4(255.0f,0.0f,0.0f,230.0f));
  //  ccColor4F g = ccc4FFromccc4B(ccc4(0.0f,255.0f,0.0f,230.0f));
  //  ccColor4F b = ccc4FFromccc4B(ccc4(0.0f,255.0f,255.0f,230.0f));
  //  ccColor4F a = ccc4FFromccc4B(ccc4(150.0f,255.0f,150.0f,230.0f));
  //  
  //  
  //  //Y
  //  //  ccDraw3DLine(5.0f,r,0.0f, 0.0f, 0.0f, 1500.0f, 0.0f, 0.0f);
  //  
  //  //Este & X
  //  ccDraw3DLine(5.0f,r,0.0f, 0.0f, 0.0f, 1500.0f, 0.0f, 0.0f);
  //  
  //  //Oeste
  //  ccDraw3DLine(5.0f,g ,0.0f, 0.0f, 0.0f, -1500.0f, 0.0f, 0.0f);
  //  
  //  //Norte & Z
  //  ccDraw3DLine(5.0f,b,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1500.0f);
  //  
  //  //Sur
  //  ccDraw3DLine(5.0f,a,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1500.0f);
  
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);	
  
  
  for(unsigned int i=0; i< objects3D.size(); i++){
    objects3D[i]->draw3D();
    //Se reordenan los objetos en el cocos2d
    reorderChild(objects3D[i], objects3D[i]->m_dDistance);
  }
  //  drawFloor();
  
	glDisable(GL_CULL_FACE);	
	glClearColor(0.0f,0.0f,0.0f,0.0f) ;		
	glPopMatrix();
  
	CCDirector::sharedDirector()->setDepthTest(true);
	glEnableClientState(GL_COLOR_ARRAY);
  
  CC_ENABLE_DEFAULT_GL_STATES();
	glClear(GL_DEPTH_BUFFER_BIT);
  
  loadObjectsButtons();
  
  CCLayer::visit();
  
  for(unsigned int i=0; i< objects3D.size(); i++){
    //     ccDrawLine(objects3D[i]->m_vScreenBox[0], objects3D[i]->m_vScreenBox[1]);
    //     ccDrawLine(objects3D[i]->m_vScreenBox[0], objects3D[i]->m_vScreenBox[2]);
    //     ccDrawLine(objects3D[i]->m_vScreenBox[3], objects3D[i]->m_vScreenBox[1]);
    //     ccDrawLine(objects3D[i]->m_vScreenBox[3], objects3D[i]->m_vScreenBox[2]);
    //     
    //     ccDrawPoint(objects3D[i]->m_vCenter);
    if (objects3D[i]->m_bModelBox) {
      if (objects3D[i]->m_layerDescription == NULL) {
        objects3D[i]->m_layerDescription = new ARObjectMenu::ARObjectMenu(this);
        this->addChild(objects3D[i]->m_layerDescription,0);
      }
    }
  }
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
  adjustedUserHeading = adjustedHeading;
  
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

int ite = 0;

void ArScene::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event){
  for(unsigned int i=0; i< objects3D.size(); i++){
    if (objects3D[i]->m_bModelBox == true) {
      objects3D[i]->m_bModelBox = false;
      if (objects3D[i]->m_layerDescription !=NULL) {
        objects3D[i]->m_layerDescription->removeLayerdescription(objects3D[i]);
      }
    }
  }
}

void ArScene::menuCloseCallback(CCObject* pSender)
{
  printf("\nmenuCloseCallback");
}

void ArScene::menuObjectPress(CCObject* pSender)
{
  CCARGeneric3DObject *myObject = (CCARGeneric3DObject *)pSender;
  
  //  printf("\nObject Name: %s", myObject->m_sObjectName.c_str());
  //  
  //  char buffer[256];
  //  sprintf(buffer,"Nombre %s",myObject->m_sObjectName.c_str());
  //  testInfo1->setString(buffer);
  //  sprintf(buffer,"Descripción %s",myObject->m_sDescription.c_str());
  //  testInfo2->setString(buffer);
  
  //  m_layerDescription = ARObjectMenu::ARObjectMenu(myObject);
  
  for(unsigned int i=0; i< objects3D.size(); i++){
    if (objects3D[i]->m_bModelBox == true && objects3D[i] != myObject) {
      objects3D[i]->m_bModelBox = false;
        if (objects3D[i]->m_layerDescription !=NULL) {
          objects3D[i]->m_layerDescription->removeLayerdescription(objects3D[i]);
        }
    }
  }
  
  if (myObject->m_bModelBox == false)
    myObject->m_bModelBox = true;
  else
    myObject->m_bModelBox = false;
  
}


void ArScene::loadTest()
{
  //  objects3D.push_back(new CCARObject3D(MESH_FLECHA, CCARType_Mesh, 1.5f, 0.0f, 0.0f, 500.0f));
  //  objects3D.push_back(new CCARGeo3DObject(MYMESH08, CCARType_Mesh, 0.1f ,-7.855653762817383f ,  42.344466179278236f));//JJ
  //  objects3D.push_back(new CCARGeo3DObject(MYMESH02, CCARType_Mesh, 0.1f ,-7.856292128562927f ,  42.34511443500709f));//Universidad
  //  objects3D.push_back(new CCARGeo3DObject(MYMESH03, CCARType_Mesh, 0.1f ,-7.855787873268127 ,  42.34516201314594f));// Universidad Norte 
  //  objects3D.push_back(new CCARGeo3DObject(TMDL_HELICOPTER, CCARType_TMDLModel, 0.25f ,-7.863861322402954f ,  42.34146865303537f));// Fuente san lazaro
  //  objects3D[4]->yTranslate = 200;
  //  objects3D[4]->xRotate = 270.0f;
  //  objects3D.push_back(new CCARGeo3DObject(MYMESH04, CCARType_Mesh, 0.1f ,-7.36083984375f ,  42.3037216984154f));// Orense oeste
  //  objects3D.push_back(new CCARGeo3DObject(MYMESH05, CCARType_Mesh, 0.1f ,-7.862091064453125f ,  42.348395259793f));// Orense rio
  //  objects3D.push_back(new CCARObject3D(MYMESH04, CCARType_Mesh,0.5f, -100.0f,0.0f, 1000.0f));
  //  objects3D.push_back(new CCARObject3D(MYMESH08, CCARType_Mesh,0.5f, -30.0f,0.0f,45.0f));
  //  objects3D.push_back(new CCARObject3D(MYMESH02, CCARType_Mesh,0.5f, -230.0f,0.0f,-300.0f));
  //  objects3D.push_back(new CCARObject3D(MYMESH06, CCARType_Mesh,0.5f, 700.0f,0.0f,100.0f));
  //  objects3D.push_back(new CCARObject3D(MYMESH02, CCARType_Mesh,0.5f, -230.0f,0.0f,-1200.0f));
  //  objects3D.push_back(new CCARObject3D(MESH_FLECHA, CCARType_Mesh,0.5f, 0.0f,0.0f,100.0f));
  //  objects3D.push_back(new CCARObject3D(MESH_FLECHA, CCARType_Mesh,0.5f, 100.0f,0.0,0.0f));
  //  objects3D.push_back(new CCARObject3D(MESH_FLECHA, CCARType_Mesh,0.5f, -100.0f,0.0f,0.0f));
  //  objects3D.push_back(new CCARObject3D(MESH_FLECHA, CCARType_Mesh,0.5f, 0.0f,0.0f,-100.0f));
  //  objects3D[1]->isRotating = false;
  //  objects3D.push_back(new CCARGeo3DObject(MESH_FLECHA, CCARType_Mesh, 0.1f ,-7.36083984375f ,  42.3037216984154f));// Orense oeste
  //  objects3D.push_back(new CCARGeo3DObject(TMDL_HELICOPTER, CCARType_TMDLModel, 0.3f ,-8.245668411254883f ,  43.494541820367246f));//Ferrol ria
  //  objects3D[0]->yTranslate = 200;
  //  objects3D[0]->xRotate = 270.0f;
  //  prueba3DObjects.push_back(new CCARObject3D(MYMESH04, CCARType_Mesh,0.5f, -100.0f,0.0f, 1000.0f)); // ERROR
  
  //  prueba3DObjects.push_back(new CCARObject3D(MYMESH02, CCARType_Mesh,0.5f, 600.0f,0.0f,-100.0f));
  //  prueba3DObjects[0]->isRotating = false;
  //  prueba3DObjects.push_back(new CCARObject3D(MYMESH01, CCARType_Mesh,1.0f, 300.0f,0.0f,-100.0f));
  //  prueba3DObjects.push_back(new CCARObject3D("CELSPDER.OBJ", CCARType_Mesh,20.0f, 50.0f,0.0f,-100.0f));
  //  prueba3DObjects.push_back(new CCARObject3D(MYMESH03, CCARType_Mesh,0.5f, 600.0f,0.0f,-100.0f));
  //  prueba3DObjects.push_back(new CCARObject3D(MYMESH05, CCARType_Mesh,0.5f, 600.0f,0.0f,-100.0f));
  //  prueba3DObjects.push_back(new CCARObject3D(MYMESH06, CCARType_Mesh,0.5f, 600.0f,0.0f,-100.0f));
  //  prueba3DObjects.push_back(new CCARObject3D(MYMESH08, CCARType_Mesh,0.5f, 600.0f,0.0f,-100.0f));
  //  prueba3DObjects.push_back(new CCARObject3D(MESH_FLECHA, CCARType_Mesh,0.5f,600.0f,0.0f,-100.0f));
  
  //  prueba3DObjects.push_back(new CCARObject3D(TMDL_HELICOPTER, CCARType_TMDLModel,0.5f, 100.0f,0.0,0.0f));
  //  prueba3DObjects.push_back(new CCARObject3D(TMDL_OSPREY, CCARType_TMDLModel,0.5f, -100.0f,0.0f,0.0f));
  
  CCARGeneric3DObject* object = NULL; 
  
  object = addARObject(new CCARObject3D(MYMESH08, CCARType_Mesh,"Objeto MYMESH08","Descripción MYMESH08", 0.5f, 300.0f,  0.0f, 300.0f));  
  object = addARObject(new CCARObject3D(MYMESH03, CCARType_Mesh,"Objeto MYMESH03","Descripción MYMESH03", 0.1f, -300.0f,  0.0f,  300.0f));
  object = addARObject(new CCARObject3D(MESH_FLECHA, CCARType_Mesh,"Objeto MESH_FLECHA1","Descripción MESH_FLECHA1",  0.5f,  300.0f,  0.0f, -300.0f));
  object = addARObject(new CCARObject3D(MESH_FLECHA, CCARType_Mesh,"Objeto MESH_FLECHA2","Descripción MESH_FLECHA2", 0.5f, -300.0f,  0.0f, -300.0f));
  object = addARObject(new CCARGeo3DObject(MYMESH02, CCARType_Mesh,"Objeto MYMESH02", "Descripción MYMESH02", 0.3,-7.856292128562927f , 42.34511443500709f));//Universidad
}


void ArScene::execTest()
{
  
}

void ccDraw3DLine(GLfloat lineWidth, cocos2d::ccColor4B color, GLfloat xOrigin, GLfloat yOrigin, GLfloat zOrigin, GLfloat xDestination, GLfloat yDestination, GLfloat zDestination)
{
	ccVertex3F vertices[2] = 
  {
    {xOrigin * CC_CONTENT_SCALE_FACTOR(), yOrigin * CC_CONTENT_SCALE_FACTOR(), zOrigin * CC_CONTENT_SCALE_FACTOR()},
    {xDestination * CC_CONTENT_SCALE_FACTOR(), yDestination * CC_CONTENT_SCALE_FACTOR(), zDestination * CC_CONTENT_SCALE_FACTOR() },
  };
  
  cocos2d::ccColor4F color4f = ccc4FFromccc4B(color);
  
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
  glLineWidth (lineWidth);
  glColor4f(color4f.r, color4f.g, color4f.b, color4f.a);
	glDrawArrays(GL_LINES, 0, 2);
	
	// restore default state
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
  glColor4f(0.0f,0.0f,0.0f,1.0f);
}

void drawFloor(){
  ccVertex3F vertices[4] = 
  {
    { 1500.0f* CC_CONTENT_SCALE_FACTOR(), 0.0f* CC_CONTENT_SCALE_FACTOR(),  1500.0f* CC_CONTENT_SCALE_FACTOR()},
    {-1500.0f* CC_CONTENT_SCALE_FACTOR(), 0.0f* CC_CONTENT_SCALE_FACTOR(),  1500.0f* CC_CONTENT_SCALE_FACTOR()},
    { 1500.0f* CC_CONTENT_SCALE_FACTOR(), 0.0f* CC_CONTENT_SCALE_FACTOR(), -1500.0f* CC_CONTENT_SCALE_FACTOR()},
    {-1500.0f* CC_CONTENT_SCALE_FACTOR(), 0.0f* CC_CONTENT_SCALE_FACTOR(), -1500.0f* CC_CONTENT_SCALE_FACTOR()},
  };
  glColor4f(0.5f,0.5f,0.5f,0.5f);
  
  glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glColor4f(1.0f,1.0f,1.0f,1.0f);
}


void ArScene::loadObjectsButtons(){
  
}

CCARGeneric3DObject* ArScene::addARObject(CCARGeneric3DObject* object){
  objects3D.push_back(object);
  object->m_labelDistance = CCLabelTTF::labelWithString("", LABEL_FONT_TYPE, 12);
  object->m_labelName = CCLabelTTF::labelWithString("", LABEL_FONT_TYPE, 12);
  object->m_labelDistance->setPosition(ccp(2000,2000));
  object->m_labelName->setPosition(ccp(2000,2000));
  addChild(object->m_labelDistance, 0);
  addChild(object->m_labelName, 0);
  m_pMenu->addChild(object, 5);
  return object;
}

void ArScene::deleteARObject(CCARGeneric3DObject* object){
  
  for(unsigned int i=0; i< models3D.size(); i++){
    if (objects3D[i] == object) {
      m_pMenu->removeChild(object, true);
      removeChild(object->m_labelName, true);
      removeChild(object->m_labelDistance, true);
      delete object;
    }
  }
}

CCARGeneric3DModel* ArScene::loadModel(string filename, CCARModelType modelType)
{
  CCARGeneric3DModel* toret = findModel(filename);
  
  if (!toret) {
    switch (modelType){
      case CCARType_Mesh: {
        Mesh *myMesh = new Mesh();
        myMesh->LoadModel(const_cast<char*> ( filename.c_str() ));
        myMesh->modelName = filename;
        models3D.push_back(myMesh);
        return myMesh;
        break; 
      }
      case CCARType_TMDLModel: 
      {
        TMDLModel *myTMDL = new TMDLModel();
        myTMDL->Init(const_cast<char*> ( filename.c_str()));
        myTMDL->SetSequence(0);
        myTMDL->modelName = filename;
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
double ArScene::getUserHeading(void){
  return adjustedUserHeading;
}
