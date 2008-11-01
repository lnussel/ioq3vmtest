#include "vm_local.h"

#include <stdio.h>
#include <stdlib.h>

static char* opnames[] = {
	"OP_UNDEF", 

	"OP_IGNORE", 

	"OP_BREAK",

	"OP_ENTER",
	"OP_LEAVE",
	"OP_CALL",
	"OP_PUSH",
	"OP_POP",

	"OP_CONST",

	"OP_LOCAL",

	"OP_JUMP",

	//-------------------

	"OP_EQ",
	"OP_NE",

	"OP_LTI",
	"OP_LEI",
	"OP_GTI",
	"OP_GEI",

	"OP_LTU",
	"OP_LEU",
	"OP_GTU",
	"OP_GEU",

	"OP_EQF",
	"OP_NEF",

	"OP_LTF",
	"OP_LEF",
	"OP_GTF",
	"OP_GEF",

	//-------------------

	"OP_LOAD1",
	"OP_LOAD2",
	"OP_LOAD4",
	"OP_STORE1",
	"OP_STORE2",
	"OP_STORE4",
	"OP_ARG",

	"OP_BLOCK_COPY",

	//-------------------

	"OP_SEX8",
	"OP_SEX16",

	"OP_NEGI",
	"OP_ADD",
	"OP_SUB",
	"OP_DIVI",
	"OP_DIVU",
	"OP_MODI",
	"OP_MODU",
	"OP_MULI",
	"OP_MULU",

	"OP_BAND",
	"OP_BOR",
	"OP_BXOR",
	"OP_BCOM",

	"OP_LSH",
	"OP_RSHI",
	"OP_RSHU",

	"OP_NEGF",
	"OP_ADDF",
	"OP_SUBF",
	"OP_DIVF",
	"OP_MULF",

	"OP_CVIF",
	"OP_CVFI"
};

static unsigned char op_argsize[256] = 
{
	[OP_ENTER]      = 4,
	[OP_LEAVE]      = 4,
	[OP_CONST]      = 4,
	[OP_LOCAL]      = 4,
	[OP_EQ]         = 4,
	[OP_NE]         = 4,
	[OP_LTI]        = 4,
	[OP_LEI]        = 4,
	[OP_GTI]        = 4,
	[OP_GEI]        = 4,
	[OP_LTU]        = 4,
	[OP_LEU]        = 4,
	[OP_GTU]        = 4,
	[OP_GEU]        = 4,
	[OP_EQF]        = 4,
	[OP_NEF]        = 4,
	[OP_LTF]        = 4,
	[OP_LEF]        = 4,
	[OP_GTF]        = 4,
	[OP_GEF]        = 4,
	[OP_ARG]        = 1,
	[OP_BLOCK_COPY] = 4,
};

#define DIMOF(x) (sizeof(x)/sizeof(x[0]))
#define NUMOPS DIMOF(opnames)

struct stats
{
	unsigned count;
	unsigned op;
};
static struct stats stats_1[NUMOPS];

static struct stats stats_2[NUMOPS*NUMOPS];

static struct stats* stats_3;

int sort_by_count(const void* A, const void* B)
{
	unsigned short a = *(const unsigned short*)A;
	unsigned short b = *(const unsigned short*)B;

	if(a < b)
		return -1;
	if(a > b)
		return 1;
	return 0;
}

void VM_Stats(vm_t* vm, vmHeader_t *header, int numhist)
{
	int pc = 0;
	char* code;
	unsigned i;
	unsigned char op;
	unsigned hmask = 0;
	unsigned ncombo = 1;

	union
	{
		unsigned op;
	} h = {0};

	for(i = 0; i < numhist; ++i)
	{
		hmask = (hmask<<8) | 0xFF;
		ncombo *= NUMOPS;
	}

	stats_3 = Z_Malloc(ncombo*sizeof(struct stats));

	code = (char *)header + header->codeOffset;

	for ( i = 0; i < header->instructionCount; ++i )
	{
		op = code[ pc ];

		h.op = ((h.op<<8) + op) & hmask;

		++pc;
		pc += op_argsize[op];

		++stats_1[op].count;

#define PREVOP ((h.op>>8)&0xFF)
#define PPREVOP ((h.op>>16)&0xFF)
#define PPPREVOP ((h.op>>24)&0xFF)

		if(PREVOP)
		{
			++stats_2[PREVOP*NUMOPS+op].count;
		}
		if(PPREVOP && numhist == 3)
		{
			++stats_3[(PPREVOP*NUMOPS+PREVOP)*NUMOPS+op].count;
		}
		if(PPPREVOP && numhist == 4)
		{
			++stats_3[((PPPREVOP*NUMOPS+PPREVOP)*NUMOPS+PREVOP)*NUMOPS+op].count;
		}
	}

	for(i = 0; i < DIMOF(stats_1); ++i)
	{
		stats_1[i].op = i;
	}
	for(i = 0; i < NUMOPS; ++i)
	{
		unsigned j;
		for(j = 0; j < NUMOPS; ++j)
			stats_2[i*NUMOPS+j].op = (i<<8) + j;
	}
	for(i = 0; i < ncombo; ++i)
	{
		unsigned a, b, c, d;
		d = i % NUMOPS;
		c = i/NUMOPS % NUMOPS;
		b = i/NUMOPS/NUMOPS % NUMOPS;
		a = i/NUMOPS/NUMOPS/NUMOPS % NUMOPS;
		stats_3[i].op = (a<<24) + (b<<16) + (c<<8) + d;
	}

	if(numhist == 1)
	{
		qsort(stats_1, NUMOPS, sizeof(struct stats), sort_by_count);

		for(i = 0; i < DIMOF(stats_1); ++i)
		{
			if(!stats_1[i].count)
				continue;
			printf("%-6d %s\n", stats_1[i].count, opnames[stats_1[i].op]);
		}
	}
	else if(numhist == 2)
	{
		qsort(stats_2, DIMOF(stats_2), sizeof(struct stats), sort_by_count);
		for(i = 0; i < DIMOF(stats_2); ++i)
		{
			if(!stats_2[i].count)
				continue;
			printf("%-6d %-13s %-13s\n", stats_2[i].count, opnames[stats_2[i].op>>8], opnames[stats_2[i].op&0xFF]);
		}
	}
	else if(numhist >= 3)
	{
		qsort(stats_3, ncombo, sizeof(struct stats), sort_by_count);
		for(i = 0; i < ncombo; ++i)
		{
			if(stats_3[i].count < 1)
				continue;

			if(numhist == 4)
				printf("%-6d %-13s %-13s %-13s %-13s\n", stats_3[i].count,
						opnames[(stats_3[i].op>>24)&0xFF],
						opnames[(stats_3[i].op>>16)&0xFF],
						opnames[(stats_3[i].op>>8)&0xFF],
						opnames[(stats_3[i].op)&0xFF]);
			else
				printf("%-6d %-13s %-13s %-13s\n", stats_3[i].count,
						opnames[(stats_3[i].op>>16)&0xFF],
						opnames[(stats_3[i].op>>8)&0xFF],
						opnames[(stats_3[i].op)&0xFF]);
		}
	}
}
