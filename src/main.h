#pragma once
#include "error_t.h"
/* structures */
struct tree_node;
struct lexem;
typedef struct tree_node tree_node;
typedef struct lexem lexem;

/* functions */
char* get_line ();
double eval (tree_node*);
bool quit_ (char*);
void rm_lexem_chain (lexem*);
void clear_tree (tree_node*);
void error (error_t);
lexem* lexemize (char*);
tree_node* lexem_list_to_tree (lexem*);
