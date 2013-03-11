//
//  TSCollectionStringDescription.m
//  TSCollections
//
//  Created by Jean-Philippe Pellet on 11.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#import "TSCollectionStringDescription.h"

@implementation TSCollectionStringDescription

- (instancetype)initWithDescriptionString:(NSString *)desc;
{
	if ((self = [super init])) {
		description = desc;
	}
	return self;
}

- (NSString *)description;
{
	return description;
}

@end
