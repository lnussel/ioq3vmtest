static char	*opnames[256] = {
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
