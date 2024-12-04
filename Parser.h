/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# "=---------------------------------------="
# "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# "=---------------------------------------="
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# "    @@                             @@    �
# "    @@           %&@@@@@@@@@@@     @@    �
# "    @@       @%% (@@@@@@@@@  @     @@    �
# "    @@      @& @   @ @       @     @@    �
# "    @@     @ @ %  / /   @@@@@@     @@    �
# "    @@      & @ @  @@              @@    �
# "    @@       @/ @*@ @ @   @        @@    �
# "    @@           @@@@  @@ @ @      @@    �
# "    @@            /@@    @@@ @     @@    �
# "    @@     @      / /     @@ @     @@    �
# "    @@     @ @@   /@/   @@@ @      @@    �
# "    @@     @@@@@@@@@@@@@@@         @@    �
# "    @@                             @@    �
# "    @@         S O F I A           @@    �
# "    @@                             @@    �
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# "                                         "
# "[READER SCRIPT .........................]"
# "                                         "
*/


/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern sofia_intg		line;
extern Token			tokenizer(sofia_void);
extern sofia_string		keywordTable[KWT_SIZE];
static sofia_intg		syntaxErrorNumber = 0;

#define LANG_WRTE		"print&"
#define LANG_READ		"input&"
#define LANG_MAIN		"main&"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_data,
	KW_code,
	KW_int,
	KW_real,
	KW_string,
	KW_if,
	KW_then,
	KW_else,
	KW_while,
	KW_do
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 12

/* Parser */
typedef struct parserData {
	sofia_intg parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
sofia_intg numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
sofia_void startParser();
sofia_void matchToken(sofia_intg, sofia_intg);
sofia_void syncErrorHandler(sofia_intg);
sofia_void printError();
sofia_void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_codeSession,								/*  1 */
	BNF_comment,									/*  2 */
	BNF_dataSession,								/*  3 */
	BNF_optVarListDeclarations,						/*  4 */
	BNF_optionalStatements,							/*  5 */
	BNF_outputStatement,							/*  6 */
	BNF_outputVariableList,							/*  7 */
	BNF_program,									/*  8 */
	BNF_statement,									/*  9 */
	BNF_statements,									/* 10 */
	BNF_statementsPrime								/* 11 */
};


/* TO_DO: Define the list of keywords */
static sofia_string BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_codeSession",
	"BNF_comment",
	"BNF_dataSession",
	"BNF_optVarListDeclarations",
	"BNF_optionalStatements",
	"BNF_outputStatement",
	"BNF_outputVariableList",
	"BNF_program",
	"BNF_statement",
	"BNF_statements",
	"BNF_statementsPrime"
};

/* TO_DO: Place ALL non-terminal function declarations */
sofia_void codeSession();
sofia_void comment();
sofia_void dataSession();
sofia_void optVarListDeclarations();
sofia_void optionalStatements();
sofia_void outputStatement();
sofia_void outputVariableList();
sofia_void program();
sofia_void statement();
sofia_void statements();
sofia_void statementsPrime();

#endif