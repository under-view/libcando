#ifndef CANDO_MACROS_H
#define CANDO_MACROS_H

/*
 * Informs the compiler that you expect a variable
 * to be unused and instructs compiler to not issue
 * a warning on the variable.
 */
#define CANDO_UNUSED __attribute__((unused))

/*
 * "always_inline" instructs GCC to
 * 1. Ignore flag -fno-inline
 * 2. Don't produce external definition of a
 *    function with external linkage.
 * 3. Ignore inlining limits. Use alloca to inline.
 */
#define CANDO_INLINE inline __attribute__((always_inline))
#define CANDO_STATIC_INLINE static inline __attribute__((always_inline))

#endif /* CANDO_MACROS_H */
