//
//  DetailArViewController.h
//  cocoAR
//
//  Created by Javier de la Peña Ojea on 23/11/11.
//  Copyright (c) 2011 Artifact. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CCAR_GenericObject.h"
#import "DetailArViewController.h"

@protocol ModalViewControllerDelegate <NSObject>

- (void)didDismissModalView;

@end

@interface DetailArViewController : UITableViewController{
  CCAR_GenericObject* m_pObject;
  NSArray *m_aSection;
    id<ModalViewControllerDelegate> delegate;
}

@property (nonatomic, assign) id<ModalViewControllerDelegate> delegate;

- (id)initWithCCARGenericObject:(CCAR_GenericObject *)pObject;

@end
