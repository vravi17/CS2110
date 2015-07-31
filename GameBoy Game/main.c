#include "myLib.h"
#include "gImage.h"
#include "play.h"
#include "over.h"
#include "targetPic.h"
#include "win.h"
#include "font.h"
#include "title.h"
#include "baseball.h"
#include <string.h>

void make_everything()
{
	initiate_player(); 
	initiate_player_balls(); 
	initiate_targets(); 
	initiate_target_bullets();
	num_of_targets_left = AMT_OF_TARGETS;
	
}
	
int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	int state = 0;
	int cd = 1;
	int rd = 1;
	int frame = 0; 	
	int moves_of_target = 0; 
	int target_move_distance = 5; 
	make_everything();

	while(1)
	{ // Game Loop
		
		blank_screen();
		//Start Screen
		while(state == 0) 
		{
			drawImage3(0, 0, 240, 160, title);
			if (KEY_DOWN_NOW(BUTTON_START)) {
				state = 1;
				frame = 0;
				moves_of_target = 0;
			}
	   	} 
		
		blank_screen();	
		frame = 0;
		
		target_move_distance = 5;
		while(state == 1) 
		{
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = 0;
				make_everything();
				frame = 0;
				moves_of_target = 0;
			}
			
			frame++;
			char lives[] = "Lives: ";
      		char lives_amt = (char)(player.lives + 48); 
      		char *lives_ptr = &lives_amt;
      		char *lives_str = strcat(lives, lives_ptr);
			drawRect(150, 180, 10, 60, BLACK);
      		drawString(150,180,lives_str, GREEN);
			WaitForVblank();	
			//go to GameOver
			if(player.lives == 0) {
				state = 2;
			}
			//go to WinScreen
			if(num_of_targets_left == 0) {
				state = 3;
			}	
			
			drawRect(player.row, player.column, 15, 15, BLACK);
			if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
				player.column = player.column + cd;
			}
			if(KEY_DOWN_NOW(BUTTON_LEFT)) {
				player.column = player.column - cd;
			}
			if(KEY_DOWN_NOW(BUTTON_UP)) {
				player.row= player.row - rd;
			}
			if(KEY_DOWN_NOW(BUTTON_DOWN)) {
				player.row= player.row + rd;
			}
			
			boundsCheck(&player.column, 239, 15);
			boundsCheck2(&player.row, 100, 15);
			drawImage3(player.row, player.column, 15, 15, play);

			//move targets
			if(frame % 5 == 0) {
				targets_moving(target_move_distance);
				if(target_move_distance > 0) {
					moves_of_target++;
				} else if(target_move_distance < 0) {
					moves_of_target--;
				}
			}

			//Move targets one direction
			if(moves_of_target == NUM_OF_TARGETS_MOVES) {
				target_move_distance = -5;
			} else if(moves_of_target == 0) {
				target_move_distance = 5;
			}

			//draw targets after moevement
			for(int i = 0; i < AMT_OF_TARGETS; i++) {
				if(targets[i].isLiving) {
					drawImage3(targets[i].row, targets[i].col, targets[i].width, targets[i].height, targetPic);
				}
			}

			//Logic for player throwing balls
			for(int i = 0; i < TARGET_BULLETS; i++) {
				if(player_balls[i].fired) {				
					drawRect(player_balls[i].row, player_balls[i].col, 7, 7, BLACK);
          				player_balls[i].row -= 2;
          				drawImage3(player_balls[i].row, player_balls[i].col, 7, 7, baseball);
					
					//Check if any player balls hit any targets
					for(int j = 0; j < AMT_OF_TARGETS; j++) {
						player_bullet_target(&player_balls[i], &targets[j], i);
					}
				}
			}

			//Logic for targets firing bullets
			for(int i = 0; i < TARGET_BULLETS; i++) {
				if(target_bullets[i].fired) {
					//Check if any Alien bullets hit Spaceship
					if(target_bullet_player(&target_bullets[i], &player, i) == 1) {

					}
					else {
						drawRect(target_bullets[i].row, target_bullets[i].col, 5, 2, BLACK);
						target_bullets[i].row += 3;
						drawRect(target_bullets[i].row, target_bullets[i].col, 5, 2, WHITE);
					}
					//Check is bullet still on screen
					if(target_bullets[i].row >= 160) {
						target_bullets[i].fired = 0;
					}
				}
			}

			shooting_target_bullets();
			if(KEY_DOWN_NOW(BUTTON_A)) {
				shooting_player_balls();	
			}
		
		}
	while(state == 2) 
	{	
		drawImage3(0, 0, 240, 160, over);
		player.lives = 3;
		if(KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = 0;	
			make_everything();
		}	
	}
	
	if(state == 3)
	{

		
		drawImage3(0, 0, 240, 160, win);
		player.lives = 3;

		while(state == 3)
		{
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = 0;
				make_everything();
			}
		}
	} //Game Loop
}
}
