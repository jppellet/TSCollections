#import <Foundation/Foundation.h>
#include "TSCollections.h"

@interface Fruit : NSObject
@end

@interface Apple : Fruit
@end

@interface Orange : Fruit
@end

@interface FruitBasket : NSObject
@property(nonatomic) TSMutableArray<Fruit *> fruits;
@end

TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(Fruit);
TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(Apple);
TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(Orange);
