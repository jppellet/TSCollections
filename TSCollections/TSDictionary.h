#ifndef _TS_DICTIONARY_H
#define _TS_DICTIONARY_H

#include "TSCollectionsConfig.h"

@class NSDictionary;

template<typename K, typename V>
class TSDictionary {
protected:
    NSDictionary *const dictionary;
    
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(K, key,   BackingKeyTape)
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(V, value, BackingValueTape)
    
public:
    
    //
    // Constructors & special members
    //
    
    // Main constructors and destructor
    TSDictionary(): dictionary(nil) {}
	TSDictionary(NSDictionary *_dictionary): dictionary(_dictionary) {
#if TS_COLLECTIONS_DEBUG
        NSArray *allKeys = dictionary.allKeys;
        TSCollectionsElementConformityCheck<K>(allKeys, nil, allKeys);
        NSArray *allValues = dictionary.allValues;
        TSCollectionsElementConformityCheck<V>(allValues, nil, allValues);
#endif
    }
    
    // Copy constructor, invariant in K, covariant in V
    template<typename V1>
    TSDictionary(const TSDictionary<K, V1>& that): dictionary(that.asNSDictionary) {
        TSTypeConstraintDerivedFrom<V1, V>();
    }
    
    // Move constructor, invariant in K, covariant in V
    template<typename V1>
    TSDictionary(TSDictionary<K, V1>&& that): dictionary(that.asNSDictionary) {
        TSTypeConstraintDerivedFrom<V1, V>();
        reinterpret_cast<TSDictionary<K, V>&>(that).dictionary = nil;
    }
    
    // Copy & move assignment together, invariant in K, covariant in V
    template<typename V1>
    TSDictionary<K, V>& operator=(TSDictionary<K, V1> that) {
        TSTypeConstraintDerivedFrom<V1, V>();
        std::swap(this->traversable, reinterpret_cast<TSDictionary<K, V>&>(that).traversable);
        return *this;
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
	
	inline TSCollectionStringDescription *collectionDescription() {
		return [[TSCollectionStringDescription alloc] initWithDescriptionString:description()];
	}
    
    // TODO only if keys are strings
    inline NSString *descriptionInStringsFileFormat() {
        return dictionary.descriptionInStringsFileFormat;
    }
    
    // TODO descriptionWithLocale, descriptionWithLocaleIndent
    // TODO enumerateKeysAndObjectsUsingBlock, enumerateKeysAndObjectsUsingBlockWithOptions
/*
    inline void enumerateKeysAndObjectsUsingBlock:(void (^block)(K key, V obj, BOOL *stop)) {
        return [dictionary enumerateKeysAndObjectsUsingBlock:block];
    }*/
    
    // TODO keysOfEntriesPassingTest, keysOfEntriesWithOptions:passingTest
    
    inline V objectForKey(K key) {
        return valueToPublicType([dictionary objectForKey:keyToBackingType(key)]);
    }
    
    inline V operator[](K key) {
        return objectForKey(key);
    }
    
    inline V valueForKey(NSString *key) {
        return valueToPublicType([dictionary valueForKey:key]);
    }

    inline TSArray<K> objectsForKeysWithNotFoundMarker(TSArray<K> keys, K anObject) {
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

static_assert(sizeof(void *) == sizeof(TSDictionary<NSObject *, NSObject *>), "TSDictionary does not have pointer size");

//
// TSDictionary builder functions
//

template<typename K, typename V>
inline void TSMutableDictionaryBuilderAppendRecusively(NSMutableDictionary *dictionary) {
	// nothing
}

template<typename K, typename V, typename K0, typename V0, typename... KVS>
inline void TSMutableDictionaryBuilderAppendRecusively(NSMutableDictionary *dictionary, K0 headKey, V0 headValue, KVS... tail) {
	TSTypeConstraintEqual<K0, K>(); // keys are not covariant
	TSTypeConstraintDerivedFrom<V0, V>();
	[dictionary setObject: headValue forKey: headKey];
	TSMutableDictionaryBuilderAppendRecusively<K, V, KVS...>(dictionary, tail...);
}

template<typename K, typename V, typename... KVS, int N = 1 + sizeof...(KVS) / 2>
inline TSDictionary<K, V> TSDictionaryMake(K headKey, V headValue, KVS... tail) {
	NSMutableDictionary *dictionary = [NSMutableDictionary dictionaryWithCapacity:N];
	TSMutableDictionaryBuilderAppendRecusively<K, V, K, V, KVS...>(dictionary, headKey, headValue, tail...);
	return [NSDictionary dictionaryWithDictionary:dictionary];
}

template<typename K, typename V>
inline TSDictionary<K, V> TSDictionaryMake() {
	return [NSDictionary dictionary];
}


#endif
