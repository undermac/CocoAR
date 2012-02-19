//
//  DetailArViewController.m
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 23/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import "DetailArViewController.h"
#import "CCAR_Object.h"
#import "CCAR_GeoObject.h"

@implementation DetailArViewController

@synthesize delegate;

- (id)initWithCCARGenericObject:(CCAR_GenericObject *)pObject
{
    self = [super initWithStyle:UITableViewStyleGrouped];
    if (self) {
      m_pObject = pObject;
      m_aSection = [[NSArray alloc] initWithObjects:@"Nombre",@"Descipción", @"Modelo 3D",@"Localización", nil];
      self.view.alpha = 0.5;
      UIImageView *myView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"coco.png"]];
      myView.contentMode = UIViewContentModeScaleAspectFit;
      myView.autoresizingMask = UIViewAutoresizingFlexibleRightMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin;
        
        self.tableView.tableFooterView = myView;
    }
    return self;
}

- (void) setHeaderView:(NSString *)headerText{
  
}

- (void)backButtonTapped {
  [UIView beginAnimations:@"View Flip left" context:nil];
  [UIView setAnimationDuration:0.90];
  [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
  
  [UIView setAnimationTransition:
   UIViewAnimationTransitionCurlUp
                         forView:self.navigationController.view cache:NO];
  
  [self.navigationController popViewControllerAnimated:YES];
  [UIView commitAnimations];
}

-(void)viewDidAppear:(BOOL)animated{
  self.view.alpha = 0.5;
}


- (void)dismissView:(id)sender {
  
  // Call the delegate to dismiss the modal view
  [delegate didDismissModalView];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
  return [m_aSection count];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
  return 1;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
//  static NSString *CellIdentifier = @"Detailed";
  UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:nil];
  if (cell == nil) {
    cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil] autorelease];
  }
  NSString* sectionName = @"";
  
  if ([m_aSection count]>= indexPath.section) {
    sectionName = [m_aSection objectAtIndex:indexPath.section];
  }
  cell.textLabel.backgroundColor = [UIColor clearColor];
  if ([sectionName isEqualToString:@"Nombre"]) {
    cell.textLabel.text = m_pObject.name;
  }
  if ([sectionName isEqualToString:@"Descipción"]) {
    cell.textLabel.text = m_pObject.description;
  }
  if ([sectionName isEqualToString:@"Modelo 3D"]) {
    cell.textLabel.text = m_pObject.nameModel;
  }
  if ([sectionName isEqualToString:@"Localización"]) {
    if ([m_pObject isKindOfClass:[CCAR_GeoObject class]]) {
      CCAR_GeoObject* geoObject = (CCAR_GeoObject *)m_pObject;
      cell.textLabel.text = [NSString stringWithFormat:@"Objeto Geolocalizado en Latitud: %f , Longitud %f", geoObject.latitude, geoObject.longitude];
    }else if ([m_pObject isKindOfClass:[CCAR_Object class]]){
      CCAR_Object* object = (CCAR_Object *)m_pObject;
      cell.textLabel.text = [NSString stringWithFormat:@"Objeto posicionado en X: %f , Y: %f , Z: %f", object.x,object.y,object.z];
    }else{
      cell.textLabel.text = [NSString stringWithString:@"Type not found"];
    }
  }
  cell.textLabel.numberOfLines = 0;
  cell.textLabel.lineBreakMode = UILineBreakModeWordWrap;
  cell.textLabel.font = [UIFont systemFontOfSize:16.0f];
  return cell;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
  return [m_aSection objectAtIndex:section]; 
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
  CGFloat toRet = 30.0f;
  
  NSString* sectionName = @"";
  
  if ([m_aSection count]>= indexPath.section) {
    sectionName = [m_aSection objectAtIndex:indexPath.section];
  }
  if ([sectionName isEqualToString:@"Nombre"]) {
    CGSize theSize = [m_pObject.name sizeWithFont:[UIFont systemFontOfSize:16.0f] constrainedToSize:CGSizeMake(265.0f, 9999.0f) lineBreakMode:UILineBreakModeWordWrap];
    toRet = theSize.height + 14;
  }
  if ([sectionName isEqualToString:@"Descipción"]) {
    CGSize theSize = [m_pObject.description sizeWithFont:[UIFont systemFontOfSize:16.0f] constrainedToSize:CGSizeMake(265.0f, 9999.0f) lineBreakMode:UILineBreakModeWordWrap];
    toRet = theSize.height + 14;
  }
  if ([sectionName isEqualToString:@"Modelo 3D"]) {
    CGSize theSize = [m_pObject.nameModel sizeWithFont:[UIFont systemFontOfSize:16.0f] constrainedToSize:CGSizeMake(265.0f, 9999.0f) lineBreakMode:UILineBreakModeWordWrap];
    toRet = theSize.height + 14;
  }
  return toRet < 44.0f ? 44.0f:toRet;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
  NSLog(@"Clicked");
}

#pragma mark - View lifecycle

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
