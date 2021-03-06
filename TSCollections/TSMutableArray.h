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

    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(T, elem, BackingElemType)
    
public:
    TSMutableArrayAccess(NSMutableArray *_array, const NSUInteger _index): array(_array), index(_index) {}
    
    inline T operator()() { return doGet(); }
    inline operator T()   { return doGet(); }
    inline void operator=(T elem) {
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
    
    //
    // Constructors & special members
    //
    
    // Main constructors and destructor
	TSMutableArray(): TSArray<T>(nil) {}
	TSMutableArray(NSMutableArray *_array): TSArray<T>(_array) {}

    // Copy constructor, invariant in T
    TSMutableArray(const TSMutableArray<T>& that): TSArray<T>(that.asNSMutableArray()) {}
    
    // Move constructor, covariant in T
    template<typename T1>
    TSMutableArray(TSMutableArray<T1>&& that): TSArray<T>(that.asNSMutableArray()) {
        TSTypeConstraintDerivedFrom<T1, T>();
        reinterpret_cast<TSMutableArray<T>&>(that).traversable = nil;
    }
    
    // Copy assignment, invariant in T
    TSMutableArray<T>& operator=(const TSMutableArray<T>& that) {
        this->traversable = reinterpret_cast<const TSMutableArray<T>&>(that).traversable;
        return *this;
    }
    
    // Move assignment, covariant in T
    template<typename T1>
    TSMutableArray<T>& operator=(TSMutableArray<T1>&& that) {
        TSTypeConstraintDerivedFrom<T1, T>();
        this->traversable = that.traversable;
        reinterpret_cast<TSMutableArray<T>&>(that).traversable = nil;
        return *this;
    }
    
    //
    // Objective-C-like interface
    //
    
    inline void addObject(T elem) {
        [mutableArray() addObject:this->elemToBackingType(elem)];
    }
    
    inline void operator+=(T elem) {
        addObject(elem);
    }
    
    template<typename T1>
    inline void addObjectsFromArray(TSArray<T1> &that) {
        TSTypeConstraintDerivedFrom<T1, T>();
        [mutableArray() addObjectsFromArray:that];
    }
        
    inline void insertObjectAtIndex(T elem, NSUInteger index) {
        [mutableArray() insertObject:unwrap(elem) atIndex:index];
    }
    
    inline void removeObject(T elem) {
        [mutableArray() removeObject:this->elemToBackingType(elem)];
    }
    
    inline void removeObjectAtIndex(NSUInteger i) {
        [mutableArray() removeObjectAtIndex:i];
    }
    
    inline void operator-=(T elem) {
        removeObject(elem);
    }
    
    inline void removeAllObjects() {
        [mutableArray() removeAllObjects];
    }
    
    inline void exchangeObjectsAtIndices(NSUInteger i, NSUInteger j) {
        [mutableArray() exchangeObjectAtIndex:i withObjectAtIndex:j];
    }
    
//    inline operator NSMutableArray *() const {
//        return mutableArray();
//    }
    
    inline TSMutableArrayAccess<T> operator[](const NSUInteger index) {
        return TSMutableArrayAccess<T>(mutableArray(), index);
    }
    
    inline NSMutableArray *asNSMutableArray() const {
        return mutableArray();
    }
    
    //
    // Collection manipulations
    //
    
    using typename TSTraversable<T>::Predicate;
    using typename TSTraversable<T>::PredicateAlt;
    
    template<typename U>
    inline TSMutableArray<U> map(U (^f)(T elem)) {
        NSMutableArray *mappedArray = this->template doMap<U>(f);
        return mappedArray;
    }
    
    inline TSMutableArray<T> filter(Predicate pred) {
        NSMutableArray *filteredArray = this->doFilter(pred);
        return filteredArray;
    }
    
    inline TSMutableArray<T> filter(PredicateAlt pred) {
        return filter(reinterpret_cast<Predicate>(pred));
    }
    
    template<typename U>
    inline TSMutableArray<T> distinctBy(U (^f)(T elem)) {
        NSMutableArray *filteredArray = this->template doDistinctBy<U>(f);
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
    inline NSMutableArray *mutableArray() const {
        return (NSMutableArray *)TSTraversable<T>::traversable;
    }
    
    //inline builderToCollectionType
    
};

TS_STATIC_ASSERT(sizeof(void *) == sizeof(TSMutableArray<NSObject *>), "TSMutableArray does not have pointer size");

//
// TSMutableArray builder functions
//

template<typename T, typename... TS, int N = 1 + sizeof...(TS)>
inline TSMutableArray<T> TSMutableArrayMake(T head, TS... tail) {
	NSMutableArray *array = [NSMutableArray arrayWithCapacity:N];
	TSMutableArrayBuilderAppendRecusively<T, T, TS...>(array, head, tail...);
	return array;
}

template<typename T>
inline TSMutableArray<T> TSMutableArrayMake() {
	return [NSMutableArray array];
}

template<typename T>
inline TSMutableArray<T> TSMutableArrayWithCapacity(NSUInteger cap) {
	return [NSMutableArray arrayWithCapacity:cap];
}
    
template<typename T>
inline TSMutableArray<T> TSMutableArrayWithArray(TSArray<T> array) {
    return [NSMutableArray arrayWithArray:array.asNSArray()];
}

	
	
#endif
