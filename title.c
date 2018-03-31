/*---------------------------------------------------------------------------------

	AGBIC2016 - title()
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

void title(void) {

	consoleInitText(0, 0, &snesfont);
	consoleSetShadowCol(0,RGB15(16,16,16));

	setMode(BG_MODE1,0);  bgSetDisable(1);  bgSetDisable(2);

	consoleDrawText(9,10,"title: press button");


	pad = padsCurrent(0); 
	while(!pad) {
		pad = padsCurrent(0);
		
		WaitForVBlank();
	}
	
	// srand(snes_vblank_count);

	return;
}
