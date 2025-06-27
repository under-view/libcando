#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * Required by cmocka
 */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "log.h"
#include "usock-tcp.h"

#define TESTING_UNIX_SOCK "/tmp/testing.sock"

/***************************************************
 * Start of test_usock_tcp_server_create functions *
 ***************************************************/

static void CANDO_UNUSED
test_usock_tcp_server_create (void CANDO_UNUSED **state)
{
	struct cando_usock_tcp *server = NULL;

	struct cando_usock_tcp_server_create_info server_info;

	server_info.connections = 1;
	server_info.unix_path = TESTING_UNIX_SOCK;
	server = cando_usock_tcp_server_create(NULL, &server_info);
	assert_non_null(server);

	cando_usock_tcp_destroy(server);
}

/*************************************************
 * End of test_usock_tcp_server_create functions *
 *************************************************/


/***************************************************
 * Start of test_usock_tcp_client_create functions *
 ***************************************************/

static void CANDO_UNUSED
test_usock_tcp_client_create (void CANDO_UNUSED **state)
{
	struct cando_usock_tcp *client = NULL;

	struct cando_usock_tcp_client_create_info client_info;

	client_info.unix_path = TESTING_UNIX_SOCK;
	client = cando_usock_tcp_client_create(NULL, &client_info);
	assert_non_null(client);

	cando_usock_tcp_destroy(client);
}

/*************************************************
 * End of test_usock_tcp_client_create functions *
 *************************************************/


/****************************************************
 * Start of test_usock_tcp_accept_connect functions *
 ****************************************************/

static void
p_test_usock_tcp_accept_connect_client (void)
{
	int err = -1;

	struct cando_usock_tcp *client = NULL;

	struct cando_usock_tcp_client_create_info client_info;

	client_info.unix_path = TESTING_UNIX_SOCK;
	client = cando_usock_tcp_client_create(NULL, &client_info);
	assert_non_null(client);

	err = cando_usock_tcp_client_connect(client);
	assert_int_equal(err, 0);

	cando_usock_tcp_destroy(client);

	exit(0);
}


static void CANDO_UNUSED
test_usock_tcp_accept_connect (void CANDO_UNUSED **state)
{
	pid_t pid;

	int client_sock = -1;

	struct cando_usock_tcp *server = NULL;

	struct cando_usock_tcp_server_create_info server_info;

	cando_log_set_level(CANDO_LOG_ALL);

	server_info.connections = 1;
	server_info.unix_path = TESTING_UNIX_SOCK;
	server = cando_usock_tcp_server_create(NULL, &server_info);
	assert_non_null(server);

	pid = fork();
	if (pid == 0) {
		p_test_usock_tcp_accept_connect_client();
	}

	client_sock = cando_usock_tcp_server_accept(server, NULL);
	assert_int_not_equal(client_sock, -1);

	waitpid(pid, NULL, -1);

	close(client_sock);
	cando_usock_tcp_destroy(server);
}

/**************************************************
 * End of test_usock_tcp_accept_connect functions *
 **************************************************/


/***********************************************
 * Start of test_usock_tcp_send_recv functions *
 ***********************************************/

static void
p_test_usock_tcp_send_recv_client (void)
{
	int err = -1;

	char buffer[512];

	ssize_t size = 0;

	struct cando_usock_tcp *client = NULL;

	struct cando_usock_tcp_client_create_info client_info;

	client_info.unix_path = TESTING_UNIX_SOCK;
	client = cando_usock_tcp_client_create(NULL, &client_info);
	assert_non_null(client);

	err = cando_usock_tcp_client_connect(client);
	assert_int_equal(err, 0);

	usleep(2000);

	memset(buffer, 'T', sizeof(buffer));
	size = cando_usock_tcp_client_send_data(client, buffer, sizeof(buffer), 0);
	assert_int_equal(size, sizeof(buffer));

	cando_usock_tcp_destroy(client);

	exit(0);
}


