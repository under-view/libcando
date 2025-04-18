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
	struct cando_mm_link *next;
	void                 *data;
};


struct cando_mm
{
	struct cando_log_error_struct err;
	size_t                        bufferSize;
	size_t                        dataSize;
	size_t                        abSize;
	struct cando_mm_link          slink;
};


static void *
new_virtual_memory_mapping (struct cando_mm *_mm, const size_t size)
{
	void *data = NULL;

	struct cando_mm *mm = _mm;

	size_t offset = 0, newDataSize = 0;

	offset = sizeof(struct cando_mm);

	newDataSize = (mm) ? mm->bufferSize + size : offset + size;

	data = mmap(NULL, newDataSize,
		    PROT_READ|PROT_WRITE,
		    MAP_PRIVATE|MAP_ANONYMOUS,
		    -1, 0);
	if (data == (void*)-1) {
		cando_log_error("mmap: %s\n", strerror(errno));
		return NULL;
	} else {
		memset(data, 0, newDataSize);

		/*
		 * This is okay because the goal would be
		 * to allocate as much memory as possible
		 * early on. So, that remapping can be
		 * avoided.
		 */
		if (mm) {
			memcpy(data, mm, mm->bufferSize);
			munmap(mm, mm->bufferSize);
		}

		mm = data;
		mm->bufferSize = newDataSize;
		mm->dataSize = mm->abSize = newDataSize - offset;
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
cando_mm_sub_alloc (struct cando_mm *mm, const size_t size)
{
	size_t bufferSize = 0;

	struct cando_mm_link *link = NULL;

	if (!mm) {
		cando_log_error("Incorrect data passed\n");
		return NULL;
	}

	if (mm->abSize <= size) {
		cando_log_set_error(mm, CANDO_LOG_ERR_UNCOMMON,
		                    "Cannot allocate %lu bytes only %lu bytes left.",
		                    size, mm->abSize);
		return NULL;
	}

	link = &(mm->slink);

	while (link && link->next)
		link = link->next;

	bufferSize = sizeof(struct cando_mm_link) + size;

	link->dataSize = size;
	link->data = (void*)((char*)link + sizeof(struct cando_mm_link));
	link->next = (struct cando_mm_link*)(((char*)link) + bufferSize);

	mm->abSize -= bufferSize;

	return link->data;
}


void
cando_mm_destroy (struct cando_mm *_mm)
{
	struct cando_mm *mm = _mm;

	if (!mm)
		return;

	munmap(mm, mm->bufferSize);
}
