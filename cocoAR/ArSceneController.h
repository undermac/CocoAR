//
//  ArSceneController.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 18/10/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//



#ifndef cocoAR_ArSceneController_h
#define cocoAR_ArSceneController_h

#import "Geo3DObject.h"
//#import "ArSceneController.h"

void arSceneObjectSelected(CCARGeneric3DObject* selectedObject);
void arSceneScreenTouched();

vector<CCARGeneric3DObject*>* getObjectQueue();

CCARGeneric3DObject* arSceneAddObject(CCARGeneric3DObject* newObject);
CCARGeneric3DObject* arSceneRemoveObject(CCARGeneric3DObject* selectedObject);
CCARGeneric3DObject* arSceneModifyObject(CCARGeneric3DObject* selectedObject);

#endif