#pragma once
typedef enum token_type
{
	EMPTY = 0, /* bloat */
	NUM = 1,
	BIN_OPRTR = 2, /* binary operator */
	PRNTH = 4, /* parentheses */
	UN_OPRTR = 8, /* unary operator */
} token_type;

typedef enum bin_operators
{
	ADD = -1,
	SUB = -2,
	MUL = -3,
	DIV = -4,
} bin_operators;

typedef enum parentheses
{
	OPEN_RB = 1, /* opening round brackets ( */
	OPEN_CB = 2, /* opening curly brackets { */
	OPEN_SB = 3, /* opening square brackets [ */
	CLOSE_RB = -1,
	CLOSE_CB = -2,
	CLOSE_SB = -3,
} parentheses;

typedef union num_or_other
{
	double num;
	int other;
} num_or_other;

struct lexem;
typedef struct lexem
{
	enum token_type what;
	union num_or_other value;
	struct lexem* next;
} lexem;
typedef int expect_t;
#define EXPECT_ANYTHING(x) {(x = ~0)}
