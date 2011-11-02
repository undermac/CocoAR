//
//  CCAR_Object.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 02/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CCAR_GenericObject.h"

@interface CCAR_Object: CCAR_GenericObject{
@public
  double x;
  double y;
  double z;
}

@property double x;
@property double y;
@property double z;

@end
