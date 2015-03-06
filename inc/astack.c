/**Title: astack
 * Description: A(run's) stack library implemented as a linked list
 * professor: John Lillis
 */

#include "astack.h"
#include <stdlib.h>

/**
 * Title: Node
 * Description: A linked list node who data segment is a union defined in astack.h
 * Properties:
 * 	union Data data - A union containing the node data
 * 	node * next - a pointer to the next node in the list
 */
typedef struct node {
	union Data data;
	struct node * next;	
} Node; 

/**
 * Title: Stack
 * Description: A data structure containing a single pointer to the beginning of the linked list
 * Properties:
 * 	node * Start - a pointer to a node
 */
typedef struct stack {
	Node * start;
} Stack;

/**
 * Title:create_node()
 * Description: Allocates memory for a new node, sets its next value to null and returns the point
 * 		to the newly created node
 * Arguments:
 * 	none
 * Returns
 * 	Node * node - a pointer to the created node
 */
Node * create_node(){
	Node * node;
	
	node = malloc(sizeof(Node));
	
	node->next = NULL;
	
	return node;
}

/**
 * Title: create_stack()
 * Description: creates a stack with an initial empty node
 * Arguments:
 * 	none
 * Returns
 * 	Stack * stack - pointer to the stack created
 */
Stack * create_stack(){
	Stack * stack;
	Node * node;
	
	stack = malloc(sizeof(Stack));
	
	node = create_node();
	
	stack->start = node;	

	return stack;
}

/**
 * Title: stk_push(Stack * stack, union Data value)
 * Description: pushes the union passed to the stack passed
 * Arguments:
 * 	Stack * stack - pointer to the stack to push data to
 * 	union Data value - the data to push in the form of a Data union
 * Returns:
 * 	int - boolean for if stack push was successful
 */
int stk_push(Stack * stack, union Data value){

	Node * newNode;

	newNode = create_node();

	if(newNode == NULL) return 0;

	newNode->next = stack->start;
	newNode->data = value;
	stack->start = newNode;

	return 1;

}

/**
 * Title: stk_pop(Stack * stack)
 * Description: pops a value off of the stack and returns the data after 
 * 		freeing the node
 * Arguments:
 * 	Stack * stack - point to the stack to pop off of
 * Returns:
 * 	union Data data - the data from the node popped off the stack
 */
union Data stk_pop(Stack * stack){
	Node * oldStart;
	union Data data;
	
	if(stack->start->next == NULL){
		data.i = 0;
		return data;
	}
	
	oldStart = stack->start;
	data = stack->start->data;
	stack->start = stack->start->next;
	
	free(oldStart);
	
	return data;
}

/**
 * Title: stk_free(Stack * stack)
 * Description: Pops all nodes off of a stack passed and frees the last empty node as well as the stack itself
 * Arguments:
 * 	Stack * stack - a pointer to the stack to be free'd
 * Returns:
 * 	void
 */
void stk_free(Stack * stack){

	while(stack->start->next != NULL)
		stk_pop(stack);

	free(stack->start);
	free(stack);

}		
