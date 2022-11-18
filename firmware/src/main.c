#include <stdint.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile uint32_t *CARTRIDGE = (volatile uint32_t *)(0x4000001C);
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
typedef void (*FunPtr)(void);
typedef uint32_t *TContext;
typedef void (*TEntry)(void *);

TContext InitContext(uint32_t *stacktop, TEntry entry, void *param);
void SwitchContext(TContext *old, TContext new);

TContext MainThread;
TContext OtherThread;

void Thread(void *param){
    ((FunPtr)((*CARTRIDGE) & 0xFFFFFFFC))();
    SwitchContext(&OtherThread,MainThread);
}
int main() {
    uint32_t ThreadStack[128];
    OtherThread = InitContext(ThreadStack + 128,Thread,(void *)0);
    while(1){
        if((*CARTRIDGE) & 0x1){
            SwitchContext(&MainThread,OtherThread);
        }
    }
    return 0;
}
