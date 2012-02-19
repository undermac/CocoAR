//
//  PreferencesViewController.m
//  Parking
//
//  Created by Javier de la Peña Ojea on 16/09/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import "PreferencesViewController.h"

@implementation PreferencesViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void) viewDidAppear:(BOOL)animated{
//  UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Preferencias" message:@"Aquí estarán las preferencias de la aplicación:\nFiltro de categorías\nDistancia máxima\nInformación adicional" 
//                                                 delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
//  [alert show];
//  [alert release];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
