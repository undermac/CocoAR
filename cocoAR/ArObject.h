//
//  ArObject.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 12/07/11.
//  Copyright 2011 Artifact. All rights reserved.
//
#include <iostream>

#ifndef cocoAR_ArObject_h
#define cocoAR_ArObject_h

class ArObject
{
public:
  std::string name;
  std::string description;
  
  double longitude;
  double latitude;
  double altitude;
};

class Ar3DObjects: public ArObject,public Mesh
{
  std::string model;
  unsigned int modelType;
  
  double scale;
  double xRotate;
  double yRotate;
  double zRotate;
  
  double xTranslate;
  double yTranslate;
  double zTranslate;
};

#endif
