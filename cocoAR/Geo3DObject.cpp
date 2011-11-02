//
//  Geo3DObject.cpp
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 09/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//


#include "Geo3DObject.h"
#include "ArScene.h"
#include "ArSceneController.h"

#define LABEL_FONT_TYPE "Thonburi"
#define DEG_TO_RAD(X) (X*M_PI/180.0)
#define RAD_TO_DEG(X) (X*180.0/M_PI)

extern cocos2d::CCLabelTTF* testInfo1,*testInfo2,*testInfo3;

USING_NS_CC;

CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType,std::string objectName,std::string objectDescription){  
  initFromNormalImage("transparent.png", NULL, NULL, this, menu_selector(ArScene::menuObjectPress));
  
  m_sObjectName = objectName;
  m_sDescription = objectDescription;
  m_labelDistance = NULL;
  m_labelName = NULL;
  m_layerDescription = NULL;
  isRotating = true;
  m_bModelBox = false;
  m_bScreenBox = true;
  m_bNew = true;
  m_bDelete = false;
  scale = 1.0f;
  xTranslate = 0.0f;
  yTranslate = 0.0f;
  zTranslate = 0.0f;
  xRotate = 0.0f;
  yRotate = 0.0f;
  zRotate = 0.0f;
  m_dXValue = 0;
  m_dYValue = 0;
  m_dZValue = 0;
  m_size = CCSize(20.0f,20.0f);
  
  model3D = ArScene::loadModel(model, modelType);
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType,std::string objectName,std::string objectDescription, double sca){
  new (this) CCARObject3D::CCARObject3D(model,modelType,objectName,objectDescription);
  scale = sca;
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType,std::string objectName,std::string objectDescription, double sca, double x, double y, double z){
  new (this) CCARObject3D::CCARObject3D(model,modelType,objectName, objectDescription ,sca);
  xTranslate = x;
  yTranslate = y;
  zTranslate = z;
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType,std::string objectName,std::string objectDescription, double sca, double x, double y, double z, double xRotate, double yRotate, double zRotate){
  new (this) CCARObject3D(model,modelType,objectName, objectDescription,sca,x,y,z);
  xRotate = xRotate;
  yRotate = yRotate;
  zRotate = zRotate;
}

void CCARObject3D::locateObject(){
  m_dBearing = 360.0f - RAD_TO_DEG(Atan2(xTranslate, zTranslate));
  m_dBearing > 360.0f ? m_dBearing -= 360.0f : true;
  m_dDistance = sqrt((xTranslate*xTranslate)+(zTranslate*zTranslate));
}

void CCARObject3D::draw3D(){
  glPushMatrix();
  glTranslatef(xTranslate, yTranslate, zTranslate);
  glScalef(scale, scale, scale );
  glRotatef(zRotate, 0, 0, 1); 
  if (isRotating) {
    if (rotationValue > 360)
      rotationValue = 0;
    else
      rotationValue += 1;
    glRotatef(yRotate + rotationValue, 0, 1, 0);
  }else
    glRotatef(yRotate, 0, 1, 0);
  glRotatef(xRotate, 1, 0, 0);
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  model3D->draw3D();
  if(m_bModelBox)
    drawBox(3.0f, cocos2d::ccc4(0.0f,255.0f,0.0f,230.0f));
  calculateScreenBox();
  glPopMatrix();
}

CCARGeo3DObject::CCARGeo3DObject(){}

CCARGeo3DObject::CCARGeo3DObject(std::string filename, CCARModelType modelType,std::string objectName,std::string objectDescription, double lon, double lat){
  
  initFromNormalImage("transparent.png", NULL, NULL, this, menu_selector(ArScene::menuObjectPress));
  
  m_sObjectName = objectName;
  m_sDescription = objectDescription;
  m_labelDistance = NULL;
  m_labelName = NULL;
  m_layerDescription = NULL;
  isRotating = true;
  m_bModelBox = false;
  m_bScreenBox = true;
  m_bNew = true;
  m_bDelete = false;
  m_size = CCSize(20.0f,20.0f);
  scale = 1.0f;
  xTranslate = 0.0f;
  yTranslate = 0.0f;
  zTranslate = 0.0f;
  xRotate = 0.0f;
  yRotate = 0.0f;
  zRotate = 0.0f;
  longitude = lon;
  latitude = lat;
  altitude = 0.0f;
  m_dXValue = 0;
  m_dYValue = 0;
  m_dZValue = 0;
  model3D = ArScene::loadModel(filename, modelType);
}


