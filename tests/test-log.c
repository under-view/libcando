#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/*
 * Required by cmocka
 */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "log.h"


static void CANDO_UNUSED
test_log (void CANDO_UNUSED **state) {
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


static void CANDO_UNUSED
test_log_print (void CANDO_UNUSED **state) {
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


static void CANDO_UNUSED
test_log_set_write_fd (void CANDO_UNUSED **state) {
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


int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_log),
		cmocka_unit_test(test_log_print),
		cmocka_unit_test(test_log_set_write_fd),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
