#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include “draw.h”


const unsigned char ANSI_ESC = 0x1B;
const unsigned char ANSI_CSI = '[';

// code for handling SIGINT
volatile sig_atomic_t killflag = 0;
void kill_handler ( int sig )
{
	killflag = 1;
	quick_exit(0);
}


void speak ( void )
{	
	draw_v(2,2,5); 
	draw_h(5,2,-3); 
	draw_h(5,4,-3);

	draw_v(7,2,4);
	draw_h(8,6,2);
	draw_v(10,5,-4);

	draw_h(15,2,-3);
	draw_v(12,3,3);
	draw_h(13,6,3);
	
	draw_v(17,2,5);
	draw_diag_down(20,2,-3);
	draw_diag_down(19,5,2);
	
	draw_v(22, 6, 1);
}

void shutdown ( void )
{
	csi("0m");
	csi("2J");
	csi("H");
	csi("?25h");
}

int main()
{
	// register SIGKILL handler
	signal(SIGINT, kill_handler);	
	at_quick_exit(shutdown);
	
	// clear screen, hide cursor, go to upper right corner
	csi("2J");
	csi("H");
	csi("?25l");
	printf("\n");
	csi("H");
	
	while (1)
	{
		for (color = 30; color < 38; color++) 
		{
			snprintf(color_csi, sizeof(color_csi), "%dm", color);
			csi(color_csi);
			speak();
		}
	}

	// clear and reset.	
	shutdown();
	return 0;
}
