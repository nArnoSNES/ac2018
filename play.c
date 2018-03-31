/*---------------------------------------------------------------------------------

	AGBIC2016 - play()
		-- n_Arno

---------------------------------------------------------------------------------*/
#include <snes.h>
#include "common.h"

extern char snesfont;
extern char bg1_tiles, bg1_tiles_end;
extern char bg1_pal, bg1_pal_end;
extern char bg1_map, bg1_map_end;

extern char col1_map, col1_map_end;

extern char sprites, sprites_end;
extern char sprites_pal, sprites_pal_end;

/*-----------------------------
        All global vars
------------------------------*/
extern int delay; // inner wait counter
extern unsigned short pad; // to receive status of 1P joypad

/*-----------------------------
       In Game Vars
------------------------------*/
#define DIR_LEFT 1
#define DIR_RIGHT 0
#define JUMPSPEED 4
#define JUMPDELTA 70
#define WALKSPEED 2

typedef struct
{
	unsigned int x,y; // x pos, y pos
	char tics, frame, state, direction, animf; // animf is only used during jump. Because for now, i can't set x, y or vflip without also knowing the current anim frame
	int jumpspeed, jumpdelta, walkspeed;
} Player;
Player player;

// Sprite State values
enum PlayerSprite {WALK0 = 0, WALK1 = 4, WALK2 = 8, WALK3 = 12, JUMP0 = 64, JUMP1 = 68, JUMP2 = 72, BONK0 = 76, BONK1 = 128};

//---- ugly collision detection function ----
int getCollisionTile(int x, int y) {
	/* divide x,y position by 8 to get the upper left tile of the sprite position */
	int x8 = (x>>3);
	int y8 = (y>>3);
   	// Check all outer tile of the sprite to avoid clipping (all must be Zero for ok)
	int *ptr1 = (int *) &col1_map + y8*32 + x8;
	int *ptr2 = (int *) &col1_map + y8*32 + x8+1;
	int *ptr3 = (int *) &col1_map + y8*32 + x8+2;
	int *ptr4 = (int *) &col1_map + y8*32 + x8+3;
	int *ptr5 = (int *) &col1_map + y8*32+32 + x8;
	int *ptr6 = (int *) &col1_map + y8*32+32 + x8+3;
	int *ptr7 = (int *) &col1_map + y8*32+64 + x8;
	int *ptr8 = (int *) &col1_map + y8*32+64 + x8+3;
	int *ptr9 = (int *) &col1_map + y8*32+96 + x8;
	int *ptr0 = (int *) &col1_map + y8*32+96 + x8+1;
	int *ptrA = (int *) &col1_map + y8*32+96 + x8+2;
	int *ptrB = (int *) &col1_map + y8*32+96 + x8+3;

   	return (*ptr1+*ptr2+*ptr3+*ptr4+*ptr5+*ptr6+*ptr7+*ptr8+*ptr9+*ptr0+*ptrA+*ptrB);
}

int getCollisionTileDown(int x, int y) {
	/* divide x,y position by 8 to get the upper left tile of the sprite position */
	int x8 = (x>>3);
	int y8 = (y>>3);
   	// Check all tile below the sprite to check if on ground (if all Zero, player is not standing on ground)
	int *ptr9 = (int *) &col1_map + y8*32+128 + x8;
	int *ptr0 = (int *) &col1_map + y8*32+128 + x8+1;
	int *ptrA = (int *) &col1_map + y8*32+128 + x8+2;
	int *ptrB = (int *) &col1_map + y8*32+128 + x8+3;

   	return (*ptr9+*ptr0+*ptrA+*ptrB);
}

//-------------------------------------------
// State Machine!!!
//---------------------------------------------------------------------------------
// Small reminder of the parameters of this function
// #define oamSet(id,  xspr, yspr, priority, hflip, vflip, gfxoffset, paletteoffset)
//---------------------------------------------------------------------------------
void move_in_world(void)
{
	if (player.direction == DIR_LEFT && player.x > 0 && getCollisionTile(player.x-player.walkspeed, player.y) == 0) player.x-=player.walkspeed;
	if (player.direction == DIR_RIGHT && player.x < 224 && getCollisionTile(player.x+player.walkspeed, player.y) == 0) player.x+=player.walkspeed;
}

void player_state_0(void)
{
	if (getCollisionTileDown(player.x, player.y) ==0) /* player is not standing on ground! let's fall! */
	{
		player.tics = 0;
		player.state = 4;
		player.jumpspeed = 1;
		return;
	}
	/* this is player's idle stance... if the tics are equal to 0, set his frame to default and his tic counter to 1 */
	if (player.tics == 0) oamSet(0, player.x, player.y, 3, player.direction, 0, WALK1, 0);
	player.tics = 1;
	/* most other states can be set in this state */
	if (pad & KEY_LEFT)
	{
		/* start player moving to the left! */
		player.tics = 0;
		player.frame = 0;
		player.state = 1;
		player.direction = DIR_LEFT;
		oamSet(0, player.x, player.y, 3, player.direction, 0, WALK0, 0);
	}
	if (pad & KEY_RIGHT)
	{
		/* start player moving to the right! */
		player.tics = 0;
		player.frame = 0;
		player.state = 1;
		player.direction = DIR_RIGHT;
		oamSet(0, player.x, player.y, 3, player.direction, 0, WALK0, 0);
	}
	if(pad & KEY_B)
	{
		/* set player in bonking state! */
		player.tics = 0;
		player.frame = 0;
		player.state = 6;
		oamSet(0, player.x, player.y, 3, player.direction, 0, BONK0, 0);
	}
	if(pad & KEY_A)
	{
		/* make player jump! */
		player.tics = 0;
		player.frame = 0;
		player.state = 2;
		oamSet(0, player.x, player.y, 3, player.direction, 0, JUMP0, 0);
	}
}
void player_state_1(void)
{
	/* player's walking state! */
	char stillwalking;
	stillwalking = 0;
	player.tics++;
	if (player.tics > 4)
	{
		player.tics = 0;
		player.frame++;
		if (player.frame > 5) player.frame = 0;
		if (player.frame == 0) oamSet(0, player.x, player.y, 3, player.direction, 0, WALK1, 0);
		if (player.frame == 1) oamSet(0, player.x, player.y, 3, player.direction, 0, WALK2, 0);
		if (player.frame == 2) oamSet(0, player.x, player.y, 3, player.direction, 0, WALK3, 0);
		if (player.frame == 3) oamSet(0, player.x, player.y, 3, player.direction, 0, WALK2, 0);
		if (player.frame == 4) oamSet(0, player.x, player.y, 3, player.direction, 0, WALK1, 0);
		if (player.frame == 5) oamSet(0, player.x, player.y, 3, player.direction, 0, WALK0, 0);
	}
	if (pad & KEY_LEFT)
	{
		stillwalking = 1;
	}
	if (pad & KEY_RIGHT)
	{
		stillwalking = 1;
	}
	if (pad & KEY_B)
	{
		/* set player in bonking state! */
		player.tics = 0;
		player.frame = 0;
		player.state = 6;
		oamSet(0, player.x, player.y, 3, player.direction, 0, BONK0, 0);
	}
	if (pad & KEY_A)
	{
		/* make player jump! */
		player.tics = 0;
		player.frame = 0;
		player.state = 2;
		oamSet(0, player.x, player.y, 3, player.direction, 0, JUMP0, 0);
	}
	if (getCollisionTileDown(player.x, player.y) ==0) /* player is not standing on ground! let's fall! */
	{
		player.tics = 0;
		player.state = 4;
		player.jumpspeed = 1;
		return;
	}
	if (stillwalking == 0)
	{
		player.state = 0;
		player.tics = 0;
		player.frame = 0;
	} else {
		move_in_world();
	}
}
void player_state_2(void)
{
	/* player's pre-jump prep state! all controls are disabled in this one */
	player.tics++;
	if (player.tics > 4)
	{
		player.jumpspeed = JUMPSPEED;
		player.jumpdelta = JUMPDELTA;
		player.tics = 0; /* tics is going to be used in a different way in state 3... you'll see! */
		player.animf = JUMP1;
		oamSet(0, player.x, player.y, 3, player.direction, 0, JUMP1, 0);
		player.state = 3;
	}
}
void player_state_3(void)
{
	/* player's jumping up state! */
	if (getCollisionTile(player.x, player.y-player.jumpspeed) != 0)
	{
		/* player hit something, start falling */
		player.tics = 0;
		player.state = 4;
		player.jumpspeed = 1;
		return;
	}
	/* change player pos */
	player.y-=player.jumpspeed;
	/* here, we're going to use tics to determine how high player has jumped, and compare it to the jump delta */
	player.tics+=player.jumpspeed;
	if (player.tics > player.jumpdelta)
	{
		/* if the total jump distance exceeds the jump delta, reduce the upward motion until it reaches zero */
		/* the reduction in upward motion makes the jump appear a little more natural than just immediately changing directions */
		player.animf = JUMP2;
		player.jumpspeed--;
		if (player.jumpspeed<0)
		{
			/* when the jump speed decreases past 0, we will need to set player in state 4 */
			player.tics = 0;
			player.state = 4;
			player.jumpspeed = 1;
		}
	}
	if (pad & KEY_LEFT)
	{
		player.direction = DIR_LEFT;
		move_in_world();
	}
	if (pad & KEY_RIGHT)
	{
		player.direction = DIR_RIGHT;
		move_in_world();
	}
	oamSet(0, player.x, player.y, 3, player.direction, 0, player.animf, 0);
}
void player_state_4(void)
{
	/* player's falling down state! */
	if (getCollisionTile(player.x, player.y+player.jumpspeed) != 0)
	{
		/* player hit something, land!! */
		player.frame = 0;
		player.tics = 0;
		player.state = 5;
		player.animf = JUMP0;
		oamSet(0, player.x, player.y, 3, player.direction, 0, JUMP0, 0);
		return;
	}
	/* execute fall */
	player.y+=player.jumpspeed;
	if (player.jumpspeed < 4) player.jumpspeed++;
	if (player.y > 18*8-1)
	{
		player.y = 18*8;
		player.frame = 0;
		player.tics = 0;
		player.state = 5;
		player.animf = JUMP0;
		oamSet(0, player.x, player.y, 3, player.direction, 0, JUMP0, 0);
	}
	if (pad & KEY_LEFT)
	{
		player.direction = DIR_LEFT;
		move_in_world();
	}
	if (pad & KEY_RIGHT)
	{
		player.direction = DIR_RIGHT;
		move_in_world();
	}
	oamSet(0, player.x, player.y, 3, player.direction, 0, player.animf, 0);
}
void player_state_5(void)
{
	/* player's landing state; same frame as state 2, but reverts to state 0 when finished rather than state 3 */
	player.tics++;
	if (player.tics > 4)
	{
		player.tics = 0;
		player.state = 0;
		player.frame = 0;
	}
}
void player_state_6(void)
{
	/* player's bonking state! all controls are disabled in this one; this is just an animation handler/state controller */
	player.tics++;
	if (player.tics == 5)
	{
		/* we need to adjust player's position, as this sprite is a few pixels to the left normally... this is why we need 'direction'! */
		if (player.direction == DIR_LEFT)
		{
			oamSet(0, player.x-8, player.y, 3, player.direction, 0, BONK1, 0);
		} else {
			oamSet(0, player.x+8, player.y, 3, player.direction, 0, BONK1, 0);
		}
		
	}
	if (player.tics > 10)
	{
		/* as this animation ends, restore the sprite to its normal posision and set the state back to 0 */
		oamSet(0, player.x, player.y, 3, player.direction, 0, BONK0, 0);
		player.tics = 0;
		player.state = 0;
	}
}

