#ifndef MINESWEEPER_H
#define MINESWEEPER_H 


#include "std_lib_facilities_4.h"
#include "Simple_window.h"
#include "Window.h"
#include "Graph.h"
#include "GUI.h"
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl.H>
#include <FL/Fl_Timer.H>

#ifndef debug
#define debug true
#endif

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
	Fl_PNG_Image imgSmile{string("Small_Squares/Smile.png").c_str()};
	Fl_PNG_Image imgSmileDown{string("Small_Squares/SmileDown.png").c_str()};
	Fl_PNG_Image imgDead{string("Small_Squares/Dead.png").c_str()};
	Fl_PNG_Image imgCool{string("Small_Squares/Cool.png").c_str()};
	Fl_PNG_Image imgScared{string("Small_Squares/Scared.png").c_str()};
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
		void put_mine(bool put);
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

struct Smile:Widget
{
	public:
		Smile(Point xy, Callback cb)
			:Widget(xy, 26, 26, "", cb)
		{
		}
		void change_image(Fl_PNG_Image& im);
		void attach(Graph_lib::Window& win);
		void move(Point xy) {Widget::move(xy.x-loc.x,xy.y-loc.y);}
	private:
		
};

struct Counter : Shape
{
		public:
		Counter(Point xy, int w, int h)
			:t{new Text(Point{xy.x, xy.y+11+h/2}, "000")}, r{new Rectangle(xy, w, h)}
		{
			add(xy);
		}
		void draw_lines() const;
		void move(Point xy);
		void set_value(int val);
		void increment_value(int change) {set_value(value+change);};
	private:
		Rectangle* r;
		Text* t;
		int value;
};

struct Game: Graph_lib::Window //make window later
{
	public:
		static constexpr int y_offset = 40;
	
		Game(Point xy, const string& title );
		~Game();
		void place_mines (int num, int row, int col); //pass start click & make private later
		void place_mine (int row, int col);  //make private later
		void show_mines(int row, int col);  // which one to skip
		void clear_board();
		void create_board(int rows, int cols, int mines);
		void lose_game(int row, int col);
		void start_game(int row, int col);  //which one clicked to start
		void win_game();
		void restart_game();
	private:
		vector<vector<Tile*>> board; //pointer may be very bad (leak) but window does it
		Smile* smiley;
		Counter* mine_counter;
		Counter* timer;
		bool game_started=false;
		bool game_over = false;
		int mine_total;
		int uncovered=0;
		
		static void cb_tile_click (Address, Address);
		static void cb_restart_click (Address, Address);
		//setting mines should increment mine count of those around
		void click (int row, int col);
		void show_scared();
		void left_click(int row, int col);
		void middle_click(int row, int col);
		void right_click(int row, int col);
};
#endif