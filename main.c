#include "mylib.h"
#include "DMA.h"
#include "pic1.h"
#include "win.h"
#include "lose.h"
#include "text.h"

int main(){
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	//----- CREATE BALL -------
	int ball_row = 150;
	int ball_col = 130;
	int ball_width = 5;
	int ball_height = 5;
	int ball_rd = -3;
	int ball_cd = 3;
	u16 ball_color = BLUE;

	BALL b = {ball_row, ball_col, ball_height, ball_width, ball_rd, ball_cd, ball_color};

	//----- CREATE PADDLE ------
	int paddle_row = 155;
	int paddle_col = 100;
	int paddle_width = 80;
	int paddle_height = 5;
	int paddle_cd = 5;
	u16 paddle_color = YELLOW;

	PADDLE p = {paddle_row, paddle_col, paddle_height, paddle_width, paddle_cd, paddle_color};

	//----- CREATE BRICKS -------
	int brick_row1 = 25;
	int brick_row2 = 40;
	int brick_row3 = 55;

	int brick_col1 = 20;
	int brick_col2 = 85;
	int brick_col3 = 150;

	int brick_width = 55;
	int brick_height = 10;

	int brick_hit =0;

	u16 brick_color = GREEN;

	BRICK bricks[9] = {
		//First row bricks
		{brick_row1, brick_col1, brick_height, brick_width, brick_hit, brick_color},
		{brick_row1, brick_col2, brick_height, brick_width, brick_hit, brick_color},
		{brick_row1, brick_col3, brick_height, brick_width, brick_hit, brick_color},

		// Second row bricks
		{brick_row2, brick_col1, brick_height, brick_width, brick_hit, brick_color},
		{brick_row2, brick_col2, brick_height, brick_width, brick_hit, brick_color},
		{brick_row2, brick_col3, brick_height, brick_width, brick_hit, brick_color},

		// Third row bricks
		{brick_row3, brick_col1, brick_height, brick_width, brick_hit, brick_color},
		{brick_row3, brick_col2, brick_height, brick_width, brick_hit, brick_color},
		{brick_row3, brick_col3, brick_height, brick_width, brick_hit, brick_color}
	};
	
	//------ CREATE GAME AREA -----------
	int ceiling_row = 10;
	int ceiling_col = 5;
	int ceiling_width = 225;
	int ceiling_height = 10;

	int wall_row = 20;
	int leftWall_col = 5;		// left vertical line
	int rightWall_col = 220;	// right vertical line
	int wall_width = 10;
	int wall_height = 160;

	u16 areas_color = WHITE;

	AREA areas[3] = {
		// HORIZONTAL LINE
		{ceiling_row, ceiling_col, ceiling_height, ceiling_width, areas_color},

		// LEFT VERTICAL LINE
		{wall_row, leftWall_col, wall_height, wall_width, areas_color},

		// RIGHT VERTICAL LINE
		{wall_row, rightWall_col, wall_height, wall_width, areas_color}
	};

	// State enum definition
	enum GBAState {
	  START,
	  READY,
	  GAMEPLAY,
	  WIN,
	  LOSE,
	  RESTART
	};

	// INITIAL STATE IS START
	enum GBAState state = START;

	// COUNT THE SCORE
	int count = 0;

	// TO KEEP WHEN BALL MOVES OR STOP
	int gameInitialized = 0;

	// INITIALIZE oldBrick
	BRICK oldBricks[9];

	while(1){
		waitForVblank();
		switch(state){

			// First state is title screen
			case(START):
			// draw the title screen
			drawImage3(0, 0, PIC1_WIDTH, PIC1_HEIGHT, pic1);

			// the move to the READY state
			state = READY;
			break;


			case(READY):
			// When BUTTON_B is pressed screen is clear first
			if(KEY_DOWN_NOW(BUTTON_B)){
				clearScreen();

				// DRAW THE BRICKS
				for (unsigned int i = 0; i < sizeof(bricks)/sizeof(BRICK); i++){
					drawRect(bricks[i].row, bricks[i].col, bricks[i].height, bricks[i].width, bricks[i].color);
				}

				// DRAW THE STRING FOR SCORE
				drawString(0, 100, "Score: ", BLUE);

				// DRAW THE STRING TO START GAME
				drawString(100, 75, "PRESS A TO START", YELLOW); 
				state = GAMEPLAY;
			}	
			break;


			case(GAMEPLAY):
				
				// DRAW THE GAME AREA
				for (unsigned int i = 0; i < sizeof(areas)/sizeof(AREA); i++){
					drawRect(areas[i].row, areas[i].col, areas[i].height, areas[i].width, areas[i].color);
				}
				
				//DRAW THE BALL
				drawRect(b.row, b.col, b.height, b.width, b.color);

				// DRAW THE PADDLE
				drawRect(p.row, p.col, p.height, p.width, p.color);

				//keep track previous movement
				PADDLE oldP = p;

				// right arrow to move to the right
				if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
					p.col += p.cd;
				} 

				// left arrow to move to the left
				else if(KEY_DOWN_NOW(BUTTON_LEFT)) {
					p.col -= p.cd;
				}

				waitForVblank();

				//draw old paddle when moved
				drawRect(oldP.row, oldP.col, oldP.height, oldP.width, BLACK);

				// DRAW THE PADDLE
				drawRect(p.row, p.col, p.height, p.width, p.color);


				//keep track of old ball movement
				BALL oldB = b;

				// ball moves when button A is pressed
				if (KEY_DOWN_NOW(BUTTON_A)) {
					gameInitialized = 1;

					// clear Press Start instruction
					drawRect(100, 75, 20, 100, BLACK);
				}

				if (KEY_DOWN_NOW(BUTTON_SELECT)){
					state = RESTART;
				}

				if (gameInitialized) {
					b.row = b.row + b.rd;
					b.col += b.cd;

					// bounce against ceiling
					if(b.row < ceiling_row + ceiling_height) {
						b.row = ceiling_row + ceiling_height;
						b.rd = -b.rd;
					}

					// bounce against paddle
					if( (b.row + b.height >= p.row) && (b.col > p.col) && (b.col< p.col + p.width) )  {

						b.row = p.row - b.height -1;
						b.rd = -b.rd;
					}

					// bounce when it doesn't hit the paddle and go to STATE = LOSE
					if((b.row + b.height > p.row)){
						state = LOSE;
					}
					
					// bounce against left wall
					if(b.col < leftWall_col + wall_width) {
						b.col = leftWall_col + wall_width;
						b.cd = -b.cd;
					}

					// bounce against right wall
					if(b.col > rightWall_col - wall_width + 5) {
						b.col = rightWall_col - wall_width + 5;
						b.cd = -b.cd;
					}

					
					// bounce against bricks
					for (unsigned int i = 0; i < sizeof(bricks)/sizeof(BRICK); i++){
						oldBricks[i] = bricks[i];

						// bounce against top bricks and
						if( ( b.row <= bricks[i].row)  && (b.row + b.height >= bricks[i].row) && (b.col > bricks[i].col) && 
							(b.col< bricks[i].col + bricks[i].width) && bricks[i].hit == 0) {
							bricks[i].hit = 1;

							b.row = bricks[i].row - bricks[i].height;
							b.rd = -b.rd;

							// increase score
							count++;

							// clear score by drawing a rectangle on top of the previous score
							drawRect(0, 135, 10,10, BLACK);

							// draw current score
							drawChar(0, 135, 48 + count + 0, BLUE);
						}
						
						// bounce against bottom side of bricks
						if( ( b.row >= bricks[i].row) && (b.row  <= bricks[i].row) && (b.col > bricks[i].col) && 
						(b.col < bricks[i].col + bricks[i].width) && bricks[i].hit == 0){
						 	bricks[i].hit = 1;
							b.row = bricks[i].row + bricks[i].height;
						 	b.rd = -b.rd;

						 	// increase score
						 	count++;

							// clear score by drawing a rectangle on top of the previous score
							drawRect(0, 135, 10,10, BLACK);

							// draw current score
							drawChar(0, 135, 48+count +0, BLUE);
						}

						// bounce against left side of bricks
						if( (b.col <= bricks[i].col)  &&
						(b.col >= (bricks[i].col - b.width)) &&
						(b.row < (bricks[i].row + bricks[i].height)) &&
						(b.row > bricks[i].row) && (bricks[i].hit == 0) ) {
							bricks[i].hit = 1;
							b.col = bricks[i].col - bricks[i].width;
							b.cd = -b.cd;

							// increase score
							count++;

							// clear score by drawing a rectangle on top of the previous score
							drawRect(0, 135, 10,10, BLACK);

							// draw current score
							drawChar(0, 135, 48+count +0, BLUE);
						}


						// bounce against right side of bricks
						if( ( b. col >= bricks[i].col) &&
						(b.col <= (bricks[i].width)) &&
						(b.col >= (bricks[i].col + bricks[i].width - b.width)) &&
						(b.row < (bricks[i].row + bricks[i].height)) &&
						(b.row > bricks[i].row) && (bricks[i].hit == 0)     ){

							bricks[i].hit = 1;
							b.col = bricks[i].col + bricks[i].width;
							b.cd = -b.cd;

							// increase score
							count++;

							// clear score by drawing a rectangle on top of the previous score
							drawRect(0, 135, 10,10, BLACK);

							// draw current score
							drawChar(0, 135, 48+count +0, BLUE);	
						}

						// If max score is reached, then move to the WIN state
						if(count == 9){
							state = WIN;
							break;
						}
				}		
			}

			for (unsigned int i = 0; i < sizeof(bricks)/sizeof(BRICK); i++){
				if(oldBricks[i].hit == 1){
				 // remove brick if hit by ball
				drawRect(oldBricks[i].row, oldBricks[i].col, oldBricks[i].height, oldBricks[i].width, BLACK);
				}	
			}


			//draw the ball with black after it moves
			drawRect(oldB.row, oldB.col, oldB.height, oldB.width, BLACK);

			//draw the current ball
			drawRect(b.row, b.col, b.height, b.width, b.color);
			break;



			case(WIN):
			// DRAW THE WINNING SCREEN
			drawImage3(0, 0, WIN_WIDTH, WIN_HEIGHT, win);

			// DRAW STRING INSTRUCTION TO RESTART
			drawString(125, 50, "PRESS SELECT TO RESTART", YELLOW); 

			// WHEN SELECT BUTTON IS PRESSED, GO TO RESTART STATE
			if (KEY_DOWN_NOW(BUTTON_SELECT)){
				state = RESTART;
			}
			break;

			case(LOSE):
			// DRAW THE LOSING SCREEN
			drawImage3(0, 0, LOSE_WIDTH, LOSE_HEIGHT, lose);

			// DRAW STRING INSTRUCTION TO RESTART
			drawString(125, 50, "PRESS SELECT TO RESTART", YELLOW);

			// WHEN SELECT BUTTON IS PRESSED, GO TO RESTART STATE
			if (KEY_DOWN_NOW(BUTTON_SELECT)){
				state = RESTART;
			}
			break;


			case(RESTART):
			//RESET BALL MOVEMENT
			gameInitialized = 0;

			//RESET SCORE COUNT
			count = 0;

			//----- RESET BALL -------
			ball_row = 150;
			ball_col = 130;
			b.row = ball_row;
			b.col = ball_col;
			b.rd = -3;
			b.cd = 3;
			oldB = b;

			//----- RESET PADDLE ------
			paddle_row = 155;
			paddle_col = 100;
			p.row = paddle_row;
			p.col = paddle_col;
			oldP = p;

			//------ RESET BRICKS ------
			brick_hit = 0;

			for (unsigned int i = 0; i < sizeof(bricks)/sizeof(BRICK); i++){
				 bricks[i].hit = brick_hit;
				 oldBricks[i] = bricks[i];
				
			}

			// Go back to START state
			state = START;
			break;
		}

	}

	

	while(1);
	return 0;
}