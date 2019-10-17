#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "../QtSpecem/h/quirks.h"
#include "../QtSpecem/z80core/env.h"


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

void list_basic()
{
   int prog, vars;
   int line, len, pos;
   unsigned char c;
 
   prog = readword(23635);
   vars = readword(23627);

   pos = prog;
   while (pos < vars )
   {
       line = readbyte(pos++) * 256;
       line += readbyte(pos++);
       printf("%d ", line);
       len = readword(pos++);
       pos++;
       while (--len > 0 )
       {
          c = readbyte(pos++);
          if (c == 0x0E)
          {
                pos += 5;
                len -= 5;
                if ( len < 0 )
                   break;
          }
          else
          {
          if (c >= 0xA5)
             printf("%s ", tokens[c - 0xA5]);
          else
             putchar(c);
          }
       }
       pos++;
       printf("\n");
   }  
}
