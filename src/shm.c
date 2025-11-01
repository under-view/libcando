#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "shm.h"

#define SHM_FILE_NAME_MAX (1<<5)


/*
 * @brief Structure defining the cando_shm instance.
 *
 * @member err            - Stores information about the error that occured
 *                          for the given instance and may later be retrieved
 *                          by caller.
 * @member free           - If structure allocated with calloc(3) member will be
 *                          set to true so that, we know to call free(3) when
 *                          destroying the instance.
 * @member fd             - Open file descriptor to POSIX shared memory.
 * @member shm_file       - Name of the POSIX shared memory file starting with '/'.
 * @member data           - Pointer to mmap(2) map'd shared memory data.
 * @member data_sz        - Total size of the shared memory region mapped with mmap(2).
 *                          to read semaphores.
 */
struct cando_shm
{
	struct cando_log_error_struct err;
	bool                          free;
	int                           fd;
	char                          shm_file[SHM_FILE_NAME_MAX];
	void                          *data;
	size_t                        data_sz;
};


/***************************************
 * Start of cando_shm_create functions *
 ***************************************/

static int
p_shm_create (struct cando_shm *shm,
              const struct cando_shm_create_info *shm_info)
{
	int err = -1, len;

	if (!(shm_info->shm_size)) {
		cando_log_set_error(shm, CANDO_LOG_ERR_UNCOMMON,
		                    "Shared memory size must not be zero",
		                    shm_info->shm_size);
		return -1;
	}

	if (shm_info->shm_file[0] != '/') {
		cando_log_set_error(shm, CANDO_LOG_ERR_UNCOMMON,
		                    "Shared memory file name '%s' doesn't start with '/'",
		                    shm_info->shm_file);
		return -1;
	}

	len = strnlen(shm_info->shm_file, SHM_FILE_NAME_MAX+32);
	if (len >= SHM_FILE_NAME_MAX) {
		cando_log_set_error(shm, CANDO_LOG_ERR_UNCOMMON,
		                    "Shared memory '%s' name length to long",
		                    shm_info->shm_file);
		return -1;
	}

	shm->fd = shm_open(shm_info->shm_file, O_RDWR|O_CREAT, 0644);
	if (shm->fd == -1) {
		cando_log_set_error(shm, errno, "shm_open: %s", strerror(errno));
		return -1;
	}

	shm->data_sz = shm_info->shm_size;
	err = ftruncate(shm->fd, shm->data_sz);
	if (err == -1) {
		cando_log_set_error(shm, errno, "ftruncate: %s", strerror(errno));
		return -1;
	}

	shm->data = mmap(NULL, shm->data_sz,
	                 PROT_READ|PROT_WRITE,
	                 MAP_SHARED, shm->fd, 0);
	if (err == -1) {
		cando_log_set_error(shm, errno, "mmap: %s", strerror(errno));
		return -1;
	}

	return 0;
}


struct cando_shm *
cando_shm_create (struct cando_shm *p_shm,
                  const void *p_shm_info)
{
	int err = -1;

	struct cando_shm *shm = p_shm;

	const struct cando_shm_create_info *shm_info = p_shm_info;

	if (!shm) {
		shm = calloc(1, sizeof(struct cando_shm));
		if (!shm) {
			cando_log_error("calloc: %s\n", strerror(errno));
			return NULL;
		}

		shm->free = true;
	}

	err = p_shm_create(shm, shm_info);
	if (err == -1) {
		cando_log_error("%s\n", cando_log_get_error(shm));
		cando_shm_destroy(shm);
		return NULL;
	}

	return shm;
}

/*************************************
 * End of cando_shm_create functions *
 *************************************/


/*************************************
 * Start of cando_shm_data functions *
 *************************************/

int
cando_shm_data_read (struct cando_shm *shm,
                     const void *p_shm_info)
{
	void *shm_data = NULL;

	const struct cando_shm_data_info *shm_info = p_shm_info;

	if (!shm)
		return -1;

	if (!shm_info || \
	    !(shm_info->data))
	{
		cando_log_set_error(shm, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	shm_data = ((char*)shm->data);
	memcpy(shm_info->data, shm_data, shm_info->size);
	memset(shm_data, 0, shm_info->size);

	return 0;
}


int
cando_shm_data_write (struct cando_shm *shm,
                      const void *p_shm_info)
{
	const struct cando_shm_data_info *shm_info = p_shm_info;

	if (!shm)
		return -1;

	if (!shm_info || \
	    !(shm_info->data))
	{
		cando_log_set_error(shm, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	memcpy(((char*)shm->data),
	       shm_info->data, shm_info->size);

	return 0;
}

/***********************************
 * End of cando_shm_data functions *
 ***********************************/


/************************************
 * Start of cando_shm_get functions *
 ************************************/

int
cando_shm_get_fd (struct cando_shm *shm)
{
	if (!shm)
		return -1;

	return shm->fd;
}


void *
cando_shm_get_data (struct cando_shm *shm)
{
	if (!shm)
		return NULL;

	return shm->data;
}


size_t
cando_shm_get_data_size (struct cando_shm *shm)
{
	if (!shm)
		return -1;

	return shm->data_sz;
}

/************************************
 * Start of cando_shm_get functions *
 ************************************/


/****************************************
 * Start of cando_shm_destroy functions *
 ****************************************/

void
cando_shm_destroy (struct cando_shm *shm)
{
	if (!shm)
		return;

	munmap(shm->data, shm->data_sz);

	if (shm->free) {
		free(shm);
	} else {
		memset(shm, 0, sizeof(struct cando_shm));
	}
}

/****************************************
 * Start of cando_shm_destroy functions *
 ****************************************/


/*************************************************
 * Start of non struct cando_shm param functions *
 *************************************************/

int
cando_shm_get_sizeof (void)
{
	return sizeof(struct cando_shm);
}

/***********************************************
 * End of non struct cando_shm param functions *
 ***********************************************/
