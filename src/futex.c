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
#define CONTENTION_LOOP_CNT  999999999

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
	int i;

	if (!fux)
		return;

	/* Handle Low Contention Case (Spin Loop/Spin Lock) */
	for (i = 0; i < CONTENTION_LOOP_CNT; i++) {
		if (__atomic_compare_exchange_n(fux, \
			&(cando_atomic_u32){CANDO_FUTEX_UNLOCK}, \
			CANDO_FUTEX_LOCK, 1, __ATOMIC_SEQ_CST, \
			__ATOMIC_SEQ_CST))
		{
			return;
		} else if (p_is_futex_funlock(fux)) {
			errno = EINTR;
			break;
		}
	}

	/* Blocking Or Sleeping Wait */
	while (1) {
		if (p_is_futex_funlock(fux))
		{
			errno = EINTR;
			break;
		}

		if (__atomic_compare_exchange_n(fux, \
			&(cando_atomic_u32){CANDO_FUTEX_UNLOCK}, \
			CANDO_FUTEX_LOCK, 0, __ATOMIC_SEQ_CST, \
			__ATOMIC_SEQ_CST))
		{
			break;
		}

		futex(fux, FUTEX_WAIT, CANDO_FUTEX_LOCK, NULL, NULL, 0);
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
