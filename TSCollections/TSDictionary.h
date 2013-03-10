#ifndef _TS_DICTIONARY_H
#define _TS_DICTIONARY_H

#include "TSCollectionsConfig.h"

@class NSDictionary;

template<typename K, typename V>
class TSDictionary {
protected:
    NSDictionary *const dictionary;
    
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(K, key,   PublicKeyType,   BackingKeyTape)
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(V, value, PublicValueType, BackingValueTape)
    
public:
    
    TSDictionary(NSDictionary *_dictionary): dictionary(_dictionary) {
#if TS_COLLECTIONS_DEBUG
        NSArray *allKeys = dictionary.allKeys;
        TSCollectionsElementConformityCheck<K>(allKeys, nil, allKeys);
        NSArray *allValues = dictionary.allValues;
        TSCollectionsElementConformityCheck<V>(allValues, nil, allValues);
#endif
    }
    
    //
    // Objective-C-like interface
    //

    inline TSArray<K> allKeys() {
        return dictionary.allKeys;
    }
    
    inline TSArray<K> allKeysForObject(V anObject) {
        return [dictionary allKeysForObject:valueToBackingType(anObject)];
    }
    
    // TODO TSComparator?
    inline TSArray<K> keysSortedByValueUsingComparator(NSComparator comparator) {
        return [dictionary keysSortedByValueUsingComparator:comparator];
    }
    
    inline TSArray<K> keysSortedByValueUsingSelector(SEL comparator) {
        return [dictionary keysSortedByValueUsingSelector:comparator];
    }
    
    inline TSArray<K> keysSortedByValueUsingSelector(NSSortOptions opts, NSComparator comparator) {
        return [dictionary keysSortedByValueWithOptions:opts usingComparator:comparator];
    }
    
    inline TSArray<V> allValues() {
        return dictionary.allValues;
    }
    
    inline NSUInteger count() {
        return dictionary.count;
    }
    
    inline NSString *description() {
        return dictionary.description;
    }
    
    // TODO only if keys are strings
    inline NSString *descriptionInStringsFileFormat() {
        return dictionary.descriptionInStringsFileFormat;
    }
    
    // TODO descriptionWithLocale, descriptionWithLocaleIndent
    // TODO enumerateKeysAndObjectsUsingBlock, enumerateKeysAndObjectsUsingBlockWithOptions
/*
    inline void enumerateKeysAndObjectsUsingBlock:(void (^block)(PublicKeyType key, PublicValueType obj, BOOL *stop)) {
        return [dictionary enumerateKeysAndObjectsUsingBlock:block];
    }*/
    
    // TODO keysOfEntriesPassingTest, keysOfEntriesWithOptions:passingTest
    
    inline PublicValueType objectForKey(PublicKeyType key) {
        return valueToPublicType([dictionary objectForKey:keyToBackingType(key)]);
    }
    
    inline PublicValueType operator[](PublicKeyType key) {
        return objectForKey(key);
    }
    
    inline PublicValueType valueForKey(NSString *key) {
        return valueToPublicType([dictionary valueForKey:key]);
    }

    inline TSArray<K> objectsForKeysWithNotFoundMarker(TSArray<K> keys, PublicKeyType anObject) {
        return [dictionary objectsForKeys:keys notFoundMarker:keyToBackingType(anObject)];
    }

    inline BOOL writeToFile(NSString *path, BOOL atomically) {
        return [dictionary writeToFile:path atomically:atomically];
    }
    
    inline BOOL writeToURL(NSURL *url, BOOL atomically) {
        return [dictionary writeToURL:url atomically:atomically];
    }
    
    inline operator NSDictionary *() const {
        return dictionary;
    }
    
    inline NSDictionary *asNSDictionary() const {
        return dictionary;
    }
    
    //
    // Collection manipulations
    //

};

#endif
