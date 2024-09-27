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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SimpleJS) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX		-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SimpleJS) .................................. */

/* Offset declaration */
typedef struct flag {
	boolean isEmpty;			/* indicates if the buffer is empty */
	boolean isFull;			/* indicates if the buffer is full */
	boolean isRead;			/* indicates if the buffer was completely read */
	boolean isMoved;			/* indicates if the buffer memory was changed */
} Flag;

/* Offset declaration */
typedef struct position {
	integer wrte;			/* the offset to the add chars (in chars) */
	integer read;			/* the offset to the get a char position (in chars) */
	integer mark;			/* the offset to the mark position (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	string		content;			/* pointer to the beginning of character array (character buffer) */
	integer		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	integer		increment;			/* character array increment factor */
	character	mode;				/* operational mode indicator */
	Flag		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position	positions;			/* Offset / position field */
	integer		histogram[NCHAR];	/* Statistics of chars */
	integer		numReaderErrors;	/* Number of errors from Reader */
	byte		checksum;
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(integer, integer, character);
BufferPointer	readerAddChar		(BufferPointer const, character);
boolean		readerClear		    (BufferPointer const);
boolean		readerFree		    (BufferPointer const);
boolean		readerIsFull		(BufferPointer const);
boolean		readerIsEmpty		(BufferPointer const);
boolean		readerSetMark		(BufferPointer const, integer);
integer		readerPrint		    (BufferPointer const);
integer		readerLoad			(BufferPointer const, FILE* const);
boolean		readerRecover		(BufferPointer const);
boolean		readerRetract		(BufferPointer const);
boolean		readerRestore		(BufferPointer const);
void		readerCalcChecksum	(BufferPointer const);
boolean		readerPrintFlags	(BufferPointer const);
void		readerPrintStat     (BufferPointer const);
/* Getters */
character	readerGetChar		(BufferPointer const);
string		readerGetContent	(BufferPointer const, integer);
integer		readerGetPosRead	(BufferPointer const);
integer		readerGetPosWrte	(BufferPointer const);
integer		readerGetPosMark	(BufferPointer const);
integer		readerGetSize		(BufferPointer const);
integer		readerGetInc		(BufferPointer const);
character	readerGetMode		(BufferPointer const);
integer		readerGetNumErrors	(BufferPointer const);

#endif
