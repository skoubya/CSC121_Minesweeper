#ifndef Fl_Menu_Bar_H
#define Fl_Menu_Bar_H

#include "Fl_Menu_.H"

class FL_EXPORT Fl_Menu_Bar : public Fl_Menu_
{
protected:
   void draw();
public:
   int handle(int);
   Fl_Menu_Bar(int X, int Y, int W, int H,const char *l=0);
};
#endif