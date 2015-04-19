#include "std_lib_facilities_4.h"
#include "Simple_window.h"
#include "Graph.h"
#include "GUI.h"
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl.H>

namespace TileImg
{
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
}

string toString(void* p)
{
	ostringstream os;
	os<<p;
	return os.str();
}

struct MyBox:Fl_Box
{
	MyBox(int x, int y, int w, int h, const char * 	l = 0 )
		:Fl_Box(x,y,w,h,l)
	{
	}
	virtual int handle(int event);
};

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

struct Tile:Widget //not a button for aesthetic purposes (added MyBox)
{
	enum class State {unclicked, clicked, flag, question};
	
	static constexpr int tileSide = 16 ;
	
	public:
		Tile(Point xy, Callback cb)
			:Widget(xy, tileSide, tileSide, "", cb)
		{
		}
		State get_state() const {return current_state;}
		void changeState(State s);
		void attach(Graph_lib::Window& win);
		void over();
	private:
		bool mine;
		int adjacent_mines;
		State current_state;
		
		void changeImage(Fl_PNG_Image& im); //possible public
};

void Tile::attach(Graph_lib::Window& win)
{
	pw = new MyBox(loc.x, loc.y, width, height, label.c_str());
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
    own = &win;
	pw->align(FL_ALIGN_IMAGE_BACKDROP); 
	changeState(State::unclicked);
}

void Tile::changeImage(Fl_PNG_Image& im)
{
	pw->image(im);
	pw->redraw(); //maybe get rid of later
}

void Tile::changeState(State s) 
{
	switch(s)
	{
		case State::unclicked: changeImage(TileImg::imgUnclicked);
							   break;
		case State::clicked: changeImage(TileImg::imgBlank); //temporary
							 break;
		case State::flag: changeImage(TileImg::imgFlag);
						  break;
		case State::question: changeImage(TileImg::imgQuestionMark);
							  break;
	}
	current_state = s;
}

struct Game:Simple_window //make window later
{
	public:
		Game(Point xy, const string& title );
		~Game();
	private:
		vector<vector<Tile*>> board; //pointer may be very bad (leak) but window does it
		
		static void cb_tile_click (Address, Address);
		void click (int row, int col);
		void left_click(int row, int col);
		void middle_click(int row, int col);
		void right_click(int row, int col);
};

Game::Game (Point xy, const string& title)
	:Simple_window{xy, 600, 400, title}
{
	for (int r=0; r<10; r++)
	{
		board.push_back(vector<Tile*>{});
		for (int c=0; c<10; c++)
		{
			board[r].push_back(new Tile(Point{c*Tile::tileSide,r*Tile::tileSide},cb_tile_click));
			attach(*board[r][c]);
		}
	}
}

Game::~Game()
{
	int rows=board.size();
	int cols=board[0].size();
	for (int r=0; r<rows; r++)
	{
		for (int c=0; c<cols; c++)
		{
			delete board[r][c];
		}
	}
}

void Game::cb_tile_click (Address pt, Address pw )
{
	int row= reference_to<MyBox>(pt).y()/Tile::tileSide;
	int col= reference_to<MyBox>(pt).x()/Tile::tileSide;
	reference_to<Game>(pw).click(row, col);
}

void Game::click (int row, int col)
{
	int which= Fl::event_button();
	switch (which)
	{
		case FL_LEFT_MOUSE: left_click(row, col);
							break;
		case FL_MIDDLE_MOUSE: middle_click(row, col);
							  break;
		case FL_RIGHT_MOUSE: right_click(row, col);
							 break;
	}
}

void Game::left_click(int row, int col) //not written
{
}

void Game::right_click(int row, int col)
{
	Tile* t =board[row][col];
	Tile::State cur = t->get_state();
	switch (cur)
	{
		case Tile::State::clicked: break;
		case Tile::State::unclicked: t->changeState(Tile::State::flag);
									 break;
		case Tile::State::flag: t->changeState(Tile::State::question);
								break;
		case Tile::State::question: t->changeState(Tile::State::unclicked);
									break;
	}
}

void Game::middle_click(int row, int col) //not written
{
}

int main()
{
	using namespace Graph_lib;
	Game win(Point{100,200},"Canvas"); 
	//Tile t{Point{20,20}, cb};
	//win.attach(t);
	win.wait_for_button();
	//t.changeState(Tile::State::clicked);
	//win.wait_for_button();
}
