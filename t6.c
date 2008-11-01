#include "syscalls.h"

static const char buf[] = "hello world\n";
int vmMain()
{
	return trap_Print(buf);
}
