/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: Yijia Xu 041061204
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=


_|          _|_|    _|      _|    _|_|
_|        _|    _|  _|_|    _|  _|    _|
_|        _|_|_|_|  _|  _|  _|  _|_|_|_|
_|        _|    _|  _|    _|_|  _|    _|
_|_|_|_|  _|    _|  _|      _|  _|    _|
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 19

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */	
	MNID_T,		/*  1: Method name identifier token (start: &) */
	INL_T,		/*  2: Integer literal token */
	FPL_T,		/*  3: Floating point literal token */
	STR_T,		/*  4: String literal token */
	LPR_T,		/*  5: Left parenthesis token */	//
	RPR_T,		/*  6: Right parenthesis token */	//
	LBR_T,		/*  7: Left brace token */	//
	RBR_T,		/*  8: Right brace token */		//
	KW_T,		/*  9: Keyword token */
	EOS_T,		/*  10: End of statement (semicolon) */		//
	RTE_T,		/*  11: Run-time error token */
	SEOF_T,		/*  12: Source end-of-file token */		//
	CMT_T,		/*  13: Comment token */
	VID_T,		/*  14: Variable identifier token */
	ARI_T,		/*  15: Arithmetic operator token */
	REL_T,		/*  16: Relational operator token */
	LOG_T,		/*  17: Logical operator token */
	ASS_T		/*  18: Assignment operator token */
};

/* TO_DO: Define the list of keywords */
static string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"FPL_T",
	"STR_T",
	"LPR_T", 
	"RPR_T", 
	"LBR_T", 
	"RBR_T", 
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"VID_T",
	"ARI_T",
	"REL_T",
	"LOG_T",
	"ASS_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	int32 codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	int32 intValue;				/* integer literal attribute (value) */
	int32 keywordIndex;			/* keyword index in the keyword table */
	int32 contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	float32 floatValue;				/* floating-point literal attribute (value) */
	rune idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	rune errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	int32 flags;			/* Flags information */
	union {
		int32 intValue;				/* Integer value */
		float32 floatValue;			/* Float value */
		string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	int32 code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	int32 scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '/'
#define CHRCOL3 '.'
#define CHRCOL4 '('
#define CHRCOL6 '\"'
#define CHRCOL7 '*'

/* These constants will be used on VID / MID function */
//#define MNID_SUF '&'
//#define COMM_SYM ''

/* TO_DO: Error states and illegal state */
#define ESNR	16		/* Error state with no retract */
#define ESWR	17		/* Error state with retract */
#define FS		18		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		18
#define CHAR_CLASSES	9

/* TO_DO: Transition table - type of states defined in separate table */
static int32 transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    /,    .,   (, SEOF,    \",     *,  other
	   L(0), D(1), C(2), Z(3), F(4), E(5), Q(6),  A(7),  O(8)    */
	{     1,   11,    4, ESNR, ESNR, ESWR,    9,  ESNR,  ESNR},	  // S0: NOFS
	{     1,    1,    7,    7,	  3, ESWR,    7,  ESNR,     2},	  // S1: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,    FS},	  // S2: FSWR (VID)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS},   // S3: FSWR (MNID)
	{  ESNR, ESNR, ESNR, ESNR, ESNR, ESWR, ESNR,     5,     4},	  // S4: NOFS
	{     5,    5,    5,    5,    5, ESWR,    5,     6,     5},   // S5: NOFS
	{  ESNR, ESNR,    8, ESNR, ESNR, ESWR, ESNR,  ESNR,     6},   // S6: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,    FS},	  // S7: FSWR (KEY)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,    FS},   // S8: FSNR (CMT)
	{     9,    9,    9,    9,    9, ESWR,	 10,     9,     9},	  // S9: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,    FS},   // S10: FSNR (SL)
	{  ESNR,   11, ESNR,   13, ESNR, ESWR, ESNR,  ESNR,    12},	  // S11: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,    FS,    FS},   // S12: FSWR (IL)
	{  ESNR,   14, ESNR, ESNR, ESNR, ESWR, ESNR,  ESNR,    12},   // S13: NOFS
	{  ESNR,   14, ESNR, ESNR, ESNR, ESWR, ESNR,  ESNR,    15},   // S14: NOFS
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,    FS},   // S15: FSWR (RL)
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,    FS},   // S16: ESNR
	{    FS,   FS,   FS,   FS,   FS,   FS,	 FS,    FS,    FS}    // S17: ESWR
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static int32 stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSWR, /* 02 VID */
	FSWR, /* 03 MNID */
	NOFS, /* 04 */
	NOFS, /* 05 */
	NOFS, /* 06 */
	FSWR, /* 07 KEY */
	FSNR, /* 08 CMT */
	NOFS, /* 09 */
	FSNR, /* 10 SL */
	NOFS, /* 11 */
	FSWR, /* 12 IL */
	NOFS, /* 13 */
	NOFS, /* 14 */
	FSWR, /* 15 RL */
	FSNR, /* 16 Error without retract */
	FSWR  /* 17 Error with retract */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
int32		startScanner(BufferPointer psc_buf);
static int32	nextClass(rune c);					/* character class function */
static int32	nextState(int32, rune);		/* state machine function */
void			printScannerData(ScannerData scData);
Token				tokenizer(void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(string lexeme);

/* Declare accepting states functions */
Token funcSL	(string lexeme);
Token funcIL	(string lexeme);
Token funcFPL	(string lexeme);
Token funcID	(string lexeme);
Token funcCMT   (string lexeme);
Token funcKEY	(string lexeme);
Token funcErr	(string lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* VID	[02] */
	funcID,		/* MNID [03] */
	NULL,		/* -    [04] */
	NULL,		/*      [05] */
	NULL,		/*      [06] */
	funcKEY,	/* -    [07] */
	funcCMT,	/*      [08] */
	NULL,	    /*      [09] */
	funcSL,		/* -    [10] */
	NULL,		/*      [11] */
	funcIL,		/* -    [12] */
	NULL,		/* -    [13] */
	NULL,		/*      [14] */
	funcFPL,	/* -    [15] */
	funcErr,	/* ERR1 [16] */
	funcErr		/* ERR2 [17] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 11

/* TO_DO: Define the list of keywords */
static string keywordTable[KWT_SIZE] = {
	"func",		/* KW00 */
	"int32",	/* KW01 */
	"int64",	/* KW02 */
	"float32",	/* KW03 */
	"rune",		/* KW04 */
	"string",	/* KW05 */
	"if",		/* KW06 */
	"then",		/* KW07 */
	"else",		/* KW08 */
	"while",	/* KW09 */
	"do"		/* KW10 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	rune indentationCharType;
	int32 indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
int32 numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