CCARGeo3DObject::CCARGeo3DObject(std::string model, CCARModelType modelType,std::string objectName,std::string objectDescription ,double sca , double lon, double lat){
  new (this) CCARGeo3DObject::CCARGeo3DObject(model, modelType,objectName,objectDescription, lon, lat);
  scale = sca;
}

CCARGeo3DObject::CCARGeo3DObject(std::string model, CCARModelType modelType,std::string objectName,std::string objectDescription,double sca , double lon, double lat, double alt){
  new (this) CCARGeo3DObject::CCARGeo3DObject(model, modelType,objectName, objectDescription,scale, lon, lat);
  altitude = alt;
}

void CCARGeo3DObject::locateObject(){
  cocos2d::CCLocation userlocation = ArScene::getUserlocation();
  m_dDistance = cocos2d::CCLocationManager::sharedCCLocationManager()->distanceFromLocation( (double)userlocation.latitude,(double)userlocation.longitude , (double)latitude, (double)longitude);
  m_dBearing = cocos2d::CCLocationManager::sharedCCLocationManager()->bearingBetweenStartLocation( (double)userlocation.latitude, (double)userlocation.longitude , (double)latitude, (double)longitude);
  
  m_dBearing = RAD_TO_DEG(m_dBearing);
  double angleC;
  if (m_dBearing > 0.0f && m_dBearing < 90.f) {           //1
    angleC = 90.0f - m_dBearing;
    m_dXValue = - cos(DEG_TO_RAD(angleC))*m_dDistance;
    m_dZValue = sin(DEG_TO_RAD(angleC))*m_dDistance;
  }else if (m_dBearing > 90.0f && m_dBearing < 180.0f){   //2
    angleC = 180.0f - m_dBearing;
    m_dXValue = -sin(DEG_TO_RAD(angleC))*m_dDistance;
    m_dZValue = -cos(DEG_TO_RAD(angleC))*m_dDistance;
  }else if (m_dBearing > 180.0f && m_dBearing < 270.0f){  //3
    angleC = 270.0f - m_dBearing;
    m_dXValue = cos(DEG_TO_RAD(angleC))*m_dDistance;
    m_dZValue = - sin(DEG_TO_RAD(angleC))*m_dDistance;
  }else{                                                  //4
    angleC = 360.0f - m_dBearing;
    m_dXValue = sin(DEG_TO_RAD(angleC))*m_dDistance;
    m_dZValue = cos(DEG_TO_RAD(angleC))*m_dDistance;
  }
}

void CCARGeo3DObject::draw3D(){
  glPushMatrix();
  glTranslatef(xTranslate + m_dXValue, yTranslate, zTranslate + m_dZValue);
  glScalef(scale, scale, scale );
  if (isRotating) {
    if (rotationValue > 360)
      rotationValue = 0;
    else
      rotationValue += 1;
    glRotatef(yRotate + rotationValue, 0, 1, 0);
  }else
    glRotatef(yRotate, 0, 1, 0);
  glRotatef(xRotate, 1, 0, 0);
  glRotatef(zRotate, 0, 0, 1);
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  model3D->draw3D();
  if(m_bModelBox)
    drawBox(3.0f, ccc4(0.0f,255.0f,0.0f,230.0f));
  calculateScreenBox();
  glPopMatrix();
}

void CCARGeneric3DObject::selectedObject(){
  arSceneObjectSelected(this);
}

