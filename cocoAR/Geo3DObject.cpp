//
//  Geo3DObject.cpp
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 09/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#include <iostream>
#include "Geo3DObject.h"
#include "ArScene.h"
#include "bMath.h"
#include <math.h>
#include "CCLocationManager.h"
#include "Matrix.h"
#include "Vector.h"

USING_NS_CC;

CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType){
  isRotating = true;
  m_bModelBox = true;
  scale = 1.0f;
  xTranslate = 0.0f;
  yTranslate = 0.0f;
  zTranslate = 0.0f;
  xRotate = 0.0f;
  yRotate = 0.0f;
  zRotate = 0.0f;

  model3D = ArScene::loadModel(model, modelType);
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType, double sca){
  new (this) CCARObject3D::CCARObject3D(model,modelType);
  scale = sca;
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType, double sca, double x, double y, double z){
  new (this) CCARObject3D::CCARObject3D(model,modelType,sca);
  xTranslate = x;
  yTranslate = y;
  zTranslate = z;
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType, double sca, double x, double y, double z, double xRotate, double yRotate, double zRotate){
  new (this) CCARObject3D(model,modelType,sca,x,y,z);
  xRotate = xRotate;
  yRotate = yRotate;
  zRotate = zRotate;
}

void CCARObject3D::draw(){
  glPushMatrix();
  glTranslatef(xTranslate, yTranslate, zTranslate);
  glScalef(scale, scale, scale );
  glRotatef(zRotate, 0, 0, 1); 
  if (isRotating) {
    if (rotationValue > 360)
      rotationValue = 0;
    else
      rotationValue += arc4random()%5;
    glRotatef(yRotate + rotationValue, 0, 1, 0);
  }else
    glRotatef(yRotate, 0, 1, 0);
  glRotatef(xRotate, 1, 0, 0);
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  model3D->Draw();
  cocos2d::ccColor4F red = ccc4FFromccc4B(cocos2d::ccc4(255.0f,0.0f,0.0f,230.0f));
  if(m_bModelBox)
    drawBox(3.0f, red);
  calculateScreenBox();
  glPopMatrix();
}

CCARGeo3DObject::CCARGeo3DObject(std::string model, CCARModelType modelType, double lon, double lat){
  isRotating = true;
  m_bModelBox = true;
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
  model3D = ArScene::loadModel(model, modelType);
}
CCARGeo3DObject::CCARGeo3DObject(std::string model, CCARModelType modelType,double sca , double lon, double lat){
  new (this) CCARGeo3DObject::CCARGeo3DObject(model, modelType, lon, lat);
  scale = sca;
}
CCARGeo3DObject::CCARGeo3DObject(std::string model, CCARModelType modelType,double sca , double lon, double lat, double alt){
  new (this) CCARGeo3DObject::CCARGeo3DObject(model, modelType,scale, lon, lat);
  altitude = alt;
}

void CCARGeo3DObject::draw(){
  cocos2d::CCLocation userlocation = ArScene::getUserlocation();
  double distanceValue = cocos2d::CCLocationManager::sharedCCLocationManager()->distanceFromLocation( (double)userlocation.latitude,(double)userlocation.longitude , (double)this->latitude, (double)this->longitude);
  double bearing = cocos2d::CCLocationManager::sharedCCLocationManager()->bearingBetweenStartLocation( (double)userlocation.latitude, (double)userlocation.longitude , (double)this->latitude, (double)this->longitude);
  
  double angleC,x,z;
  if (bearing*(180/_pi) > 0.0f && bearing*(180/_pi) < 90.f) {//1
    angleC = 90.0f - bearing*(180/_pi);
    x = - cos(angleC*(_pi/180))*distanceValue;
    z = sin(angleC*(_pi/180))*distanceValue;
  }else if (bearing*(180/_pi) > 90.0f && bearing*(180/_pi) < 180.0f){ //2
    angleC = 180.0f - bearing*(180/_pi);
    x = -sin(angleC*(_pi/180))*distanceValue;
    z = -cos(angleC*(_pi/180))*distanceValue;
  }else if (bearing*(180/_pi) > 180.0f && bearing*(180/_pi) < 270.0f){ //3
    angleC = 270.0f - bearing*(180/_pi);
    x = cos(angleC*(_pi/180))*distanceValue;
    z = - sin(angleC*(_pi/180))*distanceValue;
  }else{ //4
    angleC = 360.0f - bearing*(180/_pi);
    x = sin(angleC*(_pi/180))*distanceValue;
    z = cos(angleC*(_pi/180))*distanceValue;
  }
  
  glPushMatrix();
  glTranslatef(xTranslate + x, yTranslate, zTranslate + z);
  glScalef(scale, scale, scale );
  if (isRotating) {
    if (rotationValue > 360)
      rotationValue = 0;
    else
      rotationValue += arc4random()%5;
    glRotatef(yRotate + rotationValue, 0, 1, 0);
  }else
    glRotatef(yRotate, 0, 1, 0);
  glRotatef(xRotate, 1, 0, 0);
  glRotatef(zRotate, 0, 0, 1);
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
  model3D->Draw();
  cocos2d::ccColor4F g = ccc4FFromccc4B(cocos2d::ccc4(0.0f,255.0f,0.0f,230.0f));
  if(m_bModelBox)
    drawBox(3.0f, g);
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

  cocos2d::CCPoint maxPoint = covert3Dto2d(boxVertex[0]);; 
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
      maxPoint.x = aux.x;
    if (aux.y < minPoint.y)
      maxPoint.y = aux.y;
  }
  
  m_vScreenBox[0] = CCPoint(minPoint.x,minPoint.y);
  m_vScreenBox[1] = CCPoint(minPoint.x,maxPoint.y);
  m_vScreenBox[2] = CCPoint(maxPoint.x,minPoint.y);
  m_vScreenBox[3] = CCPoint(maxPoint.x,maxPoint.y);
}

void CCARGeneric3DObject::drawBox(GLfloat lineWidth, cocos2d::ccColor4F color)
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




