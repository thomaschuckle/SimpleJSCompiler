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

#define NUM_TOKENS 46

enum TOKENS {
	/* Basic Tokens */
	ERR_T,           /*  0: Error token */
	MNID_T,          /*  1: NOT USED (Method name identifier token (start: &)) */
	INL_T,           /*  2: Integer literal token */
	STR_T,           /*  3: String literal token */
	LPR_T,           /*  4: Left parenthesis token */
	RPR_T,           /*  5: Right parenthesis token */
	LBR_T,           /*  6: Left brace token */
	RBR_T,           /*  7: Right brace token */

	KW_T,            /*  8: Keyword token */

	EOS_T,           /*  9: End of statement (semicolon) */
	RTE_T,           /* 10: Run-time error token */
	SEOF_T,          /* 11: Source end-of-file token */
	CMT_T,           /* 12: Comment token */

	/* Arithmetic Operators */
	ASNG_T,          /* 13: Assignment operator token (=) */
	ADD_T,           /* 14: Addition operator token (+) */
	SUB_T,           /* 15: Subtraction operator token (-) */
	MUL_T,           /* 16: Multiplication operator token (*) */
	DIV_T,           /* 17: Division operator token (/) */
	MOD_T,           /* 18: Modulo operator token (%) */

	/* Comparison Operators */
	EQ_T,            /* 19: Equality operator token (==) */
	NE_T,            /* 20: Not equal operator token (!=) */
	GT_T,            /* 21: Greater than operator token (>) */
	LT_T,            /* 22: Less than operator token (<) */
	STRICT_EQ_T,     /* 23: Strict equality operator token (===) */
	STRICT_NE_T,     /* 24: Strict not equal operator token (!==) */

	/* Logical Operators */
	AND_T,           /* 25: Logical AND operator token (&&) */
	OR_T,            /* 26: Logical OR operator token (||) */
	NOT_T,           /* 27: Logical NOT operator token (!) */

	/* Compound Assignment Operators */
	ADD_ASNG_T,      /* 28: Addition assignment operator token (+=) */
	SUB_ASNG_T,      /* 29: Subtraction assignment operator token (-=) */
	MUL_ASNG_T,      /* 30: Multiplication assignment operator token (*=) */
	DIV_ASNG_T,      /* 31: Division assignment operator token (/=) */

	/* Bitwise Operators */
	BIT_AND_T,       /* 32: Bitwise AND operator token (&) */
	BIT_OR_T,        /* 33: Bitwise OR operator token (|) */
	BIT_XOR_T,       /* 34: Bitwise XOR operator token (^) */
	BIT_NOT_T,       /* 35: Bitwise NOT operator token (~) */
	LSHIFT_T,        /* 36: Left shift operator token (<<) */
	RSHIFT_T,        /* 37: Right shift operator token (>>) */
	URSHIFT_T,       /* 38: Unsigned right shift operator token (>>>) */

	/* Additional Delimiters */
	COMMA_T,         /* 39: Comma delimiter token (,) */
	COLON_T,         /* 40: Colon delimiter token (:) */
	DOT_T,           /* 41: Dot operator token (.) */

	/* Brackets */
	LBRACKET_T,      /* 42: Left square bracket token ([) */
	RBRACKET_T,      /* 43: Right square bracket token (]) */

	/* Template Literal Tokens */
	TEMPLATE_START_T,/* 44: Template literal start token (`) */
	TEMPLATE_END_T   /* 45: Template literal end token (`) */
};

static string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",           /*  0 */
	"MNID_T",          /*  1 */			// NOT USED
	"INL_T",           /*  2 */
	"STR_T",           /*  3 */
	"LPR_T",           /*  4 */
	"RPR_T",           /*  5 */
	"LBR_T",           /*  6 */
	"RBR_T",           /*  7 */
	"KW_T",            /*  8 */
	"EOS_T",           /*  9 */
	"RTE_T",           /* 10 */
	"SEOF_T",          /* 11 */
	"CMT_T",           /* 12 */

	/* Arithmetic Operators */
	"ASNG_T",          /* 13 */
	"ADD_T",           /* 14 */
	"SUB_T",           /* 15 */
	"MUL_T",           /* 16 */
	"DIV_T",           /* 17 */
	"MOD_T",           /* 18 */

	/* Comparison Operators */
	"EQ_T",            /* 19 */
	"NE_T",            /* 20 */
	"GT_T",            /* 21 */
	"LT_T",            /* 22 */
	"STRICT_EQ_T",     /* 23 */
	"STRICT_NE_T",     /* 24 */

	/* Logical Operators */
	"AND_T",           /* 25 */
	"OR_T",            /* 26 */
	"NOT_T",           /* 27 */

	/* Compound Assignment Operators */
	"ADD_ASNG_T",      /* 28 */
	"SUB_ASNG_T",      /* 29 */
	"MUL_ASNG_T",      /* 30 */
	"DIV_ASNG_T",      /* 31 */

	/* Bitwise Operators */
	"BIT_AND_T",       /* 32 */
	"BIT_OR_T",        /* 33 */
	"BIT_XOR_T",       /* 34 */
	"BIT_NOT_T",       /* 35 */
	"LSHIFT_T",        /* 36 */
	"RSHIFT_T",        /* 37 */
	"URSHIFT_T",       /* 38 */

	/* Additional Delimiters */
	"COMMA_T",         /* 39 */
	"COLON_T",         /* 40 */
	"DOT_T",           /* 41 */

	/* Brackets */
	"LBRACKET_T",      /* 42 */
	"RBRACKET_T",      /* 43 */

	/* Template Literal Tokens */
	"TEMPLATE_START_T",/* 44 */
	"TEMPLATE_END_T"   /* 45 */
};

typedef enum ArithmeticOperators {
	OP_ASNG,        /* Assignment operator (=) */
	OP_ADD,         /* Addition operator (+) */
	OP_SUB,         /* Subtraction operator (-) */
	OP_MUL,         /* Multiplication operator (*) */
	OP_DIV,         /* Division operator (/) */
	OP_MOD,         /* Modulo operator (%) */
	OP_ADD_ASNG,    /* Addition assignment operator (+=) */
	OP_SUB_ASNG,    /* Subtraction assignment operator (-=) */
	OP_MUL_ASNG,    /* Multiplication assignment operator (*=) */
	OP_DIV_ASNG,    /* Division assignment operator (/=) */
	OP_MOD_ASNG     /* Modulo assignment operator (%=) */
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
	integer contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	float floatValue;					/* floating-point literal attribute (value) */
	character idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	character errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

typedef struct idAttibutes {
	byte flags;						/* Flags information */
	union {
		integer intValue;			/* Integer value */
		float floatValue;			/* Float value */
		string stringContent;		/* String value */
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
 * assignment and comparison operators (ASNG_T, EQ_T, STRICT_EQ_T),
 * arithmetic and compound assignment operators (+, +=, -, -=, *, *=, %, /),
 * logical and bitwise operators (&, |, ^, ~, &&, ||),
 * and other special characters such as ` (backtick), & (ampersand), and ' (single quote).
 */

#define ESNR	21		/* Error state with no retract */
#define ESWR	22		/* Error state with retract */
#define FS		23		/* Illegal state */

#define NUM_STATES		21 + 2
#define CHAR_CLASSES	11

static int transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*         SL      \n     S       U       L       EQ      P       D       Q       SQ      O    */
	/*  0 */ {  1,   ESNR,  ESNR,   8,      8,      ESNR,   ESNR,   12,     14,     18,    ESNR },
	/*  1 */ {  2,   ESNR,  4,      ESNR,   ESNR,   16,     ESNR,   ESNR,   ESNR,   ESNR,  ESNR },
	/*  2 */ {  2,   3,     2,      2,      2,      2,      2,      2,      2,      2,     2    },
	/*  3 */ {  FS, FS, FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS },
	/*  4 */ {  4,    4,     5,      4,      4,      4,      4,      4,      4,      4,     4    },
	/*  5 */ {  6,    5,     5,      5,      5,      5,      5,      5,      5,      5,     5    },
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
	/* 22 */ {  FS, FS,  FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,  FS }   // ERROR STATE WITH RETRACT (ESWR)
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
	FSNR, /* 19 Sting literal*/
	FSWR, /* 20 DIV OPERATOR */
	FSNR, /* 21 (Err1 - no retract) */
	FSWR  /* 22 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

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
	funcSL,		/* -    [19] */
	funcDIVOP,	/* -    [20] */
	funcErr,	/* -    [21] */
	funcErr		/* -    [22] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

#define KWT_SIZE 11

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
	"return"	/* KW10 */
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