void CCARGeneric3DObject::calculateScreenBox(){
  
  cocos2d::ccVertex3F boxVertex[8] = 
  {
    { model3D->m_vtxMin.x, model3D->m_vtxMin.y,  model3D->m_vtxMin.z},//000
    { model3D->m_vtxMin.x, model3D->m_vtxMin.y,  model3D->m_vtxMax.z},//001
    { model3D->m_vtxMin.x, model3D->m_vtxMax.y,  model3D->m_vtxMin.z},//010
    { model3D->m_vtxMin.x, model3D->m_vtxMax.y,  model3D->m_vtxMax.z},//011
    { model3D->m_vtxMax.x, model3D->m_vtxMin.y,  model3D->m_vtxMin.z},//100
    { model3D->m_vtxMax.x, model3D->m_vtxMin.y,  model3D->m_vtxMax.z},//101
    { model3D->m_vtxMax.x, model3D->m_vtxMax.y,  model3D->m_vtxMin.z},//110
    { model3D->m_vtxMax.x, model3D->m_vtxMax.y,  model3D->m_vtxMax.z},//111
  };
  
  cocos2d::ccVertex3F center = {((model3D->m_vtxMax.x + model3D->m_vtxMin.x)*0.5f),(model3D->m_vtxMax.y + model3D->m_vtxMin.y)*0.5f,(model3D->m_vtxMax.z + model3D->m_vtxMin.z)*0.5f};
  m_vCenter = covert3Dto2d(center);
  
  cocos2d::ccVertex3F labelPoint = {((model3D->m_vtxMax.x + model3D->m_vtxMin.x)*0.5f),model3D->m_vtxMin.y,(model3D->m_vtxMax.z + model3D->m_vtxMin.z)*0.5f};
  
  cocos2d::CCPoint vLabels = covert3Dto2d(labelPoint);
  m_vCenter = covert3Dto2d(center);

  cocos2d::CCPoint maxPoint = covert3Dto2d(boxVertex[0]);
  cocos2d::CCPoint minPoint = maxPoint;
  
  for (unsigned int i=1; i < 8;i++ ) {
    cocos2d::CCPoint aux = covert3Dto2d(boxVertex[i]);
    //Max
    if (aux.x > maxPoint.x)
      maxPoint.x = aux.x;
    if (aux.y > maxPoint.y)
      maxPoint.y = aux.y;
    //min
    if (aux.x < minPoint.x)
      minPoint.x = aux.x;
    if (aux.y < minPoint.y)
      minPoint.y = aux.y;
  }
  
  CCSize newSize = CCSize((maxPoint.x - minPoint.x) < 50.0f ? 50.0f: (maxPoint.x - minPoint.x)*0.5f
                          ,(maxPoint.y - minPoint.y) < 50.0f ? 50.0f: (maxPoint.y - minPoint.y)*0.5f);
  
  if (newSize.width > 500.0f) newSize.width = 500.f;
  if (newSize.height > 500.0f) newSize.height = 500.f;
  
  this->setPosition(m_vCenter);

  setScaleX((float)newSize.width/m_size.width);
  setScaleY((float)newSize.height/m_size.height);

  m_vScreenBox[0] = CCPoint(minPoint.x,minPoint.y);
  m_vScreenBox[1] = CCPoint(minPoint.x,maxPoint.y);
  m_vScreenBox[2] = CCPoint(maxPoint.x,minPoint.y);
  m_vScreenBox[3] = CCPoint(maxPoint.x,maxPoint.y);
  
  double userHeading = ArScene::getUserHeading();
  double objectBearing = m_dBearing;
  double diference = 0.0f;
  if (userHeading < 90.0f) userHeading += 360.0f;
  if (objectBearing < 90.0f) objectBearing += 360.0f;
    
  if (userHeading > objectBearing) diference =  (userHeading+360.0f) - (objectBearing+360.0f);
  else diference = (objectBearing+360.0f) - (userHeading+360.0f);
  
  if (diference > 90.0f){ 
    setIsEnabled(false);
    m_labelDistance->setString("");
    m_labelName->setString("");
  }else{
    setIsEnabled(true);
    this->setPosition(m_vCenter);
    setScaleX((float)newSize.width/m_size.width);
    setScaleY((float)newSize.height/m_size.height);
    char buffer[256];
    if (m_dDistance > 1000.0f) sprintf(buffer,"%4.1fKm",m_dDistance*0.001);
    else sprintf(buffer,"%4.0fm",m_dDistance);
    if (m_layerDescription) {
      m_layerDescription->setPosition(m_vCenter);
      m_labelDistance->setString("");
      m_labelName->setString("");
    }else{
          m_labelDistance->setString(buffer);
          m_labelName->setString(m_sObjectName.c_str());
    }
    m_labelDistance->setPosition(ccp(vLabels.x, vLabels.y - 20));
    m_labelName->setPosition(ccp(vLabels.x, vLabels.y - 12));
  }
}

