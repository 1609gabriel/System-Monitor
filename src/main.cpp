#include "ncurses_display.h"
#include "system.h"

/*Homework from Clean Coder Training done one 4h and 5th of April 22:
Apply of some choosen Clean Code Principles in existing source code*/

/*Clean Code principles such as SOLID principles will be applied on the existing source code, which 
was subject of 2. project in Udacity Nanodegree "Become C++ developer (finished on Aug.21)"

More information to SOLID principles check on
https://www.youtube.com/watch?v=PakbXnLht1I*/

/*The source code is stored in public repository https://github.com/1609gabriel/System-Monitor
All clean code principles in this source code are named and marked by number*/

int main() {
  System system;
  //SystemESP32 systemESP32;
  NCursesDisplay::Display(system);
}