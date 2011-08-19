//
//  SyncHelper.h
//  Oramanav
//
//  Created by Undermac on 26/02/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface SyncHelper : NSObject {
	
}

+ (NSDictionary *)fetchInfoForDate:(NSString *)date;
+ (NSString *) getFecha:(NSDate *)lastUpdate;

@end
