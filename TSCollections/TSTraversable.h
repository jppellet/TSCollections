#ifndef _TS_TRAVERSABLE_H
#define _TS_TRAVERSABLE_H

#import "TSCollectionStringDescription.h"

/*@protocol TSHasCount <NSObject>
- (NSUInteger)count;
@end
*/

template<typename T>
class TSTraversable {
protected:
    id<NSFastEnumeration/*, TSHasCount*/> traversable;
    
    TSTraversable(id<NSFastEnumeration/*, TSHasCount*/> _traversable): traversable(_traversable) {
#if TS_COLLECTIONS_DEBUG
        TSCollectionsElementConformityCheck<T>(traversable, nil, traversable);
#endif
    }
    
	TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(T, elem, BackingElemType)
	
public:
	    
    typedef BOOL (^Predicate)(T);
    typedef bool (^PredicateAlt)(T);
    
    inline void foreach(void (^f)(T)) {
        for (BackingElemType elem in traversable)
            f(elemToPublicType(elem));
    }
    
    inline void foreach(void (^f)(T, NSUInteger)) {
		NSUInteger index = 0;
        for (BackingElemType elem in traversable) {
            f(elemToPublicType(elem), index);
			index++;
		}
    }
    
    inline NSUInteger countWhere(Predicate pred) {
        NSUInteger count = 0;
        for (BackingElemType elem in traversable)
            if (pred(elemToPublicType(elem)))
                count++;
        return count;
    }
    
    inline NSUInteger countWhere(PredicateAlt pred) {
        return countIf(reinterpret_cast<Predicate>(pred));
    }
    
    inline T find(Predicate pred) {
        for (BackingElemType elem in traversable) {
			auto publicElem = elemToPublicType(elem);
            if (pred(publicElem))
                return publicElem;
		}
        return nil;
    }
    
    inline T find(PredicateAlt pred) {
        return find(reinterpret_cast<Predicate>(pred));
    }
    
    inline BOOL exists(Predicate pred) {
        for (BackingElemType elem in traversable)
            if (pred(elemToPublicType(elem)))
                return YES;
        return NO;

    }
    
    inline BOOL exists(PredicateAlt pred) {
        return exists(reinterpret_cast<Predicate>(pred));
        
    }
    
    inline BOOL forall(Predicate pred) {
        for (BackingElemType elem in traversable)
            if (!pred(elemToPublicType(elem)))
                return NO;
        return YES;
    }

    inline BOOL forall(PredicateAlt pred) {
        return forall(reinterpret_cast<Predicate>(pred));
    }
    
};


#endif
