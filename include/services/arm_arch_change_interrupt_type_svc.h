#ifndef ARM_ARCH_CHANGE_INTERRUPT_TYPE_SVC_H
#define ARM_ARCH_CHANGE_INTERRUPT_TYPE_SVC_H

#include "satori.h"
#define NAME "Koi"

#define use_semaphore(l)    do{ \
            (l)--;              \
        }while(0);

#define assert_unused(l)    do{ \
        }while((l)!=1);

#define release_semaphore(l)    do{ \
            (l)++;              \
        }while(0);

/* Interrupt group definitions */
#define INTR_GROUP1S		U(0)
#define INTR_GROUP0		U(1)
#define INTR_GROUP1NS		U(2)

spinlock_t console_lock;

#define MY_ERROR(fmt, ...)     \
    spin_lock(&console_lock);  \
    ERROR(fmt, ##__VA_ARGS__); \
    spin_unlock(&console_lock)

#endif