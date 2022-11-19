#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#define SMALL_SPRITE_CTRL_OFFSET 16

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile char *VIDEO_MEMORY = (volatile char *)(0x500FE800);

int main() {
    char *ptr = malloc(12);
    ptr[0] = 'H';
    ptr[1] = 'E';
    ptr[2] = 'L';
    ptr[3] = 'L';
    ptr[4] = 'O';
    ptr[5] = ' ';
    ptr[6] = 'W';
    ptr[7] = 'O';
    ptr[8] = 'R';
    ptr[9] = 'L';
    ptr[10] = 'D';
    ptr[11] = '!';
    ptr[12] = 'X';
    ptr[13] = '\0';
    strcpy(VIDEO_MEMORY, ptr);
    return 0;
}