#ifndef CANDO_SHM_H
#define CANDO_SHM_H

#include "macros.h"

/*
 * Stores information about the cando_sock_tcp instace.
 */
struct cando_shm;


/*
 * @brief Structure passed to cando_shm_create(3) used
 *        to define shared memory and semaphore names.
 *
 * @param shm_file  - Shared memory file name. Must start
 *                    with the character '/'.
 * @param shm_size  - Size of shared memory region/file.
 * @param sem_file  - Prefix of semaphore file name. Must
 *                    start with the character '/'.
 * @param sem_count - Amount of POSIX semaphores to create.
 *
 */
struct cando_shm_create_info
{
	const char *shm_file;
	off_t      shm_size;
	const char *sem_file;
	int        sem_count;
};


/*
 * @brief Creates POSIX shared memory and semaphores.
 *
 * @param shm      - May be NULL or a pointer to a struct cando_shm.
 *                   If NULL memory will be allocated and return to
 *                   caller. If not NULL address passed will be used
 *                   to store the newly created struct cando_shm
 *                   instance.
 * @param shm_info - Implementation uses a pointer to a
 *                   struct cando_shm_create_info
 *                   no other implementation may be passed to
 *                   this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_shm
 *	on failure: NULL
 */
CANDO_API
struct cando_shm *
cando_shm_create (struct cando_shm *shm,
                  const void *shm_info);


/*
 * @brief Returns file descriptor to the POSIX shared memory
 *        created after call to cando_shm_create().
 *
 * @param shm - Pointer to a valid struct cando_shm.
 *
 * @return
 *	on success: File descriptor to POSIX shared memory
 *	on failure: -1
 */
CANDO_API
int
cando_shm_get_fd (struct cando_shm *shm);


/*
 * @brief Frees any allocated memory and closes FD's (if open)
 *        created after cando_shm_create() call.
 *
 * @param shm - Pointer to a valid struct cando_shm.
 */
CANDO_API
void
cando_shm_destroy (struct cando_shm *shm);


/*
 * @brief Returns size of the internal structure. So,
 *        if caller decides to allocate memory outside
 *        of API interface they know the exact amount
 *        of bytes.
 *
 * @return
 *	on success: sizeof(struct cando_shm)
 *	on failure: sizeof(struct cando_shm)
 */
CANDO_API
int
cando_shm_get_sizeof (void);

#endif /* CANDO_SHM_H */
