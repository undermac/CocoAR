//
//  DetailArViewController.m
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 23/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import "DetailArViewController.h"

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
    cell.textLabel.text = m_pObject.nameModel;
  }
  return cell;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
  return [m_aSection objectAtIndex:section]; 
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
