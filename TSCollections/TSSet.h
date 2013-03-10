#ifndef _TS_SET_H
#define _TS_SET_H

#include "TSCollectionsConfig.h"

@class NSSet;

template<typename T>
class TSSet : public TSTraversable<T> {
protected:    
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(T, elem, PublicElemType, BackingElemType)
    
public:
    
    TSSet(NSSet *_set): TSTraversable<T>(_set) {}
    
    //
    // Objective-C-like interface
    //

    inline NSString *componentsJoinedByString(NSString *sep) {
        return [set() componentsJoinedByString:sep];
    }
    
    inline BOOL contains(PublicElemType elem) {
        return [set() containsObject:unwrap(elem)];
    }
    
    inline NSUInteger count() {
        return set().count;
    }
    
    inline operator NSSet *() const {
        return set();
    }
    
    inline NSString *description() {
        return set().description;
    }
    
    inline NSSet *asNSSet() {
        return set();
    }
    
    //
    // Collection manipulations
    //
    
    template<typename U>
    inline TSSet<U> map(U (^f)(T elem)) {
        NSMutableSet *mappedSet = doMap(f);
        return [NSSet setWithSet:mappedSet];
    }
    
    inline TSSet<T> filter(BOOL (^f)(T elem)) {
        NSMutableSet *filteredSet = doFilter(f);
        return [NSSet setWithSet:filteredSet];
    }
    
    template<typename U>
    inline TSSet<U> flatMap(TSArray<U> (^f)(T)) {
        TSTraversable<U> (^f1)(T) = reinterpret_cast<TSTraversable<U> (^)(T)> ( f );
        return [NSSet setWithSet:doFlatMap<U>(f1)];
    }
    
    template<typename U>
    inline TSSet<U> flatMap(TSSet<U> (^f)(T)) {
        TSTraversable<U> (^f1)(T) = reinterpret_cast<TSTraversable<U> (^)(T)> ( f );
        return [NSSet setWithSet:doFlatMap<U>(f1)];
    }
    
    
protected:
    
    inline NSSet *set() {
        return (NSSet *)TSTraversable<T>::traversable;
    }
    
    template<typename U>
    inline NSMutableSet *doMap(U (^f)(T)) {
        NSMutableSet *mappedSet = [NSMutableSet set];
        for (T elem in set())
            [mappedSet addObject:f(elem)];
        return mappedSet;
    }
    
    inline NSMutableSet *doFilter(BOOL (^f)(T)) {
        NSMutableSet *filteredArray = [NSMutableSet set];
        for (T elem in set())
            if (f(elem))
                [filteredArray addObject:elem];
        return filteredArray;
    }
    
    template<typename U>
    inline NSMutableSet *doFlatMap(TSTraversable<U> (^f)(T)) {
        NSMutableSet *mappedSet = [NSMutableSet set];
        for (T elem in set()) {
            TSTraversable<U> mappedTraversable = f(elem);
            mappedTraversable.foreach(^(U mappedElem) {
                [mappedSet addObject:mappedElem];
            });
        }
        return mappedSet;
    }    
    
};
    
#endif