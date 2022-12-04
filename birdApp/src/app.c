#include <stdint.h>
#include "api.h"

extern uint8_t bird_color_palette[888];
// extern uint8_t bird_background_color_palette[4 * 16];
extern uint8_t bird_img_0[64 * 64];
extern uint8_t bird_img_1[64 * 64];
extern uint8_t bird_img_2[64 * 64];
extern uint8_t bird_background_img[288 * 512];

extern uint8_t pillar_palette[4 * 135];
extern uint8_t pillar_img_head_up[64 * 64];
extern uint8_t pillar_img_head_down[64 * 64];
extern uint8_t pillar_img_body_up[64 * 64];
extern uint8_t pillar_img_body_down[64 * 64];

extern uint8_t gameover_img_01[4096];
extern uint8_t gameover_img_02[4096];

extern volatile uint32_t running_flag;

void initVideoSetting()
{


    setGraphicsMode();

    initBackgroundPalette(0, bird_color_palette, 888);
    initSpritePalette(1, bird_color_palette, 888);
    initTransparentSpritePalette(0);


    setBackgroundControl(0, 0, 0, 0, 0);
    setLargeSpriteControl(0, 64, 64, 30, 30, 1);
    setLargeSpriteControl(1, 64, 64, 0, 0, 0);
    setLargeSpriteControl(2, 64, 64, 0, 0, 0);


    setLargeSpriteDataImage(0, bird_img_0);
    setLargeSpriteDataImage(1, bird_img_1);
    setLargeSpriteDataImage(2, bird_img_2);

    //setBackgroundDataImage(0, bird_background_img);



}



