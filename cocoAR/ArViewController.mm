//
//  ArViewController.m
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 17/10/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//


//[TODO] Temporal array ...

#import "ArSceneController.h"
#import "ArViewController.h"
#import "Geo3DObject.h"
#import "CCARModelType.h"
#import "CCAR_GeoObject.h"
#import "CCAR_Object.h"
#import "ArModels.h"
#import "ArSceneItem.h"
#import "DetailArViewController.h"

@implementation ArViewController
@synthesize navigator;

static ArViewController* _sharedMySingleton = nil;

- (id)init
{
    self = [super init];
    if (self) {
      running = false;
    }
    return self;
}

//Singleton

+ (ArViewController *) sharedInstance{
  @synchronized(self){
    if (!_sharedMySingleton) {
      _sharedMySingleton = [[self alloc] init];
    }
  }

  return _sharedMySingleton;
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
    NSLog(@"viewWillAppear");
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

- (void)viewDidAppear:(BOOL)animated{
  if (!running){
    cocos2d::CCApplication::sharedApplication().run();
    running = true;
  }else{
    cocos2d::CCDirector::sharedDirector()->resume();
  }
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)objectSelected:(CCAR_GenericObject *)selectedObject{

  DetailArViewController* viewController = [[DetailArViewController alloc] initWithCCARGenericObject:selectedObject];
  
//  [self.navigationController pushViewController:viewController animated:YES];

//  [UIView beginAnimations:@"View Flip" context:nil];
//  [UIView setAnimationDuration:0.90];
//  [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
//  
//  [UIView setAnimationTransition:
//   UIViewAnimationTransitionCurlDown
//                         forView:self.navigationController.view cache:NO];
  
  
  [self.navigationController pushViewController:viewController animated:YES];
//  [self.navigationController presentModalViewController:viewController animated:YES];
  
//  [UIView commitAnimations];
}

- (bool)addObject:(CCAR_GenericObject *)object{

  ArSceneItem* myArSceneItem = new ArSceneItem;
//  vector<ArSceneItem*>* test = getArSceneItems();
  
  myArSceneItem->externalObject = object;
  
  if (object.nameModel) {
    string stringNameModel = MESH_FLECHA;
    object.nameModel = [[NSString alloc] initWithCString:MESH_FLECHA encoding:[NSString defaultCStringEncoding]];
  }
  if (!object.name) {
    object.name = [[NSString alloc] initWithCString:"No name" encoding:[NSString defaultCStringEncoding]];
  }
  if (!object.description) {
    object.description = [[NSString alloc] initWithCString:"No description" encoding:[NSString defaultCStringEncoding]];
  }
  
  if ([object isKindOfClass:[CCAR_Object class]]) {
    CCAR_Object* myObj = (CCAR_Object *) object;
    
    myArSceneItem->internalObject = new CCARObject3D([myObj.nameModel UTF8String], myObj.modelType ,[myObj.name UTF8String], [myObj.description UTF8String], myObj->scale ,myObj->x , myObj->y,myObj->z);
    
    arSceneAddObject( myArSceneItem->internalObject);
    getArSceneItems()->push_back(myArSceneItem);
    
  }else if ([object isKindOfClass:[CCAR_GeoObject class]]){
    CCAR_GeoObject* myObj = (CCAR_GeoObject *) object;
    
    myArSceneItem->internalObject = new CCARGeo3DObject( [myObj.nameModel UTF8String] , CCARType_Mesh ,[myObj.name UTF8String] ,[myObj.description UTF8String] , myObj->scale,myObj->longitude , myObj->latitude);
    
    arSceneAddObject( myArSceneItem->internalObject);
    
    getArSceneItems()->push_back(myArSceneItem);
    
  }
  return true;
}

- (void)screenTouched:(CCAR_GenericObject *)object{
 
}

- (bool)removeObject:(CCAR_GenericObject *)object{
  
  return YES;
}
- (bool)modifyObject:(CCAR_GenericObject *)object{
  
  return YES;
}


@end







