//
//  TSCollectionStringDescription.h
//  TSCollections
//
//  Created by Jean-Philippe Pellet on 11.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TSCollectionStringDescription : NSObject {
	NSString *description;
}

- (instancetype)initWithDescriptionString:(NSString *)desc;

@end
