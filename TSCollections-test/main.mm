#import <Foundation/Foundation.h>
#import "TSCollections.h"



#import "Fruits.h"





int main(int argc, const char * argv[])
{

    @autoreleasepool {
        
        
        
        
        TSArray<Fruit*> immutableFruits = TSArrayMake<Fruit *>(Orange.alloc.init, RedOrange.alloc.init, Apple.alloc.init);
        TSMutableArray<Fruit*> mutableFruits = TSMutableArrayMake<Fruit *>(Orange.alloc.init, RedOrange.alloc.init, Apple.alloc.init);
        TSArray<Apple*> immutableApples = TSArrayMake(Apple.alloc.init);
        TSMutableArray<Apple*> mutableApples = TSMutableArrayMake(Apple.alloc.init);
        
        
        TSMutableArray<Fruit*> mutableApplesCopy = TSMutableArrayWithArray(mutableApples);
        
        
		FruitBasket *basket = FruitBasket.alloc.init;
        basket = nil;
//        if (abs(1) == 1) {
//            HolderCpp h;
//        }
        
        NSLog(@"end");
        
        return 0;
		
		
//		basket.fruits.addObject(Apple.alloc.init);
//		basket.fruits.addObject(Orange.alloc.init);
//				
//		TSLog(@"basket.fruits=%@", basket.fruits);
		
		
		TSLog(@"End");
		return 0;
		
		TSArray<NSObject *> stringsEmpty = TSArrayMake<NSString *>();
		
		TSMutableArray<NSObject *> stringsOne = TSMutableArrayMake(@"one");
		
		TSMutableArray<NSString *> stringsMore = TSMutableArrayMake(@"one", @"two", @"three");
        
        NSUInteger totalLength = stringsMore.fold<NSUInteger>(0, ^(NSUInteger acc, NSString *elem) {
            return acc + [elem length];
        });
        NSLog(@"%ld", totalLength);
		
		TSMutableArray<Fruit *> fruitsEmpty = TSMutableArrayMake<Fruit *>();
		TSMutableArray<Fruit *> fruitsOne = TSMutableArrayMake<Fruit *>(Apple.alloc.init);
		TSMutableArray<Fruit *> fruitsMore = TSMutableArrayMake<Fruit *>(Apple.alloc.init, Apple.alloc.init, Orange.alloc.init);
		
		TSMutableArray<TSMutableArray<Fruit *>> allFruits = TSMutableArrayMake(fruitsEmpty, fruitsOne, fruitsMore);
		
		
		TSLog(@"stringsEmpty=%@", stringsEmpty);
		TSLog(@"stringsOne=%@", stringsOne);
		TSLog(@"stringsMore=%@", stringsMore);
		
		__block NSUInteger i = 0;
		allFruits.foreach(^(TSMutableArray<Fruit *> fruits) {
			fruits.foreach(^(Fruit *_) {
				NSLog(@"%ld -> %@", i, _);
			});
			i++;
		});
		
		
		TSDictionary<NSNumber*, TSSet<NSObject*>> reprByNumber = TSDictionaryMake(
		    @1,  TSSetMake<NSObject*>(@"one"),
		    @5 , TSSetMake<NSObject*>(@3)
		);
		
		TSLog(@"reprByNumber=%@", reprByNumber);
		
		exit(0);
        
        TSMutableArray<Fruit *> things = [NSMutableArray arrayWithArray:@[ Apple.alloc.init, Orange.alloc.init ]];
        
        void (^doNSLog)(id) = ^(id f) {
            NSLog(@"%@", [f description]);
        };
        
        things.foreach(doNSLog);
        
        
        Fruit *(^getFruit)();
        Apple *(^getApple)();
        
        getFruit = getApple;
        
        /*
        TSTraversable<Fruit *> *(^getTraversable)();
        TSArray<Fruit *> *(^getArray)();
        
        getTraversable = getArray;
        */
        
        TSMutableArray<NSString *> descs = things.map<NSString *>(^(id obj) {
            return [NSString stringWithFormat:@"Mapped: %@", [obj description]];
        });
        
        TSMutableArray<NSString *> descsParts = descs.flatMap<NSString *>(^(NSString *str) {
            TSArray<NSString *> parts = [str componentsSeparatedByString:@" "];
            return parts;
        });
        
        descs[0] = @"changed";
        descs.foreach(doNSLog);
        descsParts.foreach(doNSLog);

        NSUInteger c = descsParts.countWhere(^(NSString *elem) { return [elem hasPrefix:@"<"]; });
        
        NSLog(@"Count: %ld", c);
    
        exit(0);
        
        things.addObject(Fruit.alloc.init);
        things.addObject(Apple.alloc.init);
        
        TSArray<Fruit *> newThings = @[];
        TSArray<Apple *> newSpecThings = @[];
        
        things.addObjectsFromArray(newThings);
        things.addObjectsFromArray(newSpecThings);
        //things.addObjectsFromArray(strings);
    
        TSDictionary<NSNumber *, TSArray<NSString *>> stringsByNumber = @{ @80: @[@"huitante", @"quatre-vingts"] };
        
        NSLog(@"stringsByNumber=%@", stringsByNumber.description());
        
        NSLog(@"stringsByNumber[80]=%@", stringsByNumber[@80][1]);
        
        exit(0);
        
        TSArray<TSMutableSet<NSString*>> a = @[ [NSMutableSet setWithArray:@[ @"a", @"b", @"c"] ] ];
        
        TSSet<NSString*> nested = a[0];
        
//        nested[2] = @"4";
        
        NSLog(@"item 0 = %@", nested.description());
        
        NSLog(@"%@", a.description());
        NSLog(@"%@", nested.description());
        
        /*
        Thing *t = Thing.thing;
        SpecializedThing *st = SpecializedThing.thing;
         */
        
        /*
         NSMutableArray *a4 = [[NSMutableArray alloc] init]; // fine
         NSMutableArray *a3 = [NSMutableArray array];        // fine, too
         NSMutableArray *a2 = [[NSArray alloc] init]; // compiler reports incompatible pointer types
         NSNumber *a1 = [NSArray array]; // compiler says nothing!
         */
        
        NSMutableArray *array = [NSMutableArray array];
        [array addObject:@"ok"];
        [array addObject:@"hey1"];
        
        TSMutableArray<NSString *> bla = array;
		
		bla.foreach(^(NSString *str) {
			NSLog(@"%@", str);
		});
		        
        bla += @"hehe";
        
        NSMutableArray *other = [NSMutableArray arrayWithObjects:@"some_more0", @"some_more1", nil];
        
        TSMutableArray<NSString *> otherTs(other);
        
        NSArray *nsImmutableArray = [NSArray.alloc initWithObjects:@"one", @"two", @"three", nil];
        TSArray<NSString*> immutableArray = nsImmutableArray;
        NSLog(@"%@", immutableArray.description());
        
        bla.addObjectsFromArray(otherTs);
        bla.addObjectsFromArray(immutableArray);
        
        NSLog(@"array = [%@]", [array componentsJoinedByString:@", "]);
        
        bla[1] = @"replaced";
        
        bla -= @"some_more1";
        
        for (i = 0; i < bla.count(); i++) {
            NSString *str = bla[i];
            NSLog(@"%ld  %@", i, str);
        }
        
    }
    return 0;
}


