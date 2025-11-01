#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>

/* Required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "log.h"
#include "futex.h"

/*********************************************
 * Start of test_futex_lock_unlock functions *
 *********************************************/

static void CANDO_UNUSED
test_futex_lock_unlock (void CANDO_UNUSED **state)
{
	pid_t pid;

	cando_atomic_u32 *fux = NULL;

	fux = mmap(NULL, sizeof(cando_atomic_u32),
	           PROT_READ | PROT_WRITE,
                   MAP_SHARED | MAP_ANONYMOUS,
	           -1, 0);
	assert_non_null(fux);

	__atomic_store_n(fux, 1, __ATOMIC_RELEASE);

	pid = fork();
	if (pid == 0) {
		cando_futex_unlock(fux);

		exit(0);
	}

	cando_futex_lock(fux);
	munmap(fux, sizeof(cando_atomic_u32));
}

/*******************************************
 * End of test_futex_lock_unlock functions *
 *******************************************/


/***************************************************
 * Start of test_futex_lock_unlock_force functions *
 ***************************************************/

static void CANDO_UNUSED
test_futex_lock_unlock_force (void CANDO_UNUSED **state)
{
	pid_t pid;

	cando_atomic_u32 *fux = NULL;

	fux = mmap(NULL, sizeof(cando_atomic_u32),
	           PROT_READ | PROT_WRITE,
                   MAP_SHARED | MAP_ANONYMOUS,
	           -1, 0);
	assert_non_null(fux);

	cando_log_set_level(CANDO_LOG_ALL);

	__atomic_store_n(fux, 1, __ATOMIC_RELEASE);

	pid = fork();
	if (pid == 0) {
		cando_futex_unlock_force(fux);

		exit(0);
	}

	cando_futex_lock(fux);
	assert_int_equal(errno, EINTR);
	munmap(fux, sizeof(cando_atomic_u32));
}

/*************************************************
 * End of test_futex_lock_unlock_force functions *
 *************************************************/

int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_futex_lock_unlock),
		cmocka_unit_test(test_futex_lock_unlock_force),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
