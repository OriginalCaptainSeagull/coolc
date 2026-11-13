#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define NaN (0.0 / 0.0)
#define ASCII_ZERO 48
#define ASCII_NINE 57

char* get_line (void)
{
	char* line = malloc (1024);
	size_t sz = 1024;
	char* iter = line;
	char* end = line + 1023;
	char ch;
	while ((ch = getchar()) != '\n')
	{
		*iter = ch;
		++iter;
		if (iter == end)
		{
			sz *= 2;
			line = realloc (line, sz);
			end = line + sz - 1;
		}
	}
	*iter = '\0';
	return line;
}

char* get_whole (char* string, double* num)
{
	double x = 0.0;
	while (isdigit (*string))
	{
		x = x * 10.0 + (*string - ASCII_ZERO);
		++string;
	}
	*num = x;
	return string;
}

char* get_rational (char* string, double* num)
{
	if (*string != '.' && *string != ',')
		return string;
	double x = 0.0;
	double exp = 10;
	while (isdigit (*string))
	{
		x += (*string - ASCII_ZERO) / exp;
		exp *= 10;
		++string;
	}
	*num = x;
	return string;
}

char* atod (char* string, double* num)
{
	double whole_pt = 0.0;
	double rational_pt = 0.0;
	bool negative = true;
	if (isdigit (*string))
		goto _CONVERSION;
	switch (*string)
	{
		case '-':
			negative = false;
			break;
			
		case '.':
		case ',':
			goto _RAT;
			break;
	
		case '+':
			break;

		default:
			*num = NaN;
			return string;
			break;
	}
_CONVERSION:
	string = get_whole (string, &whole_pt);
_RAT:
	string = get_rational (string, &rational_pt);
	*num = (negative) ? rational_pt + whole_pt : -1.0 * (rational_pt + whole_pt);
	return string;
}
