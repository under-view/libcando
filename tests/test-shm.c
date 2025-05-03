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

int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_shm_create),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
