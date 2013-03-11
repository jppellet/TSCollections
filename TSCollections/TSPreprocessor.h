#ifndef _TS_PREPROCESSOR_H
#define _TS_PREPROCESSOR_H


/**
 * Used to get the nth element of a list of parameters, where n is implicitly
 * the number of elements in the parent macro's variadic argument list. This works
 * for lists of up to 32 arguments (including 0).
 *
 * Typical usage is:
 * #define SOME_MACRO(...) TS_GET_NTH_VARIADIC(_, ##__VA_ARGS__, <if n=32>, <if n=31>, ..., <if n=0>)
 *
 * It means that all 33 values (one for each number of argument from 0 to 32)
 * have to be supplied. The initial _ is needed together with the GCC extension
 * '##__VA_ARGS__' to take care of the 0-element case.
 *
 * The returned element can itself be a macro name that can be further expanded
 * or, e.g., a number indicating the length of the variadic list (see the "num arg"
 * example).
 */
#define TS_PP_GET_NTH_VARIADIC_32(_0_DUMMY,  \
     _1, _2, _3, _4, _5, _6, _7, _8,         \
     _9,_10,_11,_12,_13,_14,_15,_16,         \
    _17,_18,_19,_20,_21,_22,_23,_24,         \
    _25,_26,_27,_28,_29,_30,_31,_32, NAME, ...) NAME 

/**
 * This returns the number of arguments in the argument list.
 * Works with up to 32 arguments.
 */
#define TS_PP_NUM_ARGS(...) TS_PP_GET_NTH_VARIADIC_32(_, ##__VA_ARGS__,  \
    32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,                     \
    16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/**
 * Applies the macro name passed as first argument to each of the later arguments
 * passed as varargs. This works up to 32 varargs. The helper macros are are to
 * force expansion of the intermediary ones.
 */
#define TS_PP_FOREACH(action, ...)   TS_PP_FOREACH_IMPL(TS_PP_FOREACH_, TS_PP_NUM_ARGS(__VA_ARGS__), action, __VA_ARGS__)
#define TS_PP_FOREACH_IMPL(baseMacroName, n, action, ...)   TS_PP_FOREACH_IMPL_(baseMacroName, n, action, __VA_ARGS__)
#define TS_PP_FOREACH_IMPL_(baseMacroName, n, action, ...)  baseMacroName ## n(action, __VA_ARGS__)

/**
 * Helper macros needed to iterate through at most 32 elements.
 */
#define TS_PP_FOREACH_0(...) 
#define TS_PP_FOREACH_1(WHAT, X)       WHAT(X) 
#define TS_PP_FOREACH_2(WHAT, X, ...)  WHAT(X)TS_PP_FOREACH_1(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_3(WHAT, X, ...)  WHAT(X)TS_PP_FOREACH_2(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_4(WHAT, X, ...)  WHAT(X)TS_PP_FOREACH_3(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_5(WHAT, X, ...)  WHAT(X)TS_PP_FOREACH_4(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_6(WHAT, X, ...)  WHAT(X)TS_PP_FOREACH_5(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_7(WHAT, X, ...)  WHAT(X)TS_PP_FOREACH_6(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_8(WHAT, X, ...)  WHAT(X)TS_PP_FOREACH_7(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_9(WHAT, X, ...)  WHAT(X)TS_PP_FOREACH_8(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_10(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_9(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_11(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_10(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_12(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_11(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_13(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_12(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_14(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_13(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_15(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_14(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_16(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_15(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_17(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_16(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_18(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_17(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_19(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_18(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_20(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_19(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_21(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_20(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_22(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_21(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_23(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_22(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_24(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_23(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_25(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_24(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_26(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_25(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_27(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_26(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_28(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_27(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_29(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_28(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_30(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_29(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_31(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_30(WHAT, __VA_ARGS__)
#define TS_PP_FOREACH_32(WHAT, X, ...) WHAT(X)TS_PP_FOREACH_31(WHAT, __VA_ARGS__)


#endif