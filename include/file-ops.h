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
 * @member dataSize   - Size in bytes caller newly created file will be.
 *                      If the file already exist or @createPipe is true
 *                      this member is ignored.
 * @member offset     - Offset within the file to mmap(2).
 *                      If the file already exist or @createPipe is true
 *                      this member is ignored.
 * @member createPipe - Boolean to enable/disable creation of a pipe(2).
 */
struct cando_file_ops_create_info
{
	char              *fileName;
	unsigned long int dataSize;
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
 * @param dataSize - Size in bytes newly created file will be
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
 * @brief Returns file data stored at a given line.
 *        Returned output excludes newline character.
 *
 * @param flops   - Pointer to a valid struct cando_file_ops
 * @param lineNum - Line in file to get data from
 *
 * @returns
 * 	on success: Pointer to file data at a given index
 * 	on failure: NULL
 */
const char *
cando_file_ops_get_line (struct cando_file_ops *flops,
			 const unsigned long int lineNum);


/*
 * @brief Returns file descriptor to open file
 *
 * @param flops - Pointer to a valid struct cando_file_ops
 *
 * @returns
 * 	on success: File descriptor to open file
 * 	on failure: NULL
 */
int
cando_file_ops_get_fd (struct cando_file_ops *flops);


/*
 * @brief Return file name of open file associated with
 *        the struct cando_file_ops context.
 *
 * @param flops - Pointer to a valid struct cando_file_ops
 *
 * @returns
 * 	on success: File name of open file
 * 	on failure: NULL
 */
const char *
cando_file_ops_get_filename (struct cando_file_ops *flops);


/*
 * @brief Returns the amount of lines a file contains
 *
 * @param flops  - Pointer to a valid struct cando_file_ops
 *
 * @returns
 * 	on success: Line count
 * 	on failure: -1
 */
long int
cando_file_ops_get_line_count (struct cando_file_ops *flops);


/*
 * @brief Cando File Operations Set Data Info
 *
 * @member offset   - Byte offset within the file
 * @member dataSize - Size in bytes to copy into file at @offset
 * @member data     - Data to copy at the given offset.
 */
struct cando_file_ops_set_data_info
{
	unsigned long int offset;
	size_t dataSize;
	const void *data;
};


/*
 * @brief Sets data in a file at a given offset up to a given size.
 *
 * @param flops    - Pointer to a valid struct cando_file_ops
 * @param fileInfo - Pointer to a struct cando_file_ops_set_data_info.
 *                   The use of pointer to a void is to limit amount
 *                   of columns required to define a function.
 *                 
 * @returns
 * 	on success: Pointer to file data at a given index
 * 	on failure: NULL
 */
int
cando_file_ops_set_data (struct cando_file_ops *flops,
                         const void *fileInfo);


/*
 * @brief Frees any allocated memory and closes FD's (if open) created after
 *        cando_file_ops_create() call.
 *
 * @param flops - Pointer to a valid struct cando_file_ops
 */
void
cando_file_ops_destroy (struct cando_file_ops *flops);


#endif /* CANDO_FILE_OPS_H */
