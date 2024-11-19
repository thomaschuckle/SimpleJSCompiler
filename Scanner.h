/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Vi Tuan Ha, Corey Lambert
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@            S J S            @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
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

#define NUM_TOKENS 49

enum TOKENS {
	/* Basic Tokens */
	ERR_T,           /*  0: Error token */
	MNID_T,          /*  1: Method name identifier token */
	VID_T,           /*  2: Variable identifier token */
	INL_T,           /*  3: Integer literal token */
	STR_T,           /*  4: String literal token */
	LPR_T,           /*  5: Left parenthesis token */
	RPR_T,           /*  6: Right parenthesis token */
	LBR_T,           /*  7: Left brace token */
	RBR_T,           /*  8: Right brace token */

	KW_T,            /*  9: Keyword token */

	EOS_T,           /* 10: End of statement (semicolon) */
	RTE_T,           /* 11: Run-time error token */
	SEOF_T,          /* 12: Source end-of-file token */
	CMT_T,           /* 13: Comment token */

	/* Arithmetic Operators */
	ASGN_T,          /* 14: Assignment operator token (=) */
	ADD_T,           /* 15: Addition operator token (+) */
	SUB_T,           /* 16: Subtraction operator token (-) */
	MUL_T,           /* 17: Multiplication operator token (*) */
	DIV_T,           /* 18: Division operator token (/) */
	MOD_T,           /* 19: Modulo operator token (%) */

	/* Comparison Operators */
	EQ_T,            /* 20: Equality operator token (==) */
	NE_T,            /* 21: Not equal operator token (!=) */
	GT_T,            /* 22: Greater than operator token (>) */
	LT_T,            /* 23: Less than operator token (<) */
	STRICT_EQ_T,     /* 24: Strict equality operator token (===) */
	STRICT_NE_T,     /* 25: Strict not equal operator token (!==) */

	/* Logical Operators */
	AND_T,           /* 26: Logical AND operator token (&&) */
	OR_T,            /* 27: Logical OR operator token (||) */
	NOT_T,           /* 28: Logical NOT operator token (!) */

	/* Compound Assignment Operators */
	ADD_ASGN_T,      /* 29: Addition assignment operator token (+=) */
	SUB_ASGN_T,      /* 30: Subtraction assignment operator token (-=) */
	MUL_ASGN_T,      /* 31: Multiplication assignment operator token (*=) */
	DIV_ASGN_T,      /* 32: Division assignment operator token (/=) */

	/* Bitwise Operators */
	BIT_AND_T,       /* 33: Bitwise AND operator token (&) */
	BIT_OR_T,        /* 34: Bitwise OR operator token (|) */
	BIT_XOR_T,       /* 35: Bitwise XOR operator token (^) */
	BIT_NOT_T,       /* 36: Bitwise NOT operator token (~) */
	LSHIFT_T,        /* 37: Left shift operator token (<<) */
	RSHIFT_T,        /* 38: Right shift operator token (>>) */
	URSHIFT_T,       /* 39: Unsigned right shift operator token (>>>) */

	/* Additional Delimiters */
	COMMA_T,         /* 40: Comma delimiter token (,) */
	COLON_T,         /* 41: Colon delimiter token (:) */
	DOT_T,           /* 42: Dot operator token (.) */

	/* Brackets */
	LBRACKET_T,      /* 43: Left square bracket token ([) */
	RBRACKET_T,      /* 44: Right square bracket token (]) */

	/* Template Literal Tokens */
	TEMPLATE_START_T,/* 45: Template literal start token (`) */
	TEMPLATE_END_T,  /* 46: Template literal end token (`) */

	/* Documenting Token */
	DOC_T, 			/* 47: Documentation token */

	/* Character Token*/
	CHAR_T,			/* 48: Character token */	
};

