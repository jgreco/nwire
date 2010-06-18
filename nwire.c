#include <ncurses.h>
#include "map_2d.h"
#include <stdio.h>

#define DEFAULT_X 100
#define DEFAULT_Y 100

enum states { EMPTY, ELECTRON_HEAD, ELECTRON_TAIL, CONDUCTOR };


map_2d grid, render;
static int size_x;
static int size_y;
static int x_offset;
static int y_offset;
static int cursor_x;
static int cursor_y;

void redraw_screen();
void step();
int check_conductor(int x, int y);
void ninterface();


int main()
{
	/* INIT MAIN AND RENDER GRID */
	grid   = map_create(DEFAULT_X, DEFAULT_Y);
	render = map_create(DEFAULT_X, DEFAULT_Y);


	/* INIT NCURSES STUFF */

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	start_color();
	use_default_colors();

	init_pair(EMPTY, COLOR_BLACK, COLOR_BLACK);
	init_pair(ELECTRON_HEAD, COLOR_RED, COLOR_RED);
	init_pair(ELECTRON_TAIL, COLOR_BLUE, COLOR_BLUE);
	init_pair(CONDUCTOR, COLOR_YELLOW, COLOR_YELLOW);

	cursor_x = 0;
	cursor_y = 0;
	x_offset = 0;
	y_offset = 0;

	ninterface();

	endwin();

	return 0;
}

void ninterface()
{
	int key = 0;

	while(key != 'Q') {
		redraw_screen();

		key = getch();

		switch(key) {
			case '.':
			case ' ':
				step();
				break;

			case 'h': cursor_x--; break;
			case 'l': cursor_x++; break;
			case 'k': cursor_y--; break;
			case 'j': cursor_y++; break;
			case 'y': cursor_y--; cursor_x--; break;
			case 'u': cursor_y--; cursor_x++; break;
			case 'b': cursor_y++; cursor_x--; break;
			case 'n': cursor_y++; cursor_x++; break;

			case '1': case '2': case '3': case '4':
				  map_write(grid, cursor_x, cursor_y, key-'0');
				  break;

			case KEY_UP: y_offset -= 10; break;
			case KEY_DOWN: y_offset += 10; break;
			case KEY_LEFT: x_offset -= 10; break;
			case KEY_RIGHT: x_offset += 10; break;

			case 'C': x_offset=0; y_offset=0; break;
		}

		if(cursor_x < 0) cursor_x=0;
		if(cursor_y < 0) cursor_y=0;
		if(cursor_x == grid->width) cursor_x--;
		if(cursor_y == grid->height) cursor_y--;
	}
}

void redraw_screen()
{
	int x, y;
	int width, height;
	uint8_t state;
	void *dumb=NULL;

	width = grid->width;
	height = grid->height;

	size_x = COLS;
	size_y = LINES;

	erase();

	for(y=0; y<size_y; y++)
	for(x=0; x<size_x; x++) {
		if(x+x_offset < 0 || x+x_offset >= grid->width
			|| y+y_offset < 0 || y+y_offset >= grid->height)
			continue;

		state = map_access(grid, x+x_offset, y+y_offset);
		mvprintw(y,x, " ");
		mvchgat(y,x, 1, COLOR_PAIR(state) | A_BOLD, state, dumb);
	}

	mvprintw(cursor_y-y_offset,cursor_x-x_offset, "_");

	return;
}

void step()
{
	int x, y;
	map_2d tmp;

	for(x=0; x<grid->width; x++)
	for(y=0; y<grid->height; y++) {

		uint8_t state = map_access(grid, x, y);
		uint8_t new_state = EMPTY;

		switch(state) {
			case EMPTY:
				new_state = EMPTY;
				break;
			case ELECTRON_HEAD:
				new_state = ELECTRON_TAIL;
				break;
			case ELECTRON_TAIL:
				new_state = CONDUCTOR;
				break;
			case CONDUCTOR:
				if(check_conductor(x,y))
					new_state = ELECTRON_HEAD;
				else
					new_state = CONDUCTOR;
		}

		map_write(render, x, y, new_state);
	}

	/* SWITCH THE RENDER GRID INTO THE REAL GRID */
	tmp = grid;
	grid = render;
	render = tmp;

	return;
}

int check_conductor(int x, int y)
{
	int a,b;
	int count = 0;
	uint8_t tmp;

	for(a=y-1; a<=y+1; a++)
	for(b=x-1; b<=x+1; b++) {
		if(a == y && b == x)
			continue;
		if(a < 0 || b < 0 || a == grid->height || b == grid->width)
			continue;

		tmp = map_access(grid, b, a);
		if(tmp == ELECTRON_HEAD)
			count++;
	}

	if(count == 1 || count == 2)
		return 1;
	return 0;
}
