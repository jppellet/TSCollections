//
//  collection_element_typeinfo.h
//  CppCollectionWrappers
//
//  Created by Jean-Philippe Pellet on 06.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#ifndef _TS_COLLECTION_ELEMENT_TYPEINFO_H
#define _TS_COLLECTION_ELEMENT_TYPEINFO_H


template<typename T>
struct TSCollectionElementTypeInfo {};


#define TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(PublicType, methodPrefix, _BackingTypeAlias)      \
    typedef typename TSCollectionElementTypeInfo<PublicType>::BackingType _BackingTypeAlias;  \
    inline static _BackingTypeAlias methodPrefix##ToBackingType(PublicType elem) {            \
        return TSCollectionElementTypeInfo<PublicType>::toBackingType(elem);                  \
    }                                                                                         \
    inline static PublicType methodPrefix##ToPublicType(_BackingTypeAlias elem) {             \
        return TSCollectionElementTypeInfo<PublicType>::toPublicType(elem);                   \
    }

template<typename T>
static void TSCollectionsElementConformityCheck(id<NSFastEnumeration> collection, NSIndexPath *parentIndexPath, id rootCollection) {
    NSIndexPath *incorrectIndexPath = nil;
    id incorrectElem = nil;
    {
        NSUInteger i = 0;
        for (id elem in collection) {
            NSIndexPath *indexPath = [parentIndexPath indexPathByAddingIndex:i] ?: [NSIndexPath indexPathWithIndex:i];
            if (!TSCollectionElementTypeInfo<T>::conforms(elem, indexPath, rootCollection)) {
                incorrectIndexPath = indexPath;
                incorrectElem = elem;
                break;
            }
            i++;
        }
    }
    if (incorrectIndexPath != nil) {
        NSUInteger numPathItems = incorrectIndexPath.length;
        NSMutableArray *pathItems = [NSMutableArray arrayWithCapacity:numPathItems];
        for (NSUInteger i = 0; i < numPathItems; i++) {
            NSUInteger index = [incorrectIndexPath indexAtPosition:i];
            [pathItems addObject:[NSNumber numberWithUnsignedInteger:index]];
        }
        NSString *details = [NSString stringWithFormat:@"found '%@' of type %@ at index path [%@] not of type %@ in %@",
                             incorrectElem, NSStringFromClass([incorrectElem class]), [pathItems componentsJoinedByString:@", "],
                             NSStringFromClass(TSCollectionElementTypeInfo<T>::backingCollectionActualElementClass()),
                             rootCollection];
        @throw([NSException exceptionWithName:@"TSCollectionsTypeMismatchException" reason:details userInfo:nil]);
    }
}


//
// TSCollectionElementTypeInfo declarations for leaf types
// (e.g., NSString *, NSNumber *, etc.)
//

