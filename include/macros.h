#ifndef CANDO_MACROS_H
#define CANDO_MACROS_H

#include <stdint.h>   /* For uintptr_t */
#include <sys/mman.h> /* For mprotect(2) */

/* Prevent c++ name mangling */
#ifdef __cplusplus
#define CANDO_API extern "C"
#else
#define CANDO_API
#endif

/*
 * Define typical page size without including
 * limits.h header.
 */
#define CANDO_PAGE_SIZE (1<<12)

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

/*
 * Retrieves the starting address of the page @ptr resides in.
 */
#define CANDO_GET_PAGE(ptr) \
	((void*)(((uintptr_t)ptr/CANDO_PAGE_SIZE)*CANDO_PAGE_SIZE))


/*
 * @brief Sets a grouping a pages write-only
 *
 * @param ptr  - Pointer to data caller wants write-only
 * @param size - Size of data that needs to be set write-only
 */
#define CANDO_SET_PAGE_WRITE(ptr, size) \
	__extension__ \
	({ \
		int err = -1; \
		void *page = CANDO_GET_PAGE(ptr); \
		err = mprotect(page, size, PROT_WRITE); \
		err; \
	})


/*
 * @brief Sets a grouping a pages read-only
 *
 * @param ptr  - Pointer to data caller wants write-only
 * @param size - Size of data that needs to be set write-only
 */
#define CANDO_SET_PAGE_READ(ptr, size) \
	__extension__ \
	({ \
		int err = -1; \
		void *page = CANDO_GET_PAGE(ptr); \
		err = mprotect(page, size, PROT_READ); \
		err; \
	})

#endif /* CANDO_MACROS_H */
