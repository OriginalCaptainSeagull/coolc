#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"

int main (void)
{
	char* string;
	lexem* lexem_chain;
	tree_node* tree;
	double result;

	puts ("Enter \'q\' to finish session");
	while (true)
	{
		printf (" > ");
		string = get_line();
		if (quit_ (string))
		{
			free (string);
			return 0;
		}
		lexem_chain = lexemize (string);
		free (string);
		string = NULL;
		if (lexem_chain == NULL)
		{
			error (E_STRANGE); /* unrecognized pattern */
			continue;
		}
		tree = lexem_list_to_tree (lexem_chain);
		rm_lexem_chain (lexem_chain);
		lexem_chain = NULL;
		if (tree == NULL)
		{
			error (E_SYNTAX);
			continue;
		}
		result = eval (tree);
		printf ("Result: %lf\n", result);
		clear_tree (tree);
		tree = NULL;
	}
	return 0;
}
