/* Copyright (c) Colorado School of Mines, 2002.*/
/* All rights reserved.                       */

#include "cwp.h"

/*********************** self documentation **********************/
/******************************************************************
STRSTUFF -- STRing manuplation subs

cwp_strdup -  duplicate a string
strchop - chop off the tail end of a string "s" after a "," returning
	  the front part of "s" as "t".



******************************************************************
Input:
char *str	input string	

Output:
none

Returns:
char *	  duplicated string

******************************************************************
Notes:
This local definition of strdup is necessary because some systems
do not have it.

******************************************************************
Author: John Stockwell, Spring 2000.
******************************************************************/
/**************** end self doc ********************************/

char *
cwp_strdup(char *str)
/******************************************************************
cwp_strdup -  duplicate a string
******************************************************************
Input:
char *str	input string	

Output:
none

Returns:
char *copy      copy of string

******************************************************************
Notes:
This local definition of strdup is necessary because some systems
do not have it.
******************************************************************
Author: John Stockwell, Spring 2000.
******************************************************************/
{
	int len;
	char *copy;

	len = strlen(str) + 1;
	if (!(copy = malloc((unsigned int) len)))
		return ((char *) NULL);

	memcpy(copy,str, len);
	return (copy);
}

void strchop(char *s, char *t)
/***********************************************************************
strchop - chop off the tail end of a string "s" after a "," returning
	  the front part of "s" as "t".
************************************************************************
Notes:
Based on strcpy in Kernighan and Ritchie's C [ANSI C] book, p. 106.
************************************************************************
Author: CWP: John Stockwell and Jack K. Cohen, July 1995
***********************************************************************/
{

	while ( (*s != ',') && (*s != '\0') ) {
		 *t++ = *s++;
	}
	*t='\0';
}

