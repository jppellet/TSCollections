#import <Foundation/Foundation.h>
#include "TSCollections.h"

@interface Fruit : NSObject
@end

@interface Apple : Fruit
@end

@interface Orange : Fruit
@end

@interface RedOrange : Orange
@end

struct HolderCpp {
    HolderCpp(): apple(Apple.alloc.init) {
        NSLog(@"Created holder at %p", this);
    }
    ~HolderCpp() {
        NSLog(@"Destructing holder at %p", this);
    }
    
    Apple *apple;
};

@interface FruitBasket : NSObject
//@property(nonatomic) TSMutableArray<Fruit *> fruits;
@property(nonatomic) HolderCpp fruits;
@end

TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(Fruit);
TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(Apple);
TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(Orange);
