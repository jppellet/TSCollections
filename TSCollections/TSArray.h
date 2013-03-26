#ifndef _TS_ARRAY_H
#define _TS_ARRAY_H

#include "TSCollectionsConfig.h"
#include "TSTraversable.h"

@class NSArray;

template<typename T>
class TSArray : public TSTraversable<T> {
protected:
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(T, elem, BackingElemType)
    
public:
	   
    //
    // Constructors & special members
    //
    
    // Main constructors and destructor
    TSArray(): TSTraversable<T>(nil) {}
    TSArray(NSArray *_array): TSTraversable<T>(_array) {}
    
    // Copy constructor, covariant in T
    template<typename T1>
    TSArray(const TSArray<T1>& that): TSTraversable<T>(that.asNSArray()) {
        TSTypeConstraintDerivedFrom<T1, T>();
    }
    
    // Move constructor, covariant in T
    template<typename T1>
    TSArray(TSArray<T1>&& that): TSTraversable<T>(that.asNSArray()) {
        TSTypeConstraintDerivedFrom<T1, T>();
        reinterpret_cast<TSArray<T>&>(that).traversable = nil;
    }
    
    // Copy & move assignment together, covariant in T
    template<typename T1>
    TSArray<T>& operator=(TSArray<T1> that) {
        TSTypeConstraintDerivedFrom<T1, T>();
        std::swap(this->traversable, reinterpret_cast<TSArray<T>&>(that).traversable);
        return *this;
    }
    
    //
    // Objective-C-like interface
    //
    
    inline NSString *componentsJoinedByString(NSString *sep) {
        return [array() componentsJoinedByString:sep];
    }
    
    inline BOOL contains(T elem) {
        return [array() containsObject:elemToBackingType(elem)];
    }
	
	inline NSUInteger indexOfObject(T elem) {
		return [array() indexOfObject:elemToBackingType(elem)];
	}
    
    inline NSUInteger count() {
        return array().count;
    }
    
    inline T objectAtIndex(const NSUInteger index) {
        return elemToPublicType([array() objectAtIndex:index]);
    }
    
    inline T operator[](const NSUInteger index) {
        return objectAtIndex(index);
    }
    
    inline NSString *description() {
        return array().description;
    }
	
	inline TSCollectionStringDescription *collectionDescription() {
		return [[TSCollectionStringDescription alloc] initWithDescriptionString:description()];
	}
    
//    inline operator NSArray *() const {
//        return array();
//    }
    
    inline NSArray *asNSArray() const {
        return array();
    }
    
    //
    // Collection manipulations
    //
    
    using typename TSTraversable<T>::Predicate;
    using typename TSTraversable<T>::PredicateAlt;
    
    template<typename U>
    inline TSArray<U> map(U (^f)(T elem)) {
        NSMutableArray *mappedArray = doMap(f);
        return [NSArray arrayWithArray:mappedArray];
    }
    
    inline TSArray<T> filter(Predicate pred) {
        NSMutableArray *filteredArray = doFilter(pred);
        return [NSArray arrayWithArray:filteredArray];
    }
    
    inline TSArray<T> filter(PredicateAlt pred) {
        return filter(reinterpret_cast<Predicate>(pred));
    }
    
    template<typename U>
    inline TSArray<T> distinctBy(U (^f)(T elem)) {
        NSMutableArray *filteredArray = doDistinctBy(f);
        return [NSArray arrayWithArray:filteredArray];
    }
    
    template<typename U>
    inline TSArray<U> flatMap(TSArray<U> (^f)(T)) {
        TSTraversable<U> (^f1)(T) = reinterpret_cast<TSTraversable<U> (^)(T)> ( f );
        return [NSArray arrayWithArray:doFlatMap<U>(f1)];
    }
    
    template<typename U>
    inline TSArray<U> flatMap(TSSet<U> (^f)(T)) {
        TSTraversable<U> (^f1)(T) = reinterpret_cast<TSTraversable<U> (^)(T)> ( f );
        return [NSArray arrayWithArray:doFlatMap<U>(f1)];
    }

    
protected:
    
    inline NSArray *array() const {
        return (NSArray *)TSTraversable<T>::traversable;
    }
    
    template<typename U>
    inline NSMutableArray *doMap(U (^f)(T)) {
        NSMutableArray *mappedArray = [NSMutableArray arrayWithCapacity:array().count];
        for (T elem in array())
            [mappedArray addObject:f(elem)];
        return mappedArray;
    }
    
    inline NSMutableArray *doFilter(BOOL (^f)(T)) {
        NSMutableArray *filteredArray = [NSMutableArray array];
        for (T elem in array())
            if (f(elem))
                [filteredArray addObject:elem];
        return filteredArray;
    }

    template<typename U>
    inline NSMutableArray *doDistinctBy(U (^f)(T elem)) {
        NSMutableArray *filteredArray = [NSMutableArray array];
        NSMutableSet *addedDerivedItems = [[NSMutableSet alloc] init];
        for (T elem in array()) {
            U derivedItem = f(elem);
            if (![addedDerivedItems containsObject:derivedItem]) {
                [addedDerivedItems addObject:derivedItem];
                [filteredArray addObject:elem];
            }
        }
        return filteredArray;
    }

    template<typename U>
    inline NSMutableArray *doFlatMap(TSTraversable<U> (^f)(T)) {
        NSMutableArray *mappedArray = [NSMutableArray array];
        for (T elem in array()) {
            TSTraversable<U> mappedTraversable = f(elem);
            mappedTraversable.foreach(^(U mappedElem) {
                [mappedArray addObject:mappedElem];
            });
        }
        return mappedArray;
    }
    
};
    
TS_STATIC_ASSERT(sizeof(void *) == sizeof(TSArray<NSObject *>), "TSArray does not have pointer size");

//
// TSArray builder functions
//

template<typename T>
inline void TSMutableArrayBuilderAppendRecusively(NSMutableArray *array) {
	// nothing
}

template<typename T, typename T0, typename... TS>
inline void TSMutableArrayBuilderAppendRecusively(NSMutableArray *array, T0 head, TS... tail) {
	TSTypeConstraintDerivedFrom<T0, T>();
	[array addObject: head];
	TSMutableArrayBuilderAppendRecusively<T, TS...>(array, tail...);
}

template<typename T, typename... TS, int N = 1 + sizeof...(TS)>
inline TSArray<T> TSArrayMake(T head, TS... tail) {
	NSMutableArray *array = [NSMutableArray arrayWithCapacity:N];
	TSMutableArrayBuilderAppendRecusively<T, T, TS...>(array, head, tail...);
	return [NSArray arrayWithArray:array];
}

template<typename T>
inline TSArray<T> TSArrayMake() {
	return [NSArray array];
}
    
template<typename T>
inline TSArray<T> TSArrayWithArray(TSArray<T> array) {
    return [NSArray arrayWithArray:array.asNSArray()];
}


#endif
