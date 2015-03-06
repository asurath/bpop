typedef union Data * data;

/////////////////////////////////////////////////
//////////// STRUCTURE DECLARATIONS /////////////
/////////////////////////////////////////////////

/**
 * Title: Location
 * Description: A simple data structure to hold
 * 		cartesian coordinates for moves and
 * 		cursors
 * Properties:
 * 	int x - x location
 * 	int y - y location
 */
typedef struct location {
	int x;
	int y;
} Location;

/**
 * Title: Move
 * Description: A simple data structure to hold all
 * 		associated information for a whole move
 * Properties:
 * 	Location location - location of a move
 * 	int nPopped - number of balloons popped by move
 * 	int isUnder - boolean for if the move is an undo
 * 	int thisTurn - boolean for if the move is a pop
 */
typedef struct move {
		Location location;
		int nPopped;
		int isUndo;
		int thisTurn;
}Move;

/**
 * Title: Game
 * Description: A simple but larger data structure containing
 *		all necessary information for the game
 * Properties:
 * 	Move move - the current move that has been entered
 * 	Location cursorLoc - the current location of the cursor
 * 	char ** grid - a nxm game grid containing the level
 *	char ** files - an array of string containing file
 *		names in the working directory
 *	void * gridStack - a pointer to a stack containing 
 *		grids from previous moves, used in undo's
 *	void * scoreStack - a pointer to a stack containing
 *		scored from previous moves, used in undo's
 *	int nfiles - the number of files in the files variable
 *	int ncols - the number of collumns in the grid variable
 *	int nrows - the number of rows in the grid variable
 *	int playing - a boolean for in the game is in playing mode
 *	int open - a boolean for if the game is open
 *	int selection - the current file selection in the loading
 *		menu
 *	int loading - a boolean for if the game is in the loading
 *		stage
 *	int score - the current score of the game being played
 */
typedef struct game {
		Move move;
		Location cursorLoc;	
		char ** grid;
		char ** files;
		void * gridStack;
		void * scoreStack;
		int nfiles;
		int ncols;
		int nrows;
		int playing;
		int open;
		int selection;
		int loading;
		int score;
}Game;


/////////////////////////////////////////////////
//////////// FUNCTION DECLARATIONS //////////////
/////////////////////////////////////////////////

/**
 *Title: ncurses_setup();
 *Description: Boilerplate for ncurses screen initialization and
 * 	color pair declarations
 *Arguments: None
 *Returns: 
 *	void
 *Called in: bpop.c
 *		functions: main()
 */
void ncurses_setup();

/**
 * Title:bpop_intro();
 * Description: a ncurses powered intro graphic 
 * Arguments: None
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main()
 */
void bpop_intro();

/**
 * Title: init_game(Game * game)
 * Description: initlizes a Game data structures properties to those needed
 * 	for the start of the program
 * Arguments:
 * 	Game * game - a pointer to a game data structure whose properties
 * 		will be initialized
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main()
 */
void init_game(Game * game);

/**
 * Title: get_files(Game * game)
 * Description: Gets all files in the working directory using the library
 * 		dirent.h and builds a string array using dynamic memory al *		  dynamic memory allocation and assigns the pointer to the
 * 		files property of the Game data structure passed by
 * 		pointer
 * Arguments:
 *	Game * game - a pointer to a game data structure containing the
 *		files property to have set to the created string array
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main()
 */ 
void get_files(Game * game);

/**
 * Title: display_load(Game * game)
 * Description: Uses ncurses to display the loading screen based on
 * 		information contained in the Game data structure passed by *		  pointer in the game argument
 * Arguments:
 * 	Game * game - a pointer to a game data structure containing the
 * 		necessary data to display the load screen
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - loading loop
 */
void display_load(Game * game);

/**
 * Title: get_load_move(Game * game)
 * Description: waits for and gets an input from the user during the game
 * 		loop, when it recieves a valid input it appropriately
 * 		changes the necessary properties in the game data
 * 		structure passed to it pointer in the game argument
 * Arguments:
 * 	Game * game - a pointer to a game data structure to change based
 * 		on a users input
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - loading loop
 */
void get_load_move(Game * game);

/**
 * Title: load_board(Game * game)
 * Description: Takes a pointer to the game variable as an argument and 
 * 		uses the files property and selection property to load
 * 		the data from the selected file and populate the
 * 		necessary properties in the Game data structure such as
 * 		nrows, ncols, and grid
 * Arguments:
 * 	Game * game - a pointer to a game data structure with the nrows
 * 		ncols and grid properties  to be filled as well as
 * 		containing the selection and files properties to be used
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - loading loop
 */
