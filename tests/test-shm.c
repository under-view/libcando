#include <string.h>
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
#include "shm.h"

/**************************************
 * Start of test_shm_create functions *
 **************************************/

static void CANDO_UNUSED
test_shm_create (void CANDO_UNUSED **state)
{
	struct cando_shm *shm = NULL;

	struct cando_shm_create_info shm_info;
	memset(&shm_info, 0, sizeof(shm_info));

	/* Test shm_size zero */
	shm_info.shm_file = "/kms-shm-testing";
	shm_info.sem_file = "/kms-sem-testing";
	shm = cando_shm_create(NULL, &shm_info);
	assert_null(shm);

	/* Test shm no leading '/' */
	shm_info.shm_file = "kms-shm-testing";
	shm_info.sem_file = "/kms-sem-testing";
	shm_info.shm_size  = CANDO_PAGE_SIZE;
	shm = cando_shm_create(NULL, &shm_info);
	assert_null(shm);

	/* Test shm name to long */
	shm_info.shm_file = "/kms-shm-testing-XXXXXXXXXXXXXXX";
	shm_info.sem_file = "/kms-sem-testing";
	shm = cando_shm_create(NULL, &shm_info);
	assert_null(shm);

	/* Test sem no leading '/' */
	shm_info.shm_file = "/kms-shm-testing";
	shm_info.sem_file = "kms-sem-testing";
	shm = cando_shm_create(NULL, &shm_info);
	assert_null(shm);

	/* Test sem name to long */
	shm_info.shm_file = "/kms-shm-testing";
	shm_info.sem_file = "/kms-sem-testing-XXXXXXXXXXXXXXX";
	shm = cando_shm_create(NULL, &shm_info);
	assert_null(shm);

	/* Test correct info passed */
	shm_info.shm_file  = "/kms-shm-testing";
	shm_info.sem_file  = "/kms-sem-testing";
	shm_info.shm_size  = CANDO_PAGE_SIZE;
	shm_info.sem_count = 1;
	shm = cando_shm_create(NULL, &shm_info);
	assert_non_null(shm);

	cando_shm_destroy(shm);
}

/************************************
 * End of test_shm_create functions *
 ************************************/


/************************************
 * Start of test_shm_data functions *
 ************************************/

static void CANDO_UNUSED
test_shm_data (void **state CANDO_UNUSED)
{
	pid_t pid = fork();
	if (pid == 0) {
		int err = -1, data = 821;

		struct cando_shm *shm = NULL;

		struct cando_shm_create_info shm_info;
		struct cando_shm_data_info shm_data_info;

		memset(&shm_info, 0, sizeof(shm_info));

		shm_info.shm_file  = "/kms-shm-testing";
		shm_info.sem_file  = "/kms-sem-testing";
		shm_info.shm_size  = CANDO_PAGE_SIZE;
		shm_info.sem_count = 1;
		shm = cando_shm_create(NULL, &shm_info);
		assert_non_null(shm);

		shm_data_info.block = 1;
		shm_data_info.offset = 0;
		shm_data_info.sem_index = 0;
		shm_data_info.size = sizeof(int);
		shm_data_info.data = (void*) &data;
		err = cando_shm_data_write(shm, &shm_data_info);
		assert_int_equal(err, 0);

		cando_shm_destroy(shm);
	} else {
		int err = -1, data = -4;

		struct cando_shm *shm = NULL;

		struct cando_shm_create_info shm_info;
		struct cando_shm_data_info shm_data_info;

		memset(&shm_info, 0, sizeof(shm_info));

		shm_info.shm_file  = "/kms-shm-testing";
		shm_info.sem_file  = "/kms-sem-testing";
		shm_info.shm_size  = CANDO_PAGE_SIZE;
		shm_info.sem_count = 1;
		shm = cando_shm_create(NULL, &shm_info);
		assert_non_null(shm);

		shm_data_info.block = 1;
		shm_data_info.offset = 0;
		shm_data_info.sem_index = 0;
		shm_data_info.size = sizeof(int);
		shm_data_info.data = (void*) &data;
		err = cando_shm_data_read(shm, &shm_data_info);
		assert_int_equal(err, 0);
		assert_int_equal(data, 821);

		cando_shm_destroy(shm);
	}
}

