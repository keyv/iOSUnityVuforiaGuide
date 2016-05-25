//
//  AppDelegate.h
//  UnityIntegration
//
//  Created by Volodya Karpliuk on 5/24/16.
//  Copyright © 2016 Volodya Karpliuk. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UnityAppController.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) UIWindow *unityWindow;

@property (strong, nonatomic) UnityAppController *unityController;

- (void)showUnityWindow;
- (void)hideUnityWindow;

- (void)shouldAttachRenderDelegate;

@end
