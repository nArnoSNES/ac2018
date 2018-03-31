/*---------------------------------------------------------------------------------

	AGBIC2016 - title()
		-- n_Arno

---------------------------------------------------------------------------------*/
#include <snes.h>
#include "common.h"

extern char title_pic, title_pic_end;
extern char title_pal;
extern char title_map, title_map_end;

/*-----------------------------
        All global vars
------------------------------*/
extern int delay; // inner wait counter
extern unsigned short pad; // to receive status of 1P joypad

void title(void) {

        // Copy tiles to VRAM
        bgInitTileSet(0, &title_pic, &title_pal, 0, (&title_pic_end - &title_pic), 16*2, BG_16COLORS, 0x4000);

        // Copy Map to VRAM
        bgInitMapSet(0, &title_map, (&title_map_end - &title_map), SC_32x32, 0x1000);

        // Now Put in 16 color mode and disable other BGs except 1st one
        setMode(BG_MODE1,0);  bgSetDisable(1); bgSetDisable(2);
        setScreenOn();

	pad = padsCurrent(0); 
	while(!pad) {
		pad = padsCurrent(0);
		
		WaitForVBlank();
	}
	
        
	// srand(snes_vblank_count);

	return;
}
