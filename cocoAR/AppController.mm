//
//  CocoArTestAppController.mm
//  CocoArTest
//
//  Created by Javier de la Peña Ojea on 08/08/11.
//  Copyright Artifact 2011. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "CCDirector.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#import "SyncHelper.h"

#import "RootViewController.h"
#import "FavoriteStreetListViewController.h"
#import "PreferencesViewController.h"
#import "DetailArViewController.h"

#import "ArViewController.h"
#import "CCAR_GeoObject.h"
#import "ArModels.h"

@implementation AppController
@synthesize tab;
@synthesize locationManager;
@synthesize delegate;


#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
  
  self.locationManager = [[[CLLocationManager alloc] init] autorelease];
  self.locationManager.delegate = self; 
  self.locationManager.desiredAccuracy = kCLLocationAccuracyNearestTenMeters;
  locationManager.distanceFilter = kCLDistanceFilterNone;
  [self.locationManager startUpdatingLocation];
    
    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
  
  
    tab = [[UITabBarController alloc] init];

    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH_COMPONENT16_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0 ];
  [__glView setMultipleTouchEnabled:YES];  
  
  
    // Use RootViewController manage EAGLView 
    ArViewController *arViewController = [ArViewController sharedInstance]; 
    arViewController.wantsFullScreenLayout = YES;
    arViewController.view = __glView;
  
    __glView.opaque = NO;
    __glView.alpha = 1.0;
    __glView.backgroundColor = [UIColor clearColor];


  arViewController.title = @"Ver";
  arViewController.tabBarItem.image = [UIImage imageNamed:@"camera_tab.png"];
  
  FavoriteStreetListViewController *favoriteViewController = [[FavoriteStreetListViewController alloc]init];
  favoriteViewController.title = @"Favoritos";
  favoriteViewController.tabBarItem.image = [UIImage imageNamed:@"ic_tab_unselected_fav.png"];
  
  PreferencesViewController *preferencesViewController = [[PreferencesViewController alloc]init];
  preferencesViewController.title = @"Preferencias";
  preferencesViewController.tabBarItem.image = [UIImage imageNamed:@"gear2.png"];
  
  UINavigationController *favNavController = [[[UINavigationController alloc]init] autorelease];
  UINavigationController *prefsNavController = [[[UINavigationController alloc]init] autorelease];
  UINavigationController *arNavController = [[UINavigationController alloc]init];
  
  arViewController.navigator = arNavController;
  
  [favNavController pushViewController:favoriteViewController animated:NO];
  [prefsNavController pushViewController:preferencesViewController animated:NO];
  [arNavController pushViewController:arViewController animated:NO];
  
  tab.viewControllers = [NSArray arrayWithObjects:prefsNavController,favNavController,arNavController,nil];
  
  // Set RootViewController to window
  window.rootViewController = tab;
  tab.selectedIndex = 0;
  

  NSSet* models = [[NSSet alloc] initWithObjects:
                            [NSString stringWithCString:MESH_FLECHA encoding:[NSString defaultCStringEncoding]],
                            [NSString stringWithCString:MYMESH01 encoding:[NSString defaultCStringEncoding]],
                            [NSString stringWithCString:MYMESH02 encoding:[NSString defaultCStringEncoding]],
                            [NSString stringWithCString:MYMESH03 encoding:[NSString defaultCStringEncoding]],
                            [NSString stringWithCString:MYMESH05 encoding:[NSString defaultCStringEncoding]],
                            [NSString stringWithCString:MYMESH06 encoding:[NSString defaultCStringEncoding]],
                            [NSString stringWithCString:MYMESH08 encoding:[NSString defaultCStringEncoding]],
                            [NSString stringWithCString:MYMESH12 encoding:[NSString defaultCStringEncoding]],
                            [NSString stringWithCString:MYMESH13 encoding:[NSString defaultCStringEncoding]], nil];
  
  NSDictionary* serverDic = [SyncHelper fetchInfoWithLastSync:@"20110101000000" fromLocation:self.locationManager.location ];
  
  for(NSDictionary* selected in [serverDic objectForKey:@"eventsToSync"])
  {
    CCAR_GeoObject* place = [[CCAR_GeoObject alloc] init];
    
    NSString* pString = nil;
    for (NSDictionary* selectedEvent in [selected objectForKey:@"placeEvents"]) {
      pString = [NSString stringWithFormat:@"Evento: %@ \nFecha %@ \n", [selectedEvent objectForKey:@"eventName"], [selectedEvent objectForKey:@"eventDate"]];
    }
    
    [pString retain];
    place.nameModel = [[models anyObject] retain];
    place.name = [[selected objectForKey:@"placeName"] retain];
    place.description = pString;
    place->scale = 0.7;
    place->longitude = [[selected objectForKey:@"placeLon"] doubleValue];
    place->latitude = [[selected objectForKey:@"placeLat"] doubleValue];
    
    [[ArViewController sharedInstance] addObject:place];
  }
  NSLog(@"%@", serverDic);
  
  [self.locationManager stopUpdatingLocation];

  [window makeKeyAndVisible];

  [[UIApplication sharedApplication] setStatusBarHidden: YES];
        
//  cocos2d::CCApplication::sharedApplication().run();
  
  
  return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
	cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
	cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
	cocos2d::CCDirector::sharedDirector()->stopAnimation();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
	cocos2d::CCDirector::sharedDirector()->startAnimation();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [super dealloc];
}


@end

