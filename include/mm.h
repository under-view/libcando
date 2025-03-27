#ifndef CANDO_MM_H
#define CANDO_MM_H

#include "macros.h"

/*
 * This interface was built to force caller to be
 * more consciously concern about heap memory management.
 */

/*
 * @brief Returns pointer to an allocated heap memory.
 *        The goal of this function similar to malloc(3).
 *        is to allocate a large block of memory once.
 *        Then sub-allocate from that larger block.
 *
 * @param size - Size of data caller may allocate. If the
 *               size is greater than the larger block
 *               remapping of memory will occur.
 *
 * @return
 *	on success: Pointer to allocated heap memory
 *	on failure: NULL
 */
void *
cando_mm_alloc (const size_t size);


/*
 * @brief Free's the large block of allocated memory created after
 *        cando_mm_alloc(3) call.
 */
void
cando_mm_destroy (void);

#endif /* CANDO_MM_H */
