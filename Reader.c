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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */

BufferPointer readerCreate(integer size, integer increment, character mode) {
	BufferPointer readerPointer;
	integer count = 0;

	if (!size || size <= 0) {
		size = READER_DEFAULT_SIZE;
	}
	if (!increment || increment <= 0) {
		increment = READER_DEFAULT_INCREMENT;
	}
	if (!mode) {
		mode = MODE_FIXED;
	}
	if (mode != 'f' && mode != 'm' && mode != 'a') {
		return UNDEFINED;
	}

	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	
	if (!readerPointer) {
		return UNDEFINED;
	}

	readerPointer->content = (string)malloc(size);
	
	if (!(readerPointer->content)) {
		free(readerPointer);
		return UNDEFINED;
	}

	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}

	readerPointer->numReaderErrors = 0;

	readerPointer->mode = mode;
	readerPointer->size = size;
	readerPointer->increment = increment;

	readerPointer->flags = (Flag) {
		.isEmpty = TRUE,
		.isFull = FALSE,
		.isRead = FALSE,
		.isMoved = FALSE
	};

	readerPointer->checksum = 0;

	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer readerPointer, character ch) {
	string tempReader = UNDEFINED;
	integer newSize = 0;

	if (!readerPointer) {
		return UNDEFINED;
	}

	readerPointer->flags.isMoved = FALSE;

	if (ch < 0 || ch >= NCHAR) {
		return UNDEFINED;  
	}

	if (readerPointer->positions.wrte * (integer)sizeof(character) < readerPointer->size) {
		readerPointer->flags.isFull = FALSE;
	}
	else {
		readerPointer->flags.isFull = TRUE;

		switch (readerPointer->mode) {
		case MODE_FIXED:
			return UNDEFINED;
			break;
		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment;

			if (newSize < 0 || newSize >= READER_MAX_SIZE) {
				return UNDEFINED;
			}

			break;
		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment;
			
			if (newSize < 0 || newSize >= READER_MAX_SIZE) {
				return UNDEFINED;
			}

			break;
		default:
			return UNDEFINED;
		}

		tempReader = (string)realloc(readerPointer->content, newSize * sizeof(character));
		
		if (!tempReader) {
			return UNDEFINED;
		}
	
		if (readerPointer->content != tempReader) {
			readerPointer->flags.isMoved = TRUE;
		}

		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}
	
	readerPointer->content[readerPointer->positions.wrte++] = ch;
	readerPointer->content[readerPointer->positions.wrte] = '\0';
	
	if (ch >= 0 && ch < NCHAR) { // Check if ch is a valid index for the histogram
		readerPointer->histogram[ch]++;
	}

	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerClear(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return false;
	}
	/* TO_DO: Adjust the write, mark and read to zero */
	readerPointer->positions.read = 0;
	readerPointer->positions.wrte = 0;
	readerPointer->positions.mark = 0;
	/* TO_DO: Adjust flags */
	readerPointer->flags.isRead = false;
	readerPointer->flags.isFull = false;
	readerPointer->flags.isEmpty = true;
	readerPointer->flags.isEnd = false;

	for (size_t i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}

	return true;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerFree(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED {
		return false;
	}
	/* TO_DO: Free pointers */
	if (readerPointer->content != UNDEFINED) {
		free(readerPointer->content);
		readerPointer->content = UNDEFINED
	}
	free((void*)readerPointer);
	return true;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return false;
	}
	/* TO_DO: Check flag if buffer is FUL */
	return readerPointer->flags.isFull;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return false;
	}
	/* TO_DO: Check flag if buffer is EMP */
	return readerPointer->flags.isEmpty;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerSetMark(BufferPointer const readerPointer, integer mark) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return false; 
	}
	if (mark < 0 || mark > readerPointer->positions.wrte) {
		return false;
	}
	/* TO_DO: Adjust mark */
	readerPointer->positions.mark = mark;
	return true;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
