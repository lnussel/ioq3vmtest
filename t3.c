int fibonacci(int num, int arg1, int arg2)
{
	if(!num) return arg1;
	--num;
	return fibonacci(num, arg2, arg1+arg2);
}
