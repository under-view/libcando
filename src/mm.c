#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#include "log.h"
#include "macros.h"
#include "mm.h"

struct cando_mm_link
{
	size_t               dataSize;
	struct cando_mm_link *prev;
	struct cando_mm_link *next;
	void                 *data;
};


struct cando_mm
{
	struct cando_log_error_struct err;
	size_t                        bufferSize;
	size_t                        dataSize;
	struct cando_mm_link          slink;
};


static void *
new_virtual_memory_mapping (struct cando_mm *_mm, const size_t size)
{
	void *data = NULL;

	struct cando_mm *mm = _mm;

	size_t offset = 0, newDataSize = 0;

	offset = sizeof(struct cando_mm);

	if (mm && mm->slink.data) {
		newDataSize = mm->bufferSize + size;
	} else {
		newDataSize = offset + size;
	}

	data = mmap(NULL, newDataSize,
		    PROT_READ|PROT_WRITE,
		    MAP_PRIVATE|MAP_ANONYMOUS,
		    -1, 0);
	if (data == (void*)-1) {
		cando_log_error("mmap: %s\n", strerror(errno));
		return NULL;
	} else {
		/*
		 * This is okay because the goal would be
		 * to allocate as much memory as possible
		 * early on. So, that remapping can be
		 * avoided.
		 */
		if (mm && mm->slink.data) {
			memcpy(data, mm, mm->bufferSize);
			munmap(mm, mm->bufferSize);
		}

		mm = data;
		mm->bufferSize = newDataSize;
		mm->dataSize = newDataSize - offset;
		mm->slink.dataSize = size;
		mm->slink.data = (void*)((char*)data)+offset;
	}

	return mm;
}


struct cando_mm *
cando_mm_alloc (struct cando_mm *mm, const size_t size)
{
	struct cando_mm *ret = NULL;

	if (!mm) {
		ret = new_virtual_memory_mapping(mm, size);
	} else if (mm && (mm->dataSize <= size)) {
		ret = new_virtual_memory_mapping(mm, size);
	}

	return ret;
}


void *
cando_mm_sub_alloc (struct cando_mm *mm, const size_t size CANDO_UNUSED)
{
	void *data = NULL;

	if (!mm) {
		cando_log_error("Incorrect data passed\n");
		return NULL;
	}

	// For now
	data = mm->slink.data;

	return data;
}


void
cando_mm_destroy (struct cando_mm *_mm)
{
	struct cando_mm *mm = _mm;

	if (!mm)
		return;

	munmap(mm, mm->bufferSize);
}