void CCARGeneric3DObject::locateObject(){
}

void CCARGeneric3DObject::drawBox(GLfloat lineWidth, cocos2d::ccColor4B color)
{ 
  cocos2d::ccVertex3F boxVectorMax = model3D->m_vtxMax;
  cocos2d::ccVertex3F boxVectorMin = model3D->m_vtxMin;
  
  ccDraw3DLine(lineWidth,color, boxVectorMax.x, boxVectorMax.y, boxVectorMin.z, boxVectorMax.x, boxVectorMax.y, boxVectorMax.z);  // 1 2
  ccDraw3DLine(lineWidth,color, boxVectorMax.x, boxVectorMax.y, boxVectorMin.z, boxVectorMin.x, boxVectorMax.y, boxVectorMin.z);  // 1 3
  ccDraw3DLine(lineWidth,color, boxVectorMax.x, boxVectorMax.y, boxVectorMin.z, boxVectorMax.x, boxVectorMin.y, boxVectorMin.z);  // 1 4  
  ccDraw3DLine(lineWidth,color, boxVectorMax.x, boxVectorMin.y, boxVectorMin.z, boxVectorMax.x, boxVectorMin.y, boxVectorMax.z);  // 4 5
  ccDraw3DLine(lineWidth,color, boxVectorMax.x, boxVectorMin.y, boxVectorMin.z, boxVectorMin.x, boxVectorMin.y, boxVectorMin.z);  // 4 6
  ccDraw3DLine(lineWidth,color, boxVectorMin.x, boxVectorMin.y, boxVectorMin.z, boxVectorMin.x, boxVectorMax.y, boxVectorMin.z);  // 6 3  
  ccDraw3DLine(lineWidth,color, boxVectorMin.x, boxVectorMin.y, boxVectorMin.z, boxVectorMin.x, boxVectorMin.y, boxVectorMax.z);  // 6 7
  ccDraw3DLine(lineWidth,color, boxVectorMin.x, boxVectorMax.y, boxVectorMin.z, boxVectorMin.x, boxVectorMax.y, boxVectorMax.z);  // 3 8
  ccDraw3DLine(lineWidth,color, boxVectorMax.x, boxVectorMax.y, boxVectorMax.z, boxVectorMax.x, boxVectorMin.y, boxVectorMax.z);  // 2 5
  ccDraw3DLine(lineWidth,color, boxVectorMax.x, boxVectorMax.y, boxVectorMax.z, boxVectorMin.x, boxVectorMax.y, boxVectorMax.z);  // 2 8
  ccDraw3DLine(lineWidth,color, boxVectorMin.x, boxVectorMin.y, boxVectorMax.z, boxVectorMin.x, boxVectorMax.y, boxVectorMax.z);  // 7 8
  ccDraw3DLine(lineWidth,color, boxVectorMin.x, boxVectorMin.y, boxVectorMax.z, boxVectorMax.x, boxVectorMin.y, boxVectorMax.z);  // 7 5
}


cocos2d::CCPoint CCARGeneric3DObject::covert3Dto2d(cocos2d::ccVertex3F vertex){
  ////http://www.opengl.org/wiki/GluProject_and_gluUnProject_code
  ////http://www.opengl.org/sdk/docs/man/xhtml/gluProject.xml
  Matrix matProjection, matModelView;
  
  glGetFloatv(GL_PROJECTION_MATRIX, matProjection.m);
  glGetFloatv(GL_MODELVIEW_MATRIX, matModelView.m);
  
  Vector Position2D=Matrix_multiplyVector(matModelView, Vector_withValues(vertex.x,vertex.y,vertex.z));
  Vector vectorp=Matrix_multiplyVector(matProjection, Position2D);
  
  // Esto es la conversion del vector transformado a Coordenadas homogeneas.
  float w=-(1/Position2D.z);
  vectorp.x*=w;
  vectorp.y*=w;
  vectorp.z*=w;
  
  // El viewport se compone de el offset + width y height
  int viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  float xPos=viewport[0]+viewport[2]*float(vectorp.x+1)/2;
  float yPos=viewport[1]+viewport[3]*float(vectorp.y+1)/2;

  return ccp(xPos, yPos);
}



