//
//  CCAR_GenericObject.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 02/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CCARModelType.h"

#import "CCARModelType.h"

@interface CCAR_GenericObject: NSObject{
@public
  NSString* name;
  NSString* description;
  NSString* objectId;
  
  NSString* nameModel;
  CCARModelType modelType;
  
  double scale;
}

@property (nonatomic, retain) NSString *name;
@property (nonatomic, retain) NSString* description;
@property (nonatomic, retain) NSString* objectId;
@property (nonatomic, retain) NSString *nameModel;
@property CCARModelType modelType;
@property double scale;

@end