/*---------------------------------------- Description -------------------------------------*/
/*   simple program which implements Langton's ant
 *          - if ant is on white tile, turn clockwise, turn current tile black, then move forward one space
 *          - if ant is on black tile, turn counterclockwise, turn current tile white, then move forward one space
 *  
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
#define WIDTH 50
#define HEIGHT 25

static PIECE *WORLD[HEIGHT][WIDTH]; 
static struct PIECE *ANT;
static const size_t SIZE_PIECE = sizeof(PIECE);

/*------------------------------------------ prototypes -----------------------------------*/

void simulationLoop(void);
PIECE *makeAnt(void);
void initWorld(void);
PIECE *makePiece(const int i, const int j);
void initWindow(void);
void initColors(void);
void printWorld(void);
void printAnt(PIECE *const ant);
void flipAnt(PIECE *const ant);
void flipPiece(PIECE *const ant);
void moveAnt(PIECE *const ant);
void moveCounterClockWise(PIECE *const ant);
void moveClockWise(PIECE *const ant);
void changeAntBKGColor(PIECE *const ant);
int getNewY(const int y);
int getNewX(const int x);

/*----------------------------------------- code ------------------------------------------*/

//cahnge ant's backgound color to make the tile it is currently on
void changeAntBKGColor(PIECE *const ant) {
    if(WORLD[ant->y][ant->x]->color == WHITE) {
        ant->color = RED_W;
    }
    else {
        ant->color = RED_B;
    }
}

//turn the ant clockwise
void moveClockWise(PIECE *const ant) {
    ant->dir += 1;
    ant->dir %= 4;
}

//move the ant coounterclockwise
void moveCounterClockWise(PIECE *const ant) {
    ant->dir -= 1;
    ant->dir %= 4;
}


int getNewX(const int x) {
    if(x < 0) {
        return WIDTH - 1;
    }
    else if(x >= WIDTH) {
        return 0;
    }
    else {
        return x;
    }
}

int getNewY(const int y) {
    if(y < 0) {
        return HEIGHT - 1;
    }
    else if(y >= HEIGHT) {
        return 0;
    }
    else {
        return y;
    }
}

//move the ant based on its current direction
void moveAnt(PIECE *const ant) {
    switch(ant->dir) {
        case UP: ant->y = getNewY(ant->y - 1);
            break;
        case DOWN: ant->y = getNewY(ant->y + 1);
            break;
        case LEFT: ant->x = getNewX(ant->x - 1);
            break;
        case RIGHT: ant->x = getNewX(ant->x + 1);
            break;
        default: //do nothing
            break;
    }
}

//change the color of the tile the ant is standing on.
void flipPiece(PIECE *const ant) {
    if(WORLD[ant->y][ant->x]->color == WHITE) {
        WORLD[ant->y][ant->x]->color = BLACK;
    }
    else {
        WORLD[ant->y][ant->x]->color = WHITE;
    }
}

//turn the ant based on color of tile it is standing on
void flipAnt(PIECE *const ant) {
    if(WORLD[ant->y][ant->x]->color == WHITE) {
        moveClockWise(ant);
    }
    else {
        moveCounterClockWise(ant);
    }
}

//print the ant to screen
void printAnt(PIECE *const ant) {
    attron(COLOR_PAIR(ant->color));
    mvaddch(ant->y,ant->x,ant->icon);
    attroff(COLOR_PAIR(ant->color));
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
PIECE *makeAnt(void) {
    PIECE *ant = malloc(SIZE_PIECE);
    ant->color = RED_W;
    ant->icon  = '#';
    ant->x     = WIDTH / 2;
    ant->y     = HEIGHT / 2;
    ant->dir    = rand() % 4;   //init driection to a random direction
    return ant;
}

void antTurn(PIECE *const ant) {
    printAnt(ant);
    flipAnt(ant);
    flipPiece(ant);
    moveAnt(ant);
    changeAntBKGColor(ant);
}

//run simulation 11K times.
void simulationLoop(void) {
    for(int i = 0; i < 11000; i++) {
        printWorld();
        mvprintw(0,0,"iteration: %d",i);
        antTurn(ANT);
        refresh();
        nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
    }
}

int main(void) {
    initWindow();
    initWorld();
    initColors();
    srand(time(0));
    ANT = makeAnt();
    simulationLoop();
    mvprintw(0,0,"simulation complete. press any key to exit\n");
    getch();
    endwin();
    return EXIT_SUCCESS;
}
