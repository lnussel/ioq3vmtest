#include "syscalls.h"

void foo(char* s);

static char m[] = "explode1";
static char msg1[] = "address < 10\n";
static char msg2[] = "address >= 10\n";

int vmMain(int num, int arg1, int arg2, int arg3, int arg4)
{
	foo(m);
	return 0;
}

void foo(char* s)
{
	trap_Print(s);
	if(s < (char*)10)
	{
		trap_Print(msg1);
	}
	else
	{
		trap_Print(msg2);
	}
}
