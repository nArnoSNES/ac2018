/*---------------------------------------------------------------------------------

	AGBIC2016 - common lib (i'll add my custom common function here)
		-- n_Arno

---------------------------------------------------------------------------------*/
#include <snes.h>

// copy paste of the consoleSetTextCol, but for color 2 of the "paletteNumber"
// see play.c for a comment on "how" i believe it works
void consoleSetShadowCol(u8 paletteNumber, u16 colorShad) {
	REG_CGADD = 0x02+(paletteNumber<<4); *CGRAM_PALETTE = colorShad & 0xFF; *CGRAM_PALETTE = colorShad>>8;
}
