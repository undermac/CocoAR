//
//  FavoriteStreetListViewController.m
//  Parking
//
//  Created by Javier de la Peña Ojea on 16/09/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FavoriteStreetListViewController.h"
#import "ArViewController.h"
#import "CCAR_GeoObject.h"
#import "CCAR_Object.h"
#import "ArModels.h"

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
  self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAction target:self action:@selector(addExample)];
}

- (void) addExample{
  
  CCAR_GeoObject* prueba = [[CCAR_GeoObject alloc] init];
  
  prueba.nameModel = [NSString stringWithCString:MYMESH01 encoding:[NSString defaultCStringEncoding]];
  prueba.name = [NSString stringWithFormat:@"El parking"];
  prueba.description = [NSString stringWithFormat:@"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."];
  prueba->scale = 0.7;
  prueba->longitude =  -7.856292128562927f;
  prueba->latitude = 42.34511443500709f;
  
  [[ArViewController sharedInstance] addObject:prueba];
  
  CCAR_Object* prueba2 = [[CCAR_Object alloc] init] ;
  
  prueba2.nameModel = [NSString stringWithCString:MESH_FLECHA encoding:[NSString defaultCStringEncoding]];
  prueba2.name = [NSString stringWithFormat:@"Una flecha"];
  prueba2.description = [NSString stringWithFormat:@"Objeto en posición x = 100 , y = 0, z = 100"];
  prueba2->scale = 0.7;
  prueba2->x =  100.0f;
  prueba2->y = 0.0f;
  prueba2->z = 100.0f;
  
  [[ArViewController sharedInstance] addObject:prueba2];
  
  CCAR_Object* prueba3 = [[CCAR_Object alloc] init];
  
  prueba3.nameModel = [NSString stringWithCString:MYMESH13 encoding:[NSString defaultCStringEncoding]];
  prueba3.name = [NSString stringWithFormat:@"Una estrella"];
  prueba3.description = [NSString stringWithFormat:@"Objeto en posición x = 100 , y = 0, z = 100"];
  prueba3->scale = 1.7;
  prueba3->x =  100.0f;
  prueba3->y = 0.0f;
  prueba3->z = -100.0f;
  
  [[ArViewController sharedInstance] addObject:prueba3];
  
  CCAR_Object* prueba4 = [[CCAR_Object alloc] init];
  
  prueba4.nameModel = [NSString stringWithCString:MYMESH02 encoding:[NSString defaultCStringEncoding]];
  prueba4.name = [NSString stringWithFormat:@"Prueba 4"];
  prueba4.description = [NSString stringWithFormat:@"Prueba objeto MYMESH02"];
  prueba4->scale = 0.2;
  prueba4->x =  50.0f;
  prueba4->y = 0.0f;
  prueba4->z = -25.0f;
  
  [[ArViewController sharedInstance] addObject:prueba4];
  
  CCAR_GeoObject* prueba5 = [[CCAR_GeoObject alloc] init];
  
  prueba5.nameModel = [NSString stringWithCString:MYMESH01 encoding:[NSString defaultCStringEncoding]];
  prueba5.name = [NSString stringWithFormat:@"Ría de ferrol"];
  prueba5.description = [NSString stringWithFormat:@"La ría de Ferrol se encuentra entre las Rías Altas, en la provincia de La Coruña, Galicia, España. En sus orillas se encuentran los municipios gallegos de Ferrol, Narón, Neda, Fene y Mugardos. En ella desemboca y la forma el río Jubia."];
  prueba5->scale = 0.7;
  prueba5->longitude =  -8.245668411254883f;
  prueba5->latitude = 43.494541820367246f;
  
  [[ArViewController sharedInstance] addObject:prueba5];
  
  CCAR_GeoObject* prueba6 = [[CCAR_GeoObject alloc] init];
  
  prueba6.nameModel = [NSString stringWithCString:MYMESH05 encoding:[NSString defaultCStringEncoding]];
  prueba6.name = [NSString stringWithFormat:@"Prueba 6"];
  prueba6.description = [NSString stringWithFormat:@"Prueba 6..."];
  prueba6->scale = 0.7;
  prueba6->longitude =  -7.858292028562927f;
  prueba6->latitude = 42.34511453500709f;
  
  [[ArViewController sharedInstance] addObject:prueba6];
  
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
