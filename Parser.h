/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Vi Tuan Ha, Corey Lambert
* Professors: Paulo Sousa
************************************************************
#
# "=---------------------------------------="
# "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# "=---------------------------------------="
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "    @@                             @@    ”
# "    @@           %&@@@@@@@@@@@     @@    ”
# "    @@       @%% (@@@@@@@@@  @     @@    ”
# "    @@      @& @   @ @       @     @@    ”
# "    @@     @ @ %  / /   @@@@@@     @@    ”
# "    @@      & @ @  @@              @@    ”
# "    @@       @/ @*@ @ @   @        @@    ”
# "    @@           @@@@  @@ @ @      @@    ”
# "    @@            /@@    @@@ @     @@    ”
# "    @@     @      / /     @@ @     @@    ”
# "    @@     @ @@   /@/   @@@ @      @@    ”
# "    @@     @@@@@@@@@@@@@@@         @@    ”
# "    @@                             @@    ”
# "    @@           S J S             @@    ”
# "    @@                             @@    ”
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "                                         "
# "[READER SCRIPT .........................]"
# "                                         "
*/


/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
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
extern BufferPointer	stringLiteralBuffer;
extern integer			line;
extern Token			tokenizer(void);
extern string			keywordTable[KWT_SIZE];
static integer			syntaxErrorNumber = 0;

#define LANG_WRTE		"log("
#define LANG_READ		"prompt("
#define LANG_MAIN		"main("

#define STR_LANGNAME	"SimpleJS"

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_var,       /* KW00 */
	KW_if,        /* KW01 */
	KW_else,      /* KW02 */
	KW_switch,    /* KW03 */
	KW_case,      /* KW04 */
	KW_do,        /* KW05 */
	KW_while,     /* KW06 */
	KW_for,       /* KW07 */
	KW_break,     /* KW08 */
	KW_continue,  /* KW09 */
	KW_function,  /* KW10 */
	KW_return,    /* KW11 */
	KW_true,      /* KW12 */
	KW_false,     /* KW13 */
};

#define NUM_BNF_RULES 16

/* Parser */
typedef struct parserData {
	integer parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
integer numParserErrors;

/* Scanner data */
ParserData psData;

/* Function definitions */
void startParser();
void matchToken(integer, integer);
void syncErrorHandler(integer);
void printError();
void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_program,									/*  1 */

	BNF_comment,									/*  2 */

	BNF_codeSession,								/*  3 */
	BNF_optionalStatements,							/*  4 */
	BNF_statements,									/*  5 */
	BNF_statementsPrime,							/*  6 */
	BNF_statement,									/*  7 */

	BNF_assignmentStatement,						/*  8 */
	BNF_optVarDeclaration,							/*  9 */

	BNF_conditionStatement,							/* 10 */
	BNF_iterationStatement,							/* 11 */
	BNF_returnStatement,							/* 12 */

	BNF_inputStatement,								/* 13 */

	BNF_outputStatement,							/* 14 */
	BNF_outputVariableValue,						/* 15 */
};

static string BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",                /*  0 */
	"BNF_program",              /*  1 */
	"BNF_comment",              /*  2 */

	"BNF_codeSession",          /*  3 */
	"BNF_optionalStatements",   /*  4 */
	"BNF_statements",           /*  5 */
	"BNF_statementsPrime",      /*  6 */
	"BNF_statement",            /*  7 */

	"BNF_assignmentStatement",  /*  8 */
	"BNF_optVarDeclaration",	/*  9 */

	"BNF_conditionStatement",   /* 10 */
	"BNF_iterationStatement",   /* 11 */
	"BNF_returnStatement",      /* 12 */

	"BNF_inputStatement",       /* 13 */
	"BNF_outputStatement",      /* 14 */
	"BNF_outputVariableValue"   /* 15 */
};

void program();                 /*  1 */
void comment();                 /*  2 */

void codeSession();             /*  3 */

void optionalStatements();      /*  4 */
void statements();              /*  5 */
void statementsPrime();         /*  6 */
void statement();               /*  7 */

void assignmentStatement();     /*  8 */
void optVarDeclaration();       /*	9 */

void conditionStatement();      /* 10 */
void iterationStatement();      /* 11 */
void returnStatement();         /* 12 */

void inputStatement();          /* 13 */
void outputStatement();         /* 14 */
void outputVariableValue();     /* 15 */

#endif
