#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include "trees.h"

tree_node* init_tree_node (token_type what, ...)
{
	va_list args;
	va_start (args, what);
	tree_node* node = malloc (sizeof (tree_node));
	node->what = what;
	node->above = NULL;
	node->right = NULL;
	node->left = NULL;
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
	tree_node* right;
	tree_node* left;
	right = root->right;
	left = root->left;
	free (root);
	clear_tree (right);
	clear_tree (left);
	return;
}

stack_of_trees* init_stack_of_trees (void)
{
	stack_of_trees* node = malloc (sizeof (stack_of_trees));
	node->next = NULL;
	node->prev = NULL;
	node->proxy_num = NULL;
	node->tree = NULL;
	node->proxy_op = NULL;
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
	stack_of_trees* temp = stack;
	stack = stack->next;
	stack->prev = temp;
	return stack;
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
	stack->proxy_op = stack->tree;
	tree_node* temp;
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
				expect = BIN_OPRTR | PRNTH;
				stack->proxy_num = init_tree_node (NUM, list->value.num);
				stack->proxy_num->above = stack->proxy_op;
				if (turn == LEFT) 
					stack->proxy_op->left = stack->proxy_num;
				else
					stack->proxy_op->right = stack->proxy_num;
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
						stack->proxy_op = init_tree_node (BIN_OPRTR, list->value.other);
						stack->proxy_op->left = stack->tree;
						stack->tree->above = stack->proxy_op;
						stack->tree = stack->proxy_op;
						turn = RIGHT;
						break;

					/* priority 2 ops */
					case MUL:
					case DIV:
						temp = stack->proxy_op->right;
						stack->proxy_op->right = init_tree_node (BIN_OPRTR, list->value.other);
						stack->proxy_op = stack->proxy_op->right;
						stack->proxy_op->left =	temp;
						temp->above = stack->proxy_op;
						turn = RIGHT;
						break;
						
				}
				break;

			case PRNTH:
				expect = PRNTH | NUM;
				switch (list->value.other)
				{
					case OPEN_RB: /* opening right bracket */
						stack = push_to_tree_stack (stack);
						stack->tree = init_tree_node (EMPTY);
						stack->proxy_op = stack->tree;
						turn = LEFT;
						break;

					case CLOSE_RB:
						expect = BIN_OPRTR | PRNTH;
						if (stack->prev == NULL)
							goto _ERR;
						stack = stack->prev;
						temp = pull_from_tree_stack (stack);
						if (temp == EMPTY)
						{
							if (temp->left != NULL)
							{
								temp = temp->left;
								free (temp->above);
							}
							else
							{
								temp->what = NUM;
								temp->value.num = 0.0;
							}
						}
						temp->above = stack->proxy_op;
						if (stack->proxy_op->left == NULL)
						{
							stack->proxy_op->left = temp;
							turn = LEFT;
							break;
						}
						stack->proxy_op->right = temp;
						stack->proxy_op = init_tree_node (EMPTY);
						stack->proxy_op->left = stack->tree;
						stack->tree->above = stack->proxy_op;
						stack->tree = stack->proxy_op;
						turn = LEFT;
						break;
				}
				break;

			case UN_OPRTR:
				goto _ERR;
			case EMPTY:
				goto _ERR;
			default:
				goto _ERR;
		}
		list = list->next;
	}
	if (stack->prev != NULL)
		goto _ERR;
	retval = stack->tree;
	kill_stack_tree (stack);
	goto _NORMAL_EXIT;
_ERR:
	kill_stack_and_trees (stack);
	retval = NULL;
_NORMAL_EXIT:
	return retval;
}
