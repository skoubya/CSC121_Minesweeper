#include "std_lib_facilities_4.h"
#include "Simple_window.h"
#include "Graph.h"
#include "GUI.h"
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Button.H>

struct Tile:Button
{
	enum class State {unclicked, clicked, flag, question};
	
	const int tileSide = 16 ;
	
	public:
		Tile(Point xy, Callback cb)
			:Button(xy, tileSide, tileSide, "", cb)
		{
		}
		void changeState(State s);
	private:
		bool mine;
		int adjacent_mines;
		State current_state;
		
		Fl_PNG_Image img1{string("Small_Squares/1.png").c_str()};
		Fl_PNG_Image img2{string("Small_Squares/2.png").c_str()};
		Fl_PNG_Image img3{string("Small_Squares/3.png").c_str()};
		Fl_PNG_Image img4{string("Small_Squares/4.png").c_str()};
		Fl_PNG_Image img5{string("Small_Squares/5.png").c_str()};
		Fl_PNG_Image img6{string("Small_Squares/6.png").c_str()};
		Fl_PNG_Image img7{string("Small_Squares/7.png").c_str()};
		Fl_PNG_Image img8{string("Small_Squares/8.png").c_str()};
		Fl_PNG_Image imgBlank{string("Small_Squares/Blank.png").c_str()};
		Fl_PNG_Image imgFlag{string("Small_Squares/Flag.png").c_str()};
		Fl_PNG_Image imgBomb{string("Small_Squares/GrayBomb.png").c_str()};
		Fl_PNG_Image imgQuestionMark{string("Small_Squares/QuestionMark.png").c_str()};
		Fl_PNG_Image imgRedBomb{string("Small_Squares/RedBomb.png").c_str()};
		Fl_PNG_Image imgUnclicked{string("Small_Squares/Unclicked.png").c_str()};
		Fl_PNG_Image imgXBomb{string("Small_Squares/XBomb.png").c_str()};
		
		void changeImage(Fl_PNG_Image& im); //possible public
};

void Tile::changeImage(Fl_PNG_Image& im)
{
	pw->image(im);
}

void Tile::changeState(State s) //maybe should attach new one
{
	switch(s)
	{
		case State::unclicked: changeImage(img1);
							   break;
		case State::clicked: changeImage(img2);
							 break;
		case State::flag: changeImage(img3);
						  break;
		case State::question: changeImage(img4);
							  break;
	}
}

void cb (Address, Address){}

int main()
{
<<<<<<< HEAD
  cout<<"Aaron has edited via git shell\n";
<<<<<<< HEAD
  cout << "Hi" << endl;
=======
  cout<<"Rey Nick \n";
>>>>>>> origin/master
=======
	using namespace Graph_lib;
	Simple_window win(Point{100,200},600,400,"Canvas"); 
	Tile t{Point{20,20}, cb};
	win.attach(t);
	win.wait_for_button();
>>>>>>> Add Headers
}