ARObjectMenu::ARObjectMenu(CCNode *pObject){
  
//  CCARGeneric3DObject* selectedObject = (CCARGeneric3DObject*)pObject;
//  
//  printf("touched %s", selectedObject->m_sObjectName.c_str());
  
//  m_labelName = NULL;
//  m_labelDescription = NULL;
//  setIsTouchEnabled(true);
//	setIsVisible(true);
  
  //300x156 fondoObjeto.png
//-------------------------------------------------------------------------------
  
//  CCSprite* my_background = CCSprite::spriteWithFile("fondoObjeto.png");
//  my_background->setPosition(ccp(0, 0));
//  addChild(my_background,0);
//
//  CCARGeneric3DObject* myObject = (CCARGeneric3DObject*)pObject;
//  
//  char buffer[256];
//  sprintf(buffer,"%s",myObject->m_sObjectName.c_str());
//  
//  m_labelName = CCLabelTTF::labelWithString( buffer , CCSize(270,15) , CCTextAlignmentCenter ,LABEL_FONT_TYPE, 16);
//  m_labelName->setPosition(ccp(0,65));
//  
//  sprintf(buffer,"%s",myObject->m_sDescription.c_str());
//  m_labelDescription = CCLabelTTF::labelWithString(buffer, CCSize(270,85) , CCTextAlignmentLeft, LABEL_FONT_TYPE, 14);
//  m_labelDescription->setPosition(ccp(0,50));
//  
//  addChild(m_labelName,1);
//  addChild(m_labelDescription,1);
//
//  setOpacity(200);
//  
//  runAction(CCSequence::actions(CCFadeIn::actionWithDuration(3.0f),NULL));
  
//-------------------------------------------------------------------------------
  
  //  CGSize maxSize = { CCDirector::sharedDirector()->getWinSize().width - 50, CCDirector::sharedDirector()->getWinSize().height - 50 };
  //  CGSize actualSize = [pObject->m_sDescription sizeWithFont:[UIFont fontWithName:@"Thonburi" size:14]
  //                        constrainedToSize:maxSize
  //                            lineBreakMode:UILineBreakModeWordWrap];
}
ARObjectMenu::~ARObjectMenu(){
  
}



void ARObjectMenu::setOpacity(GLubyte var)
{
	m_cOpacity = var;
  
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = (CCNode*) pObject;
			if (pChild)
			{
				CCRGBAProtocol *pRGBAProtocol = pChild->convertToRGBAProtocol();
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setOpacity(m_cOpacity);
				}
			}
		}
	}
}

GLubyte ARObjectMenu::getOpacity(void){
  return m_cOpacity;
}
cocos2d::ccColor3B ARObjectMenu::getColor(void)
{
  return m_tColor;
}
void ARObjectMenu::setColor(cocos2d::ccColor3B color)
{
  m_tColor = color;
	
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = (CCNode*) pObject;
			if (pChild)
			{
				CCRGBAProtocol *pRGBAProtocol = pChild->convertToRGBAProtocol();
				if (pRGBAProtocol)
				{
					pRGBAProtocol->setColor(m_tColor);
				}
			}
		}
	}
}
void ARObjectMenu::removeLayerdescription(CCARGeneric3DObject* pObject){
	setIsTouchEnabled(false);
	runAction(CCSequence::actions(CCFadeOut::actionWithDuration(2.0f),
                                CCCallFuncN::actionWithTarget(this,callfuncN_selector(ARObjectMenu::removeMenu))));
  pObject->m_layerDescription = NULL;
}

void ARObjectMenu::removeMenu(){
  removeChild(this, true);
  
}
