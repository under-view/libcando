#ifndef CANDO_VSOCK_UDP_H
#define CANDO_VSOCK_UDP_H

#include "macros.h"

#include <sys/socket.h>
#include <linux/vm_sockets.h>

/*
 * Stores information about the cando_vsock_udp instace.
 */
struct cando_vsock_udp;


/*
 * @brief Structure passed to cando_vsock_udp_server_create(3)
 *        used to define how to create the server.
 *
 * @member vcid - VM Context Identifier to recvfrom(2)/sendto(2) data with.
 * @member port - Network port to recvfrom(2)/sendto(2) data with.
 */
struct cando_vsock_udp_server_create_info
{
	unsigned int vcid;
	int          port;
};


/*
 * @brief Creates a VM socket that may be utilized for server socket operations.
 *
 * @param vsock      - May be NULL or a pointer to a struct cando_vsock_udp.
 *                     If NULL memory will be allocated and return to
 *                     caller. If not NULL address passed will be used
 *                     to store the newly created struct cando_vsock_udp
 *                     instance.
 * @param vsock_info - Implementation uses a pointer to a
 *                     struct cando_vsock_udp_server_create_info
 *                     no other implementation may be passed to
 *                     this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_vsock_udp
 *	on failure: NULL
 */
CANDO_API
struct cando_vsock_udp *
cando_vsock_udp_server_create (struct cando_vsock_udp *vsock,
                               const void *vsock_info);


/*
 * @brief Creates file descriptor that can filter for
 *        @addr data comming to server file descriptor.
 *        Useful to utilize in an epoll(2) event loop
 *        if caller wants to implement tcp like event
 *        handling with UDP sockets.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_udp.
 * @param addr  - Must pass a pointer to a populated struct sockaddr_vm.
 *
 * @return
 *	on success: File descriptor to filtered socket
 *	on failure: -1
 */
CANDO_API
int
cando_vsock_udp_server_accept (struct cando_vsock_udp *vsock,
                               const struct sockaddr_vm *addr);


/*
 * @brief Receive data from server socket file descriptor.
 *
 * @param vsock      - Pointer to a struct cando_sock_udp instance.
 * @param data       - Pointer to data to store data received from a socket.
 * @param size       - Size of data to receive from a socket.
 * @param addr       - Pointer to struct sockaddr_vm which stores the
 *                     address information of the socket that data
 *                     was received from.
 * @param vsock_info - Reserved for future usage. For now used
 *                     to set the flag argument of recvfrom(2).
 *
 * @return
 *	on success: Amount of bytes received
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_vsock_udp_server_recv_data (struct cando_vsock_udp *vsock,
                                  void *data,
                                  const size_t size,
                                  struct sockaddr_vm *addr,
                                  const void *vsock_info);


/*
 * @brief Structure passed to cando_vsock_udp_client_create(3)
 *        used to define how to create the server.
 *
 * @member vcid - VM Context Identifier to sendto(2)/recvfrom(2) data with.
 * @member port - Network port to sendto(2)/recvfrom(2) data with.
 */
struct cando_vsock_udp_client_create_info
{
	unsigned int vcid;
	int          port;
};


/*
 * @brief Creates a VM socket that may be utilized for client socket operations.
 *
 * @param vsock      - May be NULL or a pointer to a struct cando_vsock_udp.
 *                     If NULL memory will be allocated and return to
 *                     caller. If not NULL address passed will be used
 *                     to store the newly created struct cando_vsock_udp
 *                     instance.
 * @param vsock_info - Implementation uses a pointer to a
 *                     struct cando_vsock_udp_client_create_info
 *                     no other implementation may be passed to
 *                     this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_vsock_udp
 *	on failure: NULL
 */
CANDO_API
struct cando_vsock_udp *
cando_vsock_udp_client_create (struct cando_vsock_udp *vsock,
                               const void *vsock_info);


/*
 * @brief Fliters client socket to allow sending data
 *        without passing a struct sockaddr_vm to sendto(2).
 *        Address is populated with a call to cando_sock_udp_client_create.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_udp.
 *
 * @return
 *	on success: 0
 *	on failure: -1
 */
