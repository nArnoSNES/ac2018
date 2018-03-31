/*---------------------------------------------------------------------------------


	AGBIC2016 - splash()
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
void splash(void) {
   

	consoleInitText(0, 0, &snesfont);
	consoleSetShadowCol(0,RGB15(16,16,16));

	setMode(BG_MODE1,0);  bgSetDisable(1);  bgSetDisable(2);

	consoleDrawText(12,10,"splash !");


    delay = 0;
	while(delay<400) {
		delay++;
		WaitForVBlank();
	}

	return;
}
