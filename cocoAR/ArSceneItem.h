//
//  ArSceneItem.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 02/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#ifndef cocoAR_ArSceneItem_h
#define cocoAR_ArSceneItem_h
#include <vector>
#include "CCAR_GenericObject.h"
#include "Geo3DObject.h"


class ArSceneItem
{
  public:
  CCARGeneric3DObject* internalObject;
  CCAR_GenericObject* externalObject;
};

static vector<ArSceneItem*>* arSceneItems;

vector<ArSceneItem*>* getArSceneItems();

#endif
