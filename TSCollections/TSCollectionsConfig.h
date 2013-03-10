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

// TODO static assert: sizeof(BOOL) == sizeof(bool)

#import <Foundation/Foundation.h>

#define TS_UNUSED(expr) do { (void)(expr); } while (0)

#include "TSCollectionElementTypeInfo.h"

//
// Template constraints.
// See http://www.stroustrup.com/bs_faq2.html#constraints
//
template<typename Type, typename Super>
struct TSConstraintDerivedFrom {
    static void constraints(Type *p) { Super *pb = p; TS_UNUSED(pb); }
    TSConstraintDerivedFrom() { void (*p)(Type *) = constraints; TS_UNUSED(p); }
};


#endif
