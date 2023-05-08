//
//  AppDelegate.h
//  apple
//
//  Created by HOW TO GET YOUR ATTENSION on 08/05/2023.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (readonly, strong) NSPersistentContainer *persistentContainer;

- (void)saveContext;


@end

