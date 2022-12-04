#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#define SMALL_SPRITE_CTRL_OFFSET 16

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile char *VIDEO_MEMORY = (volatile char *)(0x500FE800);
volatile uint32_t *SMALL_SPRITE_CONTROL = (volatile uint32_t *)(0x500FF214);

void initVideoSetting();

int main()
{
    initVideoSetting();
    setGraphicsMode();
    while (1)
    {
    }
}