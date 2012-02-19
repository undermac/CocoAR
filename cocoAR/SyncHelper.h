//
//  SyncHelper.h
//  Oramanav
//
//  Created by Fco. Javier de la Peña Ojea undermac@gmail.com on 26/02/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>

@interface SyncHelper : NSObject {}

+ (NSDictionary *)fetchInfoWithLastSync:(NSString *)lastSync fromLocation:(CLLocation *)location;
+ (NSString *) getFecha:(NSDate *)lastUpdate;

@end