#define TS_COLLECTIONS_DECLARE_ELEMENT_TYPE_INFO(className)                               \
                                                                                          \
    @class className;                                                                     \
                                                                                          \
    template<>                                                                            \
    struct TSCollectionElementTypeInfo<className *> :                                     \
            public TSTypeConstraintDerivedFrom<className *, NSObject *> {                 \
        typedef className *BackingType;                                                   \
        static Class backingCollectionActualElementClass() {                              \
            return className.class;                                                       \
        }                                                                                 \
        static BOOL conforms(id elem, NSIndexPath *parentIndexPath, id rootCollection) {  \
            return [elem isKindOfClass: className.class];                                 \
        }                                                                                 \
        inline static BackingType toBackingType(className *elem) {                        \
            return elem;                                                                  \
        }                                                                                 \
        inline static className *toPublicType(BackingType elem) {                         \
            return elem;                                                                  \
        }                                                                                 \
    };                                                                                    \
                                                                                          \
    TS_STATIC_ASSERT(sizeof(void *) == sizeof(className *),                               \
        #className " * does not have pointer size");


TS_COLLECTIONS_DECLARE_ELEMENT_TYPE_INFO(NSObject);
TS_COLLECTIONS_DECLARE_ELEMENT_TYPE_INFO(NSString);
TS_COLLECTIONS_DECLARE_ELEMENT_TYPE_INFO(NSNumber);
TS_COLLECTIONS_DECLARE_ELEMENT_TYPE_INFO(NSDate);
TS_COLLECTIONS_DECLARE_ELEMENT_TYPE_INFO(NSData);



//
// TSCollectionElementTypeInfo declarations for collection types
//

#define TS_DECLARE_COLLECTON_TYPE_INFO_1(backingType, interfaceType)                             \
                                                                                               \
    template<typename T>  /* predeclaration of templated container */                          \
    class interfaceType;                                                                       \
                                                                                               \
    template<typename T>                                                                       \
    struct TSCollectionElementTypeInfo<interfaceType<T *>> {                                   \
        typedef backingType *BackingType;                                                      \
        static Class backingCollectionActualElementClass() {                                   \
            return backingType.class;                                                          \
        }                                                                                      \
        static BOOL conforms(id elem, NSIndexPath *parentIndexPath, id rootCollection) {       \
            if (![elem isKindOfClass: backingType.class])                                      \
                return NO;                                                                     \
            backingType *array = elem;                                                         \
            TSCollectionsElementConformityCheck<T *>(array, parentIndexPath, rootCollection);  \
            return YES;                                                                        \
        }                                                                                      \
        inline static BackingType toBackingType(interfaceType<T *> elem) {                     \
            return elem; /* automatic conversion actually occurs here */                       \
        }                                                                                      \
        inline static interfaceType<T *> toPublicType(BackingType elem) {                      \
            return elem; /* automatic conversion actually occurs here */                       \
        }                                                                                      \
    };

TS_DECLARE_COLLECTON_TYPE_INFO_1( NSArray,        TSArray        )
TS_DECLARE_COLLECTON_TYPE_INFO_1( NSMutableArray, TSMutableArray )
TS_DECLARE_COLLECTON_TYPE_INFO_1( NSSet,          TSSet          )
TS_DECLARE_COLLECTON_TYPE_INFO_1( NSMutableSet,   TSMutableSet   )

#undef TS_DECLARE_COLLECTON_TYPE_INFO_1


#define TS_DECLARE_COLLECTON_TYPE_INFO_2(backingType, interfaceType)                           \
                                                                                               \
    template<typename K, typename V>  /* predeclaration of templated container */              \
    class interfaceType;                                                                       \
                                                                                               \
    template<typename K, typename V>                                                           \
    struct TSCollectionElementTypeInfo<interfaceType<K *, V *>> {                              \
        typedef backingType *BackingType;                                                      \
        static Class backingCollectionActualElementClass() {                                   \
            return backingType.class;                                                          \
        }                                                                                      \
        static BOOL conforms(id elem, NSIndexPath *parentIndexPath, id rootCollection) {       \
            if (![elem isKindOfClass: backingType.class])                                      \
                return NO;                                                                     \
            backingType *dictionary = elem;                                                    \
            TSCollectionsElementConformityCheck<K *>(                                          \
                [dictionary allKeys], parentIndexPath, rootCollection);                        \
            TSCollectionsElementConformityCheck<V *>(                                          \
                [dictionary allValues], parentIndexPath, rootCollection);                      \
            return YES;                                                                        \
        }                                                                                      \
        inline static BackingType toBackingType(interfaceType<K *, V *> elem) {                \
            return elem; /* automatic conversion actually occurs here */                       \
        }                                                                                      \
        inline static interfaceType<K *, V *> toPublicType(BackingType elem) {                 \
            return elem; /* automatic conversion actually occurs here */                       \
        }                                                                                      \
    };

TS_DECLARE_COLLECTON_TYPE_INFO_2( NSDictionary,        TSDictionary        )
TS_DECLARE_COLLECTON_TYPE_INFO_2( NSMutableDictionary, TSMutableDictionary )

#undef TS_DECLARE_COLLECTON_TYPE_INFO_2

/*
 * TSTypeConstraintCollectionCovarianceAssignable is used to determine, when collection covariance is allowed,
 * whether a given collection type (ArgType) can be assigned (used in place of)
 * another declared collection type (ReceiverType).
 *
 * Typically, immutable collections are covariant in their element type, while
 * mutable collections are only covariant for the argument of their move constructor
 * and move assignment operator. So these objects below do NOT provide a garantee
 * that an instance of ArgType can always be used to assign an instance of
 * ReceiverType, as sometimes we are not supposed to check for covariance at all.
 *
 * Note that dictionary key are always invariant.
 */

/** A type ArgType can be used for another type ReceiverType if ArgType is a subtype */
template<typename ArgType, typename ReceiverType>
struct TSTypeConstraintCollectionCovarianceAssignable :
    public TSTypeConstraintDerivedFrom<ArgType, ReceiverType> {};

/*
 * Collections "forward" the check to their argument types; additionally, a mutable collection
 * can be assigned to its non-mutable superclass (as Cocoa does).
 */
 
template<typename T1, typename T2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSArray<T1>, TSArray<T2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<T1, T2> {};

template<typename T1, typename T2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSMutableArray<T1>, TSMutableArray<T2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<T1, T2> {};

template<typename T1, typename T2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSMutableArray<T1>, TSArray<T2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<T1, T2> {};

template<typename T1, typename T2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSSet<T1>, TSSet<T2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<T1, T2> {};

template<typename T1, typename T2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSMutableSet<T1>, TSMutableSet<T2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<T1, T2> {};

template<typename T1, typename T2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSMutableSet<T1>, TSSet<T2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<T1, T2> {};

template<typename K, typename V1, typename V2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSDictionary<K, V1>, TSDictionary<K, V2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<V1, V2> {};

template<typename K, typename V1, typename V2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSMutableDictionary<K, V1>, TSMutableDictionary<K, V2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<V1, V2> {};

template<typename K, typename V1, typename V2>
struct TSTypeConstraintCollectionCovarianceAssignable<TSMutableDictionary<K, V1>, TSDictionary<K, V2>> :
    public TSTypeConstraintCollectionCovarianceAssignable<V1, V2> {};

/*
void test();
void test() {
    TSTypeConstraintCollectionCovarianceAssignable<NSNumber *, NSObject *>();
    TSTypeConstraintCollectionCovarianceAssignable<TSMutableDictionary<NSString *, TSMutableSet<NSNumber *>>, TSDictionary<NSString *, TSSet<NSObject *>>>();
}
 */



#endif
