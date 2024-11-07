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
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           S J S             @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST8152 Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Sep 23, 2024
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/


#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */

/*
***********************************************************
* Function name: main
* Purpose: Main function
* Author: Paulo Sousa
* History/Versions: Ver F22
* Called functions: mainReader(), mainScanner(), mainParser()
* Parameters: Command line arguments - argc, argv
* Return value: Status
* Algorithm: -
*************************************************************
*/

integer main(int argc, char** argv) {
	integer i;
	printLogo();
	if (DEBUG) {
		for (i = 0; i < argc; ++i)
			printf("argv[%d] = %s\n", i, argv[i]);
	}
	if (argc < 2) {
		printf("%s%c%s%c%s%c%s", "OPTIONS:\n* [",
			PGM_READER, "] - Reader\n* [",
			PGM_SCANNER, "] - Scanner\n* [",
			PGM_PARSER, "] - Parser\n");
		return EXIT_FAILURE;
	}
	character option = argv[1][0];
	switch (option) {
	case PGM_READER:
		printf("%s%c%s", "\n[Option '", PGM_READER, "': Starting READER .....]\n\n");
		mainReader(argc, argv);
		break;

	case PGM_SCANNER:
		printf("%s%c%s", "\n[Option '", PGM_SCANNER, "': Starting SCANNER .....]\n\n");
		mainScanner(argc, argv);
		break;

	/* TO_DO: Include later PARSER (A32) */
	/*
	case PGM_PARSER:
		printf("%s%c%s", "\n[Option '", PGM_PARSER, "': Starting PARSER .....]\n\n");
		mainParser(argc, argv);
		break;
	*/
	default:
		printf("%s%c%s%c%s%c%s", "* OPTIONS:\n- [",
			PGM_READER, "] - Reader\n- [",
			PGM_SCANNER, "] - Scanner\n- [",
			PGM_PARSER, "] - Parser\n");
		break;
	}
	return EXIT_SUCCESS;
}

/*
***********************************************************
* Function name: printLogo
* Purpose: Print Logo
* Author: Paulo Sousa
* History/Versions: Ver S23
* Called functions: -
* Parameters: -
* Return value: (Null)
* Algorithm: -
*************************************************************
*/

/* TO_DO: Update your logo with your own language message */

void printLogo() {
	printf("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		"\t=---------------------------------------=\n",
		"\t|  COMPILERS - ALGONQUIN COLLEGE (F24)  |\n",
		"\t=---------------------------------------=\n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t    @@                             @@    \n",
		"\t    @@           %&@@@@@@@@@@@     @@    \n",
		"\t    @@       @%% (@@@@@@@@@  @     @@    \n",
		"\t    @@      @& @   @ @       @     @@    \n",
		"\t    @@     @ @ %  / /   @@@@@@     @@    \n",
		"\t    @@      & @ @  @@              @@    \n",
		"\t    @@       @/ @*@ @ @   @        @@    \n",
		"\t    @@           @@@@  @@ @ @      @@    \n",
		"\t    @@            /@@    @@@ @     @@    \n",
		"\t    @@     @      / /     @@ @     @@    \n",
		"\t    @@     @ @@   /@/   @@@ @      @@    \n",
		"\t    @@     @@@@@@@@@@@@@@@         @@    \n",
		"\t    @@                             @@    \n",
		"\t    @@            S J S            @@    \n",
		"\t    @@                             @@    \n",
		"\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n",
		"\t                                         \n",
		"\t[COMPILER LOGO .........................]\n",
		"\t                                         \n"
	);
}
