
#include <gba_console.h>
#include <gba_input.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

#include <stdio.h>
#include <stdlib.h>


int wincount = 0;
int rollstats[] = {0,0,0,0,0,0};

int rollDice()
{
	int die1 = rand() % 6 + 1;
	int die2 = rand() % 6 + 1;

	if (die1 == 1)
		rollstats[0] = rollstats[0] + 1;
	else if (die1 == 2)
		rollstats[1] = rollstats[1] + 1;
	else if (die1 == 3)
		rollstats[2] = rollstats[2] + 1;
	else if (die1 == 4)
		rollstats[3] = rollstats[3] + 1;
	else if (die1 == 5)
		rollstats[4] = rollstats[4] + 1;
	else if (die1 == 6)
		rollstats[5] = rollstats[5] + 1;
		
	if (die2 == 1)
		rollstats[0] = rollstats[0] + 1;
	else if (die2 == 2)
		rollstats[1] = rollstats[1] + 1;
	else if (die2 == 3)
		rollstats[2] = rollstats[2] + 1;
	else if (die2 == 4)
		rollstats[3] = rollstats[3] + 1;
	else if (die2 == 5)
		rollstats[4] = rollstats[4] + 1;
	else if (die2 == 6)
		rollstats[5] = rollstats[5] + 1;

	return die1 + die2;

}

void simulateCraps(int games)
{
	int count = 0;
	
	while (count < games)
	{
		int result = rollDice();
		
		
		if (result == 7 || result == 11)
		{
			wincount = wincount + 1;
			count = count + 1;
		}
		else if (result == 2 || result == 3 || result == 12)
		{
			count = count + 1;
		
		}
		else
		{
			count = count + 1;
			int lastResult = result;
			int cont = 1;
			while (cont == 1)
			{
				result = rollDice();
				if (result == 7)
				{
					cont = 0;
					
				}
				else if (result == lastResult)
				{
					wincount = wincount + 1;
					cont = 0;
				}
			
			}
		}
	}
}

void printStats()
{
	iprintf("\x1b[3;0H              ");
	iprintf("\x1b[3;0HOnes: %d",rollstats[0]);
	iprintf("\x1b[4;0H              ");
	iprintf("\x1b[4;0HTwos: %d",rollstats[1]);
	iprintf("\x1b[5;0H              ");
	iprintf("\x1b[5;0HThrees: %d",rollstats[2]);
	iprintf("\x1b[6;0H              ");
	iprintf("\x1b[6;0HFours: %d",rollstats[3]);
	iprintf("\x1b[7;0H              ");
	iprintf("\x1b[7;0HFives: %d",rollstats[4]);
	iprintf("\x1b[8;0H              ");
	iprintf("\x1b[8;0HSixes: %d",rollstats[5]);
}

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	// since the default dispatcher handles the bios flags no vblank handler
	// is required
	int gamecount = 1;
	
	
	irqInit();
	irqEnable(IRQ_VBLANK);

	//consoleDemoInit();
	consoleInit(	0,		/* charbase */
					4,		/* mapbase */
					0,		/* background number */
					NULL,	/* font */
					0, 		/* font size */
					15		/* 16 color palette */);
					
	BG_COLORS[0]=RGB8(0,0,0);
	BG_COLORS[241]=RGB5(31,31,31);

	SetMode(MODE_0 | BG0_ON);


	// ansi escape sequence to clear screen and home cursor
	// /x1b[line;columnH
	iprintf("\x1b[2J");

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH


	// ansi escape sequence to move cursor up
	// /x1b[linesA


	// ansi escape sequence to move cursor left
	// /x1b[columnsD


	// ansi escape sequence to move cursor down
	// /x1b[linesB
	

	// ansi escape sequence to move cursor right
	// /x1b[columnsC
	
	
	iprintf("\x1b[0;0HGames: %d",gamecount);

	while (1) {
		VBlankIntrWait();
		scanKeys();

		int keys_pressed = keysDown();
		int keys_released = keysUp();
		
		if (keys_pressed & KEY_UP)
		{
			gamecount = gamecount + 1;
			iprintf("\x1b[0;0H             ");
			iprintf("\x1b[0;0HGames: %d",gamecount);
		}
		if (keys_pressed & KEY_DOWN)
		{
			if (gamecount > 1)
			{
			gamecount = gamecount - 1;
			iprintf("\x1b[0;0H             ");
			iprintf("\x1b[0;0HGames: %d",gamecount);
			}
		}
		if (keys_pressed & KEY_A)
		{
			wincount = 0;
			rollstats[0] = 0;
			rollstats[1] = 0;
			rollstats[2] = 0;
			rollstats[3] = 0;
			rollstats[4] = 0;
			rollstats[5] = 0;
			simulateCraps(gamecount);
			iprintf("\x1b[1;0HWe simulated %d games",gamecount);
			iprintf("\x1b[2;0HYou won %d games",wincount);
			printStats();
		}
		
		
	}
}


