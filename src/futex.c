#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <linux/futex.h>  /* Definition of FUTEX_* constants */
#include <sys/syscall.h>  /* Definition of SYS_* constants */

#include "futex.h"

#define CANDO_FUTEX_LOCK 1
#define CANDO_FUTEX_UNLOCK 0
#define CANDO_FUTEX_UNLOCK_FORCE 0x66AFB55C

CANDO_STATIC_INLINE
int
futex (void *uaddr,
       int op,
       uint32_t val,
       const struct timespec *timeout,
       void *uaddr2,
       uint32_t val3)
{
	return syscall(SYS_futex, uaddr, op, val,
	               timeout, uaddr2, val3);
}


CANDO_STATIC_INLINE
unsigned char
p_is_futex_funlock (cando_atomic_u32 *fux)
{
	return __atomic_load_n(fux, __ATOMIC_ACQUIRE) == CANDO_FUTEX_UNLOCK_FORCE;
}


void
cando_futex_lock (cando_atomic_u32 *fux)
{
	unsigned int expected = CANDO_FUTEX_UNLOCK;
	const unsigned int desired = CANDO_FUTEX_LOCK;

	if (!fux)
		return;

	while (1) {
		if (p_is_futex_funlock(fux))
		{
			errno = EINTR;
			break;
		}

		if (__atomic_compare_exchange_n(fux, &expected,
			desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
		{
			break;
		}

		futex(fux, FUTEX_WAIT, desired, NULL, NULL, 0);
	}
}


void
cando_futex_unlock (cando_atomic_u32 *fux)
{
	if (!fux)
		return;

	__atomic_store_n(fux, CANDO_FUTEX_UNLOCK, __ATOMIC_RELEASE);
	futex(fux, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
}


void
cando_futex_unlock_force (cando_atomic_u32 *fux)
{
	if (!fux)
		return;

	__atomic_store_n(fux, CANDO_FUTEX_UNLOCK_FORCE, __ATOMIC_RELEASE);
	futex(fux, FUTEX_WAKE, INT_MAX, NULL, NULL, 0);
}
