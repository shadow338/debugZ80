
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


unsigned short draw_cpuregs();
void list_basic(char * s);

#include "../QtSpecem/z80core/z80.h"
#include "../QtSpecem/z80core/iglobal.h"

void print_hexdump(USHORT adress,int memory);

#include <string.h>

void build_F();

extern char ** label_table;

unsigned char * attach_speccy_shared_ram(void);
unsigned char * attach_speccy_shared_vars(void);

unsigned char        bFlashOn = 1;  /* Flash on or off      */

/* Main registers   */
union Z80Regs * Z80Regs;

/* Alternative registers */
union Z80Regs * Z80Regs2;

union Z80IX * Z80IX;
union Z80IY * Z80IY;

struct CPU_flags * flags;
struct Z80vars * Z80vars;

/* Interrupt mode - 0, 1 or 2 */
// UCHAR _IM;

/* Interrupt flag & copy
 */
// UCHAR IFF1, IFF2;

/* Used in DDCB and FDCB to keep a indice for IX and IY */
UCHAR lastbyte;

/* Memory 64k */
UCHAR * mem;
UCHAR * vars;

USHORT addr;
USHORT daddr;

int proceed_mode = 0;
int return_mode = 0;
USHORT old_PC;
USHORT old_SP;
USHORT next_PC;
USHORT pSP;

void show_help(void)
{
   printf(" T [XXXX]     - Trace\n");
   printf(" P [XXXX]     - Proceed\n");
   printf(" E            - Execute until RET\n");
   printf(" G [XXXX]     - Go\n");
   printf(" Q            - Quit\n");
   printf(" U [XXXX]     - Dissassembly\n");
   printf(" D [XXXX]     - Dump\n");
   printf(" R            - Show registers\n");
   printf(" R YY XXXX    - Load 16-bit vars\n");
   printf(" R Y XX       - Load 8-bit vars\n");
   printf(" B            - List ZX BASIC program\n");
   printf(" B list       - List ZX BASIC program w/ reveal numbers\n");
   printf(" B sysvars    - List system variables\n");
   printf(" B linebuffer - Print BASIC linebuffer\n");
   printf(" B noautorun  - Disable BASIC LOAD\"\" auto run\n");
   printf(" B autorun    - enable auto run back\n");
   printf(" K XXXX XX    - Poke hexa addr with hexa byte\n");
   printf(" ?            - Help\n");
   printf(" ENTER        - Repeats last command\n");
   printf("\n");
   printf(" --- XXXX four hexa digits\n");
   printf(" --- XX   two  hexa digits\n");
   printf(" --- YY   Z80 AF BC DE HL IX IY SP or PC\n");
   printf(" --- Y    Z80 A B C D E H L\n");
}

// print dissassembly in address
void unassemble(int addr_arg)
{
   char * s, *p = NULL, *var_label, *address_label;
   int i, cnt;

   // addr address to disassembly from
   if ( addr_arg != -1 )
      addr = addr_arg;

   // we want 20 dissassembly lines
   for (i = 0; i < 20 ; i++)
   {
      int j;

      // call the dissassembler
      // *s = len of opcode in bytes 
      s = ldissbl(addr);
      // print address
      printf("%04X ", addr);

      // print bytes in hexa
      for(j = 0; j < *s ; j++ )
         printf("%02X ", readbyte(addr+j));

      // formatting - max 5 bytes opcode
      for( ; j < 5 ; j++)
         printf("   ");

      // print disassembled mmemonic
      printf("%s", s + 1);

      // get var label, if any
      cnt = 0;      // hexa counter
      var_label = NULL;

      // find 4 hexa number pos p in s
      if ( strlen(s+1) > 6 )
      {
         int c;

         p = s+1; // take out byte len

	 // search string. If we found 4 consecutive hexa digits
         // we've found it -> break out of for.
         for ( j = 0 ; j < (int)strlen(s+1) && (cnt < 4) ; j++)
         {
            c = *p++;
            if ( isxdigit(c) )
               cnt++;
            else
               cnt = 0;
         }

	 // if found
         if (cnt == 4 )
         {
            p = p - 4; // p to beginging of hex number
           
            var_label =  *(label_table + (int)strtol(p, NULL, 16) );                             
         }
      }

      // get address label, if any
      address_label = NULL;
      address_label = *(label_table + addr);

      // do we need to print a comment?
      if ( ( (var_label != NULL ) || (address_label != NULL ) ) && (p != NULL) )
      {
         printf(" ; ");          // comments in Z80 assembly begin with ;

         if (address_label != NULL )          // if the address has a label
            printf("%s   ", address_label);   // print it


         if (var_label != NULL )           // if the var has a label
         {
            char * z = s +1;          // point to disassembly string

            while ( z < p )           // print it, up to find hexa number
               putchar(*z++);
            printf("%s", var_label);       // print label
            z += 4;		      // and skip hex string
            printf("%s", z);          // print the rest of the string
         }
      }
      // end of line
      printf("\n");

      // incremment address to start of next mmemonic
      addr += s[0];
   }
}

