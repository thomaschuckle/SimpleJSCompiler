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
# ECHO "    @@           S J S             @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
integer line;								/* Current line number of the source code */
extern integer errorNumber;					/* Defined in platy_st.c - run-time error number */

extern integer stateType[NUM_STATES];
extern string keywordTable[KWT_SIZE];

extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern integer transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */

integer startScanner(BufferPointer psc_buf) {
	for (integer i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {

	Token currentToken = { 0 };				/* token to return after pattern recognition. Set all structure members to 0 */
	character c;							/* input symbol */
	integer state = 0;						/* initial state of the FSM */
	integer lexStart;						/* start offset of a lexeme in the input char buffer (array) */
	integer lexEnd;							/* end offset of a lexeme in the input char buffer (array)*/

	integer lexLength;						/* token length */
	integer i;								/* counter */
	//character newc;						// new char

	static boolean isTemplateStart = TRUE;  // Initialize it to TRUE for the first backtick (`) seen

	/* Starting lexeme */
	string lexeme;							/* lexeme (to check the function) */
	lexeme = (string)malloc(VID_LEN * sizeof(character));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	while (1) {								/* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		if (c < 0 || c >= NCHAR) {
			/* Set an error token indicating invalid character */
			currentToken.code = ERR_T;						// Set to your defined error token type
			currentToken.attribute.errLexeme[0] = c;		// Store the problematic character (if useful)
			currentToken.attribute.errLexeme[1] = EOS_CHR;  // Ensure the string is null-terminated
			return currentToken;
		}

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		switch (c) {
		
		/* White Space and Newline */
		case SPC_CHR:
		case TAB_CHR:
			/* Ignore and continue to the next character */
			break;

		case NWL_CHR:
			line++;
			break;

		/* Basic Symbols */
		case SCL_CHR:	// ;
			currentToken.code = EOS_T;
			break;
		case LPR_CHR:  // (
			currentToken.code = LPR_T;
			break;
		case RPR_CHR:  // )
			currentToken.code = RPR_T;
			break;
		case LBR_CHR:  // {
			currentToken.code = LBR_T;
			break;
		case RBR_CHR:  // }
			currentToken.code = RBR_T;
			break;

		/* Assignment and Comparison Operators */
		case EQ_CHR:  // =
			c = readerGetChar(sourceBuffer);
			if (c == EQ_CHR) {              // ==
				currentToken.code = EQ_T;
				c = readerGetChar(sourceBuffer);
				if (c == EQ_CHR)            // ===
					currentToken.code = STRICT_EQ_T;
				else
					readerRetract(sourceBuffer);   // retract for ==
			}
			else {
				currentToken.code = ASNG_T;  // =
				readerRetract(sourceBuffer);
			}
			break;

		/* Arithmetic Operators */
		case ADD_CHR:  // +
			c = readerGetChar(sourceBuffer);
			if (c == EQ_CHR)
				currentToken.code = ADD_ASNG_T;  // +=
			else {
				currentToken.code = ADD_T;       // +
				readereRetract(sourceBuffer);
			}
			break;

		case MIN_CHR:  // -
			c = readerGetChar(sourceBuffer);
			if (c == EQ_CHR)
				currentToken.code = SUB_ASNG_T;  // -=
			else {
				currentToken.code = SUB_T;       // -
				readerRetract(sourceBuffer);
			}
			break;

		case MUL_CHR:  // *
			c = readerGetChar(sourceBuffer);
			if (c == EQ_CHR)
				currentToken.code = MUL_ASNG_T;  // *=
			else {
				currentToken.code = MUL_T;       // *
				readerRetract(sourceBuffer);
			}
			break;

		/* Modulo Operator */
		case PERC_CHR:  // %
			currentToken.code = MOD_T;
			break;

		/* Comparison Operators */
		case LT_CHR:  // <
			c = readerGetChar(sourceBuffer);
			if (c == LT_CHR)
				currentToken.code = LSHIFT_T;    // <<
			else {
				currentToken.code = LT_T;        // <
				readerRetract(sourceBuffer);
			}
			break;

		case GT_CHR:  // >
			c = readerGetChar(sourceBuffer);
			if (c == GT_CHR) {
				c = readerGetChar(sourceBuffer);
				if (c == GT_CHR)
					currentToken.code = URSHIFT_T; // >>>
				else {
					currentToken.code = RSHIFT_T;  // >>
					readerRetract(sourceBuffer);
				}
			}
			else {
				currentToken.code = GT_T;         // >
				readerRetract(sourceBuffer);
			}
			break;

		/* Logical and Bitwise Operators */
		case AMP_CHR:  // &
			c = readerGetChar(sourceBuffer);
			if (c == AMP_CHR)
				currentToken.code = AND_T;       // &&
			else {
				currentToken.code = BIT_AND_T;   // &
				readerRetract(sourceBuffer);
			}
			break;

		case PIPE_CHR:  // |
			c = readerGetChar(sourceBuffer);
			if (c == PIPE_CHR)
				currentToken.code = OR_T;        // ||
			else {
				currentToken.code = BIT_OR_T;    // |
				readerRetract(sourceBuffer);
			}
			break;

		/* Bitwise Operators */
		case CARET_CHR:  // ^
			currentToken.code = BIT_XOR_T;
			break;
		case TILDE_CHR:  // ~
			currentToken.code = BIT_NOT_T;
			break;

		/* Additional Delimiters */
		case COM_CHR:  // ,
			currentToken.code = COMMA_T;
			break;
		case COL_CHR:  // :
			currentToken.code = COLON_T;
			break;
		case DOT_CHR:  // .
			currentToken.code = DOT_T;
			break;

		/* Brackets */
		case LBRK_CHR:  // [
			currentToken.code = LBRACKET_T;
			break;
		case RBRK_CHR:  // ]
			currentToken.code = RBRACKET_T;
			break;

		/* Template Literal Tokens */
		case BCKT_CHR:  // `
			if (isTemplateStart)
				currentToken.code = TEMPLATE_START_T;
			else
				currentToken.code = TEMPLATE_END_T;
			isTemplateStart = !isTemplateStart;  // toggle for template literals
			break;

		/* End of File or String */
		case EOS_CHR:  // \0
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case EOF_CHR:  // 0xFF
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/*  Adjust / check the logic for your language */

		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((integer)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);
			// Defensive programming
			if (!lexeme) {
				readerFree(lexemeBuffer);
				return currentToken;
			}
			currentToken = (*finalStateTable[state])(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */

integer nextState(integer state, character c) {
	integer col;
	integer next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */

 /*   /,  \n,   *,    _,   L,   =,    (,    D,    ",   ',    O    */
 /*    0,   1,   2,    3,   4,   5,    6,    7,    8,    9,   10 */
integer nextClass(character c) {
	integer val = ESNR;  // Default to an error/invalid class

	switch (c) {
	case SLH_CHR:  // "/"
		val = 0;  // Slash for comments
		break;
	case NWL_CHR:  // "\n"
		val = 1;  // Newline character
		break;
	case MUL_CHR:  // "*"
		val = 2;  // Asterisk for block comments
		break;
	case UND_CHR:  // "_"
		val = 3;  // Underscore (used for identifiers)
		break;
	case EQ_CHR:  // "="
		val = 5;  // Equals sign (assignment operator)
		break;
	case LPR_CHR:  // "("
		val = 6;  // Left Parenthesis (used for function calls or expressions)
		break;
	case DBL_QT_CHR:  // "\""
		val = 8;  // Double quote (start of string literal)
		break;
	case SGL_QT_CHR:  // "'"
		val = 9;  // Single quote (start of character literal)
		break;
	default:
		if (isalpha(c))  // Letter (A-Z or a-z)
			val = 4;  // Alphabet (used in identifiers or keywords)
		else if (isdigit(c))  // Digit (0-9)
			val = 7;  // Digit (used for integer literals)
		else
			val = 10;  // Other characters (operators, punctuation, etc.)
		break;
	}

	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */

Token funcCMT(string lexeme) {
	Token currentToken = { 0 };
	integer i = 0, len = (integer)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  */
  /* TO_DO: Adjust the function for IL */

Token funcIL(string lexeme) {
	Token currentToken = { 0 };
	long tlong;
	if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (integer)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(string lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	character lastch = lexeme[length - 1];
	integer isID = FALSE;
	switch (lastch) {
		case AMP_CHR:
			currentToken.code = MNID_T;
			scData.scanHistogram[currentToken.code]++;
			isID = TRUE;
			break;
		default:
			// Test Keyword
			///lexeme[length - 1] = EOS_CHR;
			currentToken = funcKEY(lexeme);
			break;
	}
	if (isID == TRUE) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for SL */

Token funcSL(string lexeme) {
	Token currentToken = { 0 };
	integer i = 0, len = (integer)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(string lexeme) {
	Token currentToken = { 0 };
	integer kwindex = -1, j = 0;
	integer len = (integer)strlen(lexeme);
	///lexeme[len - 1] = EOS_CHR;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(string lexeme) {
	Token currentToken = { 0 };
	integer i = 0, len = (integer)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

void printToken(Token t) {
	extern string keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (integer)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (integer)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
