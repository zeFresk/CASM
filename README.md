CASM
====

Introduction
------------
CASM is a toolset to help programming in CARDIAC assembly.

It is composed of 2 modules :
* **CAssembler** : an assembler which transforms CARDIAC assembly language into binary.
* **CEmu** : an emulator of CARDIAC device which let you run and debug your assembled binaries.

What is CARDIAC ?
-----------------
*CARDIAC* is an acronym for *CARDboard Illustrative Aid to Computation* which is a tool made by David Hagelbarger and Saul Fingerman to teach students how computers work. It comes with an assembly language constituted of 10 instructions.

You can learn more about it [here](https://en.wikipedia.org/wiki/CARDboard_Illustrative_Aid_to_Computation "link to the wikipedia page")

CAssembler functionnalities
---------------------------
- [x] all 10 instructions
- [x] *.at* and *.start* meta-instructions to indicates code position in memory and entry point
- [x] case insensitive instructions
- [x] basic arithmetic expressions in parameters
- [x] explicit error messages
- [x] comments
- [x] pointers
- [ ] *@ operator* refering to address of variable
- [ ] dynamic memory size support
- [ ] warnings

CEmu
----
TODO
