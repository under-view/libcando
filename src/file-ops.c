#define _GNU_SOURCE 1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include "log.h"
#include "file-ops.h"

#define FILE_NAME_LEN_MAX (1<<20)


/*
 * @brief struct defining cando_file_ops instance
 *
 * @member fd          - File descriptor to open file.
 * @member fname       - String representing the file name.
 * @member pipefds     - File descriptors associated with an open pipe.
 * @member data        - Pointer to mmap(2) file data.
 * @member dataSize    - Total size of the file.
 * @member retData     - Pointer to data returned to caller.
 * @member retDataSize - Size of data returned to caller.
 */
struct cando_file_ops
{
	int    fd;
	char   *fname;
	int    pipefds[2];
	void   *data;
	size_t dataSize;
	void   *retData;
	size_t retDataSize;
};


/********************************************
 * Start of cando_file_ops_create functions *
 ********************************************/

struct cando_file_ops *
cando_file_ops_create (const void *_fileCreateInfo)
{
	int ret = -1;

	struct stat fstats;

	struct cando_file_ops *flops = NULL;

	const struct cando_file_ops_create_info *fileCreateInfo = _fileCreateInfo;

	if (!fileCreateInfo) {
		return NULL;
	}

	flops = calloc(1, sizeof(struct cando_file_ops));
	if (!flops) {
		return NULL;
	}

	if (fileCreateInfo->fileName) {
		/* Check if file exist */
		ret = stat(fileCreateInfo->fileName, &fstats);

		flops->fname = strndup(fileCreateInfo->fileName, FILE_NAME_LEN_MAX);
		if (!(flops->fname)) {
			cando_file_ops_destroy(flops);
			return NULL;
		}

		flops->fd = open(flops->fname, O_CREAT|O_RDWR, 0644);
		if ((flops->fd) == -1) {
			cando_file_ops_destroy(flops);
			return NULL;
		}

		/*
		 * If file exists and caller defined dataSize set to 0.
		 * 	- Then set internal dataSize to equal the size of the file.
		 * 	- Else set internal dataSize to equal the caller defined size.
		 */
		flops->dataSize = (!ret && !(fileCreateInfo->dataSize)) ? \
				  (unsigned long int) fstats.st_size : \
				  fileCreateInfo->dataSize;
	}

	if (fileCreateInfo->createPipe) {
		ret = pipe(flops->pipefds);
		if (ret == -1) {
			cando_file_ops_destroy(flops);
			return NULL;
		}
	} else {
		ret = cando_file_ops_truncate_file(flops, flops->dataSize);
		if (ret < 0 && flops->dataSize) {
			cando_file_ops_destroy(flops);
			return NULL;
		}

		flops->data = mmap(NULL,
				   flops->dataSize,
				   PROT_READ,
				   MAP_SHARED,
				   flops->fd,
				   fileCreateInfo->offset);
		if (flops->data == (void*)-1 && flops->dataSize) {
			cando_file_ops_destroy(flops);
			return NULL;
		}
	}

	return flops;
}

/******************************************
 * End of cando_file_ops_create functions *
 ******************************************/


/***************************************************
 * Start of cando_file_ops_truncate_file functions *
 ***************************************************/

int
cando_file_ops_truncate_file (struct cando_file_ops *flops,
                              const unsigned long int dataSize)
{
	int ret = -1;

	if (!flops || dataSize == 0)
		return -1;

	ret = ftruncate64(flops->fd, dataSize);
	if (ret == -1) {
		return -errno;
	}

	return 0;
}

/*************************************************
 * End of cando_file_ops_truncate_file functions *
 *************************************************/


/*****************************************
 * Start of cando_file_ops_get functions *
 *****************************************/

const void *
cando_file_ops_get_data (struct cando_file_ops *flops,
                         const unsigned long int offset)
{
	if (!flops || \
            !(flops->data))
	{
		return NULL;
	}

	return flops->data + offset;
}

/***************************************
 * End of cando_file_ops_get functions *
 ***************************************/


/*****************************************
 * Start of cando_file_ops_set functions *
 *****************************************/

int
cando_file_ops_set_data (struct cando_file_ops *flops,
                         const void *_fileInfo)
{
	int ret = -1;

	const struct cando_file_ops_set_data_info *fileInfo = _fileInfo;

	if (!flops || \
            !(flops->data) || \
	    !fileInfo || \
	    !(fileInfo->data))
	{
		return -1;
	}

	ret = mprotect(flops->data, fileInfo->dataSize, PROT_WRITE);
	if (ret == -1) {
		return -1;
	}

	memcpy(flops->data + fileInfo->offset, fileInfo->data, fileInfo->dataSize);

	ret = mprotect(flops->data, fileInfo->dataSize, PROT_READ);
	if (ret == -1) {
		return -1;
	}

	return 0;
}

/***************************************
 * End of cando_file_ops_set functions *
 ***************************************/


/*********************************************
 * Start of cando_file_ops_destroy functions *
 *********************************************/

void
cando_file_ops_destroy (struct cando_file_ops *flops)
{
	if (!flops)
		return;

	munmap(flops->data, flops->dataSize);
	close(flops->pipefds[0]);
	close(flops->pipefds[1]);
	close(flops->fd);
	free(flops->retData);
	free(flops->fname);
	free(flops);
}

/*******************************************
 * End of cando_file_ops_destroy functions *
 *******************************************/
