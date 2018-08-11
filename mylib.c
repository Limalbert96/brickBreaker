#include "mylib.h"
#include "DMA.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

// setPixel -- set the pixel at (row, col) to color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r=0; r<height; r++) {
	/*	for(int c = 0; c<width; c++) {
			setPixel(row+r, col+c, color);
		}
	*/
	DMA[3].src = &color;
	DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
	DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}


void drawImage3(int r, int c, int width, int height, const u16 * image) {
	for(int row = 0 ; row < height; row++){
		DMA[3].src = image + row * width;
		DMA[3].dst = videoBuffer + (r + row) * 240 + c;
		DMA[3].cnt = DMA_ON | width;
	}
}

void clearScreen(){
	volatile u16 black = 0;
	DMA[3].src = & black;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 240 * 160 | DMA_SOURCE_FIXED | DMA_ON;
}


void waitForVblank() {
	while(SCANLINECOUNTER > 160)
		;
	while(SCANLINECOUNTER<160)
		;
}