integer readerPrint(BufferPointer const readerPointer) {
	integer cont = 0;
	character c;
	/* TO_DO: Defensive programming (including invalid chars) */
	c = readerGetChar(readerPointer);
	while (cont < readerPointer->positions.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
integer readerLoad(BufferPointer readerPointer, FILE* const fileDescriptor) {
	integer size = 0;
	character c;
	/* TO_DO: Defensive programming */
	while (!feof(fileDescriptor)) {
		c = (character)fgetc(fileDescriptor);
		readerPointer = readerAddChar(readerPointer, c);
		size++;
	}
	/* TO_DO: Defensive programming */
	return size;
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRecover(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Recover positions: read and mark must be zero */
	/* TO_DO: Update flags */
	return TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRetract(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Retract (return 1 pos read) */
	return TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRestore(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Restore positions (read to mark) */
	return TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
character readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return '\0';
	}
	if (readerPointer->positions.read == readerPointer->positions.wrte) {
		readerPointer->flags.isRead = true;
		return '\0';
	}
	/* TO_DO: Check condition to read/wrte */
	return readerPointer->content[readerPointer->positions.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
string readerGetContent(BufferPointer const readerPointer, integer pos) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return UNDEFINED;
	}
	if (pos < 0 || pos > readerPointer->positions.wrte) {
		return UNDEFINED;
	}

	return readerPointer->content + pos;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
integer readerGetPosRead(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return -1;
	}
	/* TO_DO: Return read */
	return readerPointer->positions.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
integer readerGetPosWrte(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return -1;
	}
	/* TO_DO: Return wrte */
	return readerPointer->positions.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
integer readerGetPosMark(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return false;
	}

	if (readerPointer->positions.mark < 0 || readerPointer->positions.mark > readerPointer->positions.wrte) {
		return false;
	}
	readerPointer->positions.mark = readerPointer->positions.wrte;
	/* TO_DO: Return mark */
	return TRUE;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
integer readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return -1;
	}
	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
integer readerGetInc(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return -1;
	}
	/* TO_DO: Return increment */
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
character readerGetMode(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		return -1;
	}
	/* TO_DO: Return mode */
	return '\0';
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
void readerPrintStat(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		printf("Error: Invalid BufferPointer.\n");
		return;
	}
	/* TO_DO: Updates the histogram */
	printf("Character Histogram:\n");
	for (int i = 0; i < NCHAR; i++) {
		printf("Character %c: %d\n", (char)i, readerPointer->histogram[i]);
	}
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
integer readerGetNumErrors(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		printf("Error: Invalid BufferPointer.\n");
		return -1;
	}
	integer errorCount = 0;
	for (int i = 0; i < readerPointer->positions.wrte; i++) {
		if (readerPointer->content[i] < 0 || readerPointer->content[i] >= NCHAR) {
			errorCount++;
		}
	}
	/* TO_DO: Returns the number of errors */
	return errorCount;
}

/*
***********************************************************
* Function name: readerCalcChecksum
* Purpose: Calculates the checksum of the reader (8 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

void readerCalcChecksum(BufferPointer readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == UNDEFINED) {
		printf("Error: Invalid BufferPointer.\n");
		return;
	}
	/* TO_DO: Calculate checksum */
	unsigned char checksum = 0;
	for (size_t i = 0; i < readerPointer->positions.wrte; i++) {
		checksum += readerPointer->content[i];
	}
	checksum &= 0xFF;

	readerPointer->checksum = checksum;
}

/*
***********************************************************
* Function name: readerPrintFlags
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

boolean readerPrintFlags(BufferPointer readerPointer) {
	if (!readerPointer) {
		return FALSE;
	}
	printf("isEmpty: %d\n", readerPointer->flags.isEmpty);
	printf("isFull: %d\n", readerPointer->flags.isFull);
	printf("isRead: %d\n", readerPointer->flags.isRead);
	printf("isMoved: %d\n", readerPointer->flags.isMoved);

	return TRUE;
}
