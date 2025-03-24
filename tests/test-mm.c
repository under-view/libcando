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

#include "mm.h"

/************************************
 * Start of test_mm_alloc functions *
 ************************************/

static void CANDO_UNUSED
test_mm_alloc (void CANDO_UNUSED **state)
{

}

/**********************************
 * End of test_mm_alloc functions *
 **********************************/

int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_mm_alloc),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
