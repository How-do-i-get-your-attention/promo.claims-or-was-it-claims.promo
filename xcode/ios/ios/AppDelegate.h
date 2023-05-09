//
//  AppDelegate.h
//  ios
//
//  Created by HOW TO GET YOUR ATTENSION on 09/05/2023.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (readonly, strong) NSPersistentContainer *persistentContainer;

- (void)saveContext;


@end

