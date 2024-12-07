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

	cando_log_set_level(CANDO_LOG_DANGER);

	flopsCreateInfo.dataSize = (1<<12);
	flopsCreateInfo.fileName = "/tmp/some-file.txt";
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	ret = cando_file_ops_truncate_file(flops, 0);
	assert_int_equal(ret, -1);
	cando_log_err("%s\n", cando_log_get_error(flops));

	ret = stat(flopsCreateInfo.fileName, &fstats);
	assert_int_equal(ret, 0);
	assert_int_equal(fstats.st_size, flopsCreateInfo.dataSize);

	cando_file_ops_destroy(flops);
	remove(flopsCreateInfo.fileName);
}

/*****************************************
 * End of test_file_ops_create functions *
 *****************************************/


/**********************************************
 * Start of test_file_ops_zero_copy functions *
 **********************************************/

static void CANDO_UNUSED
test_file_ops_zero_copy (void CANDO_UNUSED **state)
{
	int ret = -1;

	const char *data = NULL;

	struct cando_file_ops *flops = NULL, *flopsTwo = NULL;

	struct cando_file_ops_create_info flopsCreateInfo;
	struct cando_file_ops_zero_copy_info zeroCopyInfo;

	memset(&zeroCopyInfo, 0, sizeof(zeroCopyInfo));
	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.createPipe = 0x01;
	flopsCreateInfo.fileName = TESTER_FILE_ONE;
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	flopsCreateInfo.createPipe = 0x00;
	flopsCreateInfo.fileName = "/tmp/test-file.txt";
	flopsCreateInfo.dataSize = cando_file_ops_get_data_size(flops);
	flopsTwo = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flopsTwo);

	zeroCopyInfo.dataSize = flopsCreateInfo.dataSize;
	zeroCopyInfo.infd = cando_file_ops_get_fd(flops);
	zeroCopyInfo.inOffset = &(off_t){0};
	zeroCopyInfo.outfd = cando_file_ops_get_fd(flopsTwo);
	zeroCopyInfo.outOffset = &(off_t){0};
	ret = cando_file_ops_zero_copy(flops, &zeroCopyInfo);
	assert_int_equal(ret, 0);

	cando_file_ops_destroy(flops); flops = NULL;
	cando_file_ops_destroy(flopsTwo); flopsTwo = NULL;

	/* Re-open newly created file */
	flopsCreateInfo.fileName = "/tmp/test-file.txt";
	flopsCreateInfo.dataSize = 0;
	flopsTwo = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flopsTwo);

	data = cando_file_ops_get_line(flopsTwo, 1);
	assert_string_equal(data, "line one");

	data = cando_file_ops_get_line(flopsTwo, 4);
	assert_string_equal(data, "line four : check me");

	cando_file_ops_destroy(flopsTwo);
	remove("/tmp/test-file.txt");
}

/********************************************
 * End of test_file_ops_zero_copy functions *
 ********************************************/


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


static void CANDO_UNUSED
test_file_ops_get_fd (void CANDO_UNUSED **state)
{
	int fd = -1;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info flopsCreateInfo;

	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.fileName = TESTER_FILE_ONE;
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	fd = cando_file_ops_get_fd(flops);
	assert_int_not_equal(fd, -1);

	fd = cando_file_ops_get_fd(NULL);
	assert_int_equal(fd, -1);

	cando_file_ops_destroy(flops);
}


static void CANDO_UNUSED
test_file_ops_get_filename (void CANDO_UNUSED **state)
{
	const char *fileName = NULL;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info flopsCreateInfo;

	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.fileName = TESTER_FILE_ONE;
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	fileName = cando_file_ops_get_filename(flops);
	assert_string_equal(fileName, TESTER_FILE_ONE);

	fileName = cando_file_ops_get_filename(NULL);
	assert_null(fileName);

	cando_file_ops_destroy(flops);
}


static void CANDO_UNUSED
test_file_ops_get_data_size (void CANDO_UNUSED **state)
{
	size_t dataSize = 0;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info flopsCreateInfo;

	memset(&flopsCreateInfo, 0, sizeof(flopsCreateInfo));

	flopsCreateInfo.fileName = TESTER_FILE_ONE;
	flops = cando_file_ops_create(&flopsCreateInfo);
	assert_non_null(flops);

	dataSize = cando_file_ops_get_data_size(flops);
	assert_int_not_equal(dataSize, -1);

	dataSize = cando_file_ops_get_data_size(NULL);
	assert_int_equal(dataSize, -1);

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
		cmocka_unit_test(test_file_ops_zero_copy),
		cmocka_unit_test(test_file_ops_get_data),
		cmocka_unit_test(test_file_ops_get_line),
		cmocka_unit_test(test_file_ops_get_line_count),
		cmocka_unit_test(test_file_ops_get_fd),
		cmocka_unit_test(test_file_ops_get_data_size),
		cmocka_unit_test(test_file_ops_get_filename),
		cmocka_unit_test(test_file_ops_set_data),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
