typedef unsigned short u16;

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define CYAN COLOR(0,31,31)
#define MAGENTA COLOR(31, 0,31)
#define YELLOW COLOR(31,31,0)
#define WHITE COLOR(31,31,31)
#define BLACK 0

#define SCANLINECOUNTER  (*(volatile unsigned short *)0x4000006)

#define OFFSET(r, c, numcols) ((r)*(numcols)+(c))

// Buttons
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT		(1<<2)
#define BUTTON_START		(1<<3)
#define BUTTON_RIGHT		(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

//Game Defines

#define PLAYER_BALLS 1000
#define TARGET_BULLETS 1000
#define AMT_OF_TARGETS 10
#define NUM_OF_TARGETS_MOVES 15

/*player Info*/
typedef struct player {
	int row;
	int column;
	int lives;
} Player;

/* bullet/ball Info*/
typedef struct bullet {
	int row;
	int col;
	int fired;
} Bullet;

/* target Info*/
typedef struct target{
	int row;
	int col;
	int height;
	int width;
	int isLiving;
} Target;

/*Creating what will be used*/
Player player;
Bullet player_balls[PLAYER_BALLS];
Target targets[AMT_OF_TARGETS];
Bullet target_bullets[TARGET_BULLETS];
int num_of_targets_left;



//DMA 
#define BUTTONS *(volatile unsigned int *)0x4000130

typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

extern unsigned short *videoBuffer;

// Prototype
void setPixel(int row, int col, u16 color);
void drawChar(int row, int col, char ch, u16 color);
void drawString(int row, int col, char *str, u16 color);
void drawRect(int row, int col, int height, int width, u16 color);
int boundsCheck(int *var, int bound, int size);
int boundsCheck2(int *var, int bound, int size);
void WaitForVblank();
void blank_screen();
void drawImage3(int r, int c, int width, int height, const
u16* image);


void initiate_player();
void initiate_player_balls();
void shooting_player_balls();
void initiate_targets();
void initiate_target_bullets();
void shooting_target_bullets();
void targets_moving(int amt);
void fun_moving();
void fun_make();
void player_bullet_target(Bullet *bullet, Target *target, int i);
int target_bullet_player(Bullet *bullet, Player *player, int i);




