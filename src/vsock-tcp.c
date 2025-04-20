#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/vm_sockets.h>

#include "log.h"
#include "macros.h"

#include "vsock-tcp.h"

/*
 * @brief Structure defining Cando VM Socket TCP interface implementation.
 *
 * @member free_sock - If structure allocated with calloc(3) member will be
 *                     set to true so that, we know to call free(3) when
 *                     destroying the instance.
 * @member fd        - File descriptor to the open VM socket.
 * @member vcid      - VM Context Identifier.
 * @member port      - TCP port number to connect(2) to or accept(2) from.
 * @member addr      - Stores network byte info about the VM socket context.
 */
struct cando_vsock_tcp
{
	bool               free_sock;
	int                fd;
	unsigned int       vcid;
	int                port;
	struct sockaddr_vm addr;
};


/*****************************************
 * Start of global to C source functions *
 *****************************************/

static unsigned int
p_vsock_get_local_vcid (void)
{
	unsigned int vcid = 0;

	int fd = -1, err = -1;

	fd = open("/dev/vsock", O_RDONLY);
	if (fd == -1) {
		cando_log_error("open('/dev/vsock'): %s\n", strerror(errno));
		return UINT32_MAX;
	}

	err = ioctl(fd, IOCTL_VM_SOCKETS_GET_LOCAL_CID, &vcid);
	if (err == -1 || vcid == UINT32_MAX) {
		close(fd);
		cando_log_error("ioctl: %s\n", strerror(errno));
		return UINT32_MAX;
	}

	close(fd);

	return vcid;
}


static struct cando_vsock_tcp *
p_create_vsock (struct cando_vsock_tcp *p_vsock,
		const void *p_sock_info,
                const bool server)
{
	struct cando_vsock_tcp *vsock = NULL;

	const struct cando_vsock_tcp_create_info {  
		unsigned int vcid;
		int          port;
	} *sock_info = p_sock_info;

	vsock = (p_vsock) ? p_vsock : calloc(1, sizeof(struct cando_vsock_tcp));
	if (!vsock) {
		cando_log_error("calloc: %s\n", strerror(errno));
		return NULL;
	}

	vsock->free_sock = (p_vsock) ? false : true;

	vsock->fd = socket(AF_VSOCK, SOCK_STREAM, 0);
	if (vsock->fd == -1) {
		cando_vsock_tcp_destroy(vsock);
		return NULL;
	}

	vsock->port = sock_info->port;
	vsock->vcid = (server && sock_info->vcid != 1) ? \
		p_vsock_get_local_vcid() : sock_info->vcid;

	vsock->addr.svm_family = AF_VSOCK;
	vsock->addr.svm_reserved1 = 0;
	vsock->addr.svm_port = sock_info->port;
	vsock->addr.svm_cid = vsock->vcid;

	return vsock;
}

/***************************************
 * End of global to C source functions *
 ***************************************/


/*********************************************
 * Start of cando_vsock_tcp_server functions *
 *********************************************/

struct cando_vsock_tcp *
cando_vsock_tcp_server_create (struct cando_vsock_tcp *p_vsock,
                               const void *p_sock_info)
{
	int err = -1;

	struct cando_vsock_tcp *vsock = p_vsock;

	const struct cando_vsock_tcp_server_create_info *sock_info = p_sock_info;

	vsock = p_create_vsock(vsock, p_sock_info, 1);
	if (!vsock)
		return NULL;

	err = bind(vsock->fd, (struct sockaddr*) &(vsock->addr),
			sizeof(struct sockaddr_vm));
	if (err == -1) {
		cando_vsock_tcp_destroy(vsock);
		cando_log_error("bind: %s\n", strerror(errno));
		return NULL;
	}

	err = listen(vsock->fd, sock_info->connections);
	if (err == -1) {
		cando_vsock_tcp_destroy(vsock);
		cando_log_error("listen: %s\n", strerror(errno));
		return NULL;
	}

	return vsock;
}


int
cando_vsock_tcp_server_accept (struct cando_vsock_tcp *vsock,
                               struct sockaddr_vm *p_addr)
{
	int sock_fd = -1;

	struct sockaddr_vm inaddr;
	struct sockaddr_vm *addr = NULL;

	socklen_t len = sizeof(struct sockaddr_vm);

	if (!vsock)
		return -1;

	if (vsock->fd <= 0) {
		cando_log_set_error(vsock, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	addr = (p_addr) ? p_addr : &inaddr;
	sock_fd = accept(vsock->fd, (struct sockaddr*)addr, &len);
	if (sock_fd == -1) {
		cando_log_set_error(vsock, errno, "accept: %s", strerror(errno));
		return -1;
	}

	cando_log(CANDO_LOG_INFO,
	          "[+] Connected client fd '%d' at '%lu:%u'\n",
	          sock_fd, addr->svm_cid, ntohs(addr->svm_port));

	return sock_fd;
}

/*******************************************
 * End of cando_vsock_tcp_server functions *
 *******************************************/


/*********************************************
 * Start of cando_vsock_tcp_client functions *
 *********************************************/

struct cando_vsock_tcp *
cando_vsock_tcp_client_create (struct cando_vsock_tcp *p_vsock,
                               const void *sock_info)
{
	struct cando_vsock_tcp *vsock = p_vsock;

	vsock = p_create_vsock(vsock, sock_info, 0);
	if (!vsock)
		return NULL;

	return vsock;
}


int
cando_vsock_tcp_client_connect (struct cando_vsock_tcp *vsock)
{
	int err = -1;

	if (!vsock)
		return -1;

	if (vsock->fd <= 0) {
		cando_log_set_error(vsock, CANDO_LOG_ERR_INCORRECT_DATA, "");
		return -1;
	}

	err = connect(vsock->fd, (struct sockaddr*)&(vsock->addr),
			sizeof(struct sockaddr_vm));
	if (err == -1) {
		cando_log_set_error(vsock, errno, "connect: %s", strerror(errno));
		return -1;
	}

	cando_log(CANDO_LOG_SUCCESS,
	          "[+] Connected to <VM cid:port> '%lu:%d'\n",
	          vsock->vcid, vsock->port);

	return 0;
}

/*******************************************
 * End of cando_vsock_tcp_client functions *
 *******************************************/


/******************************************
 * Start of cando_vsock_tcp_get functions *
 ******************************************/

int
cando_vsock_tcp_get_fd (struct cando_vsock_tcp *vsock)
{
	if (!vsock)
		return -1;

	return vsock->fd;
}


unsigned int
cando_vsock_tcp_get_vcid (struct cando_vsock_tcp *vsock)
{
	if (!vsock)
		return UINT32_MAX;

	return vsock->vcid;
}


int
cando_vsock_tcp_get_port (struct cando_vsock_tcp *vsock)
{
	if (!vsock)
		return -1;

	return vsock->port;
}

/****************************************
 * End of cando_vsock_tcp_get functions *
 ****************************************/


/**********************************************
 * Start of cando_vsock_tcp_destroy functions *
 **********************************************/

void
cando_vsock_tcp_destroy (struct cando_vsock_tcp *vsock)
{
	if (!vsock)
		return;

	close(vsock->fd);

	if (vsock->free_sock)
		free(vsock);
}

/********************************************
 * End of cando_vsock_tcp_destroy functions *
 ********************************************/


/***************************************************
 * Start of non struct cando_vsock param functions *
 ***************************************************/

unsigned int
cando_vsock_tcp_get_local_vcid (void)
{
	return p_vsock_get_local_vcid();
}


int
cando_vsock_tcp_get_sizeof (void)
{
	return sizeof(struct cando_vsock_tcp);
}

/*************************************************
 * End of non struct cando_vsock param functions *
 *************************************************/
