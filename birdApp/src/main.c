#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile uint32_t running_flag = 1;

struct position
{
    uint32_t x;
    uint32_t y;
};

void initVideoSetting();

int main()
{
    initVideoSetting();
    
    threadGraphics();
}

void threadGraphics()
{
    struct position flappyBird;
    flappyBird.y = 30;
    flappyBird.x = 30;

    int x_pos = 18;
    int sprite_inc_x;
    int sprite_inc_y;
    int move_speed = 5;
    uint32_t time_counter = 0;
    uint32_t last_time = 0;
    while (running_flag)
    {
        global = getTicks();

        sprite_inc_x = 0;
        sprite_inc_y = 0;
        if (global != last_time)
        {
            //linePrintf(0, "video interrupt: %d", getVideoInterruptSeq());
            controller_status = getStatus();
            if (controller_status)
            {
                //linePrintf(5, "controller status: %d", controller_status);
                if (controller_status & 0x1)
                {
                    sprite_inc_x = -move_speed;
                }
                if (controller_status & 0x2)
                {
                    sprite_inc_y = -move_speed;
                }
                if (controller_status & 0x4)
                {
                    sprite_inc_y = move_speed;
                }
                if (controller_status & 0x8)
                {
                    sprite_inc_x = move_speed;
                }
            }
            flappyBird.x += sprite_inc_x;
            flappyBird.y += sprite_inc_y;
            for (int i = 0; i < 3; i++)
            {
                setLargeSpriteControl(i, 64, 64, flappyBird.x, flappyBird.y, i == global % 3);
            }
            last_time = global;
        }
        //threadYield();
    }
}