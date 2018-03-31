/*---------------------------------------------------------------------------------

	AGBIC2016 - main()
		-- n_Arno

---------------------------------------------------------------------------------*/
#include <snes.h>
#include "main.h"

/*-----------------------------
        All global vars def
------------------------------*/
int delay; // inner wait counter
unsigned short pad; // to receive status of 1P joypad


//---------------------------------------------------------------------------------
int main(void) {
	consoleInit();

	// Display splash screen (once)
	splash();

	// As Usual, don't exit the main()
	while(1) {
		// Display title screen
		title();

		// Execute main function
		play();

		// Display score in a screen
		printScore();

		// Just in case
		WaitForVBlank();
	}
	return 0;
}
