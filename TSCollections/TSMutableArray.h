//
//  TSMutableArray.h
//  TSCollections
//
//  Created by Jean-Philippe Pellet on 08.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#ifndef _TS_MUTABLE_ARRAY_H
#define _TS_MUTABLE_ARRAY_H

#include "TSArray.h"

@class NSMutableArray;

template<typename T>
class TSMutableArrayAccess {
    
    NSMutableArray *const array;
    const NSUInteger index;

    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(T, elem, PublicElemType, BackingElemType)
    
public:
    TSMutableArrayAccess(NSMutableArray *_array, const NSUInteger _index): array(_array), index(_index) {}
    
    inline PublicElemType operator()() { return doGet(); }
    inline operator PublicElemType()   { return doGet(); }
    inline void operator=(PublicElemType elem) {
        [array setObject:elemToBackingType(elem) atIndexedSubscript:index];
    }
    
private:
    inline T doGet() {
        return elemToPublicType([array objectAtIndex:index]);
    }
    
};

template<typename T>
class TSMutableArray : public TSArray<T> {
public:
    TSMutableArray(NSMutableArray *_array): TSArray<T>(_array) {}
    
    using typename TSArray<T>::PublicElemType;
    
    //
    // Objective-C-like interface
    //
    
    inline void addObject(PublicElemType elem) {
        [mutableArray() addObject:this->elemToBackingType(elem)];
    }
    
    inline void operator+=(PublicElemType elem) {
        addObject(elem);
    }
    
    template<typename T1>
    inline void addObjectsFromArray(TSArray<T1> &that) {
        TSConstraintDerivedFrom<T1, T>();
        [mutableArray() addObjectsFromArray:that];
    }
    
    template<typename T1>
    inline void operator+=(TSArray<T1> &that) {
        TSConstraintDerivedFrom<T1, T>();
        addObjectsFromArray(that);
    }
    
    inline void insertObjectAtIndex(PublicElemType elem, NSUInteger index) {
        [mutableArray() insertObject:unwrap(elem) atIndex:index];
    }
    
    inline void removeObject(PublicElemType elem) {
        [mutableArray() removeObject:this->elemToBackingType(elem)];
    }
    
    inline void operator-=(PublicElemType elem) {
        removeObject(elem);
    }
    
    inline operator NSMutableArray *() const {
        return mutableArray();
    }
    
    inline TSMutableArrayAccess<T> operator[](const NSUInteger index) {
        return TSMutableArrayAccess<T>(mutableArray(), index);
    }
    
    inline NSMutableArray *asNSMutableArray() {
        return mutableArray();
    }
    
    //
    // Collection manipulations
    //

    template<typename U>
    inline TSMutableArray<U> map(U (^f)(T elem)) {
        NSMutableArray *mappedArray = this->template doMap<U>(f);
        return mappedArray;
    }
    
    inline TSMutableArray<T> filter(BOOL (^f)(T elem)) {
        NSMutableArray *filteredArray = doFilter(f);
        return filteredArray;
    }
    
    template<typename U>
    inline TSMutableArray<U> flatMap(TSArray<U> (^f)(T)) {
        TSTraversable<U> (^f1)(T) = reinterpret_cast<TSTraversable<U> (^)(T)> ( f );
        return this->template doFlatMap<U>(f1);
    }
    
    template<typename U>
    inline TSMutableArray<U> flatMap(TSSet<U> (^f)(T)) {
        TSTraversable<U> (^f1)(T) = reinterpret_cast<TSTraversable<U> (^)(T)> ( f );
        return this->template doFlatMap<U>(f1);
    }
    
private:
    inline NSMutableArray *mutableArray() {
        return (NSMutableArray *)TSTraversable<T>::traversable;
    }
    
    //inline builderToCollectionType
    
};

#endif
