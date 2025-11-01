#ifndef CANDO_ATOMIC_H
#define CANDO_ATOMIC_H

#define CANDO_ATOMIC_DEF(name, type) \
	typedef _Atomic typeof(type) name;

CANDO_ATOMIC_DEF(cando_atomic_int, int);
CANDO_ATOMIC_DEF(cando_atomic_bool, unsigned char);
CANDO_ATOMIC_DEF(cando_atomic_u32, unsigned int);
CANDO_ATOMIC_DEF(cando_atomic_addr, unsigned long long int);

#endif /* CANDO_ATOMIC_H */