void load_board(Game * game);

/**
 * Title: display_board(Game * game)
 * Description: Takes a pointer to the game variable as an argument and 
 * 		uses the data inside it, particularly the grid, ncols,
 * 		nrows, move, and score properties to display the game
 * 		board after it has been loaded
 * Arguments:
 * 	Game * game - a pointer to the game data structure to be
 * 		displayed
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop
 */
void display_board(Game * game);

/**
 * Title: get_game_move(Game * game)
 * Description: wait for the user to enter a valid input and when
 * 		a valid input is recieved it populates the move property
 * 		in the Game data structure passed as a pointer in game
 * Arguments:
 * 	Game * game - a pointer to the game data structure containing the
 * 		move property to be modified
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop
 */
void get_game_move(Game * game);

/**
 * Title: do_move(Game * game)
 * Description: takes the the Game data structure as an argument passed
 *		by the game pointer and sync's the grid with the
 *		corresponding move in the move property before clearing
 *		the move property in preperation for the next move
 * Arguments:
 * 	Game * game - a pointer to the game data structure containing
 * 	the lastest move in the move property and the previous grid and
 * 	score in the grid and score properties
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop
 */
void do_move(Game * game);

/**
 * Title: has_won(Game * game)
 * Description: Takes the current Game data structure after a move has
 * 		been performed and checks if the player has won, setting
 * 		appropriate properties such as playing if the user won
 * Arguments:
 * 	Game * game - a pointer to the game variable to check and change
 * 		if the user has won
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop
 */
void has_won(Game * game);

/**
 * Title: create_union()
 * Description: returns a pointer to a memory block the size
 * of a union Data as defined in astack.c
 * Arguments: 
 * 	none
 * Returns:
 * 	void * (A pointer of type union Data)
 * Called in: bpop.c
 * 		function: main()
 */
void * create_union();

/**
 * Title: store_grid(Game * game, union Data * unionToPush)
 * Description: Takes the grid from the Game data structure
 * 		property grid that was passed by pointer and stores
 * 		it in the union which was passed by pointer. Necessary
 * 		as we must save the grid prior to completeing a move yet
 * 		do not know if we wish to save the grid yet
 * Arguments:
 * 	Game * game - a pointer to the game variable to obtain the grid from
 * 	union Data * unionToPush - a pointer to the union to temporarily store
 * 		the grid in until we know if we want to push it
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop
 */
void store_grid(Game * game, union Data * unionToPush);

/**
 * Title: push_grid(Game * game, union Data * unionToPush);
 * Description: Takes the grid stack from the Game data structure passed
 * 		as a pointer and uses astack functions to push the old
 * 		game grid pointer stored in unionToPush onto the stack
 * Arguments:
 * 	Game * game - Game data structure containing the stack to be pushed
 * 			to in the gridStack property
 * 	union Data * unionToPush - a union containing the grid to push to the
 * 		stack
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop - if valid move block
 */
void push_grid(Game * game, union Data * unionToPush);

/**
 * Title: push_score(Game * game);
 * Description: Takes the score property from the Game data structure passed
 * 		as a pointer and uses astack functions to push the old score
 * 		as a union onto the stack for undo's in the future
 * Arguments:
 * 	Game * game - Game data structure containing the stack to be be pushed 
 * 		to in the scoreStack property as well as the score to be pushed
 * 		in the score property
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop - if valid move block
 */
void push_score(Game * game);

/**
 * Title: get_grid(Game * game);
 * Description: Takes the stack in gridStack and uses astack functions to
 * 		pop the last grid off the stack and assign it to the grid
 * 		property after freeing the current grid
 * Arguments:
 * 	Game * game - Game data structure containing both the stack to
 * 		pop from as well as the grid property to overwrite and
 * 		previous grid to free
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop - if undo move
 */
void get_grid(Game * game);

/**
 * Title: get_score(Game * game);
 * Description: Takes the stack in scoreStack and uses astack functions to
 * 		pop the last score off the stack and assign it to the score
 * 		property
 * Arguments:
 * 	Game * game - Game data structure containing both the stack to
 * 		pop from as well as the score property to overwrite
 * Returns:
 * 	void
 * Called in: bpop.c
 * 		function: main() - playing loop - if undo move
 */
void get_score(Game * game);

/**
 * Title: cleanup(Game * game);
 * Description: takes a pointer to the game variable and frees all 
 * 		dynamically allocated memory associated with it in
 * 		preparation to exit the current game
 * Arguments:
 * 	Game * game - Game data structure containing all dynamically
 * 		allocated pointers
 * Called in: bpop.c
 * 		function: main()
 */
void cleanup(Game * game);
