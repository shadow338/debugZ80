
debugZ80 v0.82
(C) 2019 Rui Fernando Ferreira Ribeiro

Z80 command line debugger.
MS-DOS DEBUG.EXE look alike for Z80/ZX Spectrum programs.

Companion of QtSpecem. Directory has to be on the same root level of QtSpecem sources for it to compile.

Real time console mode debugger. Still very beta. Posted here as a demo, code has to be improved and arranged.


"Features":

The trace/running logic is on debugz80 side. It is always slower than being on the emulator side.

TO DO

Breakpoints.
Maybe addopting the =XXXX syntax of DEBUG.EXE.

For compiling:

make

Usage:

Has to be launched *after* QtSpecem.

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

