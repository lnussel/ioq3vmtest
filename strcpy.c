#include "syscalls.h"
#include "bg_lib.h"

void function (void);

int vmMain()
{
	function();
	return 0;
}

void g(void)
{
	char buf[16];
	int num = 2000;
	memset(buf, 'a', num);

	trap_Print(buf);
}

void function (void)
{
	g();
}

void *memset( void *dest, int c, size_t count ) {
	while ( count-- ) {
		((char *)dest)[count] = c;
	}
	return dest;
}
