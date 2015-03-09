/**
* Bubble Pop Game.
* Author: Arun Surath
* Note: Canabalized from original BPOP
* little left except load from file logic which has been modified
* Class: CS141
* Professor: John lilis
*/


//Includes from standard libraries
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

//Interface for forward facing functions
#include "inc/bboard.h"

int main() {

	//Declare variables	
	Game game;
	union Data * tempGrid;

	//Seed random generator
	srand((unsigned int)time(NULL));

	init_game(&game);
	tempGrid = create_union();

	//Ncurses screen setup
	ncurses_setup();
	
	//Ncuses animated intro
	bpop_intro();
	
	//Load files from working directory
	get_files(&game);


	do{

		//Loading menu loop
		while(game.loading){
			
			//Display loading menu
			display_load(&game);	
	
			//Get a loading move from the user
			get_load_move(&game);	
			
			//If valid board then load it
			if(!game.loading && game.playing){
				load_board(&game);
				convert_board(&game);
			}
			
		}
		//In game loop
		while(game.playing){		
			//Display the board
			display_board(&game);
		
			//Get a move from the player
			get_game_move(&game);
			
			//if the move is a pop move then execute this logic
			if(game.move.thisTurn){

				store_grid(&game, tempGrid);
		
				//Do the move
				do_move(&game);
				//Increment the score
				game.score += game.move.nPopped;
				//Save the score increment for undo's later
				if(game.move.nPopped){
					//Save the grid for undo's later
					push_grid(&game, tempGrid);
					//Save the score for undo's later
					push_score(&game);	
				}
			//if the move is an undo then execute this logic
			}if(game.move.isUndo && game.score != 0){
				//Get previous grid from stack and set to grid
				get_grid(&game);
				//Get previous score from stack and set it score
				get_score(&game);
				//set grid back to the old grid			
				game.move.isUndo = 0;
			}

			//Check if we won
			has_won(&game);

		}

		if(game.open){
			cleanup(&game);
			init_game(&game);
			get_files(&game);
		}				

	}while(game.open);
              
	cleanup(&game);
	free(tempGrid);

	//close ncurses screen
	endwin();

	return 0;
}
	
	
	    
	
	
	
