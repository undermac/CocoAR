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