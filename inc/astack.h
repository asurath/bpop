/**
 * Title: Stack
 * Description: A data structure containing a single pointer to the beginning of the linked list
 * Properties:
 * 	node * Start - a pointer to a node
 */
typedef struct stack Stack;

/**
 * Title: Node
 * Description: A linked list node who data segment is a union defined in astack.h
 * Properties:
 * 	union Data data - A union containing the node data
 * 	node * next - a pointer to the next node in the list
 */
typedef struct node Node;

/**
 * Title: Data
 * Description: A forward facing union to serve as the pushable data
 * 		structure
 * Properties:
 * 	int i
 * 	char c
 * 	float f
 * 	void * obj
 * 	char ** g
 */
union Data {
	int i;
	char c;
	float f;
	void * obj;
	char ** g;
};

/**
 * Title: create_stack()
 * Description: creates a stack with an initial empty node
 * Arguments:
 * 	none
 * Returns
 * 	Stack * stack - pointer to the stack created
 */
extern Stack * create_stack();

/**
 * Title: stk_free(Stack * stack)
 * Description: Pops all nodes off of a stack passed and frees the last empty node as well as the stack itself
 * Arguments:
 * 	Stack * stack - a pointer to the stack to be free'd
 * Returns:
 * 	void
 */
extern void stk_free(Stack * stack);

/**
 * Title: stk_push(Stack * stack, union Data value)
 * Description: pushes the union passed to the stack passed
 * Arguments:
 * 	Stack * stack - pointer to the stack to push data to
 * 	union Data value - the data to push in the form of a Data union
 * Returns:
 * 	int - boolean for if stack push was successful
 */
extern int stk_push(Stack * stack , union Data value);

/**
 * Title: stk_pop(Stack * stack)
 * Description: pops a value off of the stack and returns the data after 
 * 		freeing the node
 * Arguments:
 * 	Stack * stack - point to the stack to pop off of
 * Returns:
 * 	union Data data - the data from the node popped off the stack
 */
extern union Data stk_pop(Stack * stack);

