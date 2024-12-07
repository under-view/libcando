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

	cando_log_set_level(CANDO_LOG_DANGER|CANDO_LOG_INFO);
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

	cando_log_set_level(CANDO_LOG_SUCCESS|CANDO_LOG_WARNING);
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

	cando_log_set_write_fd(fd);

	cando_log_print(CANDO_LOG_SUCCESS, "SUCCESS\n");
	cando_log_print(CANDO_LOG_DANGER, "DANGER\n");
	cando_log_print(CANDO_LOG_INFO, "INFO\n");
	cando_log_print(CANDO_LOG_WARNING, "WARNING\n");

	cando_log_set_level(CANDO_LOG_SUCCESS|CANDO_LOG_WARNING);
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
	errCode = cando_log_get_error_code(NULL);
	error = cando_log_get_error(NULL);
	assert_int_equal(errCode, UINT32_MAX);
	assert_null(error);

	/* Test common error passed */
	context.err.code = CANDO_LOG_ERR_INCORRECT_DATA;
	errCode = cando_log_get_error_code(&context);
	error = cando_log_get_error(&context);
	assert_int_equal(errCode, CANDO_LOG_ERR_INCORRECT_DATA);
	assert_string_equal(error, "Incorrect data passed");

	/* Test context passed */
	memset(&context, 0, sizeof(context));

	context.err.code = CANDO_LOG_ERR_UNCOMMON;
	strncpy(context.err.buffer, "BUFFFER 2 Copy", sizeof(context.err.buffer)-1);
	errCode = cando_log_get_error_code(&context);
	error = cando_log_get_error(&context);
	assert_int_equal(errCode, CANDO_LOG_ERR_UNCOMMON);
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
