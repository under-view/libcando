#include <stdlib.h>
#include <stdio.h>
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
#include "csock-raw.h"

/********************************************
 * Start of test_csock_raw_create functions *
 ********************************************/

static void CANDO_UNUSED
test_csock_raw_create (void CANDO_UNUSED **state)
{
	struct cando_csock_raw *csock = NULL;

	struct cando_csock_raw_create_info csock_info;

	csock_info.iface = TEST_VCAN_IFACE;
	csock = cando_csock_raw_create(NULL, &csock_info);
	assert_non_null(csock);

	cando_csock_raw_destroy(csock);
}

/******************************************
 * End of test_csock_raw_create functions *
 ******************************************/


/***********************************************
 * Start of test_csock_raw_send_recv functions *
 ***********************************************/

static void
p_test_csock_raw_send_recv_client (void)
{
	int err = -1;

	struct can_frame frame;

	struct cando_csock_raw *csock = NULL;

	struct cando_csock_raw_create_info csock_info;

	csock_info.iface = TEST_VCAN_IFACE;
	csock = cando_csock_raw_create(NULL, &csock_info);
	assert_non_null(csock);

	usleep(2000);

	frame.len = 5;
	frame.can_id = 0x555;
	snprintf((char*)frame.data, sizeof(frame.data), "Hello");
	err = cando_csock_raw_send_data(csock, &frame, NULL);
	assert_int_equal(err, sizeof(struct can_frame));

	cando_csock_raw_destroy(csock);

	exit(0);
}


static void CANDO_UNUSED
test_csock_raw_send_recv (void CANDO_UNUSED **state)
{
	pid_t pid;

	int err = -1;

	struct can_frame frame;

	struct cando_csock_raw *csock = NULL;

	struct cando_csock_raw_create_info csock_info;

	cando_log_set_level(CANDO_LOG_ALL);

	pid = fork();
	if (pid == 0) {
		p_test_csock_raw_send_recv_client();
	}

	csock_info.iface = TEST_VCAN_IFACE;
	csock = cando_csock_raw_create(NULL, &csock_info);
	assert_non_null(csock);

	err = cando_csock_raw_recv_data(csock, &frame, NULL);
	assert_int_equal(err, sizeof(struct can_frame));

	assert_int_equal(frame.can_id, 0x555);
	assert_string_equal((char*)frame.data, "Hello");

	waitpid(pid, NULL, 0);

	cando_csock_raw_destroy(csock);
}

/*********************************************
 * End of test_csock_raw_send_recv functions *
 *********************************************/


/********************************************
 * Start of test_csock_raw_get_fd functions *
 ********************************************/

static void CANDO_UNUSED
test_csock_raw_get_fd (void CANDO_UNUSED **state)
{
	int sock_fd = -1;

	struct cando_csock_raw *csock = NULL;

	struct cando_csock_raw_create_info csock_info;

	csock_info.iface = TEST_VCAN_IFACE;
	csock = cando_csock_raw_create(NULL, &csock_info);
	assert_non_null(csock);

	sock_fd = cando_csock_raw_get_fd(NULL);
	assert_int_equal(sock_fd, -1);

	sock_fd = cando_csock_raw_get_fd(csock);
	assert_int_not_equal(sock_fd, -1);

	cando_csock_raw_destroy(csock);
}

/******************************************
 * End of test_csock_raw_get_fd functions *
 ******************************************/


/***********************************************
 * Start of test_csock_raw_get_iface functions *
 ***********************************************/

static void CANDO_UNUSED
test_csock_raw_get_iface (void CANDO_UNUSED **state)
{
	const char *iface = NULL;

	struct cando_csock_raw *csock = NULL;

	struct cando_csock_raw_create_info csock_info;

	csock_info.iface = TEST_VCAN_IFACE;
	csock = cando_csock_raw_create(NULL, &csock_info);
	assert_non_null(csock);

	iface = cando_csock_raw_get_iface(NULL);
	assert_null(iface);

	iface = cando_csock_raw_get_iface(csock);
	assert_string_equal(iface, csock_info.iface);

	cando_csock_raw_destroy(csock);
}

/*********************************************
 * End of test_csock_raw_get_iface functions *
 *********************************************/


/************************************************
 * Start of test_csock_raw_get_sizeof functions *
 ************************************************/

static void CANDO_UNUSED
test_csock_raw_get_sizeof (void CANDO_UNUSED **state)
{
	int size = 0;
	size = cando_csock_raw_get_sizeof();
	assert_int_not_equal(size, 0);
}

/**********************************************
 * End of test_csock_raw_get_sizeof functions *
 **********************************************/

int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_csock_raw_create),
		cmocka_unit_test(test_csock_raw_send_recv),
		cmocka_unit_test(test_csock_raw_get_fd),
		cmocka_unit_test(test_csock_raw_get_iface),
		cmocka_unit_test(test_csock_raw_get_sizeof),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
