#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "log.h"
#include "file-ops.h"

#define FILE_NAME_LEN_MAX (1<<12)

/*
 * @brief struct defining cando_file_ops instance
 *
 * @member fd      - File descriptor to open file.
 * @member fname   - String representing the file name.
 * @member pipefds - File descriptors associated with an open pipe.
 */
struct cando_file_ops {
	int  fd;
	char *fname;
	int  pipefds[2];
};


struct cando_file_ops *
cando_file_ops_create (struct cando_file_ops_create_info *fileCreateInfo)
{
	int ret = -1;

	struct cando_file_ops *flops = NULL;

	flops = calloc(1, sizeof(struct cando_file_ops));
	if (!flops) {
		return NULL;
	}

	if (fileCreateInfo->fileName) {
		flops->fname = strndup(fileCreateInfo->fileName, FILE_NAME_LEN_MAX);
		if (!(flops->fname)) {
			cando_file_ops_destroy(flops);
			return NULL;
		}

		flops->fd = open(flops->fname, O_CREAT|O_RDWR, 0644);
		if (!(flops->fd)) {
			cando_file_ops_destroy(flops);
			return NULL;
		}
	}

	if (fileCreateInfo->createPipe) {
		ret = pipe(flops->pipefds);
		if (ret == -1) {
			cando_file_ops_destroy(flops);
			return NULL;
		}
	}

	return flops;
}


void
cando_file_ops_destroy (struct cando_file_ops *flops)
{
	if (!flops)
		return;

	close(flops->pipefds[0]);
	close(flops->pipefds[1]);
	close(flops->fd);
	free(flops->fname);
	free(flops);
}
