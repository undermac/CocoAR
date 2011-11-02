//
//  ArViewController.m
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 17/10/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//


#import "ArSceneController.h"
#import "ArViewController.h"
#import "Geo3DObject.h"
#import "CCARModelType.h"
#import "CCAR_GeoObject.h"
#import "CCAR_Object.h"
#import "ArModels.h"
#import "ArSceneItem.h"



@implementation ArViewController

- (id)init
{
    self = [super init];
    if (self) {
      running = false;
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)viewWillAppear:(BOOL)animated{
  if (!running){
    cocos2d::CCApplication::sharedApplication().run();
    running = true;
  }else{
      cocos2d::CCDirector::sharedDirector()->resume();
  }
}

- (void)viewDidDisappear:(BOOL)animated{
  cocos2d::CCDirector::sharedDirector()->pause();
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

+ (void)objectSelected:(CCAR_Object *)selectedObject{
  CCARGeneric3DObject* myObject = (CCARGeneric3DObject *) selectedObject; 
  UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[NSString stringWithUTF8String:myObject->m_sObjectName.c_str()]  
                                                  message:[NSString stringWithUTF8String:myObject->m_sDescription.c_str()] 
                                                 delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
  [alert show];
  [alert release];
}

+ (bool)addObject:(CCAR_GenericObject *)object{
  ArSceneItem myArSceneItem;
  myArSceneItem.externalObject = object;
  
  if ([object isKindOfClass:[CCAR_Object class]]) {
    CCAR_Object* myObj = (CCAR_Object *) object;

    myArSceneItem.internalObject = new CCARObject3D([myObj.nameModel UTF8String], myObj.modelType ,[myObj.name UTF8String], [myObj.description UTF8String], myObj->scale ,myObj->x , myObj->y,myObj->z);
    
    arSceneAddObject( myArSceneItem.internalObject);
    arSceneItems.push_back(myArSceneItem);
    
  }else if ([object isKindOfClass:[CCAR_GeoObject class]]){
    CCAR_GeoObject* myObj = (CCAR_GeoObject *) object;  
    
        myArSceneItem.internalObject = new CCARGeo3DObject([myObj.nameModel UTF8String], myObj.modelType ,[myObj.name UTF8String], [myObj.description UTF8String], myObj->scale,myObj->longitude , myObj->latitude);
    
    arSceneAddObject( myArSceneItem.internalObject);
    arSceneItems.push_back(myArSceneItem);
  }
  
  return true;
}

+ (void)screenTouched:(CCAR_GenericObject *)object{
 
}

+ (bool)addObjects:(NSArray*)objects{
  
  return YES;
}
+ (bool)removeObject:(CCAR_GenericObject *)object{
  
  return YES;
}
+ (bool)modifyObject:(CCAR_GenericObject *)object{
  
  return YES;
}


@end







