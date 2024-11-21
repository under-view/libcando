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


int
main (void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_file_ops_create),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
