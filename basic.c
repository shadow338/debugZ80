#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../QtSpecem/h/quirks.h"
#include "../QtSpecem/z80core/env.h"

#define MAX_LINES 20

void show_system_vars();

// ZX Spectrum BASIC tokens as they are in ROM
char tokens[][10] =
{
   "RND",
   "INKEY$",
   "PI",
   "FN",
   "POINT",
   "SCREEN$",
   "ATTR",
   "AT",
   "TAB",
   "VAL$",
   "CODE",
   "VAL",
   "LEN",
   "SIN",
   "COS",
   "TAN",
   "ASN",
   "ACS",
   "ATN",
   "LN",
   "EXP",
   "INT",
   "SQR",
   "SGN",
   "ABS",
   "PEEK",
   "IN",
   "USR",
   "STR$",
   "CHR$",
   "NOT",
   "BIN",
   "OR",
   "AND",
   "<=",
   ">=",
   "<>",
   "LINE",
   "THEN",
   "TO",
   "STEP",
   "DEF FN",
   "CAT",
   "FORMAT",
   "MOVE",
   "ERASE",
   "OPEN #",
   "CLOSE #",
   "MERGE",
   "VERIFY",
   "BEEP",
   "CIRCLE",
   "INK",
   "PAPER",
   "FLASH",
   "BRIGHT",
   "INVERSE",
   "OVER",
   "OUT",
   "LPRINT",
   "LLIST",
   "STOP",
   "READ",
   "DATA",
   "RESTORE",
   "NEW",
   "BORDER",
   "CONTINUE",
   "DIM",
   "REM",
   "FOR",
   "GO TO",
   "GO SUB",
   "INPUT",
   "LOAD",
   "LIST",
   "LET",
   "PAUSE",
   "NEXT",
   "POKE",
   "PRINT",
   "PLOT",
   "RUN",
   "SAVE",
   "RANDOMIZE",
   "IF",
   "CLS",
   "DRAW",
   "CLEAR",
   "RETURN",
   "COPY"
};

// 2 byte line number (in big-endian format)
// 2 byte length of text including NEWLINE (in little endian format, length "excludes" the line number and length, i.e. to skip between lines you add "length of text" +4 bytes.
// text (BASIC tokens)
// NEWLINE (0x76)
// When a numeric constant is included in the text of a BASIC line, an ASCII string displaying the constant value will be inserted, followed by the token 0x7E, and the next 5 bytes are the value of the constant in floating point format.

// "Small integers have a special representation in which the first byte is 0, the second is a sign byte (0 or FFh) and the third and fourth are the integer in twos complement form, the less significant byte first."

void list_basic(char * s)
{
   int prog, vars, nxtlin;
   int line, len, cnt_line = 0;
   static int pos = 0;
   unsigned char c;

   if ( (s != NULL) && !strncmp(s, "sysvars", 7) )
   {
      show_system_vars();
      return;
   }
   
   // PROG is the special system variable that tells us where the BASIC program starts in memory 
   prog   = readword(23635);
   // VARS is the special system variable for storing where BASIC ends and program variables
   // start 
   vars   = readword(23627);
   // NEXTLIN points to the BASIC line being ran
   nxtlin = readword(23637);

   printf("(PROG) %04X (VARS) %04X (NXTLIN) %04X\n\n", prog, vars, nxtlin);

   if ( !pos )
      pos = prog;

   // show BASIC lines until reaching (VARS) or MAX_LINES
   while ( (pos < vars) && (cnt_line != MAX_LINES ) )
   {
       // line is in high-endian format
       line = readbyte(pos++) * 256;
       line += readbyte(pos++);

       // print memory position of start of line and line number
       printf("[mem:$%04X]\n%d ", pos-2, line);

       len = readword(pos++);
       pos++;

       while (--len > 0 )
       {
          c = readbyte(pos++);

          // If we find the marker for integer/float internal representation
          if (c == 0x0E)
          {
             // positive integers are often messed up in games
             if ( (readbyte(pos) == 0) && (readbyte(pos+1) == 0) )
             {
                printf("[int:%d,$%04X]", readword(pos+2), readword(pos+2) );
             }

	     // skip 5 bytes of BASIC's internal number representation
             pos += 5;
             len -= 5;
             
             // failsafe for a mal-formed BASIC line
             if ( len < 0 )
                break;
          }
          else
          {
             // from 0xA5 to 0xFF we are printing tokens
             if (c >= 0xA5)
                printf("%s ", tokens[c - 0xA5]);
             else
             {
                // bellow " ", we do not want to print unprintable
                // control characters
                if (c >= 32)
                   putchar(c);
             }
          }
       }
       
       pos++; // point to next byte
       cnt_line++;
       printf("\n"); // end of BASIC line
   }  

   // if we reached the end of the BASIC program
   // point it to the beggining again
   if ( (cnt_line != MAX_LINES ) || (prog == vars) )
      pos = prog;
}

