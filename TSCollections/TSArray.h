#ifndef _TS_ARRAY_H
#define _TS_ARRAY_H

#include "TSCollectionsConfig.h"
#include "TSTraversable.h"

@class NSArray;

template<typename T>
class TSArray : public TSTraversable<T> {
protected:
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(T, elem, PublicElemType, BackingElemType)
    
public:
    
    TSArray(NSArray *_array): TSTraversable<T>(_array) {}
    
    //
    // Objective-C-like interface
    //
    
    inline NSString *componentsJoinedByString(NSString *sep) {
        return [array() componentsJoinedByString:sep];
    }
    
    inline BOOL contains(PublicElemType elem) {
        return [array() containsObject:elemToBackingType(elem)];
    }
    
    inline NSUInteger count() {
        return array().count;
    }
    
    inline PublicElemType operator[](const NSUInteger index) {
        return elemToPublicType([array() objectAtIndex:index]);
    }
    
    inline NSString *description() {
        return array().description;
    }
    
    inline operator NSArray *() {
        return array();
    }
    
    inline NSArray *asNSArray() {
        return array();
    }
    
    //
    // Collection manipulations
    //
    
    template<typename U>
    inline TSArray<U> map(U (^f)(T elem)) {
        NSMutableArray *mappedArray = doMap(f);
        return [NSArray arrayWithArray:mappedArray];
    }
    
    inline TSArray<T> filter(BOOL (^f)(T elem)) {
        NSMutableArray *filteredArray = doFilter(f);
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
    
    inline NSArray *array() {
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

#endif
