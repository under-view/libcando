#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#include "log.h"
#include "macros.h"
#include "mm.h"

struct cando_mm_link
{
	size_t          dataSize;
	void            *data;
	struct cando_mm *next;
	struct cando_mm *prev;
};


struct cando_mm
{
	struct cando_log_error_struct err;
	size_t                        bufferSize;
	struct cando_mm_link          *saddr;
};


static struct cando_mm *
new_virtual_memory_mapping (struct cando_mm *_mm, const size_t size)
{
	int err = -1;

	void *data = NULL;

	struct cando_mm *mm = _mm;

	size_t offset = 0, newDataSize = 0;

	offset = sizeof(struct cando_log_error_struct) + sizeof(size_t);

	if (mm && mm->saddr) {
		newDataSize = mm->bufferSize + size;
	} else {
		newDataSize = offset + size;
	}

	data = mmap(NULL, newDataSize,
		    PROT_READ|PROT_WRITE,
		    MAP_PRIVATE|MAP_ANONYMOUS,
		    -1, 0);
	if (data == (void*)-1) {
		cando_log_error("mremap: %s\n", strerror(errno));
		return NULL;
	} else {
		/*
		 * This is okay because the goal would be
		 * to allocate as much memory as possible
		 * early on. So, that remapping can be
		 * avoided.
		 */
		if (mm && mm->saddr) {
			memcpy(data, mm, mm->bufferSize);
			munmap(mm, mm->bufferSize);
		}

		mm = data;
		mm->saddr = (void*)((char*)data)+offset;
	}

	mm->bufferSize = newDataSize;
	err = CANDO_PAGE_SET_READ(mm, mm->bufferSize);
	if (err == -1) {
		cando_log_set_error(mm, errno, "mprotect: %s\n", strerror(errno));
		return NULL;
	}

	return mm;
}


struct cando_mm *
cando_mm_alloc (struct cando_mm *_mm, const size_t size)
{
	void *ret = NULL;

	struct cando_mm *mm = _mm;

	if (!mm || (mm->bufferSize <= size))
		ret = new_virtual_memory_mapping(mm, size);

	return ret;
}


void
cando_mm_destroy (struct cando_mm *mm)
{
	if (!mm)
		return;

	munmap(mm, mm->bufferSize);
}
