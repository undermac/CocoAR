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


CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType){
  isRotating = true;
  scale = 1.0f;
  xTranslate = 0.0f;
  yTranslate = 0.0f;
  zTranslate = 0.0f;
  xRotate = 0.0f;
  yRotate = 0.0f;
  zRotate = 0.0f;
  model3D = ArScene::loadModel(model, modelType);
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType, double scale){
  new (this) CCARObject3D::CCARObject3D(model,modelType);
  scale = scale;
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType, double scale, double x, double y, double z){
  new (this) CCARObject3D::CCARObject3D(model,modelType,scale);
  xTranslate = x;
  yTranslate = y;
  zTranslate = z;
}
CCARObject3D::CCARObject3D(std::string model, CCARModelType modelType, double scale, double x, double y, double z, double xRotate, double yRotate, double zRotate){
  new (this) CCARObject3D(model,modelType,scale,x,y,z);
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
  model3D->Draw();
  glPopMatrix();
}

CCARGeo3DObject::CCARGeo3DObject(std::string model, CCARModelType modelType, double lon, double lat){
  isRotating = true;
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
  model3D->Draw();
  glPopMatrix();
}