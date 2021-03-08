#include "satori.h"
#define uLL unsigned long long

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

void start_routine(){
    MY_ERROR(">>>>>>macro handle_interrupt_exception invoked<<<<<<\n");
    return;
}

void value_saved(){
    MY_ERROR(">>>>>>value saved<<<<<<\n");
    return;
}

void start_judge_int_type(){
    MY_ERROR(">>>>>>start judge interrupt type<<<<<<\n");
    return;
}

void change(){
    plat_ic_set_interrupt_type(33, INTR_GROUP0);
    MY_ERROR(">>>>>>change interrupt GPU_JOB_DONE type<<<<<<\n");
    return;
}

void job_interrupt_set_el3(){
    
}

// void job_interrupt_type_test(unsigned int iid)
// {
//     ERROR(">>>>>>job_interrupt_type_test<<<<<<\n");
//     if(iid == 65)
//     {
//         plat_ic_get_pending_interrupt_type();
//     }
//     else 
//     {
//         ERROR("[KKK]: interrupt %u found\n", iid);
//     }
//     return;
// }


// void print_icc_hppir0_el1(){
//     unsigned long ICC_HPPIR0_EL1;
//     asm volatile("MRS %0, ICC_HPPIR0_EL1": "=r"(ICC_HPPIR0_EL1) ::);
//     MY_ERROR("[KKK]: ICC_HPPIR0_EL1 = 0x%ld\n", ICC_HPPIR0_EL1);
// }


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
    // change();
    MY_ERROR("[KKK]: INIT COMPLETED\n");
    return 0;
} 