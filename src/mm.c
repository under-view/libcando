#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#include "log.h"
#include "macros.h"
#include "mm.h"

/*
 * Global used to keep track of starting address
 * of other addresses returned to caller.
 */
static const void *saddr = NULL;

struct cando_mm_link
{
	size_t               dataSize;
	void                 *data;
	struct cando_mm_link *prev;
	struct cando_mm_link *next;
};


struct cando_mm
{
	struct cando_log_error_struct err;
	size_t                        bufferSize;
	size_t                        dataSize;
	void                          *data;
};


static void *
new_virtual_memory_mapping (struct cando_mm *_mm, const size_t size)
{
	void *data = NULL;

	struct cando_mm *mm = _mm;

	size_t offset = 0, newDataSize = 0, newSize = 0;

	offset = sizeof(struct cando_mm);

	if (mm && mm->data) {
		newSize = mm->dataSize + size;
		newDataSize = mm->bufferSize + size;
	} else {
		newSize = size;
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
		if (mm && mm->data) {
			memcpy(data, mm, mm->bufferSize);
			munmap(mm, mm->bufferSize);
		}

		mm = data;
		saddr = mm->data = (void*)((char*)data)+offset;
	}

	mm->dataSize = newSize;
	mm->bufferSize = newDataSize;

	return mm->data;
}


void *
cando_mm_alloc (void *_mm, const size_t size)
{
	struct cando_mm *ret = NULL, *mm = _mm;

	mm = CANDO_PAGE_GET(saddr);

	if (!_mm) {
		ret = new_virtual_memory_mapping(mm, size);
	} else if (_mm && (mm->dataSize <= size)) {
		ret = new_virtual_memory_mapping(mm, size);
	}

	return ret;
}


void
cando_mm_destroy (void)
{
	struct cando_mm *mm = CANDO_PAGE_GET(saddr);
	munmap((void*)mm, mm->bufferSize);
}
