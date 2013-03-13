#import "Fruits.h"

@implementation Fruit

- (instancetype)init;
{
	if ((self = [super init])) {
		NSLog(@"New fruit of type %@ at %p", NSStringFromClass([self class]), self);
	}
	return self;
}

- (void) dealloc;
{
	NSLog(@"Dealloc'ing fruit of type %@ at %p", NSStringFromClass([self class]), self);
}


@end

@implementation Apple
@end

@implementation Orange
@end

@implementation RedOrange
@end

@implementation FruitBasket

@synthesize fruits = _fruits;

- (instancetype)init;
{
	if ((self = [super init])) {
		NSLog(@"New fruitbasket at %p", self);
	}
	return self;
}

- (void) dealloc;
{
	NSLog(@"Dealloc'ing fruitbasket at %p", self);
}


@end
