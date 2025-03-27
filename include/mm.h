#ifndef CANDO_MM_H
#define CANDO_MM_H

#include "macros.h"

void *
cando_mm_alloc (void *mm, const size_t size);

void
cando_mm_destroy (void);

#endif /* CANDO_MM_H */