void register_value(char * reg, char * value)
{
   int addr_arg;
   int i;
      char reg_z80 [][3] =
      {
       "AF",
       "BC",
       "DE",
       "HL",
       "SP",
       "PC",
       "IX",
       "IY",

       "A",
       "B",
       "C",
       "D",
       "E",
       "H",
       "L",
       "IM",
       "" 
      };

   if ( reg == NULL || value == NULL )
   {
      draw_cpuregs();
      return;
   }
   addr_arg = (int)strtol(value, NULL, 16);
   
   for (i = 0 ; strcmp(reg_z80[i], reg) && strcmp(reg_z80[i], "") ; i++ );

   // As variables are not allocated compile time
   // Cannot optimize this with a compile-time pointer array
   switch(i)
   {
      case  0:
             AF = addr_arg;
             build_F();
             break;
      case  1:
             BC = addr_arg;
             break;
      case  2:
             DE = addr_arg;
             break;
      case  3:
             HL = addr_arg;
             break;
      case  4:
             SP = addr_arg;
             break;
      case  5:
             PC = addr_arg;
             break;
      case  6:
             IX = addr_arg;
             break;
      case  7:
             IY = addr_arg;
             break;

      case  8:
             A = addr_arg;
             break;
      case  9:
             B = addr_arg;
             break;
      case 10:
             C = addr_arg;
             break;
      case 11:
             D = addr_arg;
             break;
      case 12:
             E = addr_arg;
             break;
      case 13:
             H = addr_arg;
             break;
      case 14:
             L = addr_arg;
             break;
      case 15:
	     _IM = 1;
	     break;

      default:
             printf("Invalid args\n");
             break;
      }
}

void poke(char * byte, char * v)
{
   int addr, value;

   addr  = (int)strtol(byte, NULL, 16);
   value = (int)strtol(v, NULL, 16);
   writebyte(addr, value);
}

