#include <assert.h>
#include <string.h>

#include <arch.h>
#include <arch_features.h>
#include <arch_helpers.h>
#include <bl31/bl31.h>
#include <bl31/ehf.h>
#include <drivers/arm/gic_common.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <common/runtime_svc.h>
#include <common/interrupt_props.h>
#include <drivers/console.h>
#include <lib/el3_runtime/context_mgmt.h>
#include <lib/pmf/pmf.h>
#include <lib/runtime_instr.h>
#include <lib/spinlock.h>
#include <plat/common/platform.h>
#include <services/std_svc.h>
#include <bl31/interrupt_mgmt.h>

#include <lib/utils_def.h>
#include "gic_common_private.h"
#include "gicv2_private.h"

#define uLL unsigned long long

/* Interrupt group definitions */
#define INTR_GROUP1S		U(0)
#define INTR_GROUP0		U(1)
#define INTR_GROUP1NS		U(2)

spinlock_t console_lock;

#define MY_ERROR(fmt, ...)     \
    spin_lock(&console_lock);  \
    ERROR(fmt, ##__VA_ARGS__); \
    spin_unlock(&console_lock)

#define READ_SYSREG(dst, sysreg) asm("mrs %0, " #sysreg \
                                     : "=r"(dst));

#define WRITE_SYSREG(src, sysreg) \
    {                             \
        asm("isb");               \
        asm("msr " #sysreg ", %0" \
            :                     \
            : "r"(src));          \
    }

#define READ_REG(dst, reg) asm("mov %0, " #reg \
                                     : "=r"(dst));

#define WRITE_REG(src, reg) \
    {                             \
        asm("mov " #reg ", %0" \
            :                     \
            : "r"(src));          \
    }

void check_current_EL();
void print_EL3_ra(uLL spsr_el3, uLL elr_el3);
void change_65_to_secure();
void change_65_to_non_secure();
int SATORI_INIT(); 