#ifndef HANDY_MACROS_H
#define HANDY_MACROS_H

/*
 * Informs the compiler that you expect a variable
 * to be unused and instructs compiler to not issue
 * a warning on the variable.
 */
#define HANDY_UNUSED __attribute__((unused))

/*
 * "always_inline" instructs GCC to
 * 1. Ignore flag -fno-inline
 * 2. Don't produce external definition of a
 *    function with external linkage.
 * 3. Ignore inlining limits. Use alloca to inline.
 */
#define HANDY_INLINE inline __attribute__((always_inline))
#define HANDY_STATIC_INLINE static inline __attribute__((always_inline))

#endif /* HANDY_MACROS_H */
