//
//  ArSceneController.cpp
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 18/10/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#include <iostream>
#import "ArViewController.h"
#import "ArScene.h"
#import "CCDirector.h"
#import "CCScene.h"
#import "ArSceneItem.h"

vector<CCARGeneric3DObject*> *objectQueue;

void arSceneObjectSelected(CCARGeneric3DObject* selectedObject){
  vector<ArSceneItem*>::iterator it;
  
	for (it = getArSceneItems()->begin(); it != getArSceneItems()->end(); it++)
	{
		ArSceneItem *selectedArSceneItem= *it;
    if (selectedObject == selectedArSceneItem->internalObject) {
      [[ArViewController sharedInstance] objectSelected:selectedArSceneItem->externalObject];
    }
  }
}

CCARGeneric3DObject* arSceneAddObject(CCARGeneric3DObject* newObject){
  cocos2d::CCScene* runningScene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
  if (runningScene == NULL){
    if (objectQueue == NULL) {
      objectQueue = new vector<CCARGeneric3DObject *>;
    }
    objectQueue->push_back(newObject);
    return NULL;
  }
  
  ArScene* cocoAr = (ArScene*) runningScene;
  return cocoAr->addARObject(newObject);
}

vector<CCARGeneric3DObject*>* getObjectQueue(){
  return objectQueue;
}

void arSceneScreenTouched(){
  
}

CCARGeneric3DObject* arSceneRemoveObject(CCARGeneric3DObject* selectedObject){
  
  return NULL;
}
CCARGeneric3DObject* arSceneModifyObject(CCARGeneric3DObject* selectedObject){

  return NULL;
}