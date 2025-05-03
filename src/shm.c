#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

#include <semaphore.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "shm.h"

#define SHM_FILE_NAME_MAX (1<<5)
#define SEM_FILE_NAME_SUFFIX_MAX 8
#define SEM_FILE_NAME_MAX (1<<5)+SEM_FILE_NAME_SUFFIX_MAX
#define SEM_COUNT_MAX (1<<5)


/*
 * @brief Structure defining the cando_sem (cando sempahore) intance.
 *
 * @member read_sem      - Pointer to POSIX semaphore used to synchronize reads.
 * @member read_sem_file - Name of the POSIX semaphore used to synchronize reads.
 */
struct cando_sem
{
	sem_t *read_sem;
	char  read_sem_file[SEM_FILE_NAME_MAX];
};


/*
 * @brief Structure defining the cando_shm instance.
 *
 * @member err            - Stores information about the error that occured
 *                          for the given instance and may later be retrieved
 *                          by caller.
 * @member free           - If structure allocated with calloc(3) member will be
 *                          set to true so that, we know to call free(3) when
 * @member fd             - Open file descriptor to POSIX shared memory.
 * @member shm_file       - Name of the POSIX shared memory file starting with '/'.
 * @member data_sz        - Total size of the shared memory region mapped with mmap(2).
 * @member data           - Pointer to mmap(2) shared memory data.
 * @member write_sem      - Pointer to POSIX semaphore used to syncronize writes.
 * @member write_sem_file - Name of the POSIX semaphore used to synchronize writes.
 * @member sem_count      - Amount of semaphores in @sems.
 * @member sems           - Array of pointers to read semaphores.
 */
struct cando_shm
{
	struct cando_log_error_struct err;
	bool                          free;
	int                           fd;
	char                          shm_file[SHM_FILE_NAME_MAX];
	size_t                        data_sz;
	void                          *data;
	sem_t                         *write_sem;
	char                          write_sem_file[SEM_FILE_NAME_MAX];
	int                           sem_count;
	struct cando_sem              sems[SEM_COUNT_MAX];
};


/***************************************
 * Start of cando_shm_create functions *
 ***************************************/

static int
p_shm_create (struct cando_shm *shm,
              const struct cando_shm_create_info *shm_info)
{
	int err = -1, len;

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
	                 MAP_SHARED|MAP_ANONYMOUS,
	                 shm->fd, 0);
	if (err == -1) {
		cando_log_set_error(shm, errno, "mmap: %s", strerror(errno));
		return -1;
	}

	return 0;
}


static int
p_sem_create (struct cando_shm *shm,
              const struct cando_shm_create_info *shm_info)
{
	int len, s;

	struct cando_sem *sem = NULL;

	if (shm_info->sem_file[0] != '/') {
		cando_log_set_error(shm, CANDO_LOG_ERR_UNCOMMON,
		                    "Semaphore file name '%s' doesn't start with '/'",
		                    shm_info->sem_file);
		return -1;
	}

	len = strnlen(shm_info->sem_file, SEM_FILE_NAME_MAX+32);
	if (len >= (SEM_FILE_NAME_MAX - SEM_FILE_NAME_SUFFIX_MAX)) {
		cando_log_set_error(shm, CANDO_LOG_ERR_UNCOMMON,
		                    "Semaphore file name '%s' name length to long",
		                    shm_info->sem_file);
		return -1;
	}

	/*
	 * Initialize write semaphore in unlocked state.
	 */
	snprintf(shm->write_sem_file, SEM_FILE_NAME_MAX,
	         "%s-write", shm_info->sem_file);
	shm->write_sem = sem_open(shm->write_sem_file, O_CREAT|O_RDONLY, 0644, 1);
	if (!(shm->write_sem)) {
		cando_log_set_error(shm, errno, "sem_open: %s", strerror(errno));
		return -1;
	}

	/*
	 * Initialize read semaphores in locked state.
	 */
	shm->sem_count = shm_info->sem_count;
	for (s = 0; s < shm->sem_count; s++) {
		sem = &(shm->sems[s]);

		snprintf(sem->read_sem_file, SEM_FILE_NAME_MAX,
			 "%s-read-%d", shm_info->sem_file, s);
		sem->read_sem = sem_open(sem->read_sem_file,
		                         O_CREAT|O_RDONLY, 0644, 0);
		if (!(sem->read_sem)) {
			cando_log_set_error(shm, errno, "sem_open: %s", strerror(errno));
			return -1;
		}
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

	err = p_sem_create(shm, shm_info);
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

/************************************
 * Start of cando_shm_get functions *
 ************************************/


/****************************************
 * Start of cando_shm_destroy functions *
 ****************************************/

void
cando_shm_destroy (struct cando_shm *shm)
{
	int s;

	struct cando_sem *sem = NULL;

	if (!shm)
		return;

	munmap(shm->data, shm->data_sz);

	for (s = 0; s < shm->sem_count; s++) {
		sem = &(shm->sems[s]);
		sem_close(sem->read_sem);
		sem_unlink(sem->read_sem_file);
	}

	sem_close(shm->write_sem);
	sem_unlink(shm->write_sem_file);
	shm_unlink(shm->shm_file);

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
