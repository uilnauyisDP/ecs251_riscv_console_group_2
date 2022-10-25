#include <stdint.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;

uint32_t getTicks(void);
uint32_t getStatus(void);

volatile uint32_t *SPRITE_CONTROLS[128];
volatile uint32_t *SPRITE_PALETTE = (volatile uint32_t *)(0x500FD000);
volatile uint32_t *MODE_CTRL_REG = (volatile uint32_t *)(0x50000000 + 0xFF414);

int main() {
    int last_global = 42;
    // Initialize 128 sprite controls
    for (int i = 0; i < 128; i++){
        SPRITE_CONTROLS[i] = (volatile uint32_t *)(0x50000000 + 0xFF214 + i * 4);
    }
    // Set graphics mode
    *MODE_CTRL_REG |= 0x1;
    // Set color for sprite palette 0
    SPRITE_PALETTE[0] = 0x80FFFF00;
    // Offset of X and Y
    int SMALL_SPRITE_CTRL_OFFSET = 16;
    // The template address for sprite control (H=3, W=3, Y=0, X=0, P=0)
    // For convenice of assigning X and Y later
    int TEMP_ADDRESS = 0xCC00000;

    int cur_x = 16;
    int cur_y = 16;
    int control_idx = 0;

    while (1) {
        global = getTicks();
        if(global != last_global){
            controller_status = getStatus();
            if(controller_status){
                if(controller_status & 0x1){
                    if((cur_x - SMALL_SPRITE_CTRL_OFFSET) >= 3){
                        cur_x -= 3;
                    }
                    // Re-enter the screen from right
                    else{
                        cur_x = 511 + SMALL_SPRITE_CTRL_OFFSET;
                    }
                }
                if(controller_status & 0x2){
                    if( (cur_y - SMALL_SPRITE_CTRL_OFFSET) >= 3){
                        cur_y -= 3;
                    }
                    // Re-enter the screen from bottom
                    else{
                        cur_y = 287 + SMALL_SPRITE_CTRL_OFFSET;
                    }
                }
                if(controller_status & 0x4){
                    if( (cur_y - SMALL_SPRITE_CTRL_OFFSET) <= 287 - 3){
                        cur_y += 3;
                    }
                    // Re-enter the screen from ceiling
                    else{
                        cur_y = 0 + SMALL_SPRITE_CTRL_OFFSET;
                    }
                }
                if(controller_status & 0x8){
                    if((cur_x - SMALL_SPRITE_CTRL_OFFSET) <= 511 - 3){
                        cur_x += 3;
                    }
                    // Re-enter the screen from left
                    else{
                        cur_x = 0 + SMALL_SPRITE_CTRL_OFFSET;
                    }
                }
                // Assign X and Y to controls
                *SPRITE_CONTROLS[control_idx] = (TEMP_ADDRESS | cur_x << 2 | cur_y << 12);
                control_idx++;
                if (control_idx == 129){
                    control_idx = 0;
                }
            }
            last_global = global;
        }
    }
    return 0;
}
