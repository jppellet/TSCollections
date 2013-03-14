//
//  collections.h
//  CppCollectionWrappers
//
//  Created by Jean-Philippe Pellet on 06.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#ifndef _TS_COLLECTIONS_CONFIG_H
#define _TS_COLLECTIONS_CONFIG_H

#if DEBUG
#  define TS_COLLECTIONS_DEBUG 1
#elif
#  define TS_COLLECTIONS_DEBUG 0
#endif

#if __has_feature(cxx_static_assert)
#  define TS_STATIC_ASSERT(...) static_assert(__VA_ARGS__)
#else
#  warning static_assert is off
#  define TS_STATIC_ASSERT(...)
#endif

#import <Foundation/Foundation.h>
//#include <initializer_list> // initializer_list cannot hold ARC-managed NSObjects

TS_STATIC_ASSERT(sizeof(BOOL) == sizeof(bool), "BOOL cannot be reinterpreted as bool");

#define TS_UNUSED(expr) do { (void)(expr); } while (0)

#include "TSTypeConstraints.h"
#include "TSCollectionElementTypeInfo.h"
#include "TSPreprocessor.h"

#include <utility>


#endif
