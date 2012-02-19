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
#import "DetailArViewController.h"



@interface ArViewController : UIViewController {
  UIView *arView;
  bool running;
  UINavigationController* navigator;
}

@property (retain, nonatomic) UINavigationController* navigator;

+ (ArViewController *) sharedInstance;

- (bool)addObject:(CCAR_GenericObject *)object;
- (bool)removeObject:(CCAR_GenericObject *)object;
- (bool)modifyObject:(CCAR_GenericObject *)object;

//Delegated method
- (void)objectSelected:(CCAR_GenericObject *)selectedObject;


@end

#endif

