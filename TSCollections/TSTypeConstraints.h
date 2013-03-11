//
//  TSTypeConstraints.h
//  TSCollections
//
//  Created by Jean-Philippe Pellet on 11.03.13.
//  Copyright (c) 2013 Jean-Philippe Pellet. All rights reserved.
//

#ifndef _TS_TYPE_CONSTRAINTS_H
#define _TS_TYPE_CONSTRAINTS_H

//
// Template type constraints.
// See http://www.stroustrup.com/bs_faq2.html#constraints
//
template<typename Type, typename Super>
struct TSTypeConstraintDerivedFrom {
    static void constraints(Type *p) { Super *pb = p; TS_UNUSED(pb); }
    TSTypeConstraintDerivedFrom() { void (*p)(Type *) = constraints; TS_UNUSED(p); }
};

template<typename T1, typename T2>
struct TSTypeConstraintEqual {
    static void constraints(T1 *t1, T2 *t2) { T1 *t1_ = t2; T2 *t2_ = t1; TS_UNUSED(t1_);; TS_UNUSED(t2_); }
    TSTypeConstraintEqual() { void (*p)(T1 *, T2 *) = constraints; TS_UNUSED(p); }
};

#endif
