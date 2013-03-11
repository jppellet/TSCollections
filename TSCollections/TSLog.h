//
//  TSLog.h
//  TSCollections
//
//  Created by Jean-Philippe Pellet on 11.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#ifndef _TS_LOG_H
#define _TS_LOG_H

#include "TSPreprocessor.h"

/**
 * By default, all parameters are left untouched with this
 * inline identity function.
 */
template<typename A>
inline A TSAdaptLogArgument(A a) {
   return a;
};

/*
 * The next couple of defines use partial template specialization
 * to define a version of TSAdaptLogArgument that automatically calls
 * description() on all TSCollections when logging.
 */

#define TS_DECLARE_LOG_AUTO_DESCRIPTION_1(CollectionType)             \
    template<typename A>                                              \
    inline TSCollectionStringDescription *TSAdaptLogArgument(CollectionType<A> coll) {     \
        return coll.collectionDescription();                                    \
    }

#define TS_DECLARE_LOG_AUTO_DESCRIPTION_2(CollectionType)             \
    template<typename A, typename B>                                  \
    inline TSCollectionStringDescription *TSAdaptLogArgument(CollectionType<A, B> coll) {  \
        return coll.collectionDescription();                                    \
    }

TS_PP_FOREACH(                         \
    TS_DECLARE_LOG_AUTO_DESCRIPTION_1, \
			  TSArray,                 \
			  TSMutableArray,          \
			  TSSet,                   \
			  TSMutableSet)

TS_PP_FOREACH(                         \
    TS_DECLARE_LOG_AUTO_DESCRIPTION_2, \
			  TSDictionary,            \
			  TSMutableDictionary)

#undef TS_DECLARE_LOG_AUTO_DESCRIPTION_1
#undef TS_DECLARE_LOG_AUTO_DESCRIPTION_2


#define TS_ADAPT_LOG_ARG(X) , TSAdaptLogArgument(X)

/**
 * Just calls NSLog, automatically calling description() on TSCollection parameters.
 */
#define TSLog(format, ...)  NSLog(format TS_PP_FOREACH(TS_ADAPT_LOG_ARG, ##__VA_ARGS__))


#endif
