test0

 A minimal EE demo that demostrates the compilation of a project.
 The compilation chain can start outside Eclipse environment from the Cygwin shell with the "make" 
 command inside the Debug folder of the project.
 The Debug folder can be created manually if you don't have the oil-based generator. 
 The Debug folder must contain at least the following files:
 eecfg.h, eecfg.c and the makefile (see the examples of different architectures to begin...) 

The RT-DRUID project contains the following Codewarrior files:
"Project.prm": Codewarrior linker script for Freescale S12 microcontroller.  
"conf.oil": Configuration file in OSEK implementation language specific for the application.
"Start12.c": Start-up file to initialize variables and code. 
"main.c": Main application file.
"Full_Chip_Simulation.ini": Configuration file for CW debugger simulation.
"PE_Multilink_CyclonePro.ini": Configuration file for CW debugger and Axiom TWRS12G128 demo board. 
"C_Layout.hwl": Scopes layout file for CW debugger.