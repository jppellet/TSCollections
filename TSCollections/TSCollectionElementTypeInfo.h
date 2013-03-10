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
class TSCollectionElementTypeInfo {};


#define TS_USING_TYPEINFO_OF_TEMPLATE_PARAM(T, methodPrefix, _PublicTypeAlias, _BackingTypeAlias) \
    typedef typename TSCollectionElementTypeInfo<T>::PublicType _PublicTypeAlias;                 \
    typedef typename TSCollectionElementTypeInfo<T>::BackingType _BackingTypeAlias;               \
    inline static _BackingTypeAlias methodPrefix##ToBackingType(_PublicTypeAlias elem) {          \
        return TSCollectionElementTypeInfo<T>::toBackingType(elem);                               \
    }                                                                                             \
    inline static _PublicTypeAlias methodPrefix##ToPublicType(_BackingTypeAlias elem) {           \
        return TSCollectionElementTypeInfo<T>::toPublicType(elem);                                \
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

#define TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(className)                                 \
                                                                                          \
    @class className;                                                                     \
                                                                                          \
    template<>                                                                            \
    class TSCollectionElementTypeInfo<className *> {                                      \
    public:                                                                               \
        typedef className *PublicType;                                                    \
        typedef className *BackingType;                                                   \
        static Class backingCollectionActualElementClass() {                              \
            return className.class;                                                       \
        }                                                                                 \
        static BOOL conforms(id elem, NSIndexPath *parentIndexPath, id rootCollection) {  \
            return [elem isKindOfClass: className.class];                                 \
        }                                                                                 \
        inline static BackingType toBackingType(PublicType elem) {                        \
            return elem;                                                                  \
        }                                                                                 \
        inline static PublicType toPublicType(BackingType elem) {                         \
            return elem;                                                                  \
        }                                                                                 \
    };

TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(NSObject);
TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(NSString);
TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(NSNumber);
TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(NSDate);
TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO(NSData);



//
// TSCollectionElementTypeInfo declarations for collection types
//

#define TS_DECLARE_COLLECTON_TYPE_INFO(backingType, interfaceType)                             \
                                                                                               \
    template<typename T>  /* predeclaration of templated container */                          \
    class interfaceType;                                                                       \
                                                                                               \
    template<typename T>                                                                       \
    class TSCollectionElementTypeInfo<interfaceType<T *>> {                                    \
    public:                                                                                    \
        typedef interfaceType<T *> PublicType;                                                 \
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
        inline static BackingType toBackingType(PublicType elem) {                             \
            return elem; /* automatic conversion actually occurs here */                       \
        }                                                                                      \
        inline static PublicType toPublicType(BackingType elem) {                              \
            return elem; /* automatic conversion actually occurs here */                       \
        }                                                                                      \
    };

TS_DECLARE_COLLECTON_TYPE_INFO( NSArray,        TSArray        )
TS_DECLARE_COLLECTON_TYPE_INFO( NSMutableArray, TSMutableArray )
TS_DECLARE_COLLECTON_TYPE_INFO( NSSet,          TSSet          )
TS_DECLARE_COLLECTON_TYPE_INFO( NSMutableSet,   TSMutableSet   )


//#undef TS_DECLARE_COLLECTON_ELEMENT_TYPE_INFO
//#undef TS_DECLARE_COLLECTON_TYPE_INFO


#endif
