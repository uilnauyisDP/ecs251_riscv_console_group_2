// API List
uint32_t getTicks(void);
uint32_t getStatus(void);
uint32_t genRandom(int high);
uint32_t calcSmallSpriteControl(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t p);
void setGraphicsMode(void);
void setTextMode(void);
void setColor(int palette_id, int entry_id, uint32_t rgba);


volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile char *VIDEO_MEMORY = (volatile char *)(0x500FE800);
volatile uint32_t *SMALL_SPRITE_CONTROLS[128];
volatile uint8_t *SMALL_SPRITE_DATAS[128];