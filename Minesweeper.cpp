#include "Minesweeper.h"

//there is a problem w/ some numbers thinking they have a bomb but don't

int MyBox::handle(int event)
{
	if (event==FL_PUSH) 
	{
		do_callback();
		clear_changed();
		return 1;
	}
	return 0;
}

int main()
{
	using namespace Graph_lib;
	Game win(Point{100,200},"Canvas"); 
	win.wait_for_button();
}
