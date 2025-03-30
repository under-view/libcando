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
#include "macros.h"
#include "mm.h"

/************************************
 * Start of test_mm_alloc functions *
 ************************************/

static void CANDO_UNUSED
test_mm_alloc (void CANDO_UNUSED **state)
{
	struct cando_mm *mm = NULL;

	cando_log_set_level(CANDO_LOG_ALL);

	mm = cando_mm_alloc(NULL, CANDO_PAGE_SIZE*2);
	assert_non_null(mm);

	/* Test remapping */
	mm = cando_mm_alloc(mm, CANDO_PAGE_SIZE*3);
	assert_non_null(mm);

	cando_mm_destroy(mm);
}

/**********************************
 * End of test_mm_alloc functions *
 **********************************/


/****************************************
 * Start of test_mm_sub_alloc functions *
 ****************************************/

static void CANDO_UNUSED
test_mm_sub_alloc (void CANDO_UNUSED **state)
{
	char *mem = NULL;

	struct cando_mm *mm = NULL;

	char buffer[CANDO_PAGE_SIZE];

	cando_log_set_level(CANDO_LOG_ALL);

	mm = cando_mm_alloc(NULL, CANDO_PAGE_SIZE*2);
	assert_non_null(mm);

	mem = cando_mm_sub_alloc(mm, CANDO_PAGE_SIZE);
	assert_non_null(mem);

	memset(buffer, 'G', CANDO_PAGE_SIZE);
	memcpy(mem, buffer, CANDO_PAGE_SIZE);

	/* Test remapping */
	mm = cando_mm_alloc(mm, CANDO_PAGE_SIZE*3);
	assert_non_null(mm);

	assert_memory_equal(buffer, mem, CANDO_PAGE_SIZE);

	cando_mm_destroy(mm);
}

/**************************************
 * End of test_mm_sub_alloc functions *
 **************************************/

int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_mm_alloc),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
