#include "syscalls.h"

int vmMain(int num, int arg1, int arg2, int arg3, int arg4)
{
	char msg[] = "test\n";
	char buf[1024] = "fuhbar";
	int res;
	switch(num)
	{
		case 1:
			return trap_Print(msg);
		case 2:
			res = trap_TestCallBack(arg1, arg2, arg3, arg4);
			return res;
		case 3:
			res = trap_TestCallBack2(arg1, arg2, arg3, arg4);
			return res;
		case 4:
			return arg1+arg2+arg3+arg4;
		case 5:
			trap_getstring(buf, sizeof(buf));
			trap_Print(buf);
			return 0;
	}
	return 0x99;
}
