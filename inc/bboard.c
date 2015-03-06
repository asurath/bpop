//Interface include
#include "bboard.h"

//Includes from standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>

//Need to obtain files from working directory
#include <dirent.h>

//Custom stack include
#include "astack.h"

// Constant definitions for colors/balloons
#define RED 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4
#define CURSOR 5
#define MY_NONE '.'
#define RED_BALLOON '^'
#define BLUE_BALLOON '+'
#define GREEN_BALLOON 'o'
#define YELLOW_BALLOON '=' 
#define NCOLORS 5

static char Balloons[] = {MY_NONE, RED_BALLOON, BLUE_BALLOON, GREEN_BALLOON, YELLOW_BALLOON};
 
 /**
 * Eats whitespace from input stream fptr until either
 *   a non-whitespace character is encountered or we
 *   reach the end of file.
 *   
 * returns:  the non-whitespace char found or EOF if 
 *    none exists.
 *
 * Note return type is int, rather than char
 */
 int next_symbol(FILE *fptr) {
 int ch;

   while(( (ch = fgetc(fptr)) != EOF) && isspace(ch));

     return ch;
     }


int is_balloon(int ch) {
int i;
  for(i=0; i<NCOLORS; i++){
        if(ch == Balloons[i])
          return 1;
  }
  return 0;
}

/**
 *Title: create_2darr(int nrows, int ncols)
*Description: creates a 2d array of characters with dimensions nrows and ncols on the heap and returns the pointer to that address
*returns:
*	char ** arr
*/
char ** create_2darr( int nrows, int ncols ){
	
	char ** arr;
	arr = malloc(sizeof(char *) * nrows);
	for(int i = 0; i < nrows; i++)
		arr[i] = malloc(sizeof(char) * ncols);

	return arr;
}

/**
 * Title: init_game(Game * game)
 * Description: initializes all properties of the main Game data structure
 * returns:
 * 	void
 */
void init_game(Game * game){

	//Initialize variables
	game->gridStack = create_stack();
	game->scoreStack = create_stack();
	game->grid = NULL;
	game->move.thisTurn = 0;
	game->move.isUndo = 0;
	game->move.location.x = 0;
	game->move.location.y = 0;
	game->playing = 0;
	game->open = 1;
	game->selection = 0;
	game->nfiles = 0;
	game->score = 0;
	game->loading = 1;	
	
	return;
}



/**
 * Title: copy_grid(char ** grid, char ** grid2, int nrows, int ncols)
 * Description copys one board grid to another board grid variable
 * Arugments:
 * 	char ** grid - board to be copied
 * 	char ** grid2 - board to be copied to
 * 	int nrows - y board dimension
 * 	int ncols - x board dimension
 * Returns:
 * 	void
 */
void copy_grid(char ** grid, char ** grid2, int nrows, int ncols){

	for(int i = 0; i < nrows; i++)
		for(int n = 0; n < ncols; n++)
			grid2[i][n] = grid[i][n];


}



/**
 * Title: destroy_2darr(char ** arr, int nrows, int ncols)
 * Description: frees the 2d character array created by create_2darr
 * returns:
 * 	void
 */
void destroy_2darr(char ** arr, int nrows, int ncols){
	
	for(int i = 0; i < nrows; i++)
		free(arr[i]);
	free(arr);

	return;

}

void * create_union(){
	return malloc(sizeof(union Data));
}

void store_grid(Game * game, union Data * unionToPush){

	char ** gridToPush;

	gridToPush = create_2darr(game->nrows, game->ncols);
	copy_grid(game->grid, gridToPush, game->nrows, game->ncols);
	unionToPush->g = gridToPush;

	return;
}

void push_grid(Game * game, union Data * unionToPush){

	stk_push(game->gridStack, *unionToPush);
	return;

}

void push_score(Game * game){

	union Data unionToPush;

	unionToPush.i = game->move.nPopped;
	stk_push(game->scoreStack, unionToPush);
	
	return;
}


void get_grid(Game * game){
	
	union Data unionToGet;

	//Retrieve the last grid
	unionToGet = stk_pop(game->gridStack);
	destroy_2darr(game->grid, game->nrows, game->ncols);
	game->grid = unionToGet.obj;

	return;
}

void get_score(Game * game){

	union Data unionToGet;

	//Retrieve the last score change
	unionToGet = stk_pop(game->scoreStack);
	//Undo the score change
	game->score -= unionToGet.i;

	return;
}

