#include "syscalls.h"
#include "bg_lib.h"

void function (void);

//__asm__(".section .note.GNU-stack,\"x\"");

static char buf[] = "\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc";

int main()
{
	void (*f)();

	f=(void*)buf;

	f();
	
	return 0;
}
