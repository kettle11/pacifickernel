#include <stdint.h>
#include <stddef.h>

void setResolution(uint16_t horizontalRes, uint16_t verticalRes);
void setFrameBuffer(uint64_t frameBufferAdrSet);

void drawTriangle( uint16_t center_x, uint16_t center_y, uint16_t width, uint32_t color );
void drawRectangle( uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height, uint32_t color);
void drawString(uint16_t start_x, uint16_t start_y, char* string);
size_t strlen(const char* str);