/**
* read_grid
*
* populates the given 2D matrix with "grid" stored in 
*   the given file.
* 
* number of rows/cols stored in *nrows_out, *ncols_out
*   respectively.
*
* returns 0/1 for success/failure
* Author: John Lillis
*
*/
int read_grid(char *** grid, char *file, 
		  int *nrows_out, int *ncols_out) {
FILE *fptr;
// by the way:  stdin is a global var of type FILE *
int nrows, ncols;
int i, j;
char ch;

  // attempt to open file with 'read' permission
  fptr = fopen(file, "r");
  if(fptr == NULL) {
	return 0;
  }
  // read the dimensions of the grid
  if(fscanf(fptr, "%i %i\n", &nrows, &ncols) != 2 ||
	  nrows <= 0 || ncols <= 0){
	fclose(fptr);
	return 0;
  }else{
	if(*grid != NULL)
		free(*grid);
	*grid = create_2darr(nrows, ncols);
  }


  // now populate the grid
  for(i=0; i<nrows; i++) {
	for(j=0; j<ncols; j++) {
	  if( (ch = next_symbol(fptr)) == EOF) {
		fprintf(stderr, "ERR:  file too short\n");
		fclose(fptr);
		return 0;
	  }
	  else if(!is_balloon(ch)) {
		fprintf(stderr, "ERR:  bad symbol in input (%c)\n", ch);
		fclose(fptr);
		return 0;
	  }
	  
	  (*grid)[i][j] = ch;
	}
  }
  if(next_symbol(fptr) != EOF) {
	fprintf(stderr, "ERR:  file too long\n");
	fclose(fptr);
	return 0;
  }
  // if we get here we succeeded!
  fclose(fptr);
  *nrows_out = nrows;
  *ncols_out = ncols;

  return 1;
}

/**
 * Title:ncurses_setup()
 * Description: standard ncurses screen setup and color pair initialization
 * returns:
 * 	void
 */
void ncurses_setup(){

	initscr();	
	nodelay(stdscr, TRUE);
	if(has_colors())
		start_color();
	curs_set(0);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(CURSOR, COLOR_BLUE, COLOR_WHITE);
	clear();
	
	return;


}

/**
 * Title:bpop_intro();
 * Description: a ncurses powered intro graphic 
 * Returns:
 * 	void
 */
void bpop_intro(){
	
	return;

}

/**
 * Title: get_files(int * nfiles)
 * Description: takes a point to an integer to be set as the number of files in the current working directory and returns
 * an array of string with the file names
 * Arguments:
 * 	int * nfiles - pointer to int to be set for array length to be returned
 * Returns:
 * 	char ** files - array of strings containing file names
 */
void get_files(Game * game){
	DIR *d;
	struct dirent *dir;
	int  i = 0;

	game->nfiles = 0;

	d = opendir(".");
	if (d){
    		while ((dir = readdir(d)) != NULL)
     			if(dir->d_name[0] != '.')
				game->nfiles++;
    		closedir(d);
		game->files = malloc(game->nfiles * sizeof(char *)); 
		d = opendir(".");	
    		while ((dir = readdir(d)) != NULL)
     			if(dir->d_name[0] != '.'){
				game->files[i] = malloc(strlen(dir->d_name));
				strcpy(game->files[i],dir->d_name);
				i++;	
			}
		closedir(d);
  	}

  	return;

}

/**
 * Title: display_load(char ** files, int nfiles, int selection)
 * Description: ncurses display logic for the loading portion of the game
 * Arguments:
 * 	char ** files - array of strings containing filenames
 * 	int nfiles - number of files in the array
 * 	int selection - which array choice is currently selection
 * Returns:
 * 	void
 */
void display_load(Game * game){
	
		clear();
		attron(COLOR_PAIR(2));
		move(2, 5);
		printw("Please select a board to load using w as up,\n      s as down, e as enter and q as quit:");	
		
		move(4, 5);
		if(0 == game->selection)
			attron(COLOR_PAIR(1));
		printw("1. Random\n");
		for(int i = 0; i < game->nfiles; i++){
			move(5 + i, 5);
			if(i == game->selection - 1)
				attron(COLOR_PAIR(1));
			else
				attron(COLOR_PAIR(2));
			printw("%i. %s\n", i + 2, game->files[i]);
		}

		refresh();		
			
		return;
}

