#ifndef CANDO_USOCK_UDP_H
#define CANDO_USOCK_UDP_H

#include "macros.h"

#include <sys/socket.h>
#include <sys/un.h>

/*
 * Stores information about the cando_usock_udp instace.
 * usock - Unix domain socket.
 */
struct cando_usock_udp;


/*
 * @brief Structure passed to cando_usock_udp_server_create(3)
 *        used to define how to create the server.
 *
 * @member unix_path - Absolute path to unix domain socket.
 */
struct cando_usock_udp_server_create_info
{
	const char *unix_path;
};


/*
 * @brief Creates a socket that may be utilized for server socket operations.
 *
 * @param usock      - May be NULL or a pointer to a struct cando_usock_udp.
 *                     If NULL memory will be allocated and return to
 *                     caller. If not NULL address passed will be used
 *                     to store the newly created struct cando_usock_udp
 *                     instance.
 * @param usock_info - Implementation uses a pointer to a
 *                     struct cando_usock_udp_server_create_info
 *                     no other implementation may be passed to
 *                     this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_usock_udp
 *	on failure: NULL
 */
CANDO_API
struct cando_usock_udp *
cando_usock_udp_server_create (struct cando_usock_udp *usock,
                               const void *usock_info);


/*
 * @brief Receive data from server socket file descriptor.
 *
 * @param usock      - Pointer to a struct cando_usock_udp instance.
 * @param data       - Pointer to a buffer to store data received from a socket.
 * @param size       - Size of data to receive from a socket.
 * @param addr       - Pointer to struct sockaddr_un which stores the
 *                     address information of the socket that data
 *                     was received from.
 * @param usock_info - Reserved for future usage. For now used
 *                     to set the flag argument of recvfrom(2).
 *
 * @return
 *	on success: Amount of bytes received
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_usock_udp_server_recv_data (struct cando_usock_udp *usock,
                                  void *data,
                                  const size_t size,
                                  struct sockaddr_un *addr,
                                  const void *usock_info);


/*
 * @brief Structure passed to cando_usock_udp_client_create(3)
 *        used to define how to create the client.
 *
 * @member srv_unix_path - Absolute path to unix domain socket to write to.
 * @member cli_unix_path - Absolute path to unix domain socket to read with.
 *                         Largely so the @srv_unix_path knows the path to
 *                         the client when leveraging the recvfrom(2) call.
 */
struct cando_usock_udp_client_create_info
{
	const char *srv_unix_path;
	const char *cli_unix_path;
};


/*
 * @brief Creates a socket that may be utilized for client socket operations.
 *        Fliters client socket to allow sending data without passing a
 *        struct sockaddr_un to sendto(2).
 *
 * @param usock      - May be NULL or a pointer to a struct cando_usock_udp.
 *                     If NULL memory will be allocated and return to
 *                     caller. If not NULL address passed will be used
 *                     to store the newly created struct cando_usock_udp
 *                     instance.
 * @param usock_info - Implementation uses a pointer to a
 *                     struct cando_usock_udp_client_create_info
 *                     no other implementation may be passed to
 *                     this parameter.
 *
 * @return
 *	on success: Pointer to a struct cando_usock_udp
 *	on failure: NULL
 */
CANDO_API
struct cando_usock_udp *
cando_usock_udp_client_create (struct cando_usock_udp *usock,
                               const void *usock_info);


/*
 * @brief Send data to client socket address provided via
 *        call to cando_usock_udp_client_create(3).
 *
 * @param sock       - Must pass a pointer to a struct cando_usock_udp.
 * @param data       - Pointer to data to send through socket.
 * @param size       - Size of data to send through socket.
 * @param usock_info - Reserved for future usage. For now used
 *                     to set the flag argument of sendto(2).
 *
 * @return
 *	on success: Amount of bytes sent
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_usock_udp_client_send_data (struct cando_usock_udp *usock,
                                  const void *data,
                                  const size_t size,
                                  const void *usock_info);


/*
 * @brief Acquire socket file descriptor associated with
 *        struct cando_usock_udp instance.
 *
 * @param usock - Must pass a pointer to a struct cando_usock_udp.
 *
 * @return
 *	on success: Unix domain socket file descriptor
 *	on failure: -1
 */
CANDO_API
int
cando_usock_udp_get_fd (struct cando_usock_udp *usock);


/*
 * @brief Acquire textual path to unix domain socket file in string
 *        format associated with struct cando_usock_udp instance.
 *
 * @param usock - Must pass a pointer to a struct cando_usock_tcp.
 *
 * @return
 *	on success: Textual path to unix domain socket
 *	on failure: NULL
 */
CANDO_API
const char *
cando_usock_udp_get_unix_path (struct cando_usock_udp *usock);


/*
 * @brief Frees any allocated memory and closes FD's (if open) created after
 *        cando_usock_udp_server_create() or cando_usock_udp_client_create() call.
 *
 * @param usock - Pointer to a valid struct cando_usock_udp.
 */
CANDO_API
void
cando_usock_udp_destroy (struct cando_usock_udp *usock);


/*
 * @brief Returns size of the internal structure. So,
 *        if caller decides to allocate memory outside
 *        of API interface they know the exact amount
 *        of bytes.
 *
 * @return
 *	on success: sizeof(struct cando_usock_udp)
 *	on failure: sizeof(struct cando_usock_udp)
 */
CANDO_API
int
cando_usock_udp_get_sizeof (void);


/*
 * @brief Receive data from socket file descriptor.
 *
 * @param sock_fd    - Socket file descriptor to receive data from.
 * @param data       - Pointer to data to store data received from a socket.
 * @param size       - Size of data to receive from a socket.
 * @param addr       - Pointer to struct sockaddr_un which stores the
 *                     address information of the socket that data
 *                     was received from.
 * @param usock_info - Reserved for future usage. For now used
 *                     to set the flag argument of recvfrom(2).
 *
 * @return
 *	on success: Amount of bytes received
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_usock_udp_recv_data (const int sock_fd,
                           void *data,
                           const size_t size,
                           struct sockaddr_un *addr,
                           const void *usock_info);


/*
 * @brief Send data to socket file descriptor.
 *
 * @param sock_fd    - Socket file descriptor to send data to.
 * @param data       - Pointer to data to send through socket.
 * @param size       - Size of data to send through socket.
 * @param addr       - Pointer to struct sockaddr_un which stores the
 *                     address information of a socket that data
 *                     will be sent to.
 * @param usock_info - Reserved for future usage. For now used
 *                     to set the flag argument of sendto(2).
 *
 * @return
 *	on success: Amount of bytes sent
 *	on failure: # < 0
 */
CANDO_API
ssize_t
cando_usock_udp_send_data (const int sock_fd,
                           const void *data,
                           const size_t size,
                           const struct sockaddr_un *addr,
                           const void *usock_info);

#endif /* CANDO_USOCK_UDP_H */
