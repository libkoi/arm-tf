#include "satori.h"

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

    // CurrentEL[3:2]
    el = (el_reg >> 2) & 0b11; 
    ERROR("[KKK]: CurrentEL=%llu\n", el);
}

uint32_t GPU_interrupt_handler(unsigned long long iid, unsigned long long sp)
{
    if (iid == 64 ||
        iid == 65 ||
        iid == 66 )
    {
        spin_lock(&console_lock);
        ERROR("[KKK]: Detected Interrupt, id=%llu\n", iid);
        spin_unlock(&console_lock);

        // // disable PMI
        // asm volatile("msr PMINTENCLR_EL1, %0"
        //              :
        //              : "r"(0xffffffff));

        // // disable PMU counter
        // asm volatile("msr PMCNTENCLR_EL0, %0"
        //              :
        //              : "r"(0xffffffff));

        // uint64_t pid;
        plat_ic_disable_interrupt(iid);
        MY_ERROR("[KKK]: Interrupt %llu disabled\n", iid);

        plat_ic_clear_interrupt_pending(iid);
        MY_ERROR("[KKK]: Interrupt %llu cleared\n", iid);

        check_current_EL();
        // // READ_REG(pid, x4);
        MY_ERROR("[KKK]: SP=0x%llx\n", sp);
        // MY_ERROR("[Ninja]: x21=0x%llx\n", *(unsigned long long*)(sp + 0xa8));
        
        // asm volatile("ldr %1, [%0,0xb0] "
        //              : "=r"(pid)
        //              : "r"(sp));

        // MY_ERROR("[Ninja]: PID=0x%llx\n", pid);

        // int midr;
        // READ_SYSREG(midr, MIDR_EL1);
        // MY_ERROR("[Ninja]: MIDR_EL1=0x%x\n", midr);
        // // 0x410fd033

        // int event;
        // READ_SYSREG(event, PMEVTYPER0_EL0);
        // MY_ERROR("[Ninja]: Event_TYPE=0x%x\n", event);

        // READ_SYSREG(event, pmevcntr0_el0);
        // MY_ERROR("[Ninja]: counter=0x%x\n", event);

        // READ_SYSREG(event, PMOVSCLR_EL0);
        // MY_ERROR("[Ninja]: overflow=0x%x\n", event);

        // asm volatile("msr PMOVSCLR_EL0, %0"
        //              :
        //              : "r"(0xffffffff));

        // // uint32_t inf = 0xffffffff - 4000;
        // uint32_t inf = 1;
        // WRITE_SYSREG(inf, pmevcntr0_el0);
        // MY_ERROR("[Ninja]: Reset pmevcntr0_el0 to 0x%x\n", inf);

        // enable PMI
        plat_ic_enable_interrupt(iid);
        MY_ERROR("[KKK]: Interrupt %llu enabled\n", iid);

        // asm volatile("msr PMINTENSET_EL1, %0"
        //              :
        //              : "r"(1));

        // // enable PMU counter
        // asm volatile("msr PMCNTENSET_EL0, %0"
        //              :
        //              : "r"(1));
        return 114;
    }
    return 0;
}

// void enable_PMI()
// {
//     uint32_t t = 0;
//     READ_SYSREG(t, PMINTENSET_EL1);
//     t = 0xffffffff;
//     WRITE_SYSREG(t, PMINTENSET_EL1);
//     READ_SYSREG(t, PMINTENSET_EL1);
//     ERROR("[NINJA]: change PMINTENSET_EL1 to %u\n", t);
// }

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