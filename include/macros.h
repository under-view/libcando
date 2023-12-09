#ifndef HANDY_MACROS_H
#define HANDY_MACROS_H

/*
 * Informs the compiler that you expect a variable
 * to be unused and instructs compiler to not issue
 * a warning on the variable.
 */
#define LIBHANDY_UNUSED __attribute__((unused))

/*
 * "always_inline" instructs GCC to
 * 1. Ignore flag -fno-inline
 * 2. Don't produce external definition of a
 *    function with external linkage.
 * 3. Ignore inlining limits. Use alloca to inline.
 */
#define LIBHANDY_INLINE inline __attribute__((always_inline))
#define LIBHANDY_INLINE_STATIC static inline __attribute__((always_inline))

#endif /* HANDY_MACROS_H */