CANDO_API
int
cando_vsock_udp_client_connect (struct cando_vsock_udp *vsock);


/*
 * @brief Send data to client socket address provided via
 *        call to cando_vsock_udp_client_create(3).
 *
 * @param vsock      - Must pass a pointer to a struct cando_vsock_udp.
 * @param data       - Pointer to data to send through socket.
 * @param size       - Size of data to send through socket.
 * @param vsock_info - Reserved for future usage. For now used
 *                     to set the flag argument of sendto(2).
 *
 * @return
 *	on success: Amount of bytes sent
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_vsock_udp_client_send_data (struct cando_vsock_udp *vsock,
                                  const void *data,
                                  const size_t size,
                                  const void *vsock_info);


/*
 * @brief Acquire VM socket file descriptor associated with
 *        struct cando_vsock_udp instance.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_udp.
 *
 * @return
 *	on success: VM socket file descriptor
 *	on failure: -1
 */
CANDO_API
int
cando_vsock_udp_get_fd (struct cando_vsock_udp *vsock);


/*
 * @brief Acquire VM socket context identifier associated with
 *        struct cando_vsock_udp context.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_udp.
 *
 * @return
 *	on success: VM socket context identifier
 *	on failure: UINT32_MAX
 */
CANDO_API
unsigned int
cando_vsock_udp_get_vcid (struct cando_vsock_udp *vsock);


/*
 * @brief Acquire network port associated with
 *        struct cando_vsock_udp instance.
 *
 * @param vsock - Must pass a pointer to a struct cando_vsock_udp.
 *
 * @return
 *	on success: UDP port connected to instance
 *	on failure: -1
 */
CANDO_API
int
cando_vsock_udp_get_port (struct cando_vsock_udp *vsock);


/*
 * @brief Frees any allocated memory and closes FD's (if open) created after
 *        cando_vsock_udp_server_create() or cando_vsock_udp_client_create() call.
 *
 * @param vsock - Pointer to a valid struct cando_vsock_udp.
 */
CANDO_API
void
cando_vsock_udp_destroy (struct cando_vsock_udp *vsock);


/*
 * @brief Returns size of the internal structure. So,
 *        if caller decides to allocate memory outside
 *        of API interface they know the exact amount
 *        of bytes.
 *
 * @return
 *	on success: sizeof(struct cando_vsock_udp)
 *	on failure: sizeof(struct cando_vsock_udp)
 */
CANDO_API
int
cando_vsock_udp_get_sizeof (void);


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
cando_vsock_udp_get_local_vcid (void);


/*
 * @brief Receive data from socket file descriptor.
 *
 * @param sock_fd - Socket file descriptor to receive data from.
 * @param data    - Pointer to data to store data received from a socket.
 * @param size    - Size of data to receive from a socket.
 * @param addr    - Pointer to struct sockaddr_vm which stores the
 *                  address information of the socket that data
 *                  was received from.
 * @param opts    - Reserved for future usage. For now used
 *                  to set the flag argument of recvfrom(2).
 *
 * @return
 *	on success: Amount of bytes received
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_vsock_udp_recv_data (const int sock_fd,
                           void *data,
                           const size_t size,
                           struct sockaddr_vm *addr,
                           const void *opts);


/*
 * @brief Send data to socket file descriptor.
 *
 * @param sock_fd - Socket file descriptor to send data to.
 * @param data    - Pointer to data to send through socket.
 * @param size    - Size of data to send through socket.
 * @param addr    - Pointer to struct sockaddr_vm which stores the
 *                  address information of a socket that data
 *                  will be sent to.
 * @param opts    - Reserved for future usage. For now used
 *                  to set the flag argument of sendto(2).
 *
 * @return
 *	on success: Amount of bytes sent
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_vsock_udp_send_data (const int sock_fd,
                           const void *data,
                           const size_t size,
                           const struct sockaddr_vm *addr,
                           const void *opts);

#endif /* CANDO_VSOCK_UDP_H */
