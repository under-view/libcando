#define _GNU_SOURCE 1
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/mman.h>

#include "log.h"
#include "file-ops.h"

#define FILE_NAME_LEN_MAX (1<<12)
#define PIPE_MAX_BUFF_SIZE (size_t)(1<<16)

/*
 * @brief struct defining cando_file_ops instance
 *
 * @member err      - Stores information about the error that occured
 *                    for the given instance and may later be retrieved
 *                    by caller.
 * @member free     - If structure allocated with calloc(3) member will be
 *                    set to true so that, we know to call free(3) when
 *                    destroying the instance.
 * @member fd       - File descriptor to open file.
 * @member pipe_fds - File descriptors associated with an open pipe.
 *                    pipe_fds[0] - Read end of the pipe
 *                    pipe_fds[1] - Write end of the pipe
 * @member fname    - String representing the file name.
 * @member data_sz  - Total size of the file that was mapped with mmap(2).
 * @member data     - Pointer to mmap(2) file data.
 */
struct cando_file_ops
{
	struct cando_log_error_struct err;
	bool                          free;
	int                           fd;
	int                           pipe_fds[2];
	char                          fname[FILE_NAME_LEN_MAX];
	size_t                        data_sz;
	void                          *data;
};


/********************************************
 * Start of cando_file_ops_create functions *
 ********************************************/

