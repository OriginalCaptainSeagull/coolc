#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include "error_t.h"
#include "trees.h"

tree_node* init_tree_node (token_type what, ...)
{
	va_list args;
	va_start (args, what);
	tree_node* node = malloc (sizeof (tree_node));
	node->what = what;
	node->above = NULL;
	switch (what)
	{
		case NUM:
			node->value.num = va_arg (args, double);
			break;
		case EMPTY:
			node->value.other = 0;
			break;
		default:
			node->value.other = va_arg (args, int);
			break;
	}
	va_end (args);
	return node;
}

void clear_tree (tree_node* root)
{
	if (root == NULL)
		return;
	tree_node* cleaner = root;
	while (root != NULL)
	{
		clear_tree (root->right);
		cleaner = root;
		root = root->left;
		free (cleaner);
	}
	return;
}

stack_of_trees* init_stack_of_trees (void)
{
	stack_of_trees* node = malloc (sizeof (stack_of_trees));
	node->next = NULL;
	node->prev = NULL;
	node->tree =init_tree_node (EMPTY);
	return node;
}

void kill_stack_tree (stack_of_trees* stack)
{
	if (stack == NULL)
		return;
	stack_of_trees* checkpoint = stack->prev;
	stack_of_trees* cleaner;
	while (stack != NULL)
	{
		cleaner = stack;
		stack = stack->next;
		free (cleaner);
	}
	stack = checkpoint;
	while (stack != NULL)
	{
		cleaner = stack;
		stack = stack->prev;
		free (cleaner);
	}
	return;
}

void kill_stack_and_trees (stack_of_trees* stack)
{
	if (stack == NULL)
		return;
	stack_of_trees* checkpoint = stack->prev;
	stack_of_trees* cleaner;
	while (stack != NULL)
	{
		cleaner = stack;
		stack = stack->next;
		free (cleaner);
	}
	stack = checkpoint;
	while (stack != NULL)
	{
		cleaner = stack;
		stack = stack->prev;
		clear_tree (cleaner->tree);
		free (cleaner);
	}
	return;
}

stack_of_trees* push_to_tree_stack (stack_of_trees* stack)
{
	if (stack == NULL)
		return NULL;
	while (stack->next != NULL)
		stack = stack->next;
	stack->next = init_stack_of_trees ();
	stack->next->prev = stack;
	return stack->next;
}

tree_node* pull_from_tree_stack (stack_of_trees* stack)
{
	if (stack == NULL)
		return NULL;
	while (stack->next != NULL)
		stack = stack->next;
	tree_node* tree = stack->tree;
	stack = stack->prev;
	free (stack->next);
	stack->next = NULL;
	return tree;
}

tree_node* lexem_list_to_tree (lexem* list)
{
	stack_of_trees* stack = init_stack_of_trees ();
	stack->tree = init_tree_node (EMPTY);
	tree_node* proxy_nums = NULL;
	tree_node* proxy_operators = stack->tree;
	tree_node* retval = stack->tree;
	bool turn = LEFT;
	expect_t expect = PRNTH | NUM;
	while (list != NULL)
	{
		if ((expect & list->what) == 0)
			goto _ERR;
		switch (list->what)
		{
			case NUM:
				expect = BIN_OPRTR;
				proxy_nums = init_tree_node (NUM, list->value.num);
				proxy_nums->above = proxy_operators;
				if (turn == LEFT)
					proxy_operators->left = proxy_nums;
				else
					proxy_operators->right = proxy_nums;
				break;

			case BIN_OPRTR:
				expect = PRNTH | NUM;
				turn = !turn;
				if (stack->tree->what == EMPTY)
				{
					stack->tree->what = BIN_OPRTR;
					stack->tree->value.other = list->value.other;
					break;
				}
				switch (list->value.other)
				{
					/* priority 1 ops */
					case ADD:
					case SUB:
						proxy_operators = init_tree_node (BIN_OPRTR, list->value.other);
						proxy_operators->left = stack->tree;
						stack->tree->above = proxy_operators;
						stack->tree = proxy_operators;
						turn = RIGHT;
						break;

					/* priority 2 ops */
					case MUL:
					case DIV:
						proxy_operators->right = init_tree_node (BIN_OPRTR, list->value.other);
						proxy_operators = proxy_operators->right;
						proxy_operators->left = proxy_nums;
						turn = RIGHT;
						proxy_nums->above = proxy_operators;
						break;
						
				}
				break;

			case PRNTH:
			case UN_OPRTR:
			case EMPTY:
			default:
				goto _ERR;
		}
		list = list->next;
	}
	kill_stack_tree (stack);
	goto _NORMAL_EXIT;
_ERR:
	error (E_SYNTAX);
	kill_stack_and_trees (stack);
	retval = NULL;
_NORMAL_EXIT:
	if (retval != NULL)
		GO_TO_TOP(retval);
	return retval;
}