/**
 * Title:get_load_move(int * selection, int * loading, int * playing, char ** files, int selectMax)
 * Description:takes several pointers of game values, get an input from the playing and changes the appropriate
 * values 
 * Arugments:
 * 	int * selection - pointer to the current selction
 * 	int * loading - pointer to the loading variable
 * 	int * playing - pointer to the playing variable
 * 	char ** files - array of string containing file names in the working directory
 * 	int selectMax - the maximum select value (length of ** files)
 *Returns:
* 	void
*/
void get_load_move(Game * game){

	char inp;
	
	while((inp = getch()) == ERR);
	if(inp == 's' && game->selection < game->nfiles){
		game->selection += 1;	
		return;
	}
	if(inp == 'w' && game->selection  >= 1){
		game->selection -= 1;
		return;
	}
	if(inp == 'e'){
		game->loading = 0;
		game->playing = 1;
		return;
	}
	if(inp == 'q'){
		game->loading = 0;
		game->playing = 0;
		game->open = 0;
		return ;
	}

}



/**
 * Title:get_game_move(int * location, int * playing, Move * move, int maxRow, int maxCol)
 * Description:takes several pointers of game values as well as boar dimenensions and gets 
 * an input from the playing and changes the appropriate values for the game
 * Arugments:
 * 	int * location - pointer to the current cursor location
 * 	int * playing - pointer to the playing variable
 * 	Move * move - pointer to the current move
 * 	int maxRow - y board dimension
 * 	int maxCol - x board dimension 
 *Returns:
* 	int
*/
void get_game_move(Game * game){

	char inp;
	while((inp = getch()) == ERR);	
	if(inp == 'a' && game->move.location.x - 1 >= 0)
		game->move.location.x -= 1;
	if(inp == 'd' && game->move.location.x + 1 < game->ncols)
		game->move.location.x += 1;
	
	if(inp == 'w' && game->move.location.y - 1 >= 0)
		game->move.location.y -= 1;
	if(inp == 's' && game->move.location.y + 1 < game->nrows)
		game->move.location.y += 1;

	if(inp == 'e'){
		game->move.thisTurn = 1;
		game->move.location = game->move.location;
	}
	if(inp == 'z')
		game->move.isUndo = 1;
	if(inp == 'q'){
		game->loading = 1;
		game->playing = 0;
	}
	return;
	

}

/**
 * Title:get_rand_balloon()
 * Description: returns a random baloon
 * Returns:
 * 	char balloon - random balloon
 */
char get_rand_balloon(){
	
	int num;
	num = rand() % 4;
	if(!num)
		return '^';
	if(num == 1)
		return '+';
	if(num == 2)
		return '=';
	if(num == 3)
		return 'o';

	return '=';
}

/**
 * Title: build_random_board(char *** grid, int * nrows, int * ncols)
 * Description: Builds and sets all the necessary variables for a random board of dimensions set inside the function
 * Arugments:
 * 	char *** grid - pointer to grid array to set as malloc point
 * 	int * nrows - pointer to the number of rows in the main function
 * 	int * ncols - pointer to the number of collumns in the main function
 * Returns:
 * 	void
 */
void build_random_board(char *** grid, int * nrows, int * ncols){

	*grid = create_2darr(10, 30);

	*ncols = 30;	
	*nrows = 10;	
	
	for(int i = 0; i < *ncols; i++)
		for(int n = 0; n < *nrows; n++)
			(*grid)[n][i] = get_rand_balloon();
	

}

/**
 * Title: load_board(char *** grid, char ** files, int selection, int * loading, int * playing, int * nrows, int * ncols)
 * Descriptions: loads a board and dynamically assigns the memory needed into grid
 * Arguments:
 * 	char *** grid - pointer to the 2d grid array to be set to the malloc return
 * 	char ** files - 2d array containing file names 
 * 	int selection - filename index to be loaded
 * 	int * loading - pointer to the loading variable
 * 	int * playing - pointer to the playing variable
 * 	int * nrows - pointer to the nrows variable
 * 	int * ncols - pointer to the ncols variable
 * Returns:
 * 	void
 */
void load_board(Game * game){
	char inp;
	if(game->selection){
		if(!read_grid(&game->grid, game->files[game->selection - 1], &game->nrows, &game->ncols)){
			clear();
			move(5, 2);
			printw("Invalid format, please choose a valid file\n     press any key to continue");
			game->loading = 1;
			game->playing = 0;
			refresh();
			while((inp = getch()) == ERR);
		}		
	}else
		build_random_board(&game->grid, &game->nrows, &game->ncols);
	
}

