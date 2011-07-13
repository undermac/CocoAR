//
//  main.m
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 12/07/11.
//  Copyright Artifact 2011. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"ARcocosAppController");
    [pool release];
    return retVal;
}
