typedef unsigned short u16;

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define OFFSET(r, c, rowlen) ((c) + (rowlen)*(r)) 
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31, 0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0

// Buttons

#define BUTTON_A		(1<<0)  // Z
#define BUTTON_B		(1<<1)  // X
#define BUTTON_SELECT	(1<<2)  // BACKSPACE
#define BUTTON_START	(1<<3)  // ENTER
#define BUTTON_RIGHT	(1<<4)  // RIGHT ARROW
#define BUTTON_LEFT		(1<<5)  // LEFT ARROW
#define BUTTON_UP		(1<<6)  // UP ARROW
#define BUTTON_DOWN		(1<<7)  // DOWN ARROW
#define BUTTON_R		(1<<8)  // S
#define BUTTON_L		(1<<9)  // A

#define ANY_KEY			(0x1FF)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

#define SCANLINECOUNTER *(volatile short *)0x4000006


//DRAWING BALL
typedef struct ball {
	int row;
	int col;
	int height;
	int width;
	int rd;
	int cd;
	u16 color;
} BALL;


//DRAWING PADDLE
typedef struct paddle {
	int row;
	int col;
	int height;
	int width;
	int cd;
	u16 color;
} PADDLE;

//#define CREATE_PADDLE(X) PADDLE X = {.row = 155, .col = 100, .width = 5, .height = 80, .color = YELLOW}
// PADDLE X or struct paddle X


// DRAWING BRICK
typedef struct brick {
	int row;
	int col;
	int height;
	int width;
	int hit;
	u16 color;
} BRICK;


// DRAWING GAME AREA
typedef struct area{
	int row;
	int col;
	int height;
	int width;
	u16 color;
} AREA;



// Prototypes
void setPixel(int , int , unsigned short );
void drawRect(int row, int col, int height, int width, unsigned short color);
void clearScreen();
void waitForVblank();
void drawImage3(int r, int c, int width, int height, const u16 * image);


