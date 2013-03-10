#ifndef _TS_TRAVERSABLE_H
#define _TS_TRAVERSABLE_H

/*@protocol TSHasCount <NSObject>
- (NSUInteger)count;
@end
*/

template<typename T>
class TSTraversable {
protected:
    id<NSFastEnumeration/*, TSHasCount*/> const traversable;
    
    TSTraversable(id<NSFastEnumeration/*, TSHasCount*/> _traversable): traversable(_traversable) {
#if TS_COLLECTIONS_DEBUG
        TSCollectionsElementConformityCheck<T>(traversable, nil, traversable);
#endif
    }
    
public:
    
    typedef BOOL (^Predicate)(T);
    typedef bool (^PredicateAlt)(T);
    
    inline void foreach(void (^f)(T)) {
        for (T elem in traversable)
            f(elem);
    }
    
    inline NSUInteger countWhere(Predicate pred) {
        NSUInteger count = 0;
        for (T elem in traversable)
            if (pred(elem))
                count++;
        return count;
    }
    
    inline NSUInteger countWhere(PredicateAlt pred) {
        return countIf(reinterpret_cast<Predicate>(pred));
    }
    
    inline T find(Predicate pred) {
        for (T elem in traversable)
            if (pred(elem))
                return elem;
        return nil;
    }
    
    inline T find(PredicateAlt pred) {
        return find(reinterpret_cast<Predicate>(pred));
    }
    
    inline BOOL exists(Predicate pred) {
        for (T elem in traversable)
            if (pred(elem))
                return YES;
        return NO;

    }
    
    inline BOOL exists(PredicateAlt pred) {
        return exists(reinterpret_cast<Predicate>(pred));
        
    }
    
    inline BOOL forall(Predicate pred) {
        for (T elem in traversable)
            if (!pred(elem))
                return NO;
        return YES;
    }

    inline BOOL forall(PredicateAlt pred) {
        return forall(reinterpret_cast<Predicate>(pred));
    }
    
};


#endif
