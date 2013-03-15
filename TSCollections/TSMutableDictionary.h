//
//  TSMutableDictionary.h
//  TSCollections
//
//  Created by Jean-Philippe Pellet on 08.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#ifndef _TS_MUTABLE_DICTIONARY_H
#define _TS_MUTABLE_DICTIONARY_H

#include "TSDictionary.h"

@class NSMutableDictionary;

template<typename K, typename V>
class TSMutableDictionaryAccess {
private:
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(K, key,   BackingKeyTape)
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(V, value, BackingValueTape)
    
public:
    TSMutableDictionaryAccess(NSMutableDictionary *_dictionary, BackingKeyTape _key): dictionary(_dictionary), key(_key) {}
    
    inline V operator()() { return doGet(); }
    inline operator V()   { return doGet(); }
    inline void operator=(V elem) {
        [dictionary setObject:valueToBackingType(elem) forKey:key];
    }
    
private:
    inline V doGet() {
        return valueToPublicType([dictionary objectForKey:key]);
    }
    
protected:
    NSMutableDictionary *const dictionary;
    BackingKeyTape const key;
};

template<typename K, typename V>
class TSMutableDictionary : public TSDictionary<K, V> {
public:
	
    // Main constructors and destructor
    TSMutableDictionary(): TSDictionary<K, V>(nil) {}
    TSMutableDictionary(NSMutableDictionary *_dictionary): TSDictionary<K, V>(_dictionary) {}
    
    // Copy constructor, invariant in K, V
    TSMutableDictionary(const TSMutableDictionary<K, V>& that): TSDictionary<K, V>(that.asNSMutableDictionary) {}
    
    // Move constructor, invariant in K, covariant in V
    template<typename V1>
    TSMutableDictionary(TSMutableDictionary<K, V1>&& that): TSDictionary<K, V>(that.asNSMutableDictionary) {
        TSTypeConstraintDerivedFrom<V1, V>();
        that.traversable = nil;
    }
    
    // Copy assignment, invariant in K, V
    TSMutableDictionary<K, V>& operator=(const TSMutableDictionary<K, V>& that) {
        this->dictionary = that.dictionary;
        return *this;
    }
    
    // Move assignment, invariant in K, covariant in V
    template<typename V1>
    TSMutableDictionary<K, V>& operator=(TSMutableDictionary<K, V1>&& that) {
        TSTypeConstraintDerivedFrom<V1, V>();
        this->dictionary = that.dictionary;
        that.dictionary = nil;
        return *this;
    }

    //
    // Objective-C-like interface
    //

    template<typename K1, typename V1>
    inline void addEntriesFromDictionary(TSDictionary<K1, V1> otherDictionary) {
        TSTypeConstraintDerivedFrom<K1, K>();
        TSTypeConstraintDerivedFrom<V1, V>();
        [mutableDictionary() addEntriesFromDictionary:otherDictionary];
    }
        
    inline void removeAllObjects() {
        [mutableDictionary() removeAllObjects];
    }
    
    inline void removeObjectForKey(K key) {
        [mutableDictionary() removeObjectForKey:keyToBackingType(key)];
    }
    
    inline void operator-=(K key) {
        removeObjectForKey(key);
    }
    
    inline void removeObjectsForKeys(TSArray<K> keyArray) {
        [mutableDictionary() removeObjectsForKeys:keyArray];
    }
    
    template<typename K1, typename V1>
    inline void setDictionary(TSDictionary<K1, V1> otherDictionary) {
        TSTypeConstraintDerivedFrom<K1, K>();
        TSTypeConstraintDerivedFrom<V1, V>();
        [mutableDictionary() setDictionary:otherDictionary];
    }
    
    inline void setObjectForKey(V anObject, K key) {
        [mutableDictionary() setObject:valueToBackingType(anObject) forKey:keyToBackingType(key)];
    }
    
    inline TSMutableDictionaryAccess<K, V> operator[](K key) {
        return TSMutableDictionaryAccess<K, V>(mutableDictionary(), keyToBackingType(key));
    }
    
    inline void setValueForKey(V anObject, NSString *key) {
        [mutableDictionary() setValue:valueToBackingType(anObject) forKey:key];
    }
    
    inline operator NSMutableDictionary *() const {
        return mutableDictionary();
    }

    inline NSMutableDictionary *asNSDictionary() const {
        return mutableDictionary();
    }
    
    //
    // Collection manipulations
    //

    
private:
    inline NSMutableDictionary *mutableDictionary() const {
        return (NSMutableDictionary *)TSDictionary<K, V>::dictionary;
    }
};

TS_STATIC_ASSERT(sizeof(void *) == sizeof(TSMutableDictionary<NSObject *, NSObject *>), "TSMutableDictionary does not have pointer size");
	
//
// TSMutableDictionary builder functions
//

template<typename K, typename V, typename... KVS, int N = 1 + sizeof...(KVS) / 2>
inline TSMutableDictionary<K, V> TSMutableDictionaryMake(K headKey, V headValue, KVS... tail) {
	NSMutableDictionary *dictionary = [NSMutableDictionary dictionaryWithCapacity:N];
	TSMutableDictionaryBuilderAppendRecusively<K, V, K, V, KVS...>(dictionary, headKey, headValue, tail...);
	return dictionary;
}

template<typename K, typename V>
inline TSMutableDictionary<K, V> TSMutableDictionaryMake() {
	return [NSMutableDictionary dictionary];
}

	
template<typename K, typename V>
inline TSMutableDictionary<K, V> NSMutableDictionaryWithCapacity(NSUInteger cap) {
	return [NSMutableDictionary dictionaryWithCapacity:cap];
}


#endif
