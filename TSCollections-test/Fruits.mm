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

@implementation FruitBasket
@end
