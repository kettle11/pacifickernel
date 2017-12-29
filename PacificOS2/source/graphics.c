#include "graphics.h"
#include "font.h"

uint16_t horizontalResolution;
uint16_t verticalResolution;

uint64_t frameBufferAdr;

void setResolution(uint16_t horizontalRes, uint16_t verticalRes)
{
	horizontalResolution = horizontalRes;
	verticalResolution = verticalRes;
}

void setFrameBuffer(uint64_t frameBufferAdrSet)
{
	frameBufferAdr = frameBufferAdrSet;
}

void drawTriangle( uint16_t center_x, uint16_t center_y, uint16_t width, uint32_t color ) {
    uint32_t* at = (uint32_t*)frameBufferAdr;
    uint16_t row, col;

    at += (horizontalResolution * (center_y - width / 2) + center_x - width / 2);

    for (row = 0; row < width / 2; row++) {
        for (col = 0; col < width - row * 2; col++)
            *at++ = color;
        at += (horizontalResolution - col);
        for (col = 0; col < width - row * 2; col++)
            *at++ = color;
        at += (horizontalResolution - col + 1);
    }
}

void drawRectangle( uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height, uint32_t color)
{
	uint16_t end_x = start_x + width;
	uint16_t end_y = start_y + height;

	if (width == 0 || height == 0)
	{
		return;
	}

	if (start_x >= horizontalResolution)
	{
		start_x = horizontalResolution-1;
	}

	if (end_x >= horizontalResolution)
	{
		end_x = horizontalResolution-1;
	}

	if (start_y >= verticalResolution)
	{
		start_y = verticalResolution-1;
	}

	if (end_y >= verticalResolution)
	{
		end_y = verticalResolution-1;
	}


    uint32_t* at = (uint32_t*)frameBufferAdr;
    at += horizontalResolution * start_y + start_x;

    for (uint16_t y = start_y; y <= end_y; y++)
    {
	    for (uint16_t x = start_x; x <= end_x; x++)
	    {
	    	*at++ = color;
	    }

	    at += horizontalResolution - width - 1;
    }
}

void drawString(uint16_t start_x, uint16_t start_y, char* string)
{	
	size_t stringLength = strlen(string);
	
	int mask[8]={1,2,4,8,16,32,64,128};

	uint32_t color = 0x004E7BFE;
	int width =  stringLength * 8;
	int height = 8;

	//drawRectangle(start_x, start_y - height, width, height, 0x00ff99ff);

    uint32_t* at = (uint32_t*)frameBufferAdr;
    at += horizontalResolution * (start_y-height) + start_x;

	for (int x = 0; x < stringLength; x++)
	{
		unsigned char *glyph = font8x8_basic[string[x]];

	    uint32_t* at = (uint32_t*)frameBufferAdr;
	    at += horizontalResolution * (start_y-height) + start_x + (x * 8);

		for(int y=0; y<8; y++){
			for (int i = 0; i < 8; i++)
			{
				if (glyph[y] & mask[i])
				{
			    	*at = color;
				}
		    	//*at = color;

				at++;
			}

	    	at += horizontalResolution - 8;
		}

	}
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}