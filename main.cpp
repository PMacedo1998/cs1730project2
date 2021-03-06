B0;136;0c#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <iostream> 
#include <fstream>
#include <ncurses.h>
#include <vector>
#include <string>
#include <sstream>
#include <curses.h>
using namespace std;

void wrap(int &pos1, int &pos2, int &cols){
  if(pos2 == cols){
    pos1++;
    pos2 = 1;
  }
}
void wrapback(int &pos1, int &pos2, int &cols){
  if(pos2 <= 0){
    pos1--;
    pos2 = cols;
  }
}

int main(const int argc, const char * argv []) {
  initscr();
  int screenrows, screencols;
  getmaxyx(stdscr,screenrows,screencols);
  /*boxwin is the subwindow that is derivative of the main window (stdscr).
I set the maximum height of boxwin to be the height of the main window (rows) I subtracted 6 from rows to allow for some space at the bottom for the filename, so the max heightis actually is rows-6, and did the same process for cols. The window starts at row 3 to allow for space between the header and window
  */
  WINDOW * boxwin = derwin(stdscr, (screenrows -6), screencols-6, 3,1);
  int boxrows, boxcols;
  getmaxyx(boxwin, boxrows, boxcols);
  /* Editorwin is the window where text should be placed and edited. It's a derivative window that starts 3 rows down from the box. */
  WINDOW * editorwin = derwin(boxwin, (boxrows-5), boxcols-5, 2, 2);
  int rows, cols;
  getmaxyx(editorwin,rows, cols); 
  mvprintw(0,0,"F1: Menu");
  mvprintw(0,cols/2,"CSCI 1730 Text Editor"); //cols/2 puts the heading in the center
  wborder(boxwin, '|', '|', '-', '-', '|', '|', '|', '|'); //this simply puts a border around boxwin, so that the text editor looks like Mike's in the pdf.
  int pos1, pos2;
  getyx(editorwin, pos1, pos2);
  if(argc == 2){
    mvprintw(screenrows-2,0, argv[1]);
  } else {
    mvprintw(screenrows-2,0, "Untitled.txt");
  }
  refresh();
  scrollok(editorwin, TRUE);
  keypad(editorwin, TRUE);
  idcok(editorwin, TRUE);
  noecho();
  int input;
  /*Feel free to switch the following into a switch statement*/
  while((input = wgetch(editorwin))){ //I set tilda to stop the programming just for personal debugging, the final version won't have this.
    if(input == KEY_UP){
      pos1--;
      wmove(editorwin, pos1, pos2);
    }
    if(input == KEY_DOWN){
      pos1++;
      wmove(editorwin, pos1, pos2);
    }
    if(input == KEY_RIGHT){
      pos2++;
      wrap(pos1, pos2, cols);
      wmove(editorwin, pos1, pos2);
    }
    if(input == KEY_LEFT){
      pos2--;
      wrapback(pos1, pos2, cols);
      wmove(editorwin, pos1, pos2);
    }
    if(input == KEY_BACKSPACE){
      pos2--;
      wrapback(pos1, pos2, cols);
      mvwdelch(editorwin, pos1, pos2);
    }
    if(input == 10){ //broken enter button, working on bug fix
      char * restofline = 0;
      winnstr(editorwin, restofline, cols);
      mvwaddstr(editorwin, pos1, pos2,restofline );
    }
    if(input < 0401){ // 0401 is the starting octal values of special keys; anything less than this is normal characters like 'a' or 'f' and '!'
      wrap(pos1, pos2, cols);
      mvwaddch(editorwin, pos1, pos2, input);
      pos2++;
    }
  }
  endwin();
  return 0;
}
