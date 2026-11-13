#include <stdlib.h>
#include <stdio.h>
#include "main.h"

int main (void)
{
	char* string;
	lexem* lexem_chain;
	tree_node* tree;
	double result;

	printf (" > ");
	string = get_line();
	lexem_chain = lexemize (string);
	free (string);
	tree = lexem_list_to_tree (lexem_chain);
	if (tree == NULL)
		return 1;
	rm_lexem_chain (lexem_chain);
	result = eval (tree);
	clear_tree (tree);
	printf ("Result: %lf\n", result);
	return 0;
}
