//
//  CCAR_GeoObject.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 02/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CCAR_GenericObject.h"

@interface CCAR_GeoObject: CCAR_GenericObject{
  @public
  double longitude;
  double altitude;
  double latitude;
}
@property double longitude;
@property double altitude;
@property double latitude;

@end
