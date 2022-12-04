// API List
uint32_t getTicks(void);
uint32_t getStatus(void);
uint32_t genRandom(int high);
void setColor(int palette_id, int entry_id, uint32_t rgba);
uint32_t calcSmallSpriteControl(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t p);
uint32_t calcLargeSpriteControl(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t p);
uint32_t calcBackgroundControl(uint32_t x, uint32_t y, uint32_t z, uint32_t p);
void setGraphicsMode(void);
void setTextMode(void);

uint32_t* regAddrFromMemHandle(uint32_t mem_handle);
uint32_t memLengthCheck(uint32_t mem_handle, uint32_t mem_len);
uint32_t writeTargetMem(uint32_t mem_handle, uint32_t source_addr, uint32_t mem_len);
uint32_t writeTarget(uint32_t mem_handle, uint32_t value);
int writeIndexedTarget(uint32_t offset, uint32_t mem_size, uint32_t idx, uint8_t * addr, uint32_t mem_len);
int setBackgroundDataImage(uint32_t idx, uint8_t * addr);
int initBackgroundPalette(uint32_t idx, uint8_t * addr, uint32_t mem_len);
void setBackgroundControl(uint32_t idx, uint32_t x, uint32_t y, uint32_t z, uint32_t palette);

#define DISPLAY_MODE_TEXT 0
#define DISPLAY_MODE_GRAPHICS 1
#define DISPLAY_WIDTH_PX 512
#define DISPLAY_HEIGHT_PX 288
#define DISPLAY_WIDTH_CHAR 0x40