static string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",           /*  0 */
	"MNID_T",          /*  1 */
	"VID_T",           /*  2 */
	"INL_T",           /*  3 */
	"STR_T",           /*  4 */
	"LPR_T",           /*  5 */
	"RPR_T",           /*  6 */
	"LBR_T",           /*  7 */
	"RBR_T",           /*  8 */
	"KW_T",            /*  9 */
	"EOS_T",           /* 10 */
	"RTE_T",           /* 11 */
	"SEOF_T",          /* 12 */
	"CMT_T",           /* 13 */

	/* Arithmetic Operators */
	"ASGN_T",          /* 14 */
	"ADD_T",           /* 15 */
	"SUB_T",           /* 16 */
	"MUL_T",           /* 17 */
	"DIV_T",           /* 18 */
	"MOD_T",           /* 19 */

	/* Comparison Operators */
	"EQ_T",            /* 20 */
	"NE_T",            /* 21 */
	"GT_T",            /* 22 */
	"LT_T",            /* 23 */
	"STRICT_EQ_T",     /* 24 */
	"STRICT_NE_T",     /* 25 */

	/* Logical Operators */
	"AND_T",           /* 26 */
	"OR_T",            /* 27 */
	"NOT_T",           /* 28 */

	/* Compound Assignment Operators */
	"ADD_ASGN_T",      /* 29 */
	"SUB_ASGN_T",      /* 30 */
	"MUL_ASGN_T",      /* 31 */
	"DIV_ASGN_T",      /* 32 */

	/* Bitwise Operators */
	"BIT_AND_T",       /* 33 */
	"BIT_OR_T",        /* 34 */
	"BIT_XOR_T",       /* 35 */
	"BIT_NOT_T",       /* 36 */
	"LSHIFT_T",        /* 37 */
	"RSHIFT_T",        /* 38 */
	"URSHIFT_T",       /* 39 */

	/* Additional Delimiters */
	"COMMA_T",         /* 40 */
	"COLON_T",         /* 41 */
	"DOT_T",           /* 42 */

	/* Brackets */
	"LBRACKET_T",      /* 43 */
	"RBRACKET_T",      /* 44 */

	/* Template Literal Tokens */
	"TEMPLATE_START_T",/* 45 */
	"TEMPLATE_END_T",   /* 46 */

	/* Documenting Tokens */
	"DOC_T",            /* 47 */
	"CHAR_T"			/* 48 */
};


typedef enum ArithmeticOperators {
	OP_ASGN,        /* Assignment operator (=) */
	OP_ADD,         /* Addition operator (+) */
	OP_SUB,         /* Subtraction operator (-) */
	OP_MUL,         /* Multiplication operator (*) */
	OP_DIV,         /* Division operator (/) */
	OP_MOD,         /* Modulo operator (%) */
	OP_ADD_ASGN,    /* Addition assignment operator (+=) */
	OP_SUB_ASGN,    /* Subtraction assignment operator (-=) */
	OP_MUL_ASGN,    /* Multiplication assignment operator (*=) */
	OP_DIV_ASGN,    /* Division assignment operator (/=) */
	OP_MOD_ASGN     /* Modulo assignment operator (%=) */
} AriOperator;

typedef enum RelationalOperators {
	OP_EQ,          /* Equality operator (==) */
	OP_NE,          /* Not equal operator (!=) */
	OP_GT,          /* Greater than operator (>) */
	OP_LT,          /* Less than operator (<) */
	OP_STRICT_EQ,   /* Strict equality operator (===) */
	OP_STRICT_NE    /* Strict not equal operator (!==) */
} RelOperator;

typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

typedef union TokenAttribute {
	integer codeType;					/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	integer intValue;					/* integer literal attribute (value) */
	integer keywordIndex;				/* keyword index in the keyword table */
	integer contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralBuffer->content) */
	float floatValue;					/* floating-point literal attribute (value) */
	character charValue;				/* character literal attribute (value) */
	character idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	character errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

