#ifndef CANDO_MM_H
#define CANDO_MM_H

/*
 * This interface was built to force caller to
 * be more consciously concern about virtual
 * heap memory management.
 */

#include "macros.h"

/*
 * Stores information about the cando_mm instance.
 */
struct cando_mm;


/*
 * @brief Returns pointer to an allocated heap memory.
 *        The goal of this is to allocate a large block
 *        of memory once.
 *
 *        Addresses returned from function should not
 *        be used to write to. Writable addresses
 *        are return from a call to cando_mm_sub_alloc(3).
 *
 * @param mm   - Pointer to a struct cando_mm
 * @param size - Size of data caller may allocate. If the
 *               size is greater than the larger block
 *               remapping of memory will occur.
 *
 * @return
 *	on success: Pointer to struct cando_mm
 *	on failure: NULL
 */
struct cando_mm *
cando_mm_alloc (struct cando_mm *mm, const size_t size);


/*
 * @brief Returns pointer to an allocated heap memory.
 *        The goal of this function similar to malloc(3).
 *        From an allocated large block of memory.
 *        Sub-allocate from that larger block.
 *
 *        Addresses returned from function can be
 *        used for writing.
 *
 * @param mm   - Pointer to a struct cando_mm
 * @param size - Size of smaller buffer
 *
 * @return
 * 	on success: Pointer to writable memory
 *	on failure: NULL
 */
void *
cando_mm_sub_alloc (struct cando_mm *mm, const size_t size);


/*
 * @brief Free's the large block of allocated memory created after
 *        cando_mm_alloc(3) call.
 */
void
cando_mm_destroy (struct cando_mm *mm);

#endif /* CANDO_MM_H */
