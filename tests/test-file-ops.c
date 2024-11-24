#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/* Required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "log.h"
#include "file-ops.h"

/*******************************************
 * Start of test_file_ops_create functions *
 *******************************************/

static void CANDO_UNUSED
test_file_ops_create (void CANDO_UNUSED **state)
{
	int ret = -1;

	struct cando_file_ops *flops = NULL;

	struct stat fstats;
	struct cando_file_ops_create_info flopsCreateInfo;

	memset(&fstats, 0, sizeof(fstats));
	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.fileName = "/tmp/some-file.txt";
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	ret = stat(flopsCreateInfo.fileName, &fstats);
	assert_int_equal(ret, 0);

	cando_file_ops_destroy(flops);
	remove(flopsCreateInfo.fileName);
}


static void CANDO_UNUSED
test_file_ops_create_empty_file (void CANDO_UNUSED **state)
{
	int ret = -1;

	struct cando_file_ops *flops = NULL;

	struct stat fstats;
	struct cando_file_ops_create_info flopsCreateInfo;

	memset(&fstats, 0, sizeof(fstats));
	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.dataSize = (1<<12);
	flopsCreateInfo.fileName = "/tmp/some-file.txt";
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	ret = stat(flopsCreateInfo.fileName, &fstats);
	assert_int_equal(ret, 0);
	assert_int_equal(fstats.st_size, flopsCreateInfo.dataSize);

	cando_file_ops_destroy(flops);
	remove(flopsCreateInfo.fileName);
}

/*****************************************
 * End of test_file_ops_create functions *
 *****************************************/


/****************************************
 * Start of test_file_ops_get functions *
 ****************************************/

static void CANDO_UNUSED
test_file_ops_get_data (void CANDO_UNUSED **state)
{
	const void *data = NULL;

	struct cando_file_ops *flops = NULL;

	struct stat fstats;
	struct cando_file_ops_create_info flopsCreateInfo;

	memset(&fstats, 0, sizeof(fstats));
	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.fileName = TESTER_FILE_ONE;
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	data = cando_file_ops_get_data(flops, 0);
	assert_non_null(data);

	cando_file_ops_destroy(flops);
}

/**************************************
 * End of test_file_ops_get functions *
 **************************************/


int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_file_ops_create),
		cmocka_unit_test(test_file_ops_create_empty_file),
		cmocka_unit_test(test_file_ops_get_data),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
