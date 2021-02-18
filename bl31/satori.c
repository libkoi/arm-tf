#include "satori.h"
#define uLL unsigned long long

spinlock_t console_lock;
#define MY_ERROR(fmt, ...)     \
    spin_lock(&console_lock);  \
    ERROR(fmt, ##__VA_ARGS__); \
    spin_unlock(&console_lock)

void check_current_EL()
{
    uint64_t el_reg = 0;
    uint64_t el = 0x3f;
    READ_SYSREG(el_reg, CurrentEL);

        el = (el_reg >> 2) & 0b11; // CurrentEL[3:2]
    ERROR("[KKK]: CurrentEL=%llu\n", el);
}


void print_EL3_ra(uLL spsr_el3, uLL elr_el3)
{
    ERROR(">>>>>>print_EL3_ra<<<<<<\n");
    ERROR("[KKK]: spsr_EL3=%llu\n", spsr_el3);
    ERROR("[KKK]:  elr_EL3=%llu\n", elr_el3);
    return;
}


void job_interrupt_type_test(unsigned int iid)
{
    ERROR(">>>>>>job_interrupt_type_test<<<<<<\n");
    if(iid == 65)
    {
        plat_ic_get_pending_interrupt_type();
    }
    else 
    {
        ERROR("[KKK]: interrupt %u found\n", iid);
    }
    return;
}


uint32_t GPU_interrupt_handler(uLL iid)
{
    if(iid == 65)
    {
        spin_lock(&console_lock);
        ERROR("[KKK]: Detected Interrupt, id=%llu\n", iid);
        spin_unlock(&console_lock);

        plat_ic_disable_interrupt(iid);
        MY_ERROR("[KKK]: Interrupt %llu disabled\n", iid);

        plat_ic_clear_interrupt_pending(iid);
        MY_ERROR("[KKK]: Interrupt %llu cleared\n", iid);

        check_current_EL();

        plat_ic_enable_interrupt(iid);
        MY_ERROR("[KKK]: Interrupt %llu enabled\n", iid);

        return 114;
    }
    return 0;
}

uint32_t GPU_interrupt_handler_v2(uLL iid, uLL sp)
{
    if(iid == 65)
    {
        spin_lock(&console_lock);
        ERROR("[KKK]: Detected Interrupt, id=%llu\n", iid);
        spin_unlock(&console_lock);

        plat_ic_disable_interrupt(iid);
        MY_ERROR("[KKK]: Interrupt %llu disabled\n", iid);

        plat_ic_clear_interrupt_pending(iid);
        MY_ERROR("[KKK]: Interrupt %llu cleared\n", iid);

        check_current_EL();
        MY_ERROR("[KKK]: SP=0x%llx\n", sp);

        plat_ic_enable_interrupt(iid);
        MY_ERROR("[KKK]: Interrupt %llu enabled\n", iid);

        return 14;
    }
    return 0;
}

int SATORI_INIT()
{
    MY_ERROR("[KKK]: INIT\n");
    check_current_EL();
    // enable_PMI();
    unsigned int flags = 0;
    set_interrupt_rm_flag(flags, NON_SECURE);
    set_interrupt_rm_flag(flags, SECURE);
    MY_ERROR("[KKK]: INIT COMPLETED\n");
    return 0;
} 