void player_state_machine(void)
{
	/*
player's State Machine:
0 - Idle (all buttons available)
1 - Walking (all buttons available)
2 - About to jump (all buttons disabled)
3 - Jumping (directions available only)
4 - Jumping down (directions available only)
5 - Landing (all buttons disabled)
6 - bonking (all buttons disabled)
*/
	if (player.state == 0) player_state_0();
	if (player.state == 1) player_state_1();
	if (player.state == 2) player_state_2();
	if (player.state == 3) player_state_3();
	if (player.state == 4) player_state_4();
	if (player.state == 5) player_state_5();
	if (player.state == 6) player_state_6();
}

//---------------------------------------------------------------------------------
void play(void) {
	
	player.x =  9*8;
	player.y = 18*8-1; // The ground! Well, minus 32 pix height
	player.tics = 0;
    player.frame = 0;
    player.walkspeed = WALKSPEED;

	bgInitTileSet(1, &bg1_tiles, &bg1_pal, 0, (&bg1_tiles_end - &bg1_tiles), (&bg1_pal_end-&bg1_pal), BG_16COLORS, 0x4000);
	bgInitMapSet(1, &bg1_map, (&bg1_map_end - &bg1_map),SC_32x32, 0x03000);

	consoleInitText(0, 1, &snesfont);
	consoleSetTextCol(RGB15(10,10,6),RGB15(0,0,0));
	consoleSetShadowCol(1 ,RGB15(20,20,16));

	setMode(BG_MODE1,0);  bgSetDisable(2);

	setBrightness(0);
	oamInitGfxSet(&sprites, (&sprites_end-&sprites), &sprites_pal, (&sprites_pal_end-&sprites_pal), 0, 0x6000, OBJ_SIZE32);
	setBrightness(0xF);

	consoleDrawText(15,1,"Game: X to quit");
	oamSet(0,  player.x, player.y, 3, 0, 0, 4, 0);
	oamSetEx(0, OBJ_SMALL, OBJ_SHOW);

	pad = padsCurrent(0); 
	while(1) {
		pad = padsCurrent(0);
		if (pad & KEY_X) break;
		player_state_machine();
		WaitForVBlank();
	}
	
	oamSetVisible(0, OBJ_HIDE);

	return;
}
