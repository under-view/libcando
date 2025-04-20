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

	struct stat fstats;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info finfo;

	memset(&fstats, 0, sizeof(fstats));
	memset(&finfo, 0, sizeof(finfo));

	finfo.fname = "/tmp/some-file.txt";
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	ret = stat(finfo.fname, &fstats);
	assert_int_equal(ret, 0);

	cando_file_ops_destroy(flops);
	remove(finfo.fname);
}


static void CANDO_UNUSED
test_file_ops_create_empty_file (void CANDO_UNUSED **state)
{
	int ret = -1;

	struct stat fstats;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info finfo;

	memset(&fstats, 0, sizeof(fstats));
	memset(&finfo, 0, sizeof(finfo));

	cando_log_set_level(CANDO_LOG_ERROR);

	finfo.size = (1<<12);
	finfo.fname = "/tmp/some-file.txt";
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	ret = stat(finfo.fname, &fstats);
	assert_int_equal(ret, 0);
	assert_int_equal(fstats.st_size, finfo.size);

	cando_file_ops_destroy(flops);
	remove(finfo.fname);
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

	char buffer[32];

	const char *data = NULL;

	struct cando_file_ops *flops = NULL, *flops_two = NULL;

	struct cando_file_ops_create_info finfo;
	struct cando_file_ops_zero_copy_info zcopy_info;

	memset(&zcopy_info, 0, sizeof(zcopy_info));
	memset(&finfo, 0, sizeof(finfo));

	finfo.create_pipe = 0x01;
	finfo.fname = TESTER_FILE_ONE;
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	finfo.create_pipe = 0x00;
	finfo.fname = "/tmp/test-file.txt";
	finfo.size = cando_file_ops_get_data_size(flops);
	flops_two = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops_two);

	zcopy_info.size = finfo.size;
	zcopy_info.in_fd = cando_file_ops_get_fd(flops);
	zcopy_info.in_off = &(off_t){0};
	zcopy_info.out_fd = cando_file_ops_get_fd(flops_two);
	zcopy_info.out_off = &(off_t){0};
	ret = cando_file_ops_zero_copy(flops, &zcopy_info);
	assert_int_equal(ret, 0x62);

	cando_file_ops_destroy(flops); flops = NULL;
	cando_file_ops_destroy(flops_two); flops_two = NULL;

	/* Re-open newly created file */
	finfo.fname = "/tmp/test-file.txt";
	finfo.size = 0;
	flops_two = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops_two);

	memset(buffer, 0, sizeof(buffer));
	data = cando_file_ops_get_line(flops_two, 1);
	memccpy(buffer, data, '\n', sizeof(buffer));
	assert_string_equal(buffer, "line one\n");

	memset(buffer, 0, sizeof(buffer));
	data = cando_file_ops_get_line(flops_two, 4);
	memccpy(buffer, data, '\n', sizeof(buffer));
	assert_string_equal(buffer, "line four : check me\n");

	cando_file_ops_destroy(flops_two);
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
	struct stat fstats;

	const void *data = NULL;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info finfo;

	memset(&fstats, 0, sizeof(fstats));
	memset(&finfo, 0, sizeof(finfo));

	finfo.fname = TESTER_FILE_ONE;
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	data = cando_file_ops_get_data(flops, 0);
	assert_non_null(data);

	cando_file_ops_destroy(flops);
}


static void CANDO_UNUSED
test_file_ops_get_line (void CANDO_UNUSED **state)
{
	char buffer[32];

	struct stat fstats;

	const void *data = NULL;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info finfo;

	memset(&fstats, 0, sizeof(fstats));
	memset(&finfo, 0, sizeof(finfo));

	finfo.fname = TESTER_FILE_ONE;
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	data = cando_file_ops_get_line(flops, 0);
	assert_null(data);

	memset(buffer, 0, sizeof(buffer));
	data = cando_file_ops_get_line(flops, 1);
	memccpy(buffer, data, '\n', sizeof(buffer));
	assert_string_equal(buffer, "line one\n");

	memset(buffer, 0, sizeof(buffer));
	data = cando_file_ops_get_line(flops, 2);
	memccpy(buffer, data, '\n', sizeof(buffer));
	assert_string_equal(buffer, "line two\n");

	memset(buffer, 0, sizeof(buffer));
	data = cando_file_ops_get_line(flops, 4);
	memccpy(buffer, data, '\n', sizeof(buffer));
	assert_string_equal(buffer, "line four : check me\n");

	cando_file_ops_destroy(flops);
}


static void CANDO_UNUSED
test_file_ops_get_line_count (void CANDO_UNUSED **state)
{
	unsigned long int line_count = 0;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info finfo;

	memset(&finfo, 0, sizeof(finfo));

	finfo.fname = TESTER_FILE_ONE;
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	line_count = cando_file_ops_get_line_count(flops);
	assert_int_equal(line_count, 8);

	cando_file_ops_destroy(flops);
}


static void CANDO_UNUSED
test_file_ops_get_fd (void CANDO_UNUSED **state)
{
	int fd = -1;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info finfo;

	memset(&finfo, 0, sizeof(finfo));

	finfo.fname = TESTER_FILE_ONE;
	flops = cando_file_ops_create(NULL, &finfo);
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
	const char *fname = NULL;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info finfo;

	memset(&finfo, 0, sizeof(finfo));

	finfo.fname = TESTER_FILE_ONE;
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	fname = cando_file_ops_get_filename(flops);
	assert_string_equal(fname, TESTER_FILE_ONE);

	fname = cando_file_ops_get_filename(NULL);
	assert_null(fname);

	cando_file_ops_destroy(flops);
}


static void CANDO_UNUSED
test_file_ops_get_data_size (void CANDO_UNUSED **state)
{
	size_t size = 0;

	struct cando_file_ops *flops = NULL;

	struct cando_file_ops_create_info finfo;

	memset(&finfo, 0, sizeof(finfo));

	finfo.fname = TESTER_FILE_ONE;
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	size = cando_file_ops_get_data_size(flops);
	assert_int_not_equal(size, -1);

	size = cando_file_ops_get_data_size(NULL);
	assert_int_equal(size, -1);

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

	struct cando_file_ops_create_info finfo;
	struct cando_file_ops_set_data_info sd_info;

	memset(&sd_info, 0, sizeof(sd_info));
	memset(&finfo, 0, sizeof(finfo));

	finfo.fname = "/tmp/testing-one.txt";
	finfo.size = 1 << 9;
	flops = cando_file_ops_create(NULL, &finfo);
	assert_non_null(flops);

	sd_info.offset = 0;
	sd_info.data = "Adding data on line one.\n";
	sd_info.size = strnlen(sd_info.data, finfo.size);
	ret = cando_file_ops_set_data(flops, &sd_info);
	assert_int_equal(ret, 0);

	data = cando_file_ops_get_data(flops, 0);
	assert_string_equal(data, sd_info.data);

	cando_file_ops_destroy(flops);
}

/**************************************
 * End of test_file_ops_set functions *
 **************************************/


/***********************************************
 * Start of test_file_ops_get_sizeof functions *
 ***********************************************/

static void CANDO_UNUSED
test_file_ops_get_sizeof (void CANDO_UNUSED **state)
{
	int size = 0;
	size = cando_file_ops_get_sizeof();
	assert_int_not_equal(size, 0);
}

/*********************************************
 * End of test_file_ops_get_sizeof functions *
 *********************************************/

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
		cmocka_unit_test(test_file_ops_get_sizeof),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
