#ifndef CANDO_MM_H
#define CANDO_MM_H

#include "macros.h"

/*
 * Stores information about the cando_mm instance.
 * cando_mm -> Cando Memory Manage
 */
struct cando_mm;

struct cando_mm *
cando_mm_alloc (struct cando_mm *_mm, const size_t size);

void
cando_mm_destroy (struct cando_mm *mm);

#endif /* CANDO_MM_H */
