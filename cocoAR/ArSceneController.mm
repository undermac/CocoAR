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
#import "ArSceneItem.h"

void arSceneObjectSelected(CCARGeneric3DObject* selectedObject){
  for(unsigned int i=0; i< arSceneItems.size(); i++){
    if (selectedObject == arSceneItems[i].internalObject) {
        [ArViewController objectSelected:arSceneItems[i].externalObject];
    }
  }
}

CCARGeneric3DObject* arSceneAddObject(CCARGeneric3DObject* newObject){
  ArScene* cocoAR = (ArScene*)ArScene::scene();
  
  return cocoAR->addARObject(newObject);
}

void arSceneScreenTouched(){
  
}

CCARGeneric3DObject* arSceneRemoveObject(CCARGeneric3DObject* selectedObject){
  
  return NULL;
}
CCARGeneric3DObject* arSceneModifyObject(CCARGeneric3DObject* selectedObject){

  return NULL;
}