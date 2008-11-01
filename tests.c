unsigned fibonacci(unsigned num, unsigned arg1, unsigned arg2)
{
	if(!num) return arg1;
	--num;
	return fibonacci(num, arg2, arg1+arg2);
}

long test(long testno, long arg1, long arg2)
{
	int res = 0xC0DEDBAD;
	int i1 = arg1;
	int i2 = arg2;
	unsigned u1 = arg1;
	unsigned u2 = arg2;
	float f1 = *(float*)&i1;
	float f2 = *(float*)&i2;
	short s1 = i1;
	short s2 = i2;
	signed char c1 = i1;
	signed char c2 = i2;
	float fres;

	switch(testno)
	{
		case 1:
			res = i1+i2;
			break;
		case 2:
			res = i1-i2;
			break;
		case 3:
			res = i1&i2;
			break;
		case 4:
			res = i1|i2;
			break;
		case 5:
			res = i1^i2;
			break;
		case 6:
			res = i1*i2;
			break;
		case 7:
			res = u1*u2;
			break;
		case 8:
			res = i1%i2;
			break;
		case 9:
			res = u1%u2;
			break;
		case 10:
			res = i1/i2;
			break;
		case 11:
			res = u1/u2;
			break;
		case 12:
			res = s1 * s2;
			break;
		case 13:
			res = c1 * c2;
			break;
		case 14:
			res = ~(u1);
			break;
		case 15:
			res = u1<<u2;
			break;
		case 16:
			res = u1>>u2;
			break;
		case 17:
			res = i1>>i2;
			break;
		case 18:
			res = -i1;
			break;

		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
			res = 0;
			switch(testno)
			{
				case 19: if (i1 == i2) res = 1; break;
				case 20: if (i1 != i2) res = 1; break;
				case 21: if (i1 <= i2) res = 1; break;
				case 22: if (i1 >= i2) res = 1; break;
				case 23: if (i1 < i2)  res = 1; break;
				case 24: if (i1 > i2)  res = 1; break;
			}
			break;

		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
			res = 0;
			switch(testno)
			{
				case 25: if (u1 == u2) res = 1; break;
				case 26: if (u1 != u2) res = 1; break;
				case 27: if (u1 <= u2) res = 1; break;
				case 28: if (u1 >= u2) res = 1; break;
				case 29: if (u1 < u2)  res = 1; break;
				case 30: if (u1 > u2)  res = 1; break;
			}
			break;

		case 31:
			res = fibonacci(testno, i1, i2);

		case 100:
			fres = f1/f2;
			break;
		case 101:
			fres = f1*f2;
			break;
		case 102:
			fres = f1+f2;
			break;
		case 103:
			fres = f1-f2;
			break;
		case 104:
			fres = -f1;
			break;

		case 105: if (f1 == f2) fres = 1; else fres = 0; break;
		case 106: if (f1 != f2) fres = 1; else fres = 0; break;
		case 107: if (f1 <= f2) fres = 1; else fres = 0; break;
		case 108: if (f1 >= f2) fres = 1; else fres = 0; break;
		case 109: if (f1 < f2)  fres = 1; else fres = 0; break;
		case 110: if (f1 > f2)  fres = 1; else fres = 0; break;

		case 111:
			i1 = f1; i2 = f2; fres = i1+i2; break;

		default:
			break;
	}

	if(testno>=100)
		res = *(int*)&fres;

	return res;
}
