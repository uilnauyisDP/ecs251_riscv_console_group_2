#include <stdint.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;

uint32_t getTicks(void);
uint32_t getStatus(void);

volatile char *VIDEO_MEMORY = (volatile char *)(0x500FE800);
volatile uint32_t *SPRITE_CONTROLS[128];
volatile uint32_t *SPRITE_PALETTE = (volatile uint32_t *)(0x500FD000);
volatile uint32_t *MODE_CTRL_REG = (volatile uint32_t *)(0x500FF414);
volatile uint32_t *INT_ENABLE_REG = (volatile uint32_t *)(0x40000000);

int main() {
    // Text mode context
    VIDEO_MEMORY[64] = 'T';
    VIDEO_MEMORY[65] = 'E';
    VIDEO_MEMORY[66] = 'X';
    VIDEO_MEMORY[67] = 'T';
    VIDEO_MEMORY[68] = ' ';
    VIDEO_MEMORY[69] = 'M';
    VIDEO_MEMORY[70] = 'O';
    VIDEO_MEMORY[71] = 'D';
    VIDEO_MEMORY[72] = 'E';
    VIDEO_MEMORY[73] = '!';
    VIDEO_MEMORY[74] = '!';
    VIDEO_MEMORY[75] = '!';

    int last_global = 42;
    // Initialize 128 sprite controls
    for (int i = 0; i < 128; i++){
        SPRITE_CONTROLS[i] = (volatile uint32_t *)(0x50000000 + 0xFF214 + i * 4);
    }
    // Set color for sprite palette 0
    SPRITE_PALETTE[0] = 0x80FFFF00;
    // Offset of X and Y
    int SMALL_SPRITE_CTRL_OFFSET = 16;
    int width = 6;
    // The template address for sprite control (H=6, W=6, Y=0, X=0, P=0)
    // For convenice of assigning X and Y later
    int TEMP_ADDRESS = 0x19800000;

    int screen_width = 512;
    int screen_height = 288;
    int cur_x = 16;
    int cur_y = 16;
    int control_idx = 0;
    // Enable CMIE and VIE
    *INT_ENABLE_REG |= 0x6;

    while (1) {
        // Periodic timer
        if(getTicks() % 20 == 0){
            if (SPRITE_PALETTE[0] == 0x80FF00FF){
                SPRITE_PALETTE[0] = 0x80FFFF00;
            }
            else if (SPRITE_PALETTE[0] == 0x80FFFF00){
                SPRITE_PALETTE[0] = 0x80FF00FF;
            }
        }
        global = getTicks();
        if(global != last_global){
            controller_status = getStatus();
            if(controller_status){
                if(controller_status & 0x1){
                    if((cur_x - SMALL_SPRITE_CTRL_OFFSET) >= width){
                        cur_x -= width;
                    }
                    // Re-enter the screen from right
                    else{
                        cur_x = 511 + SMALL_SPRITE_CTRL_OFFSET;
                    }
                }
                if(controller_status & 0x2){
                    if( (cur_y - SMALL_SPRITE_CTRL_OFFSET) >= width){
                        cur_y -= width;
                    }
                    // Re-enter the screen from bottom
                    else{
                        cur_y = 287 + SMALL_SPRITE_CTRL_OFFSET;
                    }
                }
                if(controller_status & 0x4){
                    if( (cur_y - SMALL_SPRITE_CTRL_OFFSET) < screen_height - width){
                        cur_y += width;
                    }
                    // Re-enter the screen from ceiling
                    else{
                        cur_y = 0 + SMALL_SPRITE_CTRL_OFFSET;
                    }
                }
                if(controller_status & 0x8){
                    if((cur_x - SMALL_SPRITE_CTRL_OFFSET) < screen_width - width){
                        cur_x += width;
                    }
                    // Re-enter the screen from left
                    else{
                        cur_x = 0 + SMALL_SPRITE_CTRL_OFFSET;
                    }
                }
                // Assign X and Y to controls
                *SPRITE_CONTROLS[control_idx] = (TEMP_ADDRESS | cur_x << 2 | cur_y << 12);
            }
            last_global = global;
        }
    }
    return 0;
}
