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
 * Min & Max macro definitions with type safety.
 */
#define CANDO_MAX(a,b) \
	({ typeof (a) _a = (a); \
	   typeof (b) _b = (b); \
	   _a > _b ? _a : _b; })

#define CANDO_MIN(a,b) \
	({ typeof (a) _a = (a); \
	   typeof (b) _b = (b); \
	   _a < _b ? _a : _b; })

/*
 * Define typical page size without including
 * limits.h header.
 */
#define CANDO_PAGE_SIZE (1<<12)


/*
 * Retrieves the starting address of the page @ptr resides in.
 */
#define CANDO_PAGE_GET(ptr) \
	((void*)((uintptr_t)ptr & ~(CANDO_PAGE_SIZE-1)))


/*
 * @brief Sets a grouping a pages write-only
 *
 * @param ptr  - Pointer to buffer caller wants write-only
 * @param size - Size of data that needs to be set write-only
 */
#define CANDO_PAGE_SET_WRITE(ptr, size) \
	__extension__ \
	({ \
		int err = -1; \
		void *page = CANDO_PAGE_GET(ptr); \
		err = mprotect(page, size, PROT_WRITE); \
		err; \
	})


/*
 * @brief Sets a grouping a pages read-only
 *
 * @param ptr  - Pointer to buffer caller wants write-only
 * @param size - Size of data that needs to be set write-only
 */
#define CANDO_PAGE_SET_READ(ptr, size) \
	__extension__ \
	({ \
		int err = -1; \
		void *page = CANDO_PAGE_GET(ptr); \
		err = mprotect(page, size, PROT_READ); \
		err; \
	})


/*
 * @brief Create atomic variable type of a
 *        caller defined data type.
 *
 * @param name - Name of atomic type.
 * @param type - Data type of the atomic type.
 */
#define CANDO_ATOMIC_DEF(name, type) \
	typedef _Atomic __typeof__(type) name;

CANDO_ATOMIC_DEF(cando_atomic_int, int);
CANDO_ATOMIC_DEF(cando_atomic_bool, unsigned char);
CANDO_ATOMIC_DEF(cando_atomic_u32, unsigned int);
CANDO_ATOMIC_DEF(cando_atomic_addr, unsigned char *);

#endif /* CANDO_MACROS_H */
