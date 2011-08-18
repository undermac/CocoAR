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

class ARObjectMenu : public cocos2d::CCLayer,public cocos2d::CCRGBAProtocol
{
public:
  ARObjectMenu(CCNode *pObject);
	~ARObjectMenu();
  cocos2d::CCLabelTTF* m_labelName,*m_labelDescription;
  void setOpacity(GLubyte var);
  GLubyte m_cOpacity;
  cocos2d::ccColor3B m_tColor;
  
  GLubyte getOpacity(void);
	cocos2d::ccColor3B getColor(void);
	void setColor(cocos2d::ccColor3B color);
  void removeLayerdescription();
	virtual cocos2d::CCRGBAProtocol* convertToRGBAProtocol() { return (cocos2d::CCRGBAProtocol*)this; }
};

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
  
  std::string m_sObjectName, m_sDescription;
  
  double scale;
  double xRotate;
  double yRotate;
  double zRotate;
  
  double xTranslate;
  double yTranslate;
  double zTranslate;
  
  double m_dDistance;
  double m_dBearing;
  
  cocos2d::CCPoint m_vCenter;
  cocos2d::CCSize m_size;
  
  virtual void draw3D()=0;
  bool isRotating;
  bool showScreenBox;
  bool m_bModelBox;
  bool m_bScreenBox;
  cocos2d::CCPoint m_vScreenBox[8];
  cocos2d::CCLabelTTF* m_labelDistance, *m_labelName;
  ARObjectMenu* m_layerDescription;
private:
  cocos2d::CCPoint covert3Dto2d(cocos2d::ccVertex3F);
protected:
  void calculateScreenBox();
  void drawBox(GLfloat lineWidth, cocos2d::ccColor4B color);
  int rotationValue; // Cambiar a private
  
};

class CCARObject3D: public CCARGeneric3DObject
{
public:
  
  CCARObject3D(std::string filename, CCARModelType modelType, std::string objectName,std::string objectDescription);
  CCARObject3D(std::string filename, CCARModelType modelType, std::string objectName,std::string objectDescription, double scale);
  CCARObject3D(std::string filename, CCARModelType modelType, std::string objectName,std::string objectDescription, double scale, double x, double y, double z);
  CCARObject3D(std::string filename, CCARModelType modelType, std::string objectName,std::string objectDescription, double scale, double x, double y, double z, double xRotate, double yRotate, double zRotate);
  void draw3D();
};

class CCARGeo3DObject: public CCARGeneric3DObject
{
public:
  CCARGeo3DObject();
  CCARGeo3DObject(std::string filename, CCARModelType modelType,std::string objectName,std::string objectDescription, double lon, double lat);
  CCARGeo3DObject(std::string filename, CCARModelType modelType, std::string objectName,std::string objectDescription, double scale, double longitude, double latitude);
  CCARGeo3DObject(std::string filename, CCARModelType modelType, std::string objectName,std::string objectDescription, double scale, double longitude, double latitude, double altitude);
  
  double longitude;
  double latitude;
  double altitude;
  
  void draw3D();
};

#endif
