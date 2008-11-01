#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <sys/stat.h>

static char* mmapfile(const char* fn, size_t* size);

int vm_debugLevel = 0;

void Com_Printf( const char *fmt, ... )
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	return;
}

#if 0
void* VM_ValueToSymbol(void)
{
	return NULL;
}
void* VM_ValueToFunctionSymbol()
{
	return NULL;
}


#endif

void* va(void)
{
	return NULL;
}

void Com_Error( int level, const char *error, ... )
{
	va_list ap;
	va_start(ap, error);
	vfprintf(stderr, error, ap);
	va_end(ap);
	fputc('\n', stderr);
	exit(99);
	return;
}

void* Hunk_Alloc(size_t size)
{
	return malloc(size);
}

void Cmd_AddCommand( void)
{
}

void Com_Memset (void* dest, const int val, const size_t count)
{
	memset(dest, val, count);
}

void Com_sprintf( char *dest, int size, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(dest, size, fmt, ap);
	va_end(ap);
	return;
}

void *Z_Malloc(int size)
{
	return malloc(size);
}

void Z_Free (void *ptr)
{
	free(ptr);
}

void	Sys_UnloadDll( void *dllHandle )
{
}

int	Hunk_MemoryRemaining( void )
{
	return 420000;
}

void FS_FreeFile(void *buf)
{
	free(buf);
}

void COM_StripExtension( const char *in, char *out )
{
	out = "";
}

char *COM_Parse (char *data)
{
	return NULL;
}

void Q_strncpyz( char *dest, const char *src, int destsize ) {
	strncpy( dest, src, destsize-1 );
    dest[destsize-1] = 0;
}

int Q_stricmp (char *s1, char *s2)
{
	return strcmp (s1, s2);
}

int FS_ReadFile( const char *qpath, void **buffer )
{
	size_t size;
	char* mem;

	if(strrchr(qpath, '/'))
	{
		qpath = strrchr(qpath, '/')+1;
	}

	mem = mmapfile(qpath, &size);
	if(!mem)
		goto out_error;

	*buffer = malloc(size);
	memcpy(*buffer, mem, size);

	Com_Printf("%s (%d bytes) loaded\n", qpath, size);

	munmap(mem, 0);

	return size;

out_error:
	*buffer = NULL;
	return -1;
}

static char* mmapfile(const char* fn, size_t* size)
{
	int fd = -1;
	char* mem = NULL;
	struct stat stb;

	fd = open(fn, O_RDONLY);
	if(fd == -1)
		goto out;

	if(fstat(fd, &stb) == -1)
		goto out;

	*size = stb.st_size;

	mem = mmap(NULL, stb.st_size, PROT_READ|PROT_EXEC, MAP_SHARED, fd, 0);
	if(mem == (void*)-1)
		mem = NULL;

out:
	if(fd != -1)
		close(fd);

	return mem;
}
