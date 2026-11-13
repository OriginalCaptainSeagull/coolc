#include <stdio.h>
#include "error_t.h"
static char* error_msgs[] =
{
	"Couldn\'t recognize pattern\n",
	"Couldn\'t calculate expression\n",
	"Wrong syntax\n",
};

void error (error_t e)
{
	if (e == E_NOE)
		return;
	e -= 1;
	printf (error_msgs[e]);
	return;
}
