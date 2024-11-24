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


static void CANDO_UNUSED
test_file_ops_get_line (void CANDO_UNUSED **state)
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

	data = cando_file_ops_get_line(flops, 0);
	assert_null(data);

	data = cando_file_ops_get_line(flops, 1);
	assert_string_equal(data, "line one");

	data = cando_file_ops_get_line(flops, 2);
	assert_string_equal(data, "line two");

	data = cando_file_ops_get_line(flops, 4);
	assert_string_equal(data, "line four : check me");

	cando_file_ops_destroy(flops);
}


static void CANDO_UNUSED
test_file_ops_get_line_count (void CANDO_UNUSED **state)
{
	unsigned long int lineCount = 0;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info flopsCreateInfo;

	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.fileName = TESTER_FILE_ONE;
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	lineCount = cando_file_ops_get_line_count(flops);
	assert_int_equal(lineCount, 8);

	cando_file_ops_destroy(flops);
}

/**************************************
 * End of test_file_ops_get functions *
 **************************************/


/****************************************
 * Start of test_file_ops_set functions *
 ****************************************/

static void CANDO_UNUSED
test_file_ops_set_data (void CANDO_UNUSED **state)
{
	int ret = -1;

	const void *data = NULL;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info flopsCreateInfo;
	struct cando_file_ops_set_data_info setDataInfo;

	memset(&setDataInfo, 0, sizeof(setDataInfo));
	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.fileName = "/tmp/testing-one.txt";
	flopsCreateInfo.dataSize = 1 << 9;
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	setDataInfo.offset = 0;
	setDataInfo.data = "Adding data on line one.\n";
	setDataInfo.dataSize = strnlen(setDataInfo.data, flopsCreateInfo.dataSize);
	ret = cando_file_ops_set_data(flops, &setDataInfo);
	assert_int_equal(ret, 0);

	data = cando_file_ops_get_data(flops, 0);
	assert_string_equal(data, setDataInfo.data);

	cando_file_ops_destroy(flops);
}

/**************************************
 * End of test_file_ops_set functions *
 **************************************/


int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_file_ops_create),
		cmocka_unit_test(test_file_ops_create_empty_file),
		cmocka_unit_test(test_file_ops_get_data),
		cmocka_unit_test(test_file_ops_get_line),
		cmocka_unit_test(test_file_ops_get_line_count),
		cmocka_unit_test(test_file_ops_set_data),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
