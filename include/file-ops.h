#ifndef CANDO_FILE_OPS_H
#define CANDO_FILE_OPS_H

/*
 * Stores information about the cando_file_ops instance.
 */
struct cando_file_ops;


/*
 * @brief Cando File Operations Create Info Structure
 *
 * @member fileName   - Full path to file caller wants to open(2)|creat(2).
 * @member dataSize   - Size in bytes caller wants newly created file to be.
 *                      If the file already exist or @createPipe is true
 *                      this member is ignored.
 * @member offset     - Offset within the file to mmap(2).
 *                      If the file already exist or @createPipe is true
 *                      this member is ignored.
 * @member createPipe - Boolean to enable/disable creation of a pipe(2).
 */
struct cando_file_ops_create_info {
	char              *fileName;
	long unsigned int dataSize;
	off_t             offset;
	unsigned char     createPipe : 1;
};


/*
 * @brief Creates or opens caller define file
 *
 * @param fileInfo - Pointer to a struct cando_file_ops_create_info.
 *                   The use of pointer to a void is to limit amount
 *                   of columns required to define a function.
 *                 
 * @returns
 * 	on success: Pointer to a struct cando_file_ops
 * 	on failure: NULL
 */
struct cando_file_ops *
cando_file_ops_create (const void *fileInfo);


/*
 * @brief Adjust file to a size of precisely length bytes
 *
 * @param flops    - Pointer to a valid struct cando_file_ops
 * @param dataSize - Size in bytes caller wants newly created file to be.
 *
 * @returns
 * 	on success: 0
 * 	on failure: # < 0
 */
int
cando_file_ops_truncate_file (struct cando_file_ops *flops,
                              const long unsigned int dataSize);


/*
 * @brief Returns file data stored at a given offset
 *
 * @param flops  - Pointer to a valid struct cando_file_ops
 * @param offset - Byte offset within the file
 *
 * @returns
 * 	on success: Pointer to file data at a given index
 * 	on failure: NULL
 */
const void *
cando_file_ops_get_data (struct cando_file_ops *flops,
                         const unsigned long int offset);


/*
 * @brief Frees any allocated memory and closes FD's (if open) created after
 *        cando_file_ops_create() call.
 *
 * @param flops - Pointer to a valid struct cando_file_ops
 */
void
cando_file_ops_destroy (struct cando_file_ops *flops);


#endif /* CANDO_FILE_OPS_H */
