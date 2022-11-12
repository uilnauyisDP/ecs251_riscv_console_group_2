#include <stdint.h>
#include <stdlib.h>
#include <time.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;

uint32_t getTicks(void);
uint32_t getStatus(void);
uint32_t genRandom(void);

volatile uint32_t *SPRITE_CONTROLS[128];
volatile uint8_t *SPRITE_DATAS[128];
volatile uint32_t *SPRITE_PALETTE = (volatile uint32_t *)(0x500FD000);
volatile uint32_t *MODE_CTRL_REG = (volatile uint32_t *)(0x500FF414);
volatile char *VIDEO_MEMORY = (volatile char *)(0x500FE800);

uint32_t CalcSmallSpriteControl(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t p);


int main() {
    int last_global = 42;
    // Initialize 128 sprite controls
    for (int i = 0; i < 128; i++){
        SPRITE_CONTROLS[i] = (volatile uint32_t *)(0x50000000 + 0xFF214 + i * 4);
    }
    for (int i = 0; i < 128; i++){
        SPRITE_DATAS[i] = (volatile uint8_t *)(0x50000000 + 0xF4000 + i * 256);
    }
    // Set graphics mode
    *MODE_CTRL_REG |= 0x1;
    // Set color for sprite palette 0
    SPRITE_PALETTE[0] = 0x8000A65F;
    SPRITE_PALETTE[1] = 0x80FFFFFF;
    SPRITE_PALETTE[2] = 0x80000000;
    // Offset of X and Y
    int SMALL_SPRITE_CTRL_OFFSET = 16;
    // The template address for sprite control (H=3, W=3, Y=0, X=0, P=0)
    // For convenice of assigning X and Y later
    int TEMP_ADDRESS = 0xCC00000;
    int pellet_x = 100;
    int pellet_y = 100;
    int center_x = pellet_x + 4;
    int center_y = pellet_y + 4;
    
    for(int y = 0; y < 16; y++){
        for(int x = 0; x < 16; x++){
            SPRITE_DATAS[0][(y<<4) + x] = ((x >= 3) & (x <= 5) & (y >= 0) & (y <= 8)) | ((x >= 2) & (x <= 6) & (y >= 3) & (y <= 5)) ? 1 : 2;
        }
    }
    for (int idx = 1; idx < 128; idx++){
        for(int y = 0; y < 16; y++){
            for(int x = 0; x < 16; x++){
                SPRITE_DATAS[idx][(y<<4) + x] = 0;
            }
        }
    }
    SPRITE_CONTROLS[0][0] = CalcSmallSpriteControl(pellet_x,pellet_y,8,8,0);

    int control_idx = 1;
    int cur_x = 0;
    int cur_y = 0;
    uint32_t current_status = 0;
    uint32_t last_status = 0;
    int budget = 5;
    int x = 255;
    int y = 255;
    int alive = 1;
    
    while (alive == 1) {
        global = getTicks();
        if(global != last_global){
            controller_status = getStatus();
            if (controller_status == 0x0){
                current_status = last_status;
            }
            else {
                current_status = controller_status;
            }
            if(current_status & 0x1){
                if(cur_x >= 3){
                    cur_x -= 3;
                }
                else{
                    break;
                }
            }
            if(current_status & 0x2){
                if( cur_y >= 3){
                    cur_y -= 3;
                }
                else{
                    break;
                }
            }
            if(current_status & 0x4){
                if( cur_y <= 287 - 3){
                    cur_y += 3;
                }
                else{
                    break;
                }
            }
            if(current_status & 0x8){
                if(cur_x <= 511 - 3){
                    cur_x += 3;
                }
                else{
                    break;
                }
            }
            if ((cur_x < center_x + 10) & (cur_y < center_y + 10) & (cur_x > center_x - 10) & (cur_y > center_y - 10) & budget <= 129){
                budget += 3;
                pellet_x = genRandom() % 512;
                pellet_y = genRandom() % 288;
                SPRITE_CONTROLS[0][0] = CalcSmallSpriteControl(pellet_x,pellet_y,8,8,0);
                center_x = pellet_x + 4;
                center_y = pellet_y + 4;
            }

            // Assign X and Y to controls
            if (cur_x != 0){
                for (int i = 1; i < budget; i++){
                    x = ((*SPRITE_CONTROLS[i] >> 2) & 0x3FF) - SMALL_SPRITE_CTRL_OFFSET;
                    y = ((*SPRITE_CONTROLS[i] >> 12) & 0x1FF) - SMALL_SPRITE_CTRL_OFFSET;
                    if (x == cur_x & y == cur_y){
                        alive = 0;
                        break;
                    }
                }
            }
            *SPRITE_CONTROLS[control_idx] = (TEMP_ADDRESS | (cur_x + SMALL_SPRITE_CTRL_OFFSET) << 2 | (cur_y + SMALL_SPRITE_CTRL_OFFSET) << 12);
            control_idx++;
            if (control_idx == budget){
                control_idx = 1;
            }
            last_global = global;
            last_status = current_status;
        }
    }
    *MODE_CTRL_REG &= 0x0;
    VIDEO_MEMORY[0] = 'G';
    VIDEO_MEMORY[1] = 'A';
    VIDEO_MEMORY[2] = 'M';
    VIDEO_MEMORY[3] = 'E';
    VIDEO_MEMORY[4] = ' ';
    VIDEO_MEMORY[5] = 'O';
    VIDEO_MEMORY[6] = 'V';
    VIDEO_MEMORY[7] = 'E';
    VIDEO_MEMORY[8] = 'R';
    VIDEO_MEMORY[9] = '!';
    VIDEO_MEMORY[10] = '!';
    VIDEO_MEMORY[11] = '!';
    return 0;
}


uint32_t CalcSmallSpriteControl(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t p){
    return ((h-1)<<25) | ((w-1)<<21) | ((y+16)<<12) | ((x+16)<<2) | p;
}