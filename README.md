
debugZ80 v0.82

(C) 2019 Rui Fernando Ferreira Ribeiro

Z80 command line debugger.

MS-DOS DEBUG.EXE look alike for Z80/ZX Spectrum programs.

Companion of QtSpecem. Directory has to be on the same root level of QtSpecem sources for it to compile.

Real time console mode debugger. Still very beta. Posted here as a demo, code has to be improved and arranged.


FEATURES

The trace/running logic is on debugz80 side. It is always slower than being on the emulator side.

TO DO

Breakpoints.

Maybe addopting the =XXXX syntax of DEBUG.EXE.

COMPILE

make

USAGE

./debugz80

Has to be launched *after* QtSpecem. z80tab.txt	and zx_labels.txt have to be on the current directory.

Commands:
   
 T [XXXX]  - Trace
 P [XXXX]  - Proceed
 E         - Execute until RET
 G [XXXX]  - Go
 Q         - Quit
 U [XXXX]  - Dissassembly
 D [XXXX]  - Dump
 R         - show registers
 R YY XXXX - load 16-bit vars
 R Y XX    - load 8-bit vars
 ?         - Help
 ENTER     - Repeats last command

 --- XXXX four hexa digits
 --- XX   two  hexa digits
 --- YY   Z80 AF BC DE HL IX IY SP or PC
 --- Y    Z80 A B C D E H L

T - step-by-step trace (all instruction, interrup routines also).

P - step over. Step over calls, no interrupt routines shown.

E - does it best to return control after abandoning a subroutine.

G - Execute from PC or from address.

Q - Leave.

U - Disassembler.

D - Dump hexadecimal and bytes.

R - Show Z80 registers or modify them.

? - Shows help.

R - Repeats last command.

Example session:

./debugz80 
- P
                            
 AF 005C  s=0  AF' 0044 s=0 
 BC 1721  z=1  BC' 171F z=1 
 DE 5CB9  h=1  DE' 00B8 h=0 
 HL 10A8  p=1  HL' 0038 p=1 
          n=0           n=0 
          c=0           c=0 
                            
 IX 03D4 IFF1=0 (sp+4) 15E1 
 IY 5C3A IFF2=0 (sp+2) FFFF 
 SP FF48 IM  =1 (sp)   15FE 
 PC 0038                    
 R  D3    F5                
 I  3F                      
          PUSH AF           
                            
- U
0038 F5             PUSH	AF ; MASK_INT   
0039 E5             PUSH	HL
003A 2A 78 5C       LD	HL,(5C78) ; LD	HL,(FRAMES1)
003D 23             INC	HL
003E 22 78 5C       LD	(5C78),HL ; LD	(FRAMES1),HL
0041 7C             LD	A,H
0042 B5             OR	L
0043 20 03          JR	NZ,0048 ; JR	NZ,KEY_INT
0045 FD 34 40       INC	(IY+40)
0048 C5             PUSH	BC ; KEY_INT   
0049 D5             PUSH	DE
004A CD BF 02       CALL	02BF ; CALL	KEYBOARD
004D D1             POP	DE
004E C1             POP	BC
004F E1             POP	HL
0050 F1             POP	AF
0051 FB             EI
0052 C9             RET
0053 E1             POP	HL ; ERROR_2   
0054 6E             LD	L,(HL)
- D
 0000 F3 AF 11 FF FF C3 CB 11 2A 5D 5C 22 5F 5C 18 43 ?...??.*]\"_\.C 
 0010 C3 F2 15 FF FF FF FF FF 2A 5D 5C 7E CD 7D 00 D0 ??......*]\~?}.? 
 0020 CD 74 00 18 F7 FF FF FF C3 5B 33 FF FF FF FF FF ?t..?...?[3..... 
 0030 C5 2A 61 5C E5 C3 9E 16 F5 E5 2A 78 5C 23 22 78 ?*a\?Þ.??*x\#"x 
 0040 5C 7C B5 20 03 FD 34 40 C5 D5 CD BF 02 D1 C1 E1 \|? .?4@??Ϳ.??? 
 0050 F1 FB C9 E1 6E FD 75 00 ED 7B 3D 5C C3 C5 16 FF ????n?u.?{=\??.. 
 0060 FF FF FF FF FF FF F5 E5 2A B0 5C 7C B5 20 01 E9 ......??*?\|? .? 
 0070 E1 F1 ED 45 2A 5D 5C 23 22 5D 5C 7E C9 FE 21 D0 ???E*]\#"]\~??!? 
 0080 FE 0D C8 FE 10 D8 FE 18 3F D8 23 FE 16 38 01 23 ?.??.??.??#?.8.# 
 0090 37 22 5D 5C C9 BF 52 4E C4 49 4E 4B 45 59 A4 50 7"]\ɿRN?INKEY?P 
 00A0 C9 46 CE 50 4F 49 4E D4 53 43 52 45 45 4E A4 41 ?F?POIN?SCREEN?A 
 00B0 54 54 D2 41 D4 54 41 C2 56 41 4C A4 43 4F 44 C5 TT?A?TA?VAL?COD? 
 00C0 56 41 CC 4C 45 CE 53 49 CE 43 4F D3 54 41 CE 41 VA?LE?SI?CO?TA?A 
 00D0 53 CE 41 43 D3 41 54 CE 4C CE 45 58 D0 49 4E D4 S?AC?AT?L?EX?IN? 
 00E0 53 51 D2 53 47 CE 41 42 D3 50 45 45 CB 49 CE 55 SQ?SG?AB?PEE?I?U 
 00F0 53 D2 53 54 52 A4 43 48 52 A4 4E 4F D4 42 49 CE S?STR?CHR?NO?BI? 
 0100 4F D2 41 4E C4 3C BD 3E BD 3C BE 4C 49 4E C5 54 O?AN?<?>?<?LIN?T 
 0110 48 45 CE 54 CF 53 54 45 D0 44 45 46 20 46 CE 43 HE?T?STE?DEF F?C 
 0120 41 D4 46 4F 52 4D 41 D4 4D 4F 56 C5 45 52 41 53 A?FORMA?MOV?ERAS 
 0130 C5 4F 50 45 4E 20 A3 43 4C 4F 53 45 20 A3 4D 45 ?OPEN ?CLOSE ?ME 
- 
 0140 52 47 C5 56 45 52 49 46 D9 42 45 45 D0 43 49 52 RG?VERIF?BEE?CIR 
 0150 43 4C C5 49 4E CB 50 41 50 45 D2 46 4C 41 53 C8 CL?IN?PAPE?FLAS? 
 0160 42 52 49 47 48 D4 49 4E 56 45 52 53 C5 4F 56 45 BRIGH?INVERS?OVE 
 0170 D2 4F 55 D4 4C 50 52 49 4E D4 4C 4C 49 53 D4 53 ?OU?LPRIN?LLIS?S 
 0180 54 4F D0 52 45 41 C4 44 41 54 C1 52 45 53 54 4F TO?REA?DAT?RESTO 
 0190 52 C5 4E 45 D7 42 4F 52 44 45 D2 43 4F 4E 54 49 R?NE?BORDE?CONTI 
 01A0 4E 55 C5 44 49 CD 52 45 CD 46 4F D2 47 4F 20 54 NU?DI?RE?FO?GO T 
 01B0 CF 47 4F 20 53 55 C2 49 4E 50 55 D4 4C 4F 41 C4 ?GO SU?INPU?LOA? 
 01C0 4C 49 53 D4 4C 45 D4 50 41 55 53 C5 4E 45 58 D4 LIS?LE?PAUS?NEX? 
 01D0 50 4F 4B C5 50 52 49 4E D4 50 4C 4F D4 52 55 CE POK?PRIN?PLO?RU? 
 01E0 53 41 56 C5 52 41 4E 44 4F 4D 49 5A C5 49 C6 43 SAV?RANDOMIZ?I?C 
 01F0 4C D3 44 52 41 D7 43 4C 45 41 D2 52 45 54 55 52 L?DRA?CLEA?RETUR 
 0200 CE 43 4F 50 D9 42 48 59 36 35 54 47 56 4E 4A 55 ?COP?BHY65TGVNJU 
 0210 37 34 52 46 43 4D 4B 49 38 33 45 44 58 0E 4C 4F 74RFCMKI83EDX.LO 
 0220 39 32 57 53 5A 20 0D 50 30 31 51 41 E3 C4 E0 E4 92WSZ .P01QA???? 
 0230 B4 BC BD BB AF B0 B1 C0 A7 A6 BE AD B2 BA E5 A5 ??????????????? 
 0240 C2 E1 B3 B9 C1 B8 7E DC DA 5C B7 7B 7D D8 BF AE ?᳹??~??\?{}ؿ? 
 0250 AA AB DD DE DF 7F B5 D6 7C D5 5D DB B6 D9 5B D7 ?????.??|?]۶?[? 
 0260 0C 07 06 04 05 08 0A 0B 09 0F E2 2A 3F CD C8 CC ..........?*???? 
 0270 CB 5E AC 2D 2B 3D 2E 2C 3B 22 C7 3C C3 3E C5 2F ?^?-+=.,;"?<?>?/ 
- G
Running...
- q
Quitting. Disabling trace.

