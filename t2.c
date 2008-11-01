long test(long testno, long arg1, long arg2);

int foo(int testno, int arg1, int arg2)
{
	return test(testno, arg1, arg2);
}

#include "tests.c"
