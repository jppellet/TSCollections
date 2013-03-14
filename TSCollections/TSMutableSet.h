//
//  TSMutableSet.h
//  TSCollections
//
//  Created by Jean-Philippe Pellet on 08.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#ifndef _TS_MUTABLE_SET_H
#define _TS_MUTABLE_SET_H

#include "TSSet.h"

@class NSMutableSet;

template<typename T>
class TSMutableSet : public TSSet<T> {
public:
	
    //
    // Constructors & special members
    //
    
    // Main constructors and destructor
    TSMutableSet(): TSSet<T>(nil) {}
    TSMutableSet(NSMutableSet *_set): TSSet<T>(_set) {}
    
    // Copy constructor, invariant in T
    TSMutableSet(const TSMutableSet<T>& that): TSSet<T>(that.asNSMutableSet()) {}
    
    // Move constructor, covariant in T
    template<typename T1>
    TSMutableSet(TSMutableSet<T1>&& that): TSSet<T>(that.asNSMutableSet()) {
        TSTypeConstraintDerivedFrom<T1, T>();
        reinterpret_cast<TSMutableSet<T>&>(that).traversable = nil;
    }
    
    // Copy assignment, invariant in T
    TSMutableSet<T>& operator=(const TSMutableSet<T>& that) {
        this->traversable = reinterpret_cast<const TSMutableSet<T>&>(that).traversable;
        return *this;
    }
    
    // Move assignment, covariant in T
    template<typename T1>
    TSMutableSet<T>& operator=(TSMutableSet<T1>&& that) {
        TSTypeConstraintDerivedFrom<T1, T>();
        this->traversable = reinterpret_cast<TSMutableSet<T>&>(that).traversable;
        reinterpret_cast<TSMutableSet<T>&>(that).traversable = nil;
        return *this;
    }
        
    //
    // Objective-C-like interface
    //

    inline void addObject(T elem) {
        [mutableSet() addObject:this->toNSObj(elem)];
    }
    
    inline void operator+=(T elem) {
        addObject(elem);
    }
    
    template<typename T1>
    inline void addObjectsFromArray(TSArray<T1> &that) {
        TSTypeConstraintDerivedFrom<T1, T>();
        [mutableSet() addObjectsFromArray:that];
    }
        
    inline void insertObjectAtIndex(T elem, NSUInteger index) {
        [mutableSet() insertObject:unwrap(elem) atIndex:index];
    }
    
    inline void removeObject(T elem) {
        [mutableSet() removeObject:this->toNSObj(elem)];
    }
    
    inline void operator-=(T elem) {
        removeObject(elem);
    }
    
    inline operator NSMutableSet *() const {
        return mutableSet();
    }

    inline NSMutableSet *asNSMutableSet() const {
        return mutableSet();
    }

    //
    // Collection manipulations
    //

private:
    inline NSMutableSet *mutableSet() const {
        return (NSMutableSet *)TSTraversable<T>::traversable;
    }
};

TS_STATIC_ASSERT(sizeof(void *) == sizeof(TSMutableSet<NSObject *>), "TSMutableSet does not have pointer size");
    
//
// TSMutableSet builder functions
//

template<typename T, typename... TS, int N = 1 + sizeof...(TS)>
inline TSMutableSet<T> TSMutableSetMake(T head, TS... tail) {
	NSMutableSet *set = [NSMutableSet setWithCapacity:N];
	TSMutableSetBuilderAppendRecusively<T, T, TS...>(set, head, tail...);
	return set;
}

template<typename T>
inline TSMutableSet<T> TSSetMake() {
	return [NSMutableSet set];
}


#endif
