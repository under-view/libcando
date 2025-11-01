#ifndef CANDO_FUTEX_H
#define CANDO_FUTEX_H

#include "macros.h"

/*
 * @brief Atomically updates futex value to the locked state.
 *        If value can't be changed inform kernel that a
 *        process needs to be put to sleep. Sets errno to
 *        EINTR if a call to cando_futex_unlock_force()
 *        is made.
 *
 * @param fux - Pointer to 32-bit integer storing futex.
 */
CANDO_API
void
cando_futex_lock (cando_atomic_u32 *fux);


/*
 * @brief Atomically update futex value to the unlocked state.
 *        Then inform kernel to wake up all processes/threads
 *        watching the futex.
 *
 * @param fux - Pointer to 32-bit integer storing futex.
 */
CANDO_API
void
cando_futex_unlock (cando_atomic_u32 *fux);


/*
 * @brief Atomically update futex value to the force unlocked state.
 *        Then inform kernel to wake up all processes/threads
 *        watching the futex. When force unlocking if a
 *        process/thread is waiting on the lock. Process/thread
 *        will exit setting errno to EINTR. Recommended to use
 *        function in a signal handler.
 *
 * @param fux - Pointer to 32-bit integer storing futex.
 */
CANDO_API
void
cando_futex_unlock_force (cando_atomic_u32 *fux);

#endif /* CANDO_FUTEX_H */
