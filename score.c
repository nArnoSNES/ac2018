/*---------------------------------------------------------------------------------


	AGBIC2016 - printScore()
		-- n_Arno


---------------------------------------------------------------------------------*/
#include <snes.h>
#include "common.h"

extern char snesfont;

/*-----------------------------
        All global vars
------------------------------*/
extern int delay; // inner wait counter
extern unsigned short pad; // to receive status of 1P joypad


//---------------------------------------------------------------------------------
void printScore(void) {

	consoleInitText(0, 0, &snesfont);
        consoleSetTextCol(RGB15(11,20,13),RGB15(0,0,0));
        consoleSetShadowCol(1 ,RGB15(6,10,6));

	setMode(BG_MODE1,0);  bgSetDisable(1);  bgSetDisable(2);

	consoleDrawText(12,10,"Game Over");


	delay = 0;
	while(delay<400) {
		delay++;
		WaitForVBlank();
	}
	return;
}
