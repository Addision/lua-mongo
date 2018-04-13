#ifndef __ATOMIC_H
#define __ATOMIC_H

#define ATOMIC_CAS(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOMIC_CAS_POINTER(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOMIC_INC(ptr) __sync_add_and_fetch(ptr, 1)
#define ATOMIC_FINC(ptr) __sync_fetch_and_add(ptr, 1)
#define ATOMIC_DEC(ptr) __sync_sub_and_fetch(ptr, 1)
#define ATOMIC_FDEC(ptr) __sync_fetch_and_sub(ptr, 1)
#define ATOMIC_ADD(ptr,n) __sync_add_and_fetch(ptr, n)
#define ATOMIC_SUB(ptr,n) __sync_sub_and_fetch(ptr, n)
#define ATOMIC_AND(ptr,n) __sync_and_and_fetch(ptr, n)

#endif