typedef struct idAttibutes {
	byte flags;						/* Flags information */
	union {
		integer intValue;			/* Integer value */
		float floatValue;			/* Float value */
		string stringContent;		/* String value */
		character charValue;		/* Character value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	integer code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	integer scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

// Lexeme Fixed Classes
#define EOS_CHR '\0'     // CH00: End of string (null terminator)
#define EOF_CHR 0xFF     // CH01: End of file
#define UND_CHR '_'      // CH02: Underscore (for identifiers)
#define AMP_CHR '&'      // CH03: Ampersand (bitwise AND, logical AND or method identifier)
#define SGL_QT_CHR '\''  // CH04: Single quote (for single-quoted strings)
#define TAB_CHR '\t'     // CH05: Tab character
#define SPC_CHR ' '      // CH06: Space character
#define NWL_CHR '\n'     // CH07: Newline character
#define SCL_CHR ';'      // CH08: Semicolon (end of statement)
#define LPR_CHR '('      // CH09: Left parenthesis (function or expression delimiter)
#define RPR_CHR ')'      // CH10: Right parenthesis (function or expression delimiter)
#define LBR_CHR '{'      // CH11: Left brace (start of scope)
#define RBR_CHR '}'      // CH12: Right brace (end of scope)
#define SLH_CHR '/'      // CH13: Slash (for division and comments)
#define MUL_CHR '*'      // CH14: Asterisk (for multiplication and block comments)
#define ADD_CHR '+'      // CH15: Plus (addition)
#define MIN_CHR '-'      // CH16: Minus (subtraction)
#define LT_CHR '<'       // CH17: Less than (comparison)
#define GT_CHR '>'       // CH18: Greater than (comparison)
#define EQ_CHR '='       // CH19: Equals (assignment and comparison)
#define EXL_CHR '!'      // CH20: Exclamation (logical NOT and inequality)
#define LBRK_CHR '['     // CH21: Left bracket (list delimiter)
#define RBRK_CHR ']'     // CH22: Right bracket (list delimiter)
#define COL_CHR ':'      // CH23: Colon (object property delimiter)
#define COM_CHR ','      // CH24: Comma (separator)
#define DOT_CHR '.'      // CH25: Dot (member access)
#define DBL_QT_CHR '"'   // CH26: Double quote (for double-quoted strings)
#define BCKT_CHR '`'     // CH27: Backtick (for template literals)
#define QST_CHR '?'      // CH28: Question mark (ternary operator)
#define BSLASH_CHR '\\'  // CH29: Backslash (escape character)
#define PIPE_CHR '|'     // CH30: Pipe (bitwise OR and logical OR)
#define CARET_CHR '^'    // CH31: Caret (bitwise XOR)
#define TILDE_CHR '~'    // CH32: Tilde (bitwise NOT)
#define PERC_CHR '%'     // CH33: Percent (modulo)
#define MNY_CHR '$'      // CH34: Dollar sign (keyword identiifer)

/* Special case tokens processed individually in the token-driven part of the scanner:
 * LPR_T, RPR_T, LBR_T, RBR_T, LBRACKET_T, RBRACKET_T, COMMA_T, COLON_T, EOS_T, SEOF_T,
 * template literal markers (TEMPLATE_START_T, TEMPLATE_END_T),
 * assignment and comparison operators (ASGN_T, EQ_T, STRICT_EQ_T),
 * arithmetic and compound assignment operators (+, +=, -, -=, *, *=, %, /),
 * logical and bitwise operators (&, |, ^, ~, &&, ||),
 * and other special characters such as ` (backtick), & (ampersand), and ' (single quote).
 */

#define ESNR	21		/* Error state with no retract */
#define ESWR	22		/* Error state with retract */
#define FS		26		/* Illegal state */

#define NUM_STATES		24 + 2
#define CHAR_CLASSES	11

static integer transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*         SL      \n     S       U       L       EQ      P       D       Q       SQ      O    */
	/*  0 */ {  1,   ESNR,  ESNR,   8,      8,      ESNR,   ESNR,   12,     14,     18,    ESNR },
	/*  1 */ {  2,   ESNR,  4,      ESNR,   ESNR,   16,     ESNR,   20,   ESNR,   ESNR,  ESNR },
	/*  2 */ {  2,   3,     2,      2,      2,      2,      2,      2,      2,      2,     2    },
	/*  3 */ {  FS, FS, FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/*  4 */ {  4,    4,     5,      4,      4,      4,      4,      4,      4,      4,     4    },
	/*  5 */ {  6,    23,     23,     23,     23,     23,     23,     23,     23,      23,     23    },
	/*  6 */ {  ESNR, 7,     ESNR,   ESNR,   ESNR,   ESNR,   ESNR,   ESNR,   ESNR,   ESNR,  ESNR },
	/*  7 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/*  8 */ {  11,   11,    11,     8,      8,      9,      10,     8,      11,     11,    11   },
	/*  9 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/* 10 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/* 11 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/* 12 */ {  13,   13,    13,     13,     13,     13,     13,     12,     13,     13,    13   },
	/* 13 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/* 14 */ {  14,   14,    14,     14,     14,     14,     14,     14,     15,     14,    14   },
	/* 15 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/* 16 */ {  ESNR, ESNR,  ESNR,   ESNR,   ESNR,   ESNR,   ESNR,   17,     ESNR,   ESNR,  ESNR },
	/* 17 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/* 18 */ {  18,   18,    18,     18,     18,     18,     18,     18,     18,     19,    18   },
	/* 19 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/* 20 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/* 21 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },  // ERROR STATE NO RETRACT (ESNR)
	/* 22 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },  // ERROR STATE WITH RETRACT (ESWR)
	/* 23 */ {  23, 23,  24,   23,   23,   23,   23,   23,   23,   23,  23 },  
	/* 24 */ {  25, ESNR,  ESNR,   ESNR,   ESNR,   ESNR,   ESNR,   ESNR,   ESNR,   ESNR,  ESNR },
	/* 25 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* List of acceptable states */
static integer stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	NOFS, /* 02 */
	FSNR, /* 03 SLC */
	NOFS, /* 04 */
	NOFS, /* 05 */
	NOFS, /* 06 */
	FSNR, /* 07 MLC */
	NOFS, /* 08 */
	FSWR, /* 09 Variables */
	FSWR, /* 10 Methods */
	FSWR, /* 11 Keywords */
	NOFS, /* 12 */
	FSWR, /* 13 Integer literal */
	NOFS, /* 14 */
	FSNR, /* 15 String literal */
	NOFS, /* 16 */
	FSWR, /* 17 /= OPERATOR */
	NOFS, /* 18 */
	FSNR, /* 19 Character literal*/
	FSWR, /* 20 DIV OPERATOR */
	FSNR, /* 21 (Err1 - no retract) */
	FSWR, /* 22 (Err2 - retract) */
	NOFS, /* 23 */
	NOFS, /* 24 */
	FSNR  /* 25 Document literals */
};

/* Static (local) function  prototypes */
integer			startScanner(BufferPointer psc_buf);
static integer	nextClass(character c);					/* character class function */
static integer	nextState(integer, character);		/* state machine function */
void			printScannerData(ScannerData scData);
Token			tokenizer(simpleJS_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

typedef Token(*PTR_ACCFUN)(string lexeme);

/* Declare accepting states functions */
Token funcSL	(string lexeme);
Token funcIL	(string lexeme);
Token funcID	(string lexeme);
Token funcCMT   (string lexeme);
Token funcKEY	(string lexeme);
Token funcErr	(string lexeme);
Token funcDIVOP	(string lexeme);
Token funcCHAR	(string lexeme);
Token funcDOC	(string lexeme);	

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	NULL,		/* -    [02] */
	funcCMT,	/* -    [03] */
	NULL,		/* -    [04] */
	NULL,		/* -    [05] */
	NULL,		/* -    [06] */
	funcCMT,	/* -    [07] */
	NULL,		/* -    [08] */
	funcID,		/* -    [09] */
	funcID,		/* -    [10] */
	funcKEY,	/* -    [11] */
	NULL,		/* -    [12] */
	funcIL,		/* -    [13] */
	NULL,		/* -    [14] */
	funcSL,		/* -    [15] */
	NULL,		/* -    [16] */
	funcDIVOP,	/* -    [17] */
	NULL,		/* -    [18] */
	funcCHAR,	/* -    [19] */
	funcDIVOP,	/* -    [20] */
	funcErr,	/* -    [21] */
	funcErr,	/* -    [22] */
	NULL,		/* -    [23] */
	NULL,		/* -    [24] */
	funcDOC		/* -    [25] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

#define KWT_SIZE 11 + 1

static string keywordTable[KWT_SIZE] = {
	"var",		/* KW00 */
	"if",		/* KW01 */
	"else",		/* KW02 */
	"switch",	/* KW03 */
	"do",		/* KW04 */
	"while",	/* KW05 */
	"for",		/* KW06 */
	"break",	/* KW07 */
	"continue",	/* KW08 */
	"function",	/* KW09 */
	"return",	/* KW10 */
	"char"		/* KW11 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	character indentationCharType;
	integer indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
integer numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