struct cando_file_ops *
cando_file_ops_create (struct cando_file_ops *p_flops,
                       const void *p_file_info)
{
	int ret = -1;

	struct stat fstats;

	struct cando_file_ops *flops = p_flops;

	const struct cando_file_ops_create_info *file_info = p_file_info;

	if (!file_info) {
		cando_log_error("Incorrect data passed\n");
		return NULL;
	}

	if (!flops) {
		flops = calloc(1, sizeof(struct cando_file_ops));
		if (!flops) {
			cando_log_error("calloc: %s\n", strerror(errno));
			return NULL;
		}

		flops->free = true;
	}

	if (file_info->fname) {
		/* Check if file exist */
		ret = stat(file_info->fname, &fstats);

		memccpy(flops->fname, file_info->fname, '\n', FILE_NAME_LEN_MAX);

		flops->fd = open(flops->fname, O_CREAT|O_RDWR, 0644);
		if (flops->fd == -1) {
			cando_log_error("open: %s\n", strerror(errno));
			cando_file_ops_destroy(flops);
			return NULL;
		}

		/*
		 * If file exists and caller defined data_sz set to 0.
		 * 	- Then set internal data_sz to equal the size of the file.
		 * 	- Else set internal data_sz to equal the caller defined size.
		 */
		flops->data_sz = (!ret && !(file_info->size)) ? \
				  (unsigned long int) fstats.st_size : \
				  file_info->size;
	}

	if (file_info->create_pipe) {
		ret = pipe(flops->pipe_fds);
		if (ret == -1) {
			cando_log_error("pipe: %s\n", strerror(errno));
			cando_file_ops_destroy(flops);
			return NULL;
		}
	} else {
		ret = cando_file_ops_truncate_file(flops, flops->data_sz);
		if (ret < 0 && flops->data_sz) {
			cando_log_error("%s\n", cando_log_get_error(flops));
			cando_file_ops_destroy(flops);
			return NULL;
		}

		flops->data = mmap(NULL,
				   flops->data_sz,
				   PROT_READ,
				   MAP_PRIVATE,
				   flops->fd,
				   file_info->offset);
		if (flops->data == (void*)-1 && flops->data_sz) {
			cando_log_error("mmap: %s\n", strerror(errno));
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
                              const off_t size)
{
	int ret = -1;

	if (!flops)
		return -1;

	if (size == 0)
	{
		cando_log_set_error(flops, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	ret = ftruncate(flops->fd, size);
	if (ret == -1) {
		cando_log_set_error(flops, errno, "ftruncate: %s", strerror(errno));
		return -errno;
	}

	return 0;
}

/*************************************************
 * End of cando_file_ops_truncate_file functions *
 *************************************************/


/***********************************************
 * Start of cando_file_ops_zero_copy functions *
 ***********************************************/

ssize_t
cando_file_ops_zero_copy (struct cando_file_ops *flops,
                          const void *p_file_info)
{
	ssize_t ret;

	const struct cando_file_ops_zero_copy_info *file_info = p_file_info;

	if (!flops)
		return -1;

	if (!file_info || \
	    file_info->size == 0)
	{
		cando_log_set_error(flops, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	ret = splice(file_info->in_fd,
		     file_info->in_off,
		     flops->pipe_fds[1], 0,
		     CANDO_MIN(file_info->size, PIPE_MAX_BUFF_SIZE),
		     SPLICE_F_MOVE|SPLICE_F_MORE);
	if (ret == 0) {
		return 0;
	} else if (ret == -1) {
		cando_log_set_error(flops, errno, "splice: %s", strerror(errno));
		return -1;
	}

	ret = splice(flops->pipe_fds[0], 0,
		     file_info->out_fd,
		     file_info->out_off,
		     CANDO_MIN(file_info->size, PIPE_MAX_BUFF_SIZE),
		     SPLICE_F_MOVE|SPLICE_F_MORE);
	if (ret == -1) {
		cando_log_set_error(flops, errno, "splice: %s", strerror(errno));
		return -1;
	}

	return ret;
}

/*********************************************
 * End of cando_file_ops_zero_copy functions *
 *********************************************/


/*****************************************
 * Start of cando_file_ops_get functions *
 *****************************************/

const void *
cando_file_ops_get_data (struct cando_file_ops *flops,
                         const size_t offset)
{
	if (!flops)
		return NULL;

	if (!(flops->data) || \
	    offset >= flops->data_sz)
	{
		cando_log_set_error(flops, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return NULL;
	}

	return ((char*)flops->data)+offset;
}


const char *
cando_file_ops_get_line (struct cando_file_ops *flops,
			 const size_t p_line)
{
	size_t offset, c, line = 0;

	if (!flops)
		return NULL;

	if (!(flops->data) || \
	    !p_line)
	{
		cando_log_set_error(flops, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return NULL;
	}

	for (offset = 0, c = 0; offset < flops->data_sz; offset++,c++) {
		if (*(((char*)flops->data)+offset) == '\n') {
			line++;

			if (line == p_line) {
				break;
			} else {
				c = 0;
			}
		}
	}

	c -= (p_line == 1) ? 0 : 1;
	return ((char*)flops->data)+(offset-c);
}


size_t
cando_file_ops_get_line_count (struct cando_file_ops *flops)
{
	size_t offset, line = 0;

	if (!flops)
		return -1;

	if (!(flops->data))
	{
		cando_log_set_error(flops, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	for (offset = 0; offset < flops->data_sz; offset++)
		if (*((char*)flops->data + offset) == '\n')
			line++;

	return line;
}


int
cando_file_ops_get_fd (struct cando_file_ops *flops)
{
	if (!flops)
		return -1;

	return flops->fd;
}


size_t
cando_file_ops_get_data_size (struct cando_file_ops *flops)
{
	if (!flops)
		return -1;

	return flops->data_sz;
}


const char *
cando_file_ops_get_filename (struct cando_file_ops *flops)
{
	if (!flops)
		return NULL;

	return flops->fname;
}

/***************************************
 * End of cando_file_ops_get functions *
 ***************************************/


/*****************************************
 * Start of cando_file_ops_set functions *
 *****************************************/

int
cando_file_ops_set_data (struct cando_file_ops *flops,
                         const void *p_file_info)
{
	int ret = -1;

	void *data = NULL;

	const struct cando_file_ops_set_data_info *file_info = p_file_info;

	if (!flops)
		return -1;

	if (!file_info || \
	    !(flops->data) || \
	    !(file_info->data) || \
	    (file_info->size+file_info->offset) >= flops->data_sz)
	{
		cando_log_set_error(flops, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	data = (void*)(((char*)flops->data)+file_info->offset);

	ret = CANDO_PAGE_SET_WRITE(data, file_info->size);
	if (ret == -1) {
		cando_log_set_error(flops, errno, "mprotect: %s", strerror(errno));
		return -1;
	}

	memcpy(data, file_info->data, file_info->size);

	ret = CANDO_PAGE_SET_READ(data, file_info->size);
	if (ret == -1) {
		cando_log_set_error(flops, errno, "mprotect: %s", strerror(errno));
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

	munmap(flops->data, flops->data_sz);
	close(flops->pipe_fds[0]);
	close(flops->pipe_fds[1]);
	close(flops->fd);

	if (flops->free) {
		free(flops);
	} else {
		memset(flops, 0, sizeof(struct cando_file_ops));
	}
}

/*******************************************
 * End of cando_file_ops_destroy functions *
 *******************************************/


/******************************************************
 * Start of non struct cando_file_ops param functions *
 ******************************************************/

int
cando_file_ops_get_sizeof (void)
{
	return sizeof(struct cando_file_ops);
}


int
cando_file_ops_set_fd_flags (const int fd, const int flags)
{
	int opt = 0, err = -1;

	opt = fcntl(fd, F_GETFL);
	if (opt < 0) {
		cando_log_error("fcntl: %s\n", strerror(errno));
		return -1;
	}

	opt |= flags;
	err = fcntl(fd, F_SETFL, opt);
	if (err < 0) {
		cando_log_error("fcntl: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

/****************************************************
 * End of non struct cando_file_ops param functions *
 ****************************************************/
