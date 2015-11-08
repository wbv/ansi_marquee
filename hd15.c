//
// Copyright LOLOLOL who would want this garbage
//

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

const unsigned char ANSI_ESC = 0x1B;
const unsigned char ANSI_CSI = '[';

const unsigned FRAMEDELAY = 10; // in ms

// cursor positional variables
unsigned cursor_x = 1;
unsigned cursor_y = 1;

// code for handling SIGINT
volatile sig_atomic_t killflag = 0;
void kill_handler ( int sig )
{
	killflag = 1;
}

void delay ( unsigned t )
{
	usleep(t * 1000); // in ns
	printf("\n");
}

void csi ( char* text )
{
	printf("%c%c", ANSI_ESC, ANSI_CSI);
	printf(text);
}

void csi_wait ( char* text )
{
	csi(text);
	delay(FRAMEDELAY);
}

void cursor_move ( unsigned x, unsigned y )
{
	const char* fmt_pos = "%d;%dH";
	int len_x_fmt = snprintf(NULL, 0, fmt_pos, x);
	int len_y_fmt = snprintf(NULL, 0, fmt_pos, y);
	char fmt_buffer[len_x_fmt+len_y_fmt+1];
	snprintf(fmt_buffer, sizeof(fmt_buffer), fmt_pos, x, y);
	csi(fmt_buffer);
}

void cursor_update ( void )
{
	cursor_move(cursor_y, cursor_x);
}

void draw ( void )
{
	cursor_update();
	printf("\u2588");
	delay(FRAMEDELAY);
}

void draw_h ( unsigned x, unsigned y, signed len )
{
	signed short shift = len / abs(len);
	cursor_y = y;
	for (cursor_x = x; cursor_x != x+len; cursor_x += shift)
	{
		draw();
	}
}

void draw_v ( unsigned x, unsigned y, signed len )
{
	signed short shift = len / abs(len);
	cursor_x = x;
	for (cursor_y = y; cursor_y != y+len; cursor_y += shift)
	{
		draw();
	}
}

void draw_diag_down ( unsigned x, unsigned y, signed len )
{
	signed short shift = len / abs(len);
	cursor_y = y;
	for (cursor_x = x; cursor_x != x+len; cursor_x += shift)
	{
		draw();
		cursor_y++;
	}
}

void draw_diag_up ( unsigned x, unsigned y, signed len )
{
	signed short shift = len / abs(len);
	cursor_y = y;
	for (cursor_x = x; cursor_x != x+len; cursor_x += shift)
	{
		draw();
		cursor_y--;
	}
}

void speak ( void )
{
	// 1-cell padded origin
	int x = 2;
	int y = 2;
	
	// H
	draw_v(0+x,0+y,5);
	draw_h(1+x,2+y,2);
	draw_v(3+x,0+y,5);
	
	x += 5;
	
	// A
	draw_v(0+x,1+y,4);
	draw_h(1+x,0+y,2);
	draw_v(3+x,1+y,4);
	draw_h(1+x,2+y,2);
	
	x += 5;
	
	// C
	draw_h(3+x,0+y,-3);
	draw_v(0+x,1+y,3);
	draw_h(1+x,4+y,3);
	
	x += 5;
	
	// K
	draw_v(0+x,0+y,5);
	draw_diag_down(3+x,0+y,-3);
	draw_diag_down(2+x,3+y,2);
	
	// \n \t
	x -= 10;
	y += 6;	

	// D
	draw_v(0+x,0+y,5);
	draw_h(1+x,0+y,2);
	draw_v(3+x,1+y,3);
	draw_h(2+x,4+y,-2);	

	x += 5;
	
	// U
	draw_v(0+x,0+y,4);
	draw_h(1+x,4+y,2);
	draw_v(3+x,3+y,-4);

	x += 5;
	
	// K
	draw_v(0+x,0+y,5);
	draw_diag_down(3+x,0+y,-3);
	draw_diag_down(2+x,3+y,2);
	
	x += 5;
	
	// E
	draw_v(0+x,0+y,5);
	draw_h(1+x,0+y,3);
	draw_h(1+x,2+y,2);
	draw_h(1+x,4+y,3);	

	x += 5;

	// !
	draw_v(1+x,0+y,3);
	draw_v(1+x,4+y,1);
	
	// \t ^/2
	x += 7;
	y -= 3;
	
	// 2
	draw_v(0+x,1+y,1);
	draw_h(1+x,0+y,2);
	draw_diag_down(3+x,1+y,-4);
	draw_h(1+x,4+y,3);

	x += 5;
	
	// 0
	draw_v(0+x,3+y,-3);
	draw_h(1+x,0+y,2);
	draw_v(3+x,1+y,3);
	draw_h(2+x,4+y,-2);
	
	x += 5;

	// 1
	draw_h(1+x,1+y,1);
	draw_v(2+x,0+y,5);
	draw_h(0+x,4+y,4);

	x += 5;

	// 5
	draw_h(3+x,0+y,-4);
	draw_v(0+x,1+y,2);
	draw_h(1+x,2+y,2);
	draw_h(3+x,3+y,1);
	draw_h(2+x,4+y,-3);
}

int main()
{
	// register SIGKILL handler
	signal(SIGINT, kill_handler);	

	// clear screen, hide cursor, go to upper right corner
	csi("2J");
	csi("H");
	csi("?25l");
	printf("\n");
	csi("H");
	
	// cycle through regular SGR modes 
	char color_csi[4]; // contains CSI code for `color` in string format
	unsigned color = 30;
	while (1)
	{
		for (color = 30; color < 38; color++) 
		{
			snprintf(color_csi, sizeof(color_csi), "%dm", color);
			csi(color_csi);
			speak();
			if(killflag) goto end_nice;
		}
	}

	// wait until character input, then clear and reset.	
	end_nice:
	csi("0m");
	csi("2J");
	csi("H");
	csi("?25h");
	return 0;
}
