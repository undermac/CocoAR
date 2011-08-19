//
//  SyncHelper.m
//  Oramanav
//
//  Created by Undermac on 26/02/11.
//  Copyright 2011 Artifact. All rights reserved.
//

#import "SyncHelper.h"
#import "JSON.h"


@implementation SyncHelper

+ (NSString *)oramaHostname
{
	return @"oramadev.appspot.com"; 
    //return @"oramaapp.appspot.com"; // No furrula hasta nueva orden :)
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
    NSLog(@"Updating with: %@",[NSString stringWithFormat:@"%i/%i/%i_%i:%i:%i", day,month,year,hour,minute,second]);
	return [NSString stringWithFormat:@"%i/%i/%i_%i:%i:%i", day,month,year,hour,minute,second];
	
}

+ (id)fetchJSONValueForURL:(NSURL *)url
{
    NSString *jsonString = [[NSString alloc] initWithContentsOfURL:url encoding:NSUTF8StringEncoding error:nil];
    id jsonValue = [jsonString JSONValue];
	[jsonString release];
    return jsonValue;
}



+ (NSDictionary *)fetchInfoForDate:(NSString *)date{
	NSString *urlString = [NSString stringWithFormat:@"http://%@/GetPointOfInterestServlet?lastupdate=%@", [self oramaHostname],date];
	//NSString *urlString = [NSString stringWithFormat:@"http://oramaapp.appspot.com/GetPointOfInterestServlet?lastupdate=10/10/2009_10:10:10"];
    NSURL *url = [NSURL URLWithString:urlString];
    return [self fetchJSONValueForURL:url];
}

@end