/**********************************
 * End of test_shm_data functions *
 **********************************/


/**************************************
 * Start of test_shm_get_fd functions *
 **************************************/

static void CANDO_UNUSED
test_shm_get_fd (void **state CANDO_UNUSED)
{
	int fd = -1;

	struct cando_shm *shm = NULL;

	struct cando_shm_create_info shm_info;
	memset(&shm_info, 0, sizeof(shm_info));

	shm_info.shm_file  = "/kms-shm-testing";
	shm_info.sem_file  = "/kms-sem-testing";
	shm_info.shm_size  = CANDO_PAGE_SIZE;
	shm_info.sem_count = 1;
	shm = cando_shm_create(NULL, &shm_info);
	assert_non_null(shm);

	fd = cando_shm_get_fd(NULL);
	assert_int_equal(fd, -1);

	fd = cando_shm_get_fd(shm);
	assert_int_not_equal(fd, -1);

	cando_shm_destroy(shm);
}

/************************************
 * End of test_shm_get_fd functions *
 ************************************/


/****************************************
 * Start of test_shm_get_data functions *
 ****************************************/

static void CANDO_UNUSED
test_shm_get_data (void **state CANDO_UNUSED)
{
	void *shm_data = NULL;

	char buffer[CANDO_PAGE_SIZE];

	struct cando_shm *shm = NULL;

	struct cando_shm_create_info shm_info;
	memset(&shm_info, 0, sizeof(shm_info));

	memset(buffer, 'C', sizeof(buffer));

	shm_info.shm_file  = "/kms-shm-testing";
	shm_info.sem_file  = "/kms-sem-testing";
	shm_info.shm_size  = CANDO_PAGE_SIZE;
	shm_info.sem_count = 1;
	shm = cando_shm_create(NULL, &shm_info);
	assert_non_null(shm);

	shm_data = cando_shm_get_data(NULL);
	assert_null(shm_data);

	shm_data = cando_shm_get_data(shm);
	assert_non_null(shm_data);

	memset(shm_data, 'C', shm_info.shm_size);
	assert_memory_equal(shm_data, buffer, shm_info.shm_size);

	cando_shm_destroy(shm);
}

/**************************************
 * End of test_shm_get_data functions *
 **************************************/


/*********************************************
 * Start of test_shm_get_data_size functions *
 *********************************************/

static void CANDO_UNUSED
test_shm_get_data_size (void **state CANDO_UNUSED)
{
	size_t data_sz = 0;

	struct cando_shm *shm = NULL;

	struct cando_shm_create_info shm_info;
	memset(&shm_info, 0, sizeof(shm_info));

	shm_info.shm_file  = "/kms-shm-testing";
	shm_info.sem_file  = "/kms-sem-testing";
	shm_info.shm_size  = CANDO_PAGE_SIZE;
	shm_info.sem_count = 1;
	shm = cando_shm_create(NULL, &shm_info);
	assert_non_null(shm);

	data_sz = cando_shm_get_data_size(NULL);
	assert_int_equal(data_sz, -1);

	data_sz = cando_shm_get_data_size(shm);
	assert_int_equal(data_sz, shm_info.shm_size);

	cando_shm_destroy(shm);
}

/*******************************************
 * End of test_shm_get_data_size functions *
 *******************************************/

int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_shm_create),
		cmocka_unit_test(test_shm_data),
		cmocka_unit_test(test_shm_get_fd),
		cmocka_unit_test(test_shm_get_data),
		cmocka_unit_test(test_shm_get_data_size),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
