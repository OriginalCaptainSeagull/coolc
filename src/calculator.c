#include <stdlib.h>
#include <stdio.h>
#include "trees.h"
#define NaN (0.0/0.0)

double eval (tree_node* root)
{
	if (root == NULL)
		return 0.0;
	if (root->what == NUM)
		return root->value.num;
	double left, right;
	right = eval (root->right);
	left = eval (root->left);
	switch (root->value.other)
	{
		case ADD:
			printf ("%lf + %lf", left, right);
			left += right;
			break;

		case SUB:
			printf ("%lf - %lf", left, right);
			left -= right;
			break;

		case MUL:
			printf ("%lf * %lf", left, right);
			left *= right;
			break;

		case DIV:
			printf ("%lf / %lf", left, right);
			left /= right;
			break;
		
		default:
			return left;
	}
	printf (" = %lf\n", left);
	return left;
}
