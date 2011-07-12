//
//  cocoARAppController.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 12/07/11.
//  Copyright Artifact 2011. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController	*viewController;
}

@end