static void CANDO_UNUSED
test_usock_tcp_send_recv (void CANDO_UNUSED **state)
{
	pid_t pid;

	int client_sock = -1, recv = 15;

	char buffer[512], buffer_two[512];

	struct cando_usock_tcp *server = NULL;

	struct cando_usock_tcp_server_create_info server_info;

	cando_log_set_level(CANDO_LOG_ALL);

	server_info.connections = 1;
	server_info.unix_path = TESTING_UNIX_SOCK;
	server = cando_usock_tcp_server_create(NULL, &server_info);
	assert_non_null(server);

	pid = fork();
	if (pid == 0) {
		p_test_usock_tcp_send_recv_client();
	}

	client_sock = cando_usock_tcp_server_accept(server, NULL);
	assert_int_not_equal(client_sock, -1);

	memset(buffer, 'T', sizeof(buffer));
	cando_usock_tcp_recv_data(client_sock, buffer_two, sizeof(buffer_two), 0);
	assert_memory_equal(buffer, buffer_two, sizeof(buffer));

	/* Test client disconnects */
	memset(buffer_two, 0, sizeof(buffer_two));
	recv = cando_usock_tcp_recv_data(client_sock, buffer_two, sizeof(buffer_two), 0);
	assert_int_equal(recv, 0);

	waitpid(pid, NULL, -1);

	close(client_sock);
	cando_usock_tcp_destroy(server);
}

/*********************************************
 * End of test_usock_tcp_send_recv functions *
 *********************************************/


/********************************************
 * Start of test_usock_tcp_get_fd functions *
 ********************************************/

static void CANDO_UNUSED
test_usock_tcp_get_fd (void CANDO_UNUSED **state)
{
	int sock_fd = -1;

	struct cando_usock_tcp *client = NULL;

	struct cando_usock_tcp_client_create_info client_info;

	client_info.unix_path = TESTING_UNIX_SOCK;
	client = cando_usock_tcp_client_create(NULL, &client_info);
	assert_non_null(client);

	sock_fd = cando_usock_tcp_get_fd(NULL);
	assert_int_equal(sock_fd, -1);

	sock_fd = cando_usock_tcp_get_fd(client);
	assert_int_not_equal(sock_fd, -1);

	cando_usock_tcp_destroy(client);
}

/******************************************
 * End of test_usock_tcp_get_fd functions *
 ******************************************/


/***************************************************
 * Start of test_usock_tcp_get_unix_path functions *
 ***************************************************/

static void CANDO_UNUSED
test_usock_tcp_get_unix_path (void CANDO_UNUSED **state)
{
	const char *unix_path = NULL;

	struct cando_usock_tcp *server = NULL;

	struct cando_usock_tcp_server_create_info server_info;

	server_info.connections = 1;
	server_info.unix_path = TESTING_UNIX_SOCK;
	server = cando_usock_tcp_server_create(NULL, &server_info);
	assert_non_null(server);

	unix_path = cando_usock_tcp_get_unix_path(NULL);
	assert_null(unix_path);

	unix_path = cando_usock_tcp_get_unix_path(server);
	assert_string_equal(unix_path, server_info.unix_path);

	cando_usock_tcp_destroy(server);
}

/*************************************************
 * End of test_usock_tcp_get_unix_path functions *
 *************************************************/


/************************************************
 * Start of test_usock_tcp_get_sizeof functions *
 ************************************************/

static void CANDO_UNUSED
test_usock_tcp_get_sizeof (void CANDO_UNUSED **state)
{
	int size = 0;
	size = cando_usock_tcp_get_sizeof();
	assert_int_not_equal(size, 0);
}

/**********************************************
 * End of test_usock_tcp_get_sizeof functions *
 **********************************************/

int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_usock_tcp_server_create),
		cmocka_unit_test(test_usock_tcp_client_create),
		cmocka_unit_test(test_usock_tcp_accept_connect),
		cmocka_unit_test(test_usock_tcp_send_recv),
		cmocka_unit_test(test_usock_tcp_get_fd),
		cmocka_unit_test(test_usock_tcp_get_unix_path),
		cmocka_unit_test(test_usock_tcp_get_sizeof),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
