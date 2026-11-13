#include <stdlib.h>
#include <stdio.h>
#include "trees.h"
#include "error_t.h"
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
			left += right;
			break;

		case SUB:
			left -= right;
			break;

		case MUL:
			left *= right;
			break;

		case DIV:
			left /= right;
			break;
	}
	return left;
}
