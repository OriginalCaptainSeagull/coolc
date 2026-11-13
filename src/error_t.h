#pragma once
typedef enum error_t
{
	E_NOE = 0,
	E_STRANGE = 1, /* will print error message about unrecognized pattern */
	E_CALC = 2, /* calculation error */
	E_SYNTAX = 3, /* wrong syntax */
} error_t;
void error (const error_t);
