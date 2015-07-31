#include "myLib.h"
#include "font.h"
#include <stdlib.h>

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawChar(int row, int col, char ch, u16 color) 
{
	int r,c;
	for(r=0; r<8; r++){
		for(c=0; c<6; c++){
			if(fontdata_6x8[OFFSET(r,c,6) + 48*ch]){
				setPixel(r + row, c + col, color);
			}	
		}
	}
}

void drawString(int row, int col, char *str, u16 color) 
{
	int chars_drawn = 0;
	while(*str) {
		drawChar(row, col + 6*chars_drawn++, *str++, color);
	}
}


void drawRect(int row, int col, int height, int width, volatile u16 color)
{
	for(int r=0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
		DMA[3].cnt = DMA_ON | width | DMA_SOURCE_FIXED;
	}
}


int boundsCheck(int *var, int bound, int size)
{
		if(*var < 0)
		{
			*var = 0;
		}
		if(*var > bound-size+1)
		{
			*var = bound-size+1;
		}
		return 0;

}

int boundsCheck2(int *var, int bound, int size)
{
		if(*var > 130)
		{
			*var = 130;
		}
		if(*var < bound-size+1)
		{
			*var = bound-size+1;
		}
		return 0;

}

void WaitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void blank_screen() 
{
	DMA[3].src = COLOR(0, 0, 0);
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;
}

void drawImage3(int r, int c, int width, int height, const
u16* image)
{
	for(int i = 0; i < height; i++) {
	DMA[3].src = image + (i * width);
	DMA[3].dst = &videoBuffer[OFFSET(r + i, c, 240)];
	DMA[3].cnt = width|DMA_ON;
	}
}

/* Place player and initialize number of Lives*/
void initiate_player()
{
	player.row = 135;
	player.column = 120;
	player.lives = 3;
}

/* Initialize player Balls*/
void initiate_player_balls()
{
	for(int i = 0; i < PLAYER_BALLS; i++) {
		player_balls[i].row = 0;
		player_balls[i].col = 0;
		player_balls[i].fired = 0;
	}
}

/* Fires player balls */
void shooting_player_balls()
{
	int pos_of_ball = 0;
	int need_fire = -1;
	
	for(int i = 0; i < 1000; i++) {
		if(!(player_balls[i].fired) && need_fire == -1) {
			need_fire = i;
		}
		
		if(player_balls[i].fired && 
		player_balls[i].row > pos_of_ball) {
			pos_of_ball = player_balls[i].row;
		}
	}
	
	if(need_fire != -1 && pos_of_ball < 120) {
		player_balls[need_fire].row = player.row + 7;
		player_balls[need_fire].col = player.column + 7;
		player_balls[need_fire].fired = 1;
	}
}	

/* Place targets*/
void initiate_targets() 
{
	for(int i = 0; i < AMT_OF_TARGETS; i++) {
		targets[i].row = 10;
		targets[i].col = 10 + (15 * i);
		targets[i].isLiving = 1;
		targets[i].width = 15;
		targets[i].height = 15;
	}
}

/* Initialize target Bullets*/
void initiate_target_bullets()
{
	for(int i = 0; i < TARGET_BULLETS; i++) {
		target_bullets[i].row = 0;
    		target_bullets[i].col = 0;
    		target_bullets[i].fired = 0;
	}
}

/* Shooting Bullets for targets*/
void shooting_target_bullets()
{
	int target;
	int shoot = rand() % 150;
	for(int i = 0; i < TARGET_BULLETS; i++) {
		if(!target_bullets[i].fired) {
			if(shoot <= 1) {
				do {
					target = rand() % 10;
				} while (!targets[target].isLiving);
				
				//Shooting bullets
					target_bullets[i].row = 20;
        			target_bullets[i].col = targets[target].col + 5;
        			target_bullets[i].fired = 1;
        			return;
			}
		}
	}
}

/* Making targets Move*/
void targets_moving(int amt)
{
	for(int i = 0; i < AMT_OF_TARGETS; i++) {
		drawRect(targets[i].row, targets[i].col, 15, 15, BLACK);
		targets[i].col += amt;
		//targets[i].row += 2;
	}
}


/* Collision check for player balls hitting targets*/
void player_bullet_target(Bullet *bullet, Target *target, int i)
{
	if(target -> isLiving) {
		if(target -> row + 5 >= bullet -> row && (target -> col <= bullet -> col
		&& target -> col + 10 >= bullet -> col)) {
			
			target -> isLiving = 0;
		    drawRect(bullet[i].row, bullet[i].col, 7, 7, BLACK);
			num_of_targets_left--;
			bullet -> fired = 0;
		}
	}
}

/* Collision check for target bullets hitting player*/
int target_bullet_player(Bullet *bullet, Player *player, int i)
{
	if(bullet -> fired) {
		if(bullet -> row + 5 >= player -> row && (bullet -> col + 2 >= player -> column
		&& bullet -> col <= player -> column + 15)) {
			
			bullet -> fired = 0;
			drawRect(bullet[i].row, bullet[i].col, 6, 3, BLACK);
			player -> lives--;
			return 1;
		}
	}
return 0;
}
