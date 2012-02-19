//
//  SyncHelper.m
//  Oramanav
//
//  Created by Fco. Javier de la Peña Ojea undermac@gmail.com on 26/02/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#import "SyncHelper.h"
#import "JSON.h"

@implementation SyncHelper

+ (NSString *)hostname
{
  return @"goovent.com"; //http://goovent.com/mobile/GetEvents.php?firstSync=20111002000000&lastSync=00000000000000
}

- (NSArray *)listOfEvents:(NSString *)json
{
	return nil;
}

+ (NSString *) getFecha:(NSDate *)lastUpdate{
	NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
	NSCalendarUnit unitFlags = NSYearCalendarUnit | NSMonthCalendarUnit| NSWeekCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit;
	NSDate *date = lastUpdate;
	NSDateComponents *dateComponents = [calendar components:unitFlags fromDate:date];
	NSInteger year = [dateComponents year];
	NSInteger month = [dateComponents month];
	NSInteger day = [dateComponents day];
	NSInteger hour = [dateComponents hour];
	NSInteger minute = [dateComponents minute];
	NSInteger second = [dateComponents second];
	
  [calendar release];
//  NSLog(@"Updating with: %@",[NSString stringWithFormat:@"%i/%i/%i_%i:%i:%i", day,month,year,hour,minute,second]);
	return [NSString stringWithFormat:@"%i/%i/%i_%i:%i:%i", day,month,year,hour,minute,second];
	
}

+ (id)fetchJSONValueForURL:(NSURL *)url
{
  NSString *jsonString = [[NSString alloc] initWithContentsOfURL:url encoding:NSUTF8StringEncoding error:nil];
  id jsonValue = [jsonString JSONValue];
	[jsonString release];
  return jsonValue;
}



+ (NSDictionary *)fetchInfoWithLastSync:(NSString *)lastSync fromLocation:(CLLocation *)location{
//  http://www.goovent.com/mobile/GetEvents.php?lastSync=20111101000000&userLat=40.4166909&userLon=-3.7003454&from=51&num=20&out=json
  
	NSString *urlString = [NSString stringWithFormat:@"http://%@/mobile/GetEvents.php?lastSync=%@&userLat=%f&userLon=%f&from=1&num=10&out=json", [self hostname],lastSync,location.coordinate.latitude,location.coordinate.longitude];
  NSLog(@"%@", urlString);
  NSURL *url = [NSURL URLWithString:urlString];
  return [self fetchJSONValueForURL:url];
}

@end
