#include "vm_local.h"
#include "vmstats.h"
#include "ops.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static vmInterpret_t interpret = VMI_COMPILED;

vm_t* currentVM;

static FILE* _asout;

static cvar_t _com_developer = { integer : 0 };
cvar_t	*com_developer = &_com_developer;

char* mmapfile(const char* fn, size_t* size);

long printsyscall(long* args)
{
	printf("callnr %ld, args: %ld %ld %ld %ld\n",
			args[0],
			args[1],
			args[2],
			args[3],
			args[4]);

	switch( args[0] ) {
	case 0:
	case 1:
		{
			char * s = VMA(1)?VMA(1):"(NULL)\n";
			fputs(s, stderr);
			if(s[strlen(s)-1] != '\n')
				putc('\n', stderr);
		}
		return 0;
	case 999:
		{
			int a[5];
			a[0] = 3;
			a[1] = args[1];
			a[2] = args[2];
			a[3] = args[3];
			a[4] = args[4];
			if(!currentVM->compiled)
				return VM_CallInterpreted(currentVM, a);
			else
				return VM_CallCompiled(currentVM, a);
		}
	case 1000:
		{
			int a[5];
			a[0] = 4;
			a[1] = args[1];
			a[2] = args[2];
			a[3] = args[3];
			a[4] = args[4];
			if(!currentVM->compiled)
				return VM_CallInterpreted(currentVM, a);
			else
				return VM_CallCompiled(currentVM, a);
		}
	case 1001:
		printf("got buffer with content '%s', length %d\n", (char*)VMA(1), (int)args[2]);
		strncpy(VMA(1), "blah\n", args[2]);
		return 0;
	}

	return 0x66;
}

#include "tests.c"

int testops(vm_t* vm)
{
	int i1, i2, vmres;
	int i;
	float f1, f2, fres, fvmres;
	int numitests = 32;
	int numftests = 11;
	int ret = 0;
	int testno;
	int res = 0xC0DEDBAD;

	srand(time(NULL));

	i1 = 1 + (int) (1000.0 * (rand() / (RAND_MAX + 1.0)));
	i2 = 1 + (int) (1000.0 * (rand() / (RAND_MAX + 1.0)));

	if(i1 < i2)
	{
		i = i1;
		i1 = i2;
		i2 = i;
	}

	f1 = i1/1.5;
	f2 = i2/1.5;

	if(!i2) i2=i1;
	if(!f2) f2=f1;

	printf("i1: %d i2: %d\n", i1, i2);
	printf("f1: %f f2: %f\n", f1, f2);

testintops:
	for (testno = 1; testno < numitests; ++testno)
	{
		printf("int test %d ... ", testno);
		fflush(stdout);

		res = test(testno, i1, i2);
		vmres = VM_Call(vm, testno, i1, i2);

		if(vmres == res)
		{
			printf("ok: 0x%X == 0x%X\n", res, vmres);
		}
		else
		{
			printf("failed: 0x%X != 0x%X\n", res, vmres);
			ret = 1;
		}
	}
	if(i1 != i2)
	{
		i2 = i1;
		goto testintops;
	}

testfops:
	i1 = *(int*)&f1;
	i2 = *(int*)&f2;

	for (testno = 100; testno < 100+numftests; ++testno)
	{
		printf("float test %d ... ", testno);
		fflush(stdout);

		res = test(testno, i1, i2);
		vmres = VM_Call(vm, testno, i1, i2);

		fres = *(float*)&res;
		fvmres = *(float*)&vmres;

		if(fvmres == fres)
		{
			printf("ok: %f == %f\n", fres, fvmres);
		}
		else
		{
			printf("failed: %f != %f\n", fres, fvmres);
			ret = 1;
		}
	}
	if(f1 > f2)
	{
		float t = f1;
		f1 = f2;
		f2 = t;
		goto testfops;
	}
	else if(f1 < f2)
	{
		f2 = f1;
		goto testfops;
	}

	return ret;
}

void VM_VmInfo_f( void );

int main(int argc, char* argv[])
{
	vm_t* vm[3];
	int i;
	long args[11] = {0};
	int ret = 0xDEADBEEF;

	char* file = argv[1];

	char module[128];

	if(argc < 2)
		return -1;

	if(argc > 2 && (!strcmp(argv[1], "--stats") || !strcmp(argv[1], "--da")))
	{
		vm_t VM = {0};
		vmHeader_t* header;

		vm[0] = &VM;

		*vm[0]->name = 0;
		strncat(vm[0]->name, argv[2], sizeof(vm[0]->name)-strlen(vm[0]->name)-1);

		vmHeader_t* VM_LoadQVM(vm_t *vm, qboolean alloc, qboolean unpure);
		if(!( header = VM_LoadQVM( vm[0], qtrue, qtrue )))
			return 1;

		if (!strcmp(argv[1], "--da"))
		{
			int pc = 0;
			byte* code = (byte *)header + header->codeOffset;

			for ( i = 0; i < header->instructionCount; ++i )
			{
				int op = code[ pc ];
				++pc;

				if (op_argsize[op] == 4) {
					int arg;
					memcpy(&arg, &code[pc], 4);
					arg = LittleLong(arg);
					printf("%s %d\n", opnames[op], arg);
				} else if (op_argsize[op] == 1) {
					unsigned char arg = code[pc];
					printf("%s %hhd\n", opnames[op], arg);
				} else {
					printf("%s\n", opnames[op]);
				}

				pc += op_argsize[op];
			}
		}
		else
		{
			VM_Stats(vm[0], header, 3);
		}

		FS_FreeFile(header);

		return 0;
	}

	strcpy(module, file);
	if(strchr(module, '.'))
		*strchr(module, '.') = '\0';

	vm[0] = VM_Create( module, printsyscall, interpret );

	VM_VmInfo_f();

	if(argc > 2)
	{
		for(i = 2; i < argc; ++i)
		{
			args[i-2] = strtol(argv[i],NULL,0);
		}

		ret  = VM_Call(vm[0], args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
	}
	else
	{
		ret = testops(vm[0]);
	}


	printf("ret: %d [%X]\n", ret, ret);

	return 0;
}

fileHandle_t FS_FOpenFileWrite( const char *filename )
{
	_asout = fopen(filename, "w");
	return 0;
}

int FS_Write( const void *buffer, int len, fileHandle_t h )
{
	return fwrite(buffer, 1, len, _asout);
}

void FS_Printf( fileHandle_t h, const char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(_asout, fmt, ap);
	va_end(ap);
}

void	FS_Flush( fileHandle_t f )
{
	fflush(_asout);
}

void	FS_FCloseFile( fileHandle_t f )
{
	fclose(_asout);
}

char *FS_BuildOSPath( const char *base, const char *game, const char *qpath )
{
	static char buf[4096];
	strcpy(buf, "./");
	strcat(buf, qpath);
	return buf;
}

cvar_t *Cvar_Get( const char *var_name, const char *var_value, int flags ) {
	static cvar_t c = { .string = "" };
	return &c;
}

float Cvar_VariableValue( const char *var_name )
{
	return 0.0;
}

void	* QDECL Sys_LoadGameDll( const char *name, intptr_t (QDECL **entryPoint)(int, ...),
				  intptr_t (QDECL *systemcalls)(intptr_t, ...) )
{
	return NULL;
}

vmInterpret_t FS_FindVM(void **startSearch, char *found, int foundlen, const char *name, int enableDll)
{
	return VMI_COMPILED;
}

#if id386 || idx64
int (QDECL *Q_VMftol)(void) = qvmftolsse;
#endif
