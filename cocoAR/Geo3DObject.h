//
//  Geo3DObject.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 09/08/11.
//  Copyright 2011 Artifact. All rights reserved.
//
#include "cocos2d.h"

#ifndef cocoAR_Geo3DObject_h
#define cocoAR_Geo3DObject_h

class CCARGeneric3DObject;

typedef enum 
{
	CCARType_Mesh,
	CCARType_TMDLModel,
} CCARModelType;

class CCARGeneric3DModel
{
  public:
  cocos2d::ccVertex3F m_vtxMax;
  cocos2d::ccVertex3F m_vtxMin;
  void Init(char *Filename);
	virtual void AdvanceFrame(float Time)=0;
  void LoadModel(char* filename);
  virtual void draw3D()=0;
  void DrawModel();
  std::string modelName;
  cocos2d::CCPoint covert3Dto2d();
  
};


class CCARGeneric3DObject: public cocos2d::CCMenuItemImage{
  public:
  CCARGeneric3DModel* model3D;
  
  double scale;
  double xRotate;
  double yRotate;
  double zRotate;
  
  double xTranslate;
  double yTranslate;
  double zTranslate;
  
  cocos2d::CCPoint m_vCenter;
  cocos2d::CCSize m_size;
  
  virtual void draw3D()=0;
  bool isRotating;
  bool showScreenBox;
  bool m_bModelBox;
  bool m_bScreenBox;
  cocos2d::CCPoint m_vScreenBox[8];
  
  private:
    cocos2d::CCPoint covert3Dto2d(cocos2d::ccVertex3F);
  protected:
  void calculateScreenBox();
  void drawBox(GLfloat lineWidth, cocos2d::ccColor4F color);
  int rotationValue; // Cambiar a private

};

class CCARObject3D: public CCARGeneric3DObject
{
public:
  
  CCARObject3D(std::string filename, CCARModelType modelType);
  CCARObject3D(std::string filename, CCARModelType modelType, double scale);
  CCARObject3D(std::string filename, CCARModelType modelType, double scale, double x, double y, double z);
  CCARObject3D(std::string filename, CCARModelType modelType, double scale, double x, double y, double z, double xRotate, double yRotate, double zRotate);
  void draw3D();
};

class CCARGeo3DObject: public CCARGeneric3DObject
{
public:
  CCARGeo3DObject(std::string filename, CCARModelType modelType, double longitude, double latitude);
  CCARGeo3DObject(std::string filename, CCARModelType modelType, double scale, double longitude, double latitude);
  CCARGeo3DObject(std::string filename, CCARModelType modelType, double scale, double longitude, double latitude, double altitude);
  ~CCARGeo3DObject();
  std::string name;
  std::string description;
  
  double longitude;
  double latitude;
  double altitude;
  
  void draw3D();
};

#endif
