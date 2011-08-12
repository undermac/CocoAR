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
  glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
  model3D->Draw();
  cocos2d::ccColor4F red = ccc4FFromccc4B(cocos2d::ccc4(255.0f,0.0f,0.0f,230.0f));
  model3D->covert3Dto2d();
  model3D->drawBox(3.0f,model3D->m_vtxMax,model3D->m_vtxMin, red);
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
  glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
  model3D->Draw();
  cocos2d::ccColor4F g = ccc4FFromccc4B(cocos2d::ccc4(0.0f,255.0f,0.0f,230.0f));
  model3D->covert3Dto2d();
  model3D->drawBox(3.0f,model3D->m_vtxMax,model3D->m_vtxMin, g);
  glPopMatrix();
}

void CCARGeneric3DModel::drawBox(GLfloat lineWidth, cocos2d::ccVertex3F maxVertex,cocos2d::ccVertex3F minVertex, cocos2d::ccColor4F color)
{ 
  cocos2d::ccVertex3F boxVectorMax = maxVertex;
  cocos2d::ccVertex3F boxVectorMin = minVertex;
  
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


cocos2d::CCPoint CCARGeneric3DModel::covert3Dto2d(){
  Matrix matProjection, matModelView;
  
  glGetFloatv(GL_PROJECTION_MATRIX, matProjection.m);
  glGetFloatv(GL_MODELVIEW_MATRIX, matModelView.m);
  
  //printMatrix(&matModelView.m[0]);
  //printMatrix(&matProjection.m[0]);
  Vector Position2D=Matrix_multiplyVector(matModelView, Vector_withValues(0,0,0));
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
  
  printf("\n xPos:%2.0f %2.0f", xPos,yPos);

  return ccp(xPos, yPos);
}
//
//
////http://www.opengl.org/wiki/GluProject_and_gluUnProject_code
////http://www.opengl.org/sdk/docs/man/xhtml/gluProject.xml
//
//int glhProjectf(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate)
//{
//  //Transformation vectors
//  float fTempo[8];
//  //Modelview transform
//  fTempo[0]=modelview[0]*objx+modelview[4]*objy+modelview[8]*objz+modelview[12];  //w is always 1
//  fTempo[1]=modelview[1]*objx+modelview[5]*objy+modelview[9]*objz+modelview[13];
//  fTempo[2]=modelview[2]*objx+modelview[6]*objy+modelview[10]*objz+modelview[14];
//  fTempo[3]=modelview[3]*objx+modelview[7]*objy+modelview[11]*objz+modelview[15];
//  
//  LOGV(" After Modelview %2.2f %2.2f %2.2f %2.2f ",fTempo[0],fTempo[1],fTempo[2],fTempo[3]);
//  //Projection transform, the final row of projection matrix is always [0 0 -1 0]
//  //so we optimize for that.
//  fTempo[4]=projection[0]*fTempo[0]+projection[4]*fTempo[1]+projection[8]*fTempo[2]+projection[12]*fTempo[3];
//  fTempo[5]=projection[1]*fTempo[0]+projection[5]*fTempo[1]+projection[9]*fTempo[2]+projection[13]*fTempo[3];
//  fTempo[6]=projection[2]*fTempo[0]+projection[6]*fTempo[1]+projection[10]*fTempo[2]+projection[14]*fTempo[3];
//  
//  fTempo[7]=-fTempo[2];
//  
//  LOGV(" Projection by vector %2.2f %2.2f %2.2f %2.2f ",fTempo[4],fTempo[5],fTempo[6],fTempo[7]);
//  
//  //The result normalizes between -1 and 1
//  if(fTempo[7]==0.0)      //The w value
//    return 0;
//  fTempo[7]=1.0/fTempo[7];
//  //Perspective division
//  fTempo[4]*=fTempo[7];
//  fTempo[5]*=fTempo[7];
//  fTempo[6]*=fTempo[7];
//  //Window coordinates
//  //Map x, y to range 0-1
//  
//  LOGV(" After Projection %2.2f %2.2f %2.2f %2.2f ",fTempo[4],fTempo[5],fTempo[6],fTempo[7]);
//  
//  windowCoordinate[0]=(fTempo[4]*0.5+0.5)*viewport[2]+viewport[0];
//  windowCoordinate[1]=(fTempo[5]*0.5+0.5)*viewport[3]+viewport[1];
//  //This is only correct when glDepthRange(0.0, 1.0)
//  windowCoordinate[2]=(1.0+fTempo[6])*0.5;        //Between 0 and 1
//  return 1;
//}