/**
 * Title: balloon_to_color(char baloon)
 * Description: takes a balloon and returns a color constant's value
 * Arguments:
 * 	char balloon - to be converted
 * Returns:
 * 	int color
 */
int balloon_to_color(char balloon){
	switch(balloon){
		case '^':
			return 1;
		case '=':
			return 2;
		case '+':
			return 3;
		case 'o':
			return 4;
		default:
			return 0;
	}
	return 0;

}

/**
 * Title:display_board(char ** grid, int nrows, int ncols, Location location, int score)
 * Description: displays a board from the information in grid, ncols, nrows location and score
 * Returns:
 * 	Void
 */
void display_board(Game * game){
	
	clear();	
	char inp;
	for(int i = 0; i < game->ncols; i++){
		for(int n = 0; n < game->nrows; n++){
			if(game->grid[n][i] == '.') game->grid[n][i] = ' ';
			if(game->move.location.y == n && game->move.location.x == i)		
				attron(COLOR_PAIR(5));
			else
				attron(COLOR_PAIR(balloon_to_color(game->grid[n][i])));
			
			move(2 + n, 5 + i);

			if(has_colors() && game->grid[n][i] != ' ')
				printw("O");
			else
				printw("%c", game->grid[n][i]);
			
			attron(COLOR_PAIR(2));
		}
	}	

	attron(COLOR_PAIR(2));
	move(4 + game->nrows, 5);
	printw("Score: %i\n", game->score);
	move(5 + game->nrows, 5);
	printw("controls: w -> up, s -> down,\n    a -> left, d -> right, q -> quit,\n     z -> undo, e -> pop balloon");

	refresh();

}

/**
 * Title: board_is_compact(char ** grid, int nrows, int ncols)
 * Description: takes a grid and its dimensions and returns true false depending on if it is compact
 * Arguments:
 * 	char ** grid - 2d grid array
 * 	int nrows - y dimension
 * 	int ncols - x dimension
 * Returns:
 * 	int - true/false (is compact)
 */
int board_is_compact(char ** grid, int nrows, int ncols){
	

	for(int i = 0; i < nrows - 1; i++)
		for(int n = 0; n < ncols; n++)
			if(grid[i][n] == ' ' || grid[i][n] == '.')
				if(grid[i + 1][n] != ' ' || grid[i + 1][n] == '.')
					return 0;	

	return 1;
}

/**
 * Title: compact_board(char ** grid, int nrows, int ncols)
 * Description: takes a board and if it can be compacted compacts it
 * Arguments:
 * 	char ** grid - board
 * 	int nrows - y dimension
 * 	nit ncols - x dimension
 * Returns:
 * 	void
 */

void compact_board(char ** grid, int nrows, int ncols){
	

	for(int i = 0; i < nrows - 1; i++)
		for(int n = 0; n < ncols; n++)
			if(grid[i][n] == ' ' || grid[i][n] == '.')
				if(grid[i + 1][n] != ' ' || grid[i + 1][n] == '.'){
					grid[i][n] = grid[i + 1][n];
					grid[i + 1][n] = ' ';
				}

		
}


/**
 * Title: pop_balloon(char ** grid, Location start, int indx, int nrows, int ncols)
 * Description: pops a balloon and checks if any adjacent balloons can be popped if so it recursivesly
 * calls itself. Includes logic for if the balloon is the first baloon
 * Arugments:
 * 	char ** grid - board
 * 	Location start - location to be popped
 * 	int indx - place in recursive chain (begins at 0)
 * 	int nrows - grid y dimensions
 * 	int ncols - grid x dimension
 * Returns:
 * 	int popped - number of balloons popped
 */
