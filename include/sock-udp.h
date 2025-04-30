#ifndef CANDO_VSOCK_UDP_H
#define CANDO_VSOCK_UDP_H

#include "macros.h"

#include <netinet/in.h>

/*
 * Stores information about the cando_sock_udp instace.
 */
struct cando_sock_udp;


/*
 * @brief Structure passed to cando_sock_udp_server_create(3)
 *        used to define how to create the server.
 *
 * @member ip_addr - IP address to receive/send data with.
 * @member port    - UDP port to receive/send data with.
 * @member ipv6    - Boolean select address family
 *                   AF_INET (ipv4) or AF_INET6 (ipv6).
 */
struct cando_sock_udp_server_create_info
{
	const char    *ip_addr;
	int           port;
	unsigned char ipv6 : 1;
};


/*
 * @brief Creates a VM socket that may be utilized for server socket operations.
 *
 * @param sock      - May be NULL or a pointer to a struct cando_sock_udp.
 *                    If NULL memory will be allocated and return to
 *                    caller. If not NULL address passed will be used
 *                    to store the newly created struct cando_sock_udp
 *                    instance.
 * @param sock_info - Implementation uses a pointer to a
 *                    struct cando_sock_udp_server_create_info
 *                    no other implementation may be passed to
 *                    this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_sock_udp
 *	on failure: NULL
 */
CANDO_API
struct cando_sock_udp *
cando_sock_udp_server_create (struct cando_sock_udp *sock,
                              const void *sock_info);


/*
 * @brief Accepts client connections returns file descriptor
 *        to the connected client.
 *
 * @param sock - Must pass a pointer to a struct cando_sock_udp.
 * @param addr - Must pass a pointer to a populated struct sockaddr_in6.
 * @param ipv6 - Specify if @addr will be an ipv6 or ipv4 socket.
 *
 * @return
 *	on success: File descriptor to accepted client
 *	on failure: -1
 */
CANDO_API
int
cando_sock_udp_server_accept (struct cando_sock_udp *sock,
                              const struct sockaddr_in6 *addr,
                              const unsigned char ipv6);


/*
 * @brief Receive data from socket file descriptor.
 *
 * @param sock      - Pointer to a struct cando_sock_udp instance.
 * @param data      - Pointer to a buffer to store data received from a socket.
 * @param size      - Size of data to receive from a socket.
 * @param addr      - Pointer to struct sockaddr_in6 which stores the
 *                    address information of the socket that data
 *                    was received from.
 * @param sock_info - Reserved for future usage. For now used
 *                    to set the flag argument of recvfrom(2).
 *
 * @return
 *	on success: Amount of bytes received
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_sock_udp_server_recv_data (struct cando_sock_udp *sock,
                                 void *data,
                                 const size_t size,
                                 struct sockaddr_in6 *addr,
                                 const void *sock_info);


/*
 * @brief Structure passed to cando_sock_udp_client_create(3)
 *        used to define how to create the server.
 *
 * @member ip_addr - IP address to send/receive data with.
 * @member port    - UDP port to send/receive data with.
 * @member ipv6    - Boolean select address family
 *                   AF_INET (ipv4) or AF_INET6 (ipv6).
 */
struct cando_sock_udp_client_create_info
{
	const char    *ip_addr;
	int           port;
	unsigned char ipv6 : 1;
};


/*
 * @brief Creates a VM socket that may be utilized for client socket operations.
 *
 * @param sock      - May be NULL or a pointer to a struct cando_sock_udp.
 *                    If NULL memory will be allocated and return to
 *                    caller. If not NULL address passed will be used
 *                    to store the newly created struct cando_sock_udp
 *                    instance.
 * @param sock_info - Implementation uses a pointer to a
 *                    struct cando_sock_udp_client_create_info
 *                    no other implementation may be passed to
 *                    this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_sock_udp
 *	on failure: NULL
 */
CANDO_API
struct cando_sock_udp *
cando_sock_udp_client_create (struct cando_sock_udp *sock,
                              const void *sock_info);


