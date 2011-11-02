//
//  ArSceneItem.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 02/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#ifndef cocoAR_ArSceneItem_h
#define cocoAR_ArSceneItem_h

typedef struct 
{
  CCARGeneric3DObject* internalObject;
  CCAR_GenericObject* externalObject;
} ArSceneItem;

static vector<ArSceneItem> arSceneItems;

#endif
