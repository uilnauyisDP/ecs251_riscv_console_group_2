#include <stdint.h>

extern uint8_t _erodata[];
extern uint8_t _data[];
extern uint8_t _edata[];
extern uint8_t _sdata[];
extern uint8_t _esdata[];
extern uint8_t _bss[];
extern uint8_t _ebss[];

// Adapted from https://stackoverflow.com/questions/58947716/how-to-interact-with-risc-v-csrs-by-using-gcc-c-code
__attribute__((always_inline)) inline uint32_t csr_mstatus_read(void){
    uint32_t result;
    asm volatile ("csrr %0, mstatus" : "=r"(result));
    return result;
}

__attribute__((always_inline)) inline void csr_mstatus_write(uint32_t val){
    asm volatile ("csrw mstatus, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_write_mie(uint32_t val){
    asm volatile ("csrw mie, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_enable_interrupts(void){
    asm volatile ("csrsi mstatus, 0x8");
}

__attribute__((always_inline)) inline void csr_disable_interrupts(void){
    asm volatile ("csrci mstatus, 0x8");
}

#define MTIME_LOW       (*((volatile uint32_t *)0x40000008))
#define MTIME_HIGH      (*((volatile uint32_t *)0x4000000C))
#define MTIMECMP_LOW    (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH   (*((volatile uint32_t *)0x40000014))
#define CONTROLLER      (*((volatile uint32_t *)0x40000018))

void init(void){
    uint8_t *Source = _erodata;
    uint8_t *Base = _data < _sdata ? _data : _sdata;
    uint8_t *End = _edata > _esdata ? _edata : _esdata;

    while(Base < End){
        *Base++ = *Source++;
    }
    Base = _bss;
    End = _ebss;
    while(Base < End){
        *Base++ = 0;
    }

    csr_write_mie(0x888);       // Enable all interrupt soruces
    csr_enable_interrupts();    // Global interrupt enable
    MTIMECMP_LOW = 1;
    MTIMECMP_HIGH = 0;
}

extern volatile int global;
extern volatile uint32_t controller_status;
volatile uint32_t *INT_PEND_REG = (volatile uint32_t *)(0x40000004);
volatile uint32_t *MODE_CTRL_REG = (volatile uint32_t *)(0x500FF414);
volatile uint32_t *INT_ENABLE_REG = (volatile uint32_t *)(0x40000000);

void c_interrupt_handler(uint32_t mcause){
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare += 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    global++;
    controller_status = CONTROLLER;
    // When command interrupt occurs, disable VIE and set to graphic mode
    if (((*INT_PEND_REG) & 0x4) >> 2){
        if (*INT_ENABLE_REG = 0x6){
            *INT_ENABLE_REG = 0x4;
        }
        *MODE_CTRL_REG = 0x1;
        // Clear CMIP by setting 1
        (*INT_PEND_REG) |= 0x4;
    }
    
    // When video interrupt occurs, switch between text mode and graphic mode
    if (((*INT_ENABLE_REG) == 0x6) & ((*INT_PEND_REG) & 0x2) >> 1){
        if (*MODE_CTRL_REG == 0x0){
            *MODE_CTRL_REG = 0x1;
        }
        else if (*MODE_CTRL_REG == 0x1){
            *MODE_CTRL_REG = 0x0;
        }
        // Clear VIP by setting 1
        (*INT_PEND_REG) |= 0x2;
    }
}

uint32_t c_system_call(uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t call){
    if(call == 0){
        return global;
    }
    else if(call == 1){
        return CONTROLLER;
    }
    return -1;
}