/*
 * @brief Fliters client socket to allow sending data
 *        without passing a struct sockadd_in to sendto(2).
 *        Address is populated with a call to cando_sock_udp_client_create.
 *
 * @param sock - Must pass a pointer to a struct cando_sock_udp.
 *
 * @return
 *	on success: 0
 *	on failure: -1
 */
CANDO_API
int
cando_sock_udp_client_connect (struct cando_sock_udp *sock);


/*
 * @brief Send data to client socket address provided via
 *        call to cando_sock_udp_client_create(3).
 *
 * @param sock      - Must pass a pointer to a struct cando_sock_udp.
 * @param data      - Pointer to data to send through socket.
 * @param size      - Size of data to send through socket.
 * @param sock_info - Reserved for future usage. For now used
 *                    to set the flag argument of sendto(2).
 *
 * @return
 *	on success: Amount of bytes sent
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_sock_udp_client_send_data (struct cando_sock_udp *sock,
                                 const void *data,
                                 const size_t size,
                                 const void *sock_info);


/*
 * @brief Acquire VM socket file descriptor associated with
 *        struct cando_sock_udp instance.
 *
 * @param sock - Must pass a pointer to a struct cando_sock_udp.
 *
 * @return
 *	on success: VM socket file descriptor
 *	on failure: -1
 */
CANDO_API
int
cando_sock_udp_get_fd (struct cando_sock_udp *sock);


/*
 * @brief Acquire VM socket context identifier associated with
 *        struct cando_sock_udp context.
 *
 * @param sock - Must pass a pointer to a struct cando_sock_udp.
 *
 * @return
 *	on success: VM socket context identifier
 *	on failure: UINT32_MAX
 */
CANDO_API
const char *
cando_sock_udp_get_ip_addr (struct cando_sock_udp *sock);


/*
 * @brief Acquire UDP port associated with struct cando_sock_udp instance.
 *
 * @param sock - Must pass a pointer to a struct cando_sock_udp.
 *
 * @return
 *	on success: UDP port connected to instance
 *	on failure: -1
 */
CANDO_API
int
cando_sock_udp_get_port (struct cando_sock_udp *sock);


/*
 * @brief Frees any allocated memory and closes FD's (if open) created after
 *        cando_sock_udp_server_create() or cando_sock_udp_client_create() call.
 *
 * @param sock - Pointer to a valid struct cando_sock_udp.
 */
CANDO_API
void
cando_sock_udp_destroy (struct cando_sock_udp *sock);


/*
 * @brief Returns size of the internal structure. So,
 *        if caller decides to allocate memory outside
 *        of API interface they know the exact amount
 *        of bytes.
 *
 * @return
 *	on success: sizeof(struct cando_sock_udp)
 *	on failure: sizeof(struct cando_sock_udp)
 */
CANDO_API
int
cando_sock_udp_get_sizeof (void);


/*
 * @brief Receive data from socket file descriptor.
 *
 * @param sock_fd   - Socket file descriptor to receive data from.
 * @param data      - Pointer to data to store data received from a socket.
 * @param size      - Size of data to receive from a socket.
 * @param addr      - Pointer to struct sockaddr_in6 which stores the
 *                    address information of the socket that data
 *                    was received from.
 * @param sock_info - Reserved for future usage. For now used
 *                    to set the flag argument of recvfrom(2).
 *
 * @return
 *	on success: Amount of bytes received
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_sock_udp_recv_data (const int sock_fd,
                          void *data,
                          const size_t size,
                          struct sockaddr_in6 *addr,
                          const void *sock_info);


/*
 * @brief Send data to socket file descriptor.
 *
 * @param sock_fd   - Socket file descriptor to send data to.
 * @param data      - Pointer to data to send through socket.
 * @param size      - Size of data to send through socket.
 * @param addr      - Pointer to struct sockaddr_in6 which stores the
 *                    address information of a socket that data
 *                    will be sent to.
 * @param sock_info - Reserved for future usage. For now used
 *                    to set the flag argument of sendto(2).
 *
 * @return
 *	on success: Amount of bytes sent
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_sock_udp_send_data (const int sock_fd,
                          const void *data,
                          const size_t size,
                          const struct sockaddr_in6 *addr,
                          const void *sock_info);

#endif /* CANDO_VSOCK_UDP_H */
