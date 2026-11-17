#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include "tokens.h"
char* atod (char*, double*);

lexem* init_lexem (enum token_type what, ...)
{
	va_list args;
	va_start (args, what);
	lexem* node = malloc (sizeof (lexem));
	node->what = what;
	node->next = NULL;
	switch (what)
	{
		case NUM:
			node->value.num = va_arg (args, double);
			break;

		case EMPTY:
			node->value.num = 0;
			break;

		default:
			node->value.other = va_arg (args, int);
	}
	va_end (args);
	return node;
}

void rm_lexem_chain (lexem* chain)
{
	lexem* cleaner = chain;
	while (chain != NULL)
	{
		cleaner = chain;
		chain = chain->next;
		free (cleaner);
	}
	return;
}

lexem* lexemize (char* string)
{
	lexem* start = init_lexem (EMPTY);
	lexem* end = start;
	while (*string != '\0')
	{
		while (isspace (*string)) ++string;

		if (isdigit (*string))
		{
			double num;
			string = atod (string, &num);
			end->next = init_lexem (NUM, num);
		}
		else if (*string == '-')
			end->next = init_lexem (BIN_OPRTR, SUB);
		else if (*string == '+')
			end->next = init_lexem (BIN_OPRTR, ADD);
		else if (*string == '/')
			end->next = init_lexem (BIN_OPRTR, DIV);
		else if (*string == '*')
			end->next = init_lexem (BIN_OPRTR, MUL);
		else if (*string == '(')
			end->next = init_lexem (PRNTH, OPEN_RB);
		else if (*string == ')')
			end->next = init_lexem (PRNTH, CLOSE_RB);
		else
		{
			rm_lexem_chain (start);
			start = NULL;
			goto ENDD;
		}

		++string;
		end = end->next;
	}
	lexem* temp = start;
	start = start->next;
	free (temp);
	temp = start;
ENDD:
	return start;
}
