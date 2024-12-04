/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Vi Tuan Ha, Corey Lamber
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
# "    @@            S J S            @@    ”
# "    @@                             @@    ”
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "                                         "
# "[READER SCRIPT .........................]"
# "                                         "
*/


/*
************************************************************
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
extern ParserData psData; /* BNF statistics */

int return_flag = 0;

/*
************************************************************
 * Process Parser
 ***********************************************************
 */

void startParser() {
	integer i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}

/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
void matchToken(integer tokenCode, integer tokenAttribute) {
	integer matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		printf("Here is my current: %d\n", lookahead.code);
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printf("Here is my current: %d\n", lookahead.code);
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
void syncErrorHandler(integer syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
void printError() {
	extern numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralBuffer, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
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
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, MNID_T (main&), SEOF_T}.
 ***********************************************************
 */
void program() {
	/* Update program statistics */
	psData.parsHistogram[BNF_program]++;
	/* Program code */

	while (lookahead.code != SEOF_T) {
		switch (lookahead.code) {
		case CMT_T:
			comment();
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR);
			optionalStatements();
			matchToken(EOS_T, NO_ATTR);
			break;
		case INL_T:
			matchToken(INL_T, NO_ATTR);
			optionalStatements();
			matchToken(EOS_T, NO_ATTR);
			break;
		case ADD_T:
			matchToken(ADD_T, NO_ATTR);
			optionalStatements();
			matchToken(EOS_T, NO_ATTR);
			break;
		case SUB_T:
			matchToken(SUB_T, NO_ATTR);
			optionalStatements();
			matchToken(EOS_T, NO_ATTR);
			break;
		case EOS_T:
			matchToken(EOS_T, NO_ATTR);
			optionalStatements();
			break;
		case KW_T:
			if (strcmp(keywordTable[lookahead.attribute.codeType], "var") == 0) {
				matchToken(KW_T, KW_var);
				matchToken(VID_T, NO_ATTR);
				assignmentStatement();
				matchToken(EOS_T, NO_ATTR);
				break;
			}
			else if (strcmp(keywordTable[lookahead.attribute.codeType], "function") == 0) {
				matchToken(KW_T, KW_function);
				matchToken(MNID_T, NO_ATTR);
				matchToken(RPR_T, NO_ATTR);
				matchToken(LBR_T, NO_ATTR);
				codeSession();
				matchToken(RBR_T, NO_ATTR);
				break;
			}
			else if (strcmp(keywordTable[lookahead.attribute.codeType], "if") == 0) {
				matchToken(KW_T, KW_if);
				matchToken(LPR_T, NO_ATTR);
				conditionStatement();
				matchToken(RPR_T, NO_ATTR);
				matchToken(LBR_T, NO_ATTR);
				codeSession();
				matchToken(RBR_T, NO_ATTR);
				break;
			}
			else if (strcmp(keywordTable[lookahead.attribute.codeType], "while") == 0) {
				matchToken(KW_T, KW_while);
				matchToken(LPR_T, NO_ATTR);
				conditionStatement();
				matchToken(RPR_T, NO_ATTR);
				matchToken(LBR_T, NO_ATTR);
				codeSession();
				matchToken(RBR_T, NO_ATTR);
				break;
			}
			else if (strcmp(keywordTable[lookahead.attribute.codeType], "return") == 0) {
				matchToken(KW_T, KW_return);
				returnStatement();
				matchToken(EOS_T, NO_ATTR);
				break;
			}
			else if (strcmp(keywordTable[lookahead.attribute.codeType], "true") == 0) {
				matchToken(KW_T, KW_true);
				optionalStatements();
				matchToken(EOS_T, NO_ATTR);
				break;
			}
			else if (strcmp(keywordTable[lookahead.attribute.codeType], "false") == 0) {
				matchToken(KW_T, KW_false);
				optionalStatements();
				matchToken(EOS_T, NO_ATTR);
				break;
			}
			else {
				printError();
				break; 
			}
		case SEOF_T:
			; // Empty
			break;
		default:
			lookahead = tokenizer();
			break;
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

/*
 ************************************************************
 * assignmentStatement
 * BNF: <assignment statement>  -> <assignment expression>
 *		<assignment expression> ->  <var_declaration> = <arithmetic expression>
						| <var_declaration> = <string expression>
 						| <var_declaration>= <boolean expression>
 ***********************************************************
 */
void assignmentStatement() {
	psData.parsHistogram[BNF_assignmentStatement]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
	default:
		optVarDeclaration();
		printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
	}
}

/*
 ************************************************************
 * Optional Var Declaration
 * BNF: <var_declaration> -> var <integer_value> |
					  var <string_value> |
					  var <boolean_value> | ϵ
 ***********************************************************
 */
void optVarDeclaration() {
	psData.parsHistogram[BNF_optVarDeclaration]++;
	lookahead = tokenizer();
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable Declaration parsed");
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (KW_code)}.
 ***********************************************************
 */
void codeSession() {
	psData.parsHistogram[BNF_codeSession]++;

	while (!return_flag) {
		switch (lookahead.code) {
		case CMT_T:
			comment();
		default:
			optionalStatements();
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
void optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;
	
	switch (lookahead.code) {
	case CMT_T:
		comment();
	case STR_T:
		statement();
		break;
	case INL_T:
		statement();
		break;
	case ADD_T:
		statement();
		break;
	case SUB_T:
		statement();
		break;
	case EOS_T:
		statement();
		break;
	default:
		statements();
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	//statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
//void statementsPrime() {
//	psData.parsHistogram[BNF_statementsPrime]++;
//	switch (lookahead.code) {
//	case MNID_T:
//		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0 || 
//			strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
//			statements();
//			break;
//		}
//	case KW_T:
//		if (strcmp(keywordTable[lookahead.attribute.codeType], "var") == 0) {
//			statements();
//			break;
//		}
//		else if (strcmp(keywordTable[lookahead.attribute.codeType], "function") == 0) {
//			statements();
//			break;
//		}
//		else if (strcmp(keywordTable[lookahead.attribute.codeType], "if") == 0) {
//			statements();
//			break;
//		}
//		else if (strcmp(keywordTable[lookahead.attribute.codeType], "while") == 0) {
//			statements();
//			break;
//		}
//		else if (strcmp(keywordTable[lookahead.attribute.codeType], "return") == 0) {
//			statements();
//			break;
//		}
//		else if (strcmp(keywordTable[lookahead.attribute.codeType], "true") == 0) {
//			statements();
//			break;
//		}
//		else if (strcmp(keywordTable[lookahead.attribute.codeType], "false") == 0) {
//			statements();
//			break;
//		}
//		else {
//			printError();
//		}
//		break;
//	default:
//		; // epsilon
//	}
//}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statement() {
	psData.parsHistogram[BNF_statement]++;
	switch (lookahead.code) {
	case STR_T:
	case INL_T:
	case ADD_T:
	case SUB_T:
	case EOS_T:
		break;
	case KW_T:
		if (strcmp(keywordTable[lookahead.attribute.codeType], "var") == 0) {
			matchToken(KW_T, KW_var);
			matchToken(VID_T, NO_ATTR);
			assignmentStatement();
			matchToken(EOS_T, NO_ATTR);
			break;
		}
		else if (strcmp(keywordTable[lookahead.attribute.codeType], "function") == 0) {
			matchToken(KW_T, KW_function);
			matchToken(MNID_T, NO_ATTR);
			matchToken(RPR_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else if (strcmp(keywordTable[lookahead.attribute.codeType], "if") == 0) {
			matchToken(KW_T, KW_if);
			matchToken(LPR_T, NO_ATTR);
			conditionStatement();
			matchToken(RPR_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else if (strcmp(keywordTable[lookahead.attribute.codeType], "while") == 0) {
			matchToken(KW_T, KW_while);
			matchToken(LPR_T, NO_ATTR);
			conditionStatement();
			matchToken(RPR_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			codeSession();
			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else if (strcmp(keywordTable[lookahead.attribute.codeType], "return") == 0) {
			matchToken(KW_T, KW_return);
			returnStatement();
			matchToken(EOS_T, NO_ATTR);
			break;
		}
		else if (strcmp(keywordTable[lookahead.attribute.codeType], "true") == 0) {
			matchToken(KW_T, KW_true);
			optionalStatements();
			matchToken(EOS_T, NO_ATTR);
			break;
		}
		else if (strcmp(keywordTable[lookahead.attribute.codeType], "false") == 0) {
			matchToken(KW_T, KW_false);
			optionalStatements();
			matchToken(EOS_T, NO_ATTR);
			break;
		}
		else {
			printError();
			break;
		}
	case MNID_T:
		if (strcmp(lookahead.attribute.idLexeme, LANG_WRTE) == 0) {
			outputStatement();
		}
		else if (strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0) {
			inputStatement();
		}
		break;
	default:
		lookahead = tokenizer();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

void returnStatement() {
	return_flag = 1;
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case KW_T:
		if (strcmp(keywordTable[lookahead.attribute.codeType], "true") == 0) {
			matchToken(KW_T, KW_true);
			break;
		}
		else if (strcmp(keywordTable[lookahead.attribute.codeType], "false") == 0) {
			matchToken(KW_T, KW_false);
			break;
		}
		else {
			printError();
			break;
		}
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Return statement parsed");
}
/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
void outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(MNID_T, NO_ATTR);
	outputVariableValue();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
void outputVariableValue() {
	psData.parsHistogram[BNF_outputVariableValue]++;
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case KW_T:
		if (strcmp(keywordTable[lookahead.attribute.codeType], "true") == 0) {
			matchToken(KW_T, KW_true);
			break;
		}
		else if (strcmp(keywordTable[lookahead.attribute.codeType], "false") == 0) {
			matchToken(KW_T, KW_false);
			break;
		}
		else {
			printError();
			break;
		}
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable parsed");
}

void conditionStatement() {
	psData.parsHistogram[BNF_outputVariableValue]++;

	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	default:
		; //epsilon
	}

	switch (lookahead.code) {
	case EQ_T:
		matchToken(EQ_T, NO_ATTR);
		break;
	case NE_T:
		matchToken(NE_T, NO_ATTR);
		break;
	case GT_T:
		matchToken(GT_T, NO_ATTR);
		break;
	case LT_T:
		matchToken(LT_T, NO_ATTR);
		break;
	case STRICT_EQ_T:
		matchToken(STRICT_EQ_T, NO_ATTR);
		break;
	case STRICT_NE_T:
		matchToken(STRICT_NE_T, NO_ATTR);
		break;
	default:
		; //epsilon
	}

	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	default:
		; //epsilon
	}

	printf("%s%s\n", STR_LANGNAME, ": Condition statement parsed");
}

void iterationStatement() {
	return;
}

void inputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(MNID_T, NO_ATTR);

	psData.parsHistogram[BNF_outputVariableValue]++;
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}

	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

/*
 ************************************************************
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
void printBNFData(ParserData psData) {
}
*/
void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}