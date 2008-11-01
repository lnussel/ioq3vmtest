
#include "syscalls.h"
#include "bg_lib.h"

void function (void (*f)(char*));
void print(char* buf);

static char str[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

int vmMain()
{
	function(print);
	return 0;
}

void print(char* buf)
{
	trap_Print(buf);
}

void function(void (*f)(char*))
{
	char buf[16];
	strcpy(buf, str);

	f(buf);
}

char *strcpy( char *strDestination, const char *strSource ) {
	char *s;

	s = strDestination;
	while ( *strSource ) {
		*s++ = *strSource++;
	}
	*s = 0;
	return strDestination;
}