int pop_balloon(char ** grid, Location start, int indx,int nrows,int ncols){
		
	Location nextLocation;	
	char balloon = grid[start.y][start.x];
	int popped = 0;

	if(balloon == ' ')
		return 0;

	if(!indx){
		if(start.y + 1 < nrows && grid[start.y + 1][start.x] == balloon){
			grid[start.y][start.x] = ' ';
			nextLocation.x = start.x;
			nextLocation.y = start.y +1;
			popped += pop_balloon(grid, nextLocation, indx + 1, nrows, ncols);	
		}

		if(start.y - 1 >= 0 &&  grid[start.y - 1][start.x] == balloon){
			grid[start.y][start.x] = ' ';
			nextLocation.x = start.x;
			nextLocation.y = start.y - 1;
			popped += pop_balloon(grid, nextLocation, indx + 1, nrows, ncols);	
		}	

		if(start.x + 1 < ncols &&  grid[start.y][start.x + 1] == balloon){
			grid[start.y][start.x] = ' ';
			nextLocation.x = start.x + 1;
			nextLocation.y = start.y;
			popped += pop_balloon(grid, nextLocation, indx + 1, nrows, ncols);	
		}	

		if(start.x - 1 >= 0 &&  grid[start.y][start.x - 1] == balloon){
			grid[start.y][start.x] = ' ';
			nextLocation.x = start.x - 1;
			nextLocation.y = start.y;
			popped += pop_balloon(grid, nextLocation, indx + 1, nrows, ncols);	
		}		
	}else{
		grid[start.y][start.x] = ' ';
		popped++;
		if(start.y + 1 < nrows && grid[start.y + 1][start.x] == balloon){	
			nextLocation.x = start.x;
			nextLocation.y = start.y +1;
			popped += pop_balloon(grid, nextLocation, indx + 1, nrows, ncols);	
		}

		if(start.y - 1 >= 0 && grid[start.y - 1][start.x] == balloon){
			nextLocation.x = start.x;
			nextLocation.y = start.y - 1;
			popped += pop_balloon(grid, nextLocation, indx + 1, nrows, ncols);	
		}	

		if(start.x + 1 < ncols && grid[start.y][start.x + 1] == balloon){
			nextLocation.x = start.x + 1;
			nextLocation.y = start.y;
			popped += pop_balloon(grid, nextLocation, indx + 1, nrows, ncols);	
		}	

		if(start.x - 1 >= 0 && grid[start.y][start.x - 1] == balloon){
			nextLocation.x = start.x - 1;
			nextLocation.y = start.y;
			popped += pop_balloon(grid, nextLocation, indx + 1, nrows, ncols);	
		}		
	}

	return popped;
}

/**
 * Title: pop_balloons;
 * Description a wrapper for pop_balloon() to perform a pop move
 * Arguments:
 * 	Move * move - pointer to move variable
 * 	char ** grid - board
 * 	int nrows - y board dimension
 * 	int ncols - x board dimension
 * Returns:
 * 	void
 */
void pop_balloons(Move * move, char ** grid, int nrows, int ncols){

	Location start;
	start = move->location;
	move->nPopped = pop_balloon(grid, start, 0, nrows, ncols);
	if(move->nPopped)
		move->nPopped++;

}

/**
 * Title: do_move(Move * mpve. char ** grid, int nrows, int ncols, Location location, int score)
 * Description: performs a popping move, pops the balloons and compacts the board with animation
 * Arguments:
 * 	Move * move - pointer to move variable
 * 	char ** grid - board
 * 	int nrows - board y dimensions
 * 	int ncols - board x dimensions
 * 	Location location - loctions of pop move
 * 	int score - curent score
 * Returns:
 * 	void
 */
void do_move(Game * game){

	
	pop_balloons(&game->move, game->grid, game->nrows, game->ncols);
	
	while(!board_is_compact(game->grid, game->nrows,game-> ncols)){
		compact_board(game->grid, game->nrows, game->ncols);
		display_board(game);
		napms(200);
	}
	
	game->move.thisTurn = 0;
	game->move.isUndo = 0;

	return;

}


/**
 * Title: has_won(int playing, int score, int nrows, int ncols)
 * Description: checks if the player as won to display a variable and returns a boolean to stop the game
 * Arugments:
 * 	int playing - playing variable
 * 	int score - score variable
 * 	int nrows - y board dimension
 * 	int ncols - x board dimension
 * Returns:
 * 	int - playing variable
 */
void has_won(Game * game){

	char inp;

	if(!game->playing)
		return;

	if(game->score == game->nrows * game->ncols){
		clear();
		move(5 , 5);
		printw("Congratulations!, you won\n     Press any key to exit");
		refresh();
		while((inp = getch()) == ERR); 
		game->playing = 0;
	}

	return;
}

void cleanup(Game * game){

	union Data tempUnion;

	for(int i = 0; i < game->nfiles; i++)
		free(game->files[i]);

	stk_free(game->scoreStack);
	
	tempUnion = stk_pop(game->gridStack);
	while(tempUnion.i)stk_pop(game->gridStack);

	stk_free(game->gridStack);

	free(game->files);

	//Memory cleanup
	if(game->grid != NULL)
		destroy_2darr(game->grid, game->nrows, game->ncols);

}

