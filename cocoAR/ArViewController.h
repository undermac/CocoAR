//
//  ArViewController.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 17/10/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//


//#import "Geo3DObject.h"

#ifdef __OBJC__
#import <UIKit/UIKit.h>
#import "CCAR_GenericObject.h"

@interface ArViewController : UIViewController{
  UIView *arView;
  bool running;
}

+ (void)objectSelected:(CCAR_GenericObject *)object;
+ (void)screenTouched:(CCAR_GenericObject *)object;

+ (bool)addObject:(CCAR_GenericObject *)object;
+ (bool)addObjects:(NSArray*)objects;
+ (bool)removeObject:(CCAR_GenericObject *)object;
+ (bool)modifyObject:(CCAR_GenericObject *)object;

@end

#endif

