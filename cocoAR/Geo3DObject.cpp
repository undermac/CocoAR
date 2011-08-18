//
//  Geo3DObject.cpp
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 09/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#include <iostream>
#include "cocos2d.h"
#include "Geo3DObject.h"
#include "ArScene.h"
#include "bMath.h"
#include <math.h>
#include "CCLocationManager.h"
#include "Matrix.h"
#include "Vector.h"


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
//  m_layerDescription = NULL;
  isRotating = true;
  m_bModelBox = false;
  m_bScreenBox = true;
  scale = 1.0f;
  xTranslate = 0.0f;
  yTranslate = 0.0f;
  zTranslate = 0.0f;
  xRotate = 0.0f;
  yRotate = 0.0f;
  zRotate = 0.0f;
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

void CCARObject3D::draw3D(){
  m_dBearing = 360.0f - RAD_TO_DEG(Atan2(xTranslate, zTranslate));
  m_dBearing > 360.0f ? m_dBearing -= 360.0f : true;
  m_dDistance = sqrt((xTranslate*xTranslate)+(zTranslate*zTranslate));
//  printf("\nDistance: %f, bearing: %f", m_dDistance, m_dBearing);
  
  
  glPushMatrix();
  glTranslatef(xTranslate, yTranslate, zTranslate);
  glScalef(scale, scale, scale );
  glRotatef(zRotate, 0, 0, 1); 
  if (isRotating) {
    if (rotationValue > 360)
      rotationValue = 0;
    else
      rotationValue += 3;
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
//  m_layerDescription = NULL;
  isRotating = true;
  m_bModelBox = false;
  m_bScreenBox = true;
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

void CCARGeo3DObject::draw3D(){
  cocos2d::CCLocation userlocation = ArScene::getUserlocation();
  m_dDistance = cocos2d::CCLocationManager::sharedCCLocationManager()->distanceFromLocation( (double)userlocation.latitude,(double)userlocation.longitude , (double)this->latitude, (double)this->longitude);
  m_dBearing = cocos2d::CCLocationManager::sharedCCLocationManager()->bearingBetweenStartLocation( (double)userlocation.latitude, (double)userlocation.longitude , (double)this->latitude, (double)this->longitude);
  
  m_dBearing = RAD_TO_DEG(m_dBearing);
  double angleC,x,z;
  if (m_dBearing > 0.0f && m_dBearing < 90.f) {           //1
    angleC = 90.0f - m_dBearing;
    x = - cos(DEG_TO_RAD(angleC))*m_dDistance;
    z = sin(DEG_TO_RAD(angleC))*m_dDistance;
  }else if (m_dBearing > 90.0f && m_dBearing < 180.0f){   //2
    angleC = 180.0f - m_dBearing;
    x = -sin(DEG_TO_RAD(angleC))*m_dDistance;
    z = -cos(DEG_TO_RAD(angleC))*m_dDistance;
  }else if (m_dBearing > 180.0f && m_dBearing < 270.0f){  //3
    angleC = 270.0f - m_dBearing;
    x = cos(DEG_TO_RAD(angleC))*m_dDistance;
    z = - sin(DEG_TO_RAD(angleC))*m_dDistance;
  }else{                                                  //4
    angleC = 360.0f - m_dBearing;
    x = sin(DEG_TO_RAD(angleC))*m_dDistance;
    z = cos(DEG_TO_RAD(angleC))*m_dDistance;
  }
  
  glPushMatrix();
  glTranslatef(xTranslate + x, yTranslate, zTranslate + z);
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
  
//  cocos2d::ccVertex3F center = {(model3D->m_vtxMax.x - (model3D->m_vtxMin.x*0.5f)),(model3D->m_vtxMax.y - (model3D->m_vtxMin.y*0.5f)),(model3D->m_vtxMax.z - (model3D->m_vtxMin.z*0.5f))};
//  m_vCenter = covert3Dto2d(center);

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
  
  m_vCenter = ccp(((maxPoint.x + minPoint.x)*0.5f), ((maxPoint.y + minPoint.y)*0.5f));
  
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
  }else{
    setIsEnabled(true);
    this->setPosition(m_vCenter);
    setScaleX((float)newSize.width/m_size.width);
    setScaleY((float)newSize.height/m_size.height);
    char buffer[256];
    if (m_dDistance > 1000.0f) sprintf(buffer,"%4.1fKm",m_dDistance);
    else sprintf(buffer,"%4.0fm",m_dDistance);
//    if (m_layerDescription) {
//      m_layerDescription->setPosition(m_vCenter);
//      m_labelDistance->setString("");
//      m_labelName->setString("");
//    }else{
          m_labelDistance->setString(buffer);
          m_labelName->setString(m_sObjectName.c_str());
//    }
    m_labelDistance->setPosition(ccp(((maxPoint.x + minPoint.x)*0.5f), minPoint.y - 18));
    m_labelName->setPosition(ccp(((maxPoint.x + minPoint.x)*0.5f), minPoint.y - 8));
  }
}

void CCARGeneric3DObject::drawBox(GLfloat lineWidth, cocos2d::ccColor4B color)
{ 
  cocos2d::ccVertex3F boxVectorMax = model3D->m_vtxMax;
  cocos2d::ccVertex3F boxVectorMin = model3D->m_vtxMin;
  
//  cocos2d::ccColor4F r = ccc4FFromccc4B(cocos2d::ccc4(1.0f,0.0f,0.0f,1.0f));
//  cocos2d::ccColor4F g = ccc4FFromccc4B(cocos2d::ccc4(0.0f,1.0f,0.0f,1.0f));
//  cocos2d::ccColor4F b = ccc4FFromccc4B(cocos2d::ccc4(0.0f,0.0f,1.0f,1.0f));
//    
//  ccDraw3DLine(10.0f,r, 0.0f, 0.0f, 0.0f, 80.0f, 0.0f, 0.0f);  // x
//  ccDraw3DLine(10.0f,g, 0.0f, 0.0f, 0.0f, 0.0f, 80.0f, 0.0f);  // y
//  ccDraw3DLine(10.0f,b, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 80.0f);  // z
  
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
