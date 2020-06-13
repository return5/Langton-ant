/*---------------------------------------- Description -------------------------------------*/
/*   short program which implements Langton's ant
 *          - if ant is on white tile, turn clockwise, turn current tile black, then move forward one space
 *          - if ant is on black tile, turn counterclockwise, turn current tile white, then move forward one space
 *  license: GPL 3.0        Author: github/return5
 *
 *  Copyright (C) <2020>  <return5>
 *
 *  This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
/*---------------------------------------- includes -------------------------------------*/
#include "pieces.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

/*---------------------------------------- globals ---------------------------------------*/
#define WIDTH 80
#define HEIGHT 40

static PIECE *WORLD[HEIGHT][WIDTH]; 
static struct PIECE *ANT;
static const size_t SIZE_PIECE = sizeof(PIECE);
static DIRECTION ANT_DIR;


/*------------------------------------------ prototypes -----------------------------------*/
void flipAnt(void);
void moveCounterClockWise(void);
void simulationLoop(void);
void makeAnt(void);
void initWorld(void);
PIECE *makePiece(const int i, const int j);
void initWindow(void);
void initColors(void);
void printWorld(void);
void printAnt(void);
void flipAnt(void);
void flipPiece(void);
void moveAnt(void);
void changeAntBKGColor(void);
void moveClockWise(void);

/*----------------------------------------- code ------------------------------------------*/

//cahnge ant's backgound color to make the tile it is currently on
void changeAntBKGColor(void) {
    if(WORLD[ANT->y][ANT->x]->color == WHITE) {
        ANT->color = RED_W;
    }
    else {
        ANT->color = RED_B;
    }
}

//turn the ant clockwise
void moveClockWise(void) {
    ANT_DIR += 1;
    ANT_DIR %= 4;
}

//move the ant coounterclockwise
void moveCounterClockWise(void) {
    ANT_DIR -= 1;
    ANT_DIR %= 4;
}

//move the ant based on it's current direction
void moveAnt(void) {
    switch(ANT_DIR) {
        case UP:ANT->y--;
            break;
        case DOWN:ANT->y++;
            break;
        case LEFT:ANT->x--;
            break;
        case RIGHT:ANT->x++;
            break;
        default: //do nothing
            break;
    }
}


//change the color of the tile the ant is standing on.
void flipPiece(void) {
    if(WORLD[ANT->y][ANT->x]->color == WHITE) {
        WORLD[ANT->y][ANT->x]->color = BLACK;
    }
    else {
        WORLD[ANT->y][ANT->x]->color = WHITE;
    }
}

//turn the ant based on color of tile it is standing on
void flipAnt(void) {
    if(WORLD[ANT->y][ANT->x]->color == WHITE) {
        moveClockWise();
    }
    else {
        moveCounterClockWise();
    }
}

//print the ant to screen
void printAnt(void) {
    attron(COLOR_PAIR(ANT->color));
    mvaddch(ANT->y,ANT->x,ANT->icon);
    attroff(COLOR_PAIR(ANT->color));
}

//print the entire world to screen
void printWorld(void) {
    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {
            attron(COLOR_PAIR(WORLD[j][i]->color));
            mvaddch(j,i,WORLD[j][i]->icon);
            attroff(COLOR_PAIR(WORLD[j][i]->color));
        }
    }
}

//set up the colors to use
void initColors(void) {
    start_color();
	init_pair(WHITE,COLOR_WHITE,COLOR_WHITE);
	init_pair(BLACK,COLOR_BLACK,COLOR_BLACK);
	init_pair(RED_W,COLOR_RED,COLOR_WHITE);
	init_pair(RED_B,COLOR_RED,COLOR_BLACK);
}

//set up the ncurses window
void initWindow(void) {
    initscr();					//start ncurses
	noecho();	                //dont display key strokes
	cbreak();	    		   //disable line buffering
	curs_set(0);              //hide cursor
	refresh();				//refresh the window
}

//make a piece object
PIECE *makePiece(const int i, const int j) {
    PIECE *piece = malloc(SIZE_PIECE);
    piece->x     = i;
    piece->y     = j;
    piece->color = WHITE;
    piece->icon  = ' ';
    return piece;
}

//create the world
void initWorld(void) {
    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {
            WORLD[j][i] = makePiece(i,j);
        }
    }
}

//make the ant
void makeAnt(void) {
    ANT = malloc(SIZE_PIECE);
    ANT->color = RED_W;
    ANT->icon  = '#';
    ANT->x     = WIDTH/2;
    ANT->y     = HEIGHT/2;
    ANT_DIR    = rand() % 4;   //init driection to a random direction
}


//run simulation 11K times.
void simulationLoop(void) {
    for(int i = 0; i < 11000; i++) {
        printWorld();
        printAnt();
        mvprintw(0,0,"iteration: %d",i);
        refresh();
        flipAnt();
        flipPiece();
        moveAnt();
        changeAntBKGColor();
        nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
    }
}

int main(void) {
    initWindow();
    initWorld();
    initColors();
    srand(time(0));
    makeAnt();
    simulationLoop();
    mvprintw(0,0,"simulation complete. press any key to exit\n");
    getch();
    endwin();
    return EXIT_SUCCESS;
}
