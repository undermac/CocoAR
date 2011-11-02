//
//  FavoriteStreetListViewController.m
//  Parking
//
//  Created by Javier de la Peña Ojea on 16/09/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import "FavoriteStreetListViewController.h"
#import "ArViewController.h"
#import "CCAR_GeoObject.h"

@implementation FavoriteStreetListViewController

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

- (void) viewDidAppear:(BOOL)animated{
  UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Lista de Favoritos" message:@"Aquí el usuario podrá visualizar las localizaciones que marque como favoritos.\nPor ejemplo: casa" 
                                                 delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
  [alert show];
  [alert release];
  

}

- (void)viewDidLoad
{
  CCAR_GeoObject* prueba;
  prueba.name = @"----- Ria de ferrol ----";
  prueba.description = @"La puta ria de ferrol";
  prueba->scale = 0.3;
  prueba->longitude = 0;
  prueba->latitude = 0;
  
  
  [ArViewController addObject:nil];
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
