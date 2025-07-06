#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "log.h"
#include "macros.h"

#include "csock-raw.h"

/*
 * @brief Structure defining Cando CAN Socket Raw interface implementation.
 *
 * @member err   - Stores information about the error that occured
 *                 for the given instance and may later be retrieved
 *                 by caller.
 * @member free  - If structure allocated with calloc(3) member will be
 *                 set to true so that, we know to call free(3) when
 *                 destroying the instance.
 * @member fd    - File descriptor to the open CAN socket.
 * @member iface - Textual CAN interface name in string format to bind(2) to.
 */
struct cando_csock_raw
{
	struct cando_log_error_struct err;
	bool                          free;
	int                           fd;
	char                          iface[IFNAMSIZ];
};


/*********************************************
 * Start of cando_csock_raw_create functions *
 *********************************************/

struct cando_csock_raw *
cando_csock_raw_create (struct cando_csock_raw *p_csock,
                        const void *p_csock_info)
{
	int err = -1;

	struct ifreq ifr;
	struct sockaddr_can addr;

	struct cando_csock_raw *csock = p_csock;

	const struct cando_csock_raw_create_info *csock_info = p_csock_info;

	if (!csock_info) {
		cando_log_error("Incorrect data passed\n");
		return NULL;
	}

	if (!csock) {
		csock = calloc(1, sizeof(struct cando_csock_raw));
		if (!csock) {
			cando_log_error("calloc: %s\n", strerror(errno));
			return NULL;
		}
	}

	csock->fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (csock->fd < 0) {
		cando_log_error("socket: %s\n", strerror(errno));
		cando_csock_raw_destroy(csock);
		return NULL;
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	memset(&addr, 0, sizeof(struct sockaddr_can));

	strncpy(ifr.ifr_name, csock_info->iface, IFNAMSIZ);
	err = ioctl(csock->fd, SIOCGIFINDEX, &ifr);
	if (err == -1) {
		cando_log_error("ioctl: %s\n", strerror(errno));
		cando_csock_raw_destroy(csock);
		return NULL;
	}

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	err = bind(csock->fd, (struct sockaddr*) &addr,
			sizeof(struct sockaddr_can));
	if (err == -1) {
		cando_log_error("bind: %s\n", strerror(errno));
		cando_csock_raw_destroy(csock);
		return NULL;
	}

	strncpy(csock->iface, csock_info->iface, IFNAMSIZ-1);

	return csock;
}

/*******************************************
 * End of cando_csock_raw_create functions *
 *******************************************/


/*****************************************
 * End of cando_csock_raw_send functions *
 *****************************************/

ssize_t
cando_csock_raw_send_data (struct cando_csock_raw *csock,
                           const struct can_frame *frame,
                           const void *csock_info)
{
	ssize_t ret = 0;

	const int size = sizeof(struct can_frame);

	const int flags = (csock_info) ? *((const int*)csock_info) : 0;

	if (!csock)
		return -1;

	if (csock->fd < 0 || !frame)
	{
		cando_log_set_error(csock, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	ret = send(csock->fd, frame, size, flags);
	if (ret != size) {
		cando_log_set_error(csock, CANDO_LOG_ERR_UNCOMMON,
			"send: incomplete CAN frame");
		return -1;
	} else if (errno == EINTR || errno == EAGAIN) {
		return -errno;
	} else if (ret == -1) {
		cando_log_set_error(csock, errno, "send: %s", strerror(errno));
		return -1;
	}

	return ret;
}

/*****************************************
 * End of cando_csock_raw_send functions *
 *****************************************/


/*******************************************
 * Start of cando_csock_raw_recv functions *
 *******************************************/

ssize_t
cando_csock_raw_recv_data (struct cando_csock_raw *csock,
                           struct can_frame *frame,
                           const void *p_csock_info)
{
	ssize_t ret = 0;

	const int size = sizeof(struct can_frame);

	const int flags = (p_csock_info) ? *((const int*)p_csock_info) : 0;

	if (!csock)
		return -1;

	if (csock->fd < 0 || !frame)
	{
		cando_log_set_error(csock, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	ret = recv(csock->fd, frame, size, flags);
	if (ret != size) {
		cando_log_set_error(csock, CANDO_LOG_ERR_UNCOMMON,
			"recv: incomplete CAN frame");
		return -1;
	} else if (errno == EINTR || errno == EAGAIN) {
		return -errno;
	} else if (ret == -1) {
		cando_log_set_error(csock, errno, "recv: %s", strerror(errno));
		return -1;
	}

	return ret;
}

/*****************************************
 * End of cando_csock_raw_recv functions *
 *****************************************/


/******************************************
 * Start of cando_csock_raw_get functions *
 ******************************************/

int
cando_csock_raw_get_fd (struct cando_csock_raw *csock)
{
	if (!csock)
		return -1;

	return csock->fd;
}


const char *
cando_csock_raw_get_iface (struct cando_csock_raw *csock)
{
	if (!csock || \
	    !(*csock->iface))
	{
		return NULL;
	}

	return csock->iface;
}

/****************************************
 * End of cando_csock_raw_get functions *
 ****************************************/


/**********************************************
 * Start of cando_csock_raw_destroy functions *
 **********************************************/

void
cando_csock_raw_destroy (struct cando_csock_raw *csock)
{
	if (!csock)
		return;

	close(csock->fd);

	if (csock->free) {
		free(csock);
	} else {
		memset(csock, 0, sizeof(struct cando_csock_raw));
		csock->fd = -1;
	}
}

/********************************************
 * End of cando_csock_raw_destroy functions *
 ********************************************/


/*******************************************************
 * Start of non struct cando_csock_raw param functions *
 *******************************************************/

int
cando_csock_raw_get_sizeof (void)
{
	return sizeof(struct cando_csock_raw);
}


/*****************************************************
 * End of non struct cando_csock_raw param functions *
 *****************************************************/
