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
	size_t     shm_size;
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
 * @brief Structure defining what operations to perform
 *        and data to retrieve during calls to
 *        cando_shm_read() and cando_shm_write().
 *
 * @member data      - Pointer to a buffer that will either be used
 *                     to store shm data or write to shm data.
 * @member size      - Size in bytes to read from or write to shared memory.
 * @member offset    - Byte offset in shared memory to either read
 *                     from or write to.
 * @member sem_index - Read semaphore index to either lock or unlock.
 * @member block     - Determines if caller wants to perform a blocking
 *                     or busy wait read/write operation.
 */
struct cando_shm_data_info
{
	void          *data;
	size_t        size;
	size_t        offset;
	int           sem_index;
	unsigned char block : 1;
};


/*
 * @brief Reads data stored in shared memory at
 *        caller defined offset and writes into
 *        a caller defined buffer.
 *
 * @param shm      - Pointer to a valid struct cando_shm.
 * @param shm_info - Must pass a pointer to a struct cando_shm_data_info.
 *
 * @return
 *	on success: 0
 *	on failure: -1
 */
CANDO_API
int
cando_shm_data_read (struct cando_shm *shm,
                     const void *shm_info);


/*
 * @brief Write data stored int caller defined buffer
 *        into shared memory at a caller defined
 *        shared memory offset.
 *
 * @param shm      - Pointer to a valid struct cando_shm.
 * @param shm_info - Must pass a pointer to a struct cando_shm_data_info.
 *
 * @return
 *	on success: 0
 *	on failure: -1
 *
 */
CANDO_API
int
cando_shm_data_write (struct cando_shm *shm,
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
 * @brief Returns mmap(2) map'd POSIX shared memory buffer
 *        created after call to cando_shm_create().
 *
 * @param shm - Pointer to a valid struct cando_shm.
 *
 * @return
 *	on success: Pointer to map'd memory
 *	on failure: NULL
 */
CANDO_API
void *
cando_shm_get_data (struct cando_shm *shm);


/*
 * @brief Returns size of mmap(2) POSIX shared memory buffer
 *        created after call to cando_shm_create().
 *
 * @param shm - Pointer to a valid struct cando_shm.
 *
 * @return
 *	on success: Pointer to map'd memory
 *	on failure: NULL
 */
CANDO_API
size_t
cando_shm_get_data_size (struct cando_shm *shm);


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
