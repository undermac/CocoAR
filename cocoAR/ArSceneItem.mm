//
//  ArSceneItem.cpp
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 23/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#include "ArSceneItem.h"


vector<ArSceneItem*>* getArSceneItems(){
  if (arSceneItems == NULL) {
    arSceneItems = new vector<ArSceneItem*>;
  }
  return arSceneItems;  
}


CCARGeneric3DObject* findObject(CCAR_GenericObject *obj){
  vector<ArSceneItem*>::iterator it;
		CCARGeneric3DObject* toret = NULL;
		
		for (it = getArSceneItems()->begin(); it != getArSceneItems()->end(); it++)
		{
				ArSceneItem *selectedArSceneItem= *it;
				if (selectedArSceneItem->externalObject == obj) {
						toret = selectedArSceneItem->internalObject;
						break;
				}
  }
		return toret;
}