#ifndef CANDO_FILE_OPS_H
#define CANDO_FILE_OPS_H

/*
 * Stores information about the cando_file_ops instance.
 */
struct cando_file_ops;


/*
 * @brief Cando File Operations Create Info Structure
 *
 * @member fileName   - Full path to file caller wants to open|create.
 * @member byteSize   - Size in bytes caller wants newly created file to be.
 * @member createPipe - Boolean to enable/disable creation of a pipe(2).
 */
struct cando_file_ops_create_info {
	const char        *fileName;
	long unsigned int byteSize;
	unsigned int      createPipe : 1;
};


/*
 * @brief Creates or opens caller define file
 *
 * @param fileInfo - Pointer to a struct cando_file_ops_create_info.
 *                 
 * @returns
 * 	on success: Pointer to a struct cando_file_ops
 * 	on failure: NULL
 */
struct cando_file_ops *
cando_file_ops_create (struct cando_file_ops_create_info *fileInfo);


/*
 * @brief Frees any allocated memory and closes FD's (if open) created after
 *        cando_file_ops_create() call.
 *
 * @param flops - Pointer to a valid struct cando_file_ops
 *
 *                Free'd members with fd's closed
 *                struct cando_file_ops {
 *          	      int pipefds[2];
 *                }
 */
void
cando_file_ops_destroy (struct cando_file_ops *flops);


#endif /* CANDO_FILE_OPS_H */
