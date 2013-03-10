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
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(K, key,   PublicKeyType,   BackingKeyTape)
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(V, value, PublicValueType, BackingValueTape)
    
public:
    TSMutableDictionaryAccess(NSMutableDictionary *_dictionary, BackingKeyTape _key): dictionary(_dictionary), key(_key) {}
    
    inline PublicValueType operator()() { return doGet(); }
    inline operator PublicValueType()   { return doGet(); }
    inline void operator=(PublicValueType elem) {
        [dictionary setObject:valueToBackingType(elem) forKey:key];
    }
    
private:
    inline PublicValueType doGet() {
        return valueToPublicType([dictionary objectForKey:key]);
    }
    
protected:
    NSMutableDictionary *const dictionary;
    BackingKeyTape const key;
};

template<typename K, typename V>
class TSMutableDictionary : public TSDictionary<K, V> {
public:
    TSMutableDictionary(NSMutableDictionary *_dictionary): TSDictionary<K, V>(_dictionary) {}
    
    using typename TSDictionary<K, V>::PublicKeyType;
    using typename TSDictionary<K, V>::PublicValueTyoe;
    
    //
    // Objective-C-like interface
    //

    template<typename K1, typename V1>
    inline void addEntriesFromDictionary(TSDictionary<K1, V1> otherDictionary) {
        TSConstraintDerivedFrom<K1, K>();
        TSConstraintDerivedFrom<V1, V>();
        [mutableDictionary() addEntriesFromDictionary:otherDictionary];
    }
    
    template<typename K1, typename V1>
    inline void operator+=(TSDictionary<K1, V1> otherDictionary) {
        TSConstraintDerivedFrom<K1, K>();
        TSConstraintDerivedFrom<V1, V>();
        addEntriesFromDictionary(otherDictionary);
    }
    
    inline void removeAllObjects() {
        [mutableDictionary() removeAllObjects];
    }
    
    inline void removeObjectForKey(PublicKeyType key) {
        [mutableDictionary() removeObjectForKey:keyToBackingType(key)];
    }
    
    inline void operator-=(PublicKeyType key) {
        removeObjectForKey(key);
    }
    
    inline void removeObjectsForKeys(TSArray<K> keyArray) {
        [mutableDictionary() removeObjectsForKeys:keyArray];
    }
    
    template<typename K1, typename V1>
    inline void setDictionary(TSDictionary<K1, V1> otherDictionary) {
        TSConstraintDerivedFrom<K1, K>();
        TSConstraintDerivedFrom<V1, V>();
        [mutableDictionary() setDictionary:otherDictionary];
    }
    
    inline void setObjectForKey(PublicValueTyoe anObject, PublicKeyType key) {
        [mutableDictionary() setObject:valueToBackingType(anObject) forKey:keyToBackingType(key)];
    }
    
    inline TSMutableDictionaryAccess<K, V> operator[](PublicKeyType key) {
        return TSMutableDictionaryAccess<K, V>(mutableDictionary(), keyToBackingType(key));
    }
    
    inline void setValueForKey(PublicValueTyoe anObject, NSString *key) {
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
    inline NSMutableDictionary *mutableDictionary() {
        return (NSMutableDictionary *)TSDictionary<K, V>::dictionary;
    }
};

#endif
