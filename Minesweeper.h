#ifndef MINESWEEPER_H
#define MINESWEEPER_H 


#include "std_lib_facilities_4.h"
#include "Simple_window.h"
#include "Graph.h"
#include "GUI.h"
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl.H>

#ifndef TileImg
#define TileImg
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

#endif

struct MyBox:Fl_Box
{
	MyBox(int x, int y, int w, int h, const char * 	l = 0 )
		:Fl_Box(x,y,w,h,l)
	{
	}
	virtual int handle(int event);
};

struct Tile:Widget //not a button for aesthetic purposes (added MyBox)
{
	enum class State {unclicked, clicked, flag, question};
	
	static constexpr int tileSide = 16 ;
	
	public:
		Tile(Point xy, Callback cb)
			:Widget(xy, tileSide, tileSide, "", cb),
			 mine{false}, adj_mines{0}, clicked_img{&TileImg::imgBlank}
		{
		}
		State get_state() const {return current_state;}
		void change_state(State s);
		void attach(Graph_lib::Window& win);
		void put_mine();
		bool get_mine() const {return mine;}
		void add_adj_mines();
		void set_adj_mines(int numMines); //maybe make private
		int get_adj_mines() const {return adj_mines;}
		void loss();
	private:
		bool mine;
		int adj_mines;  
		State current_state;
		Fl_PNG_Image* clicked_img;  
		
		void changeImage(Fl_PNG_Image& im); //possible public
};

struct Game:Simple_window //make window later
{
	public:
		Game(Point xy, const string& title );
		~Game();
		void place_mines (int num, int row, int col); //pass start click & make private later
		void place_mine (int row, int col);  //make private later
		void show_mines(int row, int col);  // which one to skip
		void lose_game(int row, int col);
		void start_game(int row, int col);  //which one clicked to start
		void win_game();
	private:
		vector<vector<Tile*>> board; //pointer may be very bad (leak) but window does it
		bool game_started=false;
		bool game_over = false;
		int mine_total;
		int uncovered=0;
		
		static void cb_tile_click (Address, Address);
		//setting mines should increment mine count of those around
		void click (int row, int col);
		void left_click(int row, int col);
		void middle_click(int row, int col);
		void right_click(int row, int col);
};
#endif