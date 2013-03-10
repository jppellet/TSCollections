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
    TSMutableSet(NSMutableSet *_set): TSSet<T>(_set) {}
    
    using typename TSSet<T>::PublicElemType;
    
    //
    // Objective-C-like interface
    //

    inline void addObject(PublicElemType elem) {
        [mutableSet() addObject:this->toNSObj(elem)];
    }
    
    inline void operator+=(PublicElemType elem) {
        addObject(elem);
    }
    
    template<typename T1>
    inline void addObjectsFromArray(TSArray<T1> &that) {
        TSConstraintDerivedFrom<T1, T>();
        [mutableSet() addObjectsFromArray:that];
    }
    
    template<typename T1>
    inline void operator+=(TSArray<T1> &that) {
        TSConstraintDerivedFrom<T1, T>();
        addObjectsFromArray(that);
    }
    
    inline void insertObjectAtIndex(PublicElemType elem, NSUInteger index) {
        [mutableSet() insertObject:unwrap(elem) atIndex:index];
    }
    
    inline void removeObject(PublicElemType elem) {
        [mutableSet() removeObject:this->toNSObj(elem)];
    }
    
    inline void operator-=(PublicElemType elem) {
        removeObject(elem);
    }
    
    inline operator NSMutableSet *() const {
        return mutableSet();
    }

    inline NSMutableSet *asNSMutableSet() {
        return mutableSet();
    }

    //
    // Collection manipulations
    //

private:
    inline NSMutableSet *mutableSet() {
        return (NSMutableSet *)TSTraversable<T>::traversable;
    }
};

#endif