// parse user line command
void do_commands(char * str)
{
    char command;

    char * pch, * token[8];
    int i = 0;
    int  addr_arg;
    FILE * f;

    // field separator por extracting tokens is " "
    pch = strtok (str," ");

    // create array of tokens
    while (pch != NULL)
    {
       token[i++] = pch;
       pch = strtok (NULL, " ");
    }
   
    // two to simply testing / parameter passing
    token[i++] = NULL;
    token[i++] = NULL;

    if (i == 2)
    {
       printf("No commands\n");
       return; 
    }

    if (token[1] != NULL)
    {
       addr_arg = (int)strtol(token[1], NULL, 16);
    }
    else
       addr_arg = -1;

    command = toupper(*token[0]);
    switch(command)
    {
        case 'T':	// Trace
                if ( addr_arg != -1 )
                   PC = addr_arg;
                addr = PC;
                build_F();
                draw_cpuregs();
                TraceOn = 3;
		break;
        case 'P':	// Proceed
               if ( addr_arg != -1 )
                   PC = addr_arg;
                proceed_mode = 1;
                old_PC = PC;
                old_SP = SP;
		addr = PC;
		build_F();
		next_PC = draw_cpuregs();
		TraceOn = 3;
		break;
        case 'E':	// Execute Until Return
                return_mode = 1;
                pSP = readword(SP);
		addr = PC;
		build_F();
		draw_cpuregs();
                TraceOn = 2;
		break;
        case 'G':	// Go
               if ( addr_arg != -1 )
                   PC = addr_arg;
                printf("Running...\n");
                TraceOn = 0;
		break;
        case 'Q':	// Quit
                printf("Quitting. Disabling trace.\n");
                TraceOn = 0;
                exit(0);
	        break;
	case 'D':	// Dump
                if ( addr_arg != -1 )
                   daddr = addr_arg;
                print_hexdump(daddr, 0);
                daddr += 0x140;
                break;
        case 'U':
                unassemble(addr_arg);
		break;

        case 'R':
                register_value(token[1], token[2]);
                break;

        case 'K':
		poke(token[1], token[2]);
                break;

	case 'B':
		list_basic(token[1]);
		break;

        case 'S':
                f = fopen ("/tmp/a", "w" );
                for (i=0x5D43 ; i < 0x7106 ; i++ )
                   fputc(readbyte(i), f);
                fclose(f);
                break;
                   
        case 'L':
                i = 0x5CCB;
                f= fopen("/tmp/a", "r" );
                while(!feof(f))
                {
                   writebyte(i++, fgetc(f) );
                }
                fclose(f);
                break;

	case '?':
                show_help();
		break;

	default:
		printf(" Unknown command\n");
		break;
    }
}

void init_shm_client()
{

   mem = attach_speccy_shared_ram();
   vars = attach_speccy_shared_vars();

   Z80vars  = (struct Z80vars *)  vars;
   Z80Regs  = (union Z80Regs *)   vars + sizeof(struct Z80vars);
   Z80Regs2 = (union Z80Regs *)   vars + sizeof(struct Z80vars) + sizeof(union Z80Regs);
   flags    = (struct CPU_flags *)vars + sizeof(struct Z80vars) + sizeof(union Z80Regs) + sizeof(union Z80Regs);
   Z80IX    = (union Z80IX *)     vars + sizeof(struct Z80vars) + sizeof(union Z80Regs) + sizeof(union Z80Regs) + sizeof(struct CPU_flags);
   Z80IY    = (union Z80IY *)     vars + sizeof(struct Z80vars) + sizeof(union Z80Regs) + sizeof(union Z80Regs) + sizeof(struct CPU_flags) + sizeof(union Z80IX);

}

int main()
{
   char string[256];      // actual command line
   char old_string[256];  // copy of command line for replaying it

   init_shm_client();	  // attach to memory of QtSpecem 

   strcpy(old_string, "");

   // address for debugging pointing to the corrent Z80 PC
   addr = PC;

   while (1)
   {
      // if not in debugging mode
      if (!proceed_mode && !return_mode)
      {
         printf("- ");                    // print prompt

         fgets (string, 255, stdin);      // wait for command line
  
         if ( strcmp(string, "\n") )      // if not empty
         {
             strcpy(old_string, string);  // copy new command line
             do_commands(string); 	  // execute it
         }
         else
            do_commands(old_string);     // if empty, repeast last line
      }
      else
      {
         // handle P/proceed mode
         if (proceed_mode)
         {
            if (TraceOn == 2)
            {
               // if PC the same as last time we were here
               if ( PC == old_PC )
                 TraceOn = 3;   
               else
                  // if not next instruct and SP lower in stack than we need
                  if ( ( PC != next_PC ) && (SP < old_SP ) )
                     TraceOn = 3;
                  else
                     proceed_mode = 0; // disable proceed mode
            }
         }

	 // handle E/return after RET
         if (return_mode)
         {
            if (TraceOn == 2)
            {
               // if Z80 PC was in the address in the stack
               if ( PC == pSP )
               {
                  draw_cpuregs();

		  // disable E mode
                  return_mode = 0;
               }
               else
               {
                  // save the top address on stack
                  pSP = readword(SP);
                  // continue debugging
                  TraceOn = 3;
               }
            }
         }
      }
   }
}

