#ifndef _TS_SET_H
#define _TS_SET_H

#include "TSCollectionsConfig.h"

@class NSSet;

template<typename T>
class TSSet : public TSTraversable<T> {
protected:    
    TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(T, elem, BackingElemType)
    
public:
    
    //
    // Constructors & special members
    //
    
    // Main constructors and destructor
    TSSet(): TSTraversable<T>(nil) {}
    TSSet(NSSet *_set): TSTraversable<T>(_set) {}
    
    // Copy constructor, covariant in T
    template<typename T1>
    TSSet(const TSSet<T1>& that): TSTraversable<T>(that.asNSSet()) {
        TSTypeConstraintDerivedFrom<T1, T>();
    }
    
    // Move constructor, covariant in T
    template<typename T1>
    TSSet(TSSet<T1>&& that): TSTraversable<T>(that.asNSSet()) {
        TSTypeConstraintDerivedFrom<T1, T>();
        reinterpret_cast<TSSet<T>&>(that).traversable = nil;
    }
    
    // Copy & move assignment together, covariant in T
    template<typename T1>
    TSSet<T>& operator=(TSSet<T1> that) {
        TSTypeConstraintDerivedFrom<T1, T>();
        std::swap(this->traversable, reinterpret_cast<TSSet<T>&>(that).traversable);
        return *this;
    }
    
    //
    // Objective-C-like interface
    //

    inline NSString *componentsJoinedByString(NSString *sep) {
        return [set() componentsJoinedByString:sep];
    }
    
    inline BOOL contains(T elem) {
        return [set() containsObject:unwrap(elem)];
    }
    
    inline NSUInteger count() {
        return set().count;
    }
    
//    inline operator NSSet *() const {
//        return set();
//    }
    
    inline NSString *description() {
        return set().description;
    }
	
	inline TSCollectionStringDescription *collectionDescription() {
		return [[TSCollectionStringDescription alloc] initWithDescriptionString:description()];
	}
    
    inline NSSet *asNSSet() const {
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
    
    inline NSSet *set() const {
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
    
TS_STATIC_ASSERT(sizeof(void *) == sizeof(TSSet<NSObject *>), "TSSet does not have pointer size");

//
// TSSet builder functions
//

template<typename T>
inline void TSMutableSetBuilderAppendRecusively(NSMutableSet *set) {
	// nothing
}

template<typename T, typename T0, typename... TS>
inline void TSMutableSetBuilderAppendRecusively(NSMutableSet *set, T0 head, TS... tail) {
	TSTypeConstraintDerivedFrom<T0, T>();
	[set addObject: head];
	TSMutableSetBuilderAppendRecusively<T, TS...>(set, tail...);
}

template<typename T, typename... TS, int N = 1 + sizeof...(TS)>
inline TSSet<T> TSSetMake(T head, TS... tail) {
	NSMutableSet *set = [NSMutableSet setWithCapacity:N];
	TSMutableSetBuilderAppendRecusively<T, T, TS...>(set, head, tail...);
	return [NSSet setWithSet:set];
}

template<typename T>
inline TSSet<T> TSSetMake() {
	return [NSSet set];
}

template<typename T>
inline TSSet<T> TSSetWithSet(TSSet<T> set) {
    return [NSSet setWithSet:set.asNSSet()];
}


#endif