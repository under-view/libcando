#ifndef CANDO_VSOCK_TCP_H
#define CANDO_VSOCK_TCP_H

#include "macros.h"

#include <sys/socket.h>
#include <linux/vm_sockets.h>

/*
 * Stores information about the cando_vsock_tcp instace.
 */
struct cando_vsock_tcp;


/*
 * @brief Structure passed to cando_vsock_tcp_server_create(3)
 *        used to define how to create the server.
 *
 * @member vcid        - VM Context Identifier to accept(2) with.
 * @member port        - Network port to accept(2) with.
 * @member connections - Amount of connections that may be queued
 *                       at a given moment.
 */
struct cando_vsock_tcp_server_create_info
{
	unsigned int vcid;
	int          port;
	int          connections;
};


/*
 * @brief Creates a VM socket that may be utilized for server socket operations.
 *
 * @param vsock      - May be NULL or a pointer to a struct cando_vsock_tcp.
 *                     If NULL memory will be allocated and return to
 *                     caller. If not NULL address passed will be used
 *                     to store the newly created struct cando_vsock_tcp
 *                     instance.
 * @param vsock_info - Implementation uses a pointer to a
 *                     struct cando_vsock_tcp_server_create_info
 *                     no other implementation may be passed to
 *                     this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_vsock_tcp
 *	on failure: NULL
 */
CANDO_API
struct cando_vsock_tcp *
cando_vsock_tcp_server_create (struct cando_vsock_tcp *vsock,
                               const void *vsock_info);


/*
 * @brief Accepts client connections returns file descriptor
 *        to the connected client.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_tcp.
 * @param addr  - May be NULL or a pointer to a struct sockaddr_vm
 *                If not NULL @addr is filled in via accept(2) call.
 *
 * @return
 *	on success: File descriptor to accepted client
 *	on failure: -1
 */
CANDO_API
int
cando_vsock_tcp_server_accept (struct cando_vsock_tcp *vsock,
                               struct sockaddr_vm *addr);


/*
 * @brief Structure passed to cando_vsock_tcp_client_create(3)
 *        used to define how to create the client.
 *
 * @member vcid - VM Context Identifier to connect(2)/send(2) to.
 * @member port - Network port to connect(2)/send(2) to.
 */
struct cando_vsock_tcp_client_create_info
{
	unsigned int vcid;
	int          port;
};


/*
 * @brief Creates a VM socket that may be utilized for client socket operations.
 *
 * @param vsock      - May be NULL or a pointer to a struct cando_vsock_tcp.
 *                     If NULL memory will be allocated and return to
 *                     caller. If not NULL address passed will be used
 *                     to store the newly created struct cando_vsock_tcp
 *                     instance.
 * @param vsock_info - Implementation uses a pointer to a
 *                     struct cando_vsock_tcp_client_create_info
 *                     no other implementation may be passed to
 *                     this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_vsock_tcp
 *	on failure: NULL
 */
CANDO_API
struct cando_vsock_tcp *
cando_vsock_tcp_client_create (struct cando_vsock_tcp *vsock,
                               const void *vsock_info);


/*
 * @brief Connects client socket to address provided via
 *        call to cando_vsock_tcp_client_create.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_tcp.
 *
 * @return
 *	on success: 0
 *	on failure: -1
 */
CANDO_API
int
cando_vsock_tcp_client_connect (struct cando_vsock_tcp *vsock);


/*
 * @brief Send data to client socket address provided via
 *        call to cando_vsock_tcp_client_create(3).
 *
 * @param vsock      - Must pass a pointer to a struct cando_vsock_tcp.
 * @param data       - Pointer to buffer to send through socket.
 * @param size       - Size of data to send through socket.
 * @param vsock_info - Reserved for future usage. For now used
 *                     to set the flag argument of send(2).
 *
 * @return
 *	on success: Amount of bytes sent
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_vsock_tcp_client_send_data (struct cando_vsock_tcp *vsock,
                                  const void *data,
                                  const size_t size,
                                  const void *vsock_info);


/*
 * @brief Acquire VM socket file descriptor associated with
 *        struct cando_vsock_tcp instance.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_tcp.
 *
 * @return
 *	on success: VM socket file descriptor
 *	on failure: -1
 */
CANDO_API
int
cando_vsock_tcp_get_fd (struct cando_vsock_tcp *vsock);


/*
 * @brief Acquire VM socket context identifier associated with
 *        struct cando_vsock_tcp context.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_tcp.
 *
 * @return
 *	on success: VM socket context identifier
 *	on failure: UINT32_MAX
 */
CANDO_API
unsigned int
cando_vsock_tcp_get_vcid (struct cando_vsock_tcp *vsock);


/*
 * @brief Acquire network port associated with struct cando_vsock_tcp instance.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_tcp.
 *
 * @return
 *	on success: Network port connected to instance
 *	on failure: -1
 */
CANDO_API
int
cando_vsock_tcp_get_port (struct cando_vsock_tcp *vsock);


/*
 * @brief Frees any allocated memory and closes FD's (if open) created after
 *        cando_vsock_tcp_server_create() or cando_vsock_tcp_client_create() call.
 *
 * @param vsock - Pointer to a valid struct cando_vsock_tcp.
 */
CANDO_API
void
cando_vsock_tcp_destroy (struct cando_vsock_tcp *vsock);


/*
 * @brief Returns size of the internal structure. So,
 *        if caller decides to allocate memory outside
 *        of API interface they know the exact amount
 *        of bytes.
 *
 * @return
 *	on success: sizeof(struct cando_vsock_tcp)
 *	on failure: sizeof(struct cando_vsock_tcp)
 */
CANDO_API
int
cando_vsock_tcp_get_sizeof (void);


/*
 * @brief Returns the local CID of the VM/Hypervisor after
 *        acquiring it from /dev/vsock.
 *
 * @return
 *	on success: Local VM context identifer
 *	on failure: UINT32_MAX
 */
CANDO_API
unsigned int
cando_vsock_tcp_get_local_vcid (void);


/*
 * @brief Receive data from socket file descriptor.
 *
 * @param sock_fd    - Socket file descriptor to receive data from.
 * @param data       - Pointer to buffer to store data received from a socket.
 * @param size       - Size of data to receive from a socket.
 * @param vsock_info - Reserved for future usage. For now used
 *                     to set the flag argument of recv(2).
 *
 * @return
 *	on success: Amount of bytes received
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_vsock_tcp_recv_data (const int sock_fd,
                           void *data,
                           const size_t size,
                           const void *vsock_info);


/*
 * @brief Send data to socket file descriptor.
 *
 * @param sock_fd    - Socket file descriptor to send data to.
 * @param data       - Pointer to buffer to send through socket.
 * @param size       - Size of data to send through socket.
 * @param vsock_info - Reserved for future usage. For now used
 *                     to set the flag argument of send(2).
 *
 * @return
 *	on success: Amount of bytes sent
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_vsock_tcp_send_data (const int sock_fd,
                           const void *data,
                           const size_t size,
                           const void *vsock_info);

#endif /* CANDO_VSOCK_TCP_H */
