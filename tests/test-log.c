#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
 * Required by cmocka
 */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "log.h"


/*******************************
 * Start of test_log functions *
 *******************************/

static void CANDO_UNUSED
test_log (void CANDO_UNUSED **state)
{
	cando_log(CANDO_LOG_SUCCESS, "SUCCESS\n");
	cando_log(CANDO_LOG_DANGER, "DANGER\n");
	cando_log(CANDO_LOG_INFO, "INFO\n");
	cando_log(CANDO_LOG_WARNING, "WARNING\n");

	cando_log_level_set(CANDO_LOG_DANGER|CANDO_LOG_INFO);
	cando_log(CANDO_LOG_SUCCESS, "SUCCESS: After log level set\n");
	cando_log(CANDO_LOG_DANGER, "DANGER: After log level set\n");
	cando_log(CANDO_LOG_INFO, "INFO: After log level set\n");
	cando_log(CANDO_LOG_WARNING, "WARNING: After log level set\n");
}

/*****************************
 * End of test_log functions *
 *****************************/


/*************************************
 * Start of test_log_print functions *
 *************************************/

static void CANDO_UNUSED
test_log_print (void CANDO_UNUSED **state)
{
	cando_log_print(CANDO_LOG_SUCCESS, "SUCCESS\n");
	cando_log_print(CANDO_LOG_DANGER, "DANGER\n");
	cando_log_print(CANDO_LOG_INFO, "INFO\n");
	cando_log_print(CANDO_LOG_WARNING, "WARNING\n");

	cando_log_level_set(CANDO_LOG_SUCCESS|CANDO_LOG_WARNING);
	cando_log_print(CANDO_LOG_SUCCESS, "SUCCESS: After log level set\n");
	cando_log_print(CANDO_LOG_DANGER, "DANGER: After log level set\n");
	cando_log_print(CANDO_LOG_INFO, "INFO: After log level set\n");
	cando_log_print(CANDO_LOG_WARNING, "WARNING: After log level set\n");
}

/***********************************
 * End of test_log_print functions *
 ***********************************/


/********************************************
 * Start of test_log_set_write_fd functions *
 ********************************************/

static void CANDO_UNUSED
test_log_set_write_fd (void CANDO_UNUSED **state)
{
	int fd = -1;

	const char *testFile = "/tmp/test-file.txt";
	fd = open(testFile, O_CREAT|O_RDWR, 0644);
	assert_int_not_equal(fd, -1);

	cando_log_write_fd_set(fd);

	cando_log_print(CANDO_LOG_SUCCESS, "SUCCESS\n");
	cando_log_print(CANDO_LOG_DANGER, "DANGER\n");
	cando_log_print(CANDO_LOG_INFO, "INFO\n");
	cando_log_print(CANDO_LOG_WARNING, "WARNING\n");

	cando_log_level_set(CANDO_LOG_SUCCESS|CANDO_LOG_WARNING);
	cando_log_print(CANDO_LOG_SUCCESS, "SUCCESS: After log level set\n");
	cando_log_print(CANDO_LOG_DANGER, "DANGER: After log level set\n");
	cando_log_print(CANDO_LOG_INFO, "INFO: After log level set\n");
	cando_log_print(CANDO_LOG_WARNING, "WARNING: After log level set\n");

	close(fd);
	remove(testFile);
}

/******************************************
 * End of test_log_set_write_fd functions *
 ******************************************/


/*************************************
 * Start of test_log_error functions *
 *************************************/

static void CANDO_UNUSED
test_log_error (void CANDO_UNUSED **state)
{
	unsigned int errCode = 0;
	const char *error = NULL;

	struct some_context
	{
		struct cando_log_error_struct err;
		int someData;
	} context;

	/* Test NULL passed */
	cando_log_set_global_error(55, "some arbitrary buffer");
	errCode = cando_log_get_error_code(NULL);
	error = cando_log_get_error(NULL);
	assert_int_equal(errCode, 55);
	assert_string_equal(error, "some arbitrary buffer");

	/* Test context passed */
	context.err.code = 871;
	strncpy(context.err.buffer, "BUFFFER 2 Copy", CANDO_PAGE_SIZE-1);
	errCode = cando_log_get_error_code(&context);
	error = cando_log_get_error(&context);
	assert_int_equal(errCode, 871);
	assert_string_equal(error, "BUFFFER 2 Copy");
}

/***********************************
 * End of test_log_error functions *
 ***********************************/


int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_log),
		cmocka_unit_test(test_log_print),
		cmocka_unit_test(test_log_set_write_fd),
		cmocka_unit_test(test_log_error),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
