#pragma once
#include "tokens.h"
struct tree_node;
union tree_value
{
	double num;
	int other;
};
typedef struct tree_node
{
	token_type what;
	union tree_value value;
	struct tree_node* above;
	struct tree_node* left;
	struct tree_node* right;
} tree_node;
struct stack_of_trees;
typedef struct stack_of_trees
{
	tree_node* tree;
	struct stack_of_trees* next;
	struct stack_of_trees* prev;
} stack_of_trees;
#define GO_TO_TOP(tree)	\
{ \
	while (tree->above != NULL) \
		tree = tree->above; \
}
#define LEFT false
#define RIGHT true
