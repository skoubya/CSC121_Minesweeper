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
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_.H>

//debug mode?
#ifndef Debug
#define Debug
extern bool debug;
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
	Fl_PNG_Image img1Debug{string("Small_Squares/1Debug.png").c_str()};
	Fl_PNG_Image img2Debug{string("Small_Squares/2Debug.png").c_str()};
	Fl_PNG_Image img3Debug{string("Small_Squares/3Debug.png").c_str()};
	Fl_PNG_Image img4Debug{string("Small_Squares/4Debug.png").c_str()};
	Fl_PNG_Image img5Debug{string("Small_Squares/5Debug.png").c_str()};
	Fl_PNG_Image img6Debug{string("Small_Squares/6Debug.png").c_str()};
	Fl_PNG_Image img7Debug{string("Small_Squares/7Debug.png").c_str()};
	Fl_PNG_Image img8Debug{string("Small_Squares/8Debug.png").c_str()};
	Fl_PNG_Image imgBombDebug{string("Small_Squares/BombDebug.png").c_str()};
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
		void set_adj_mines(int numMines); 
		int get_adj_mines() const {return adj_mines;}
		void loss();
		void clear_damage() {pw->clear_damage();}

	private:
		bool mine;
		int adj_mines;  
		State current_state;
		Fl_PNG_Image* unclicked_img = &TileImg::imgUnclicked;
		Fl_PNG_Image* clicked_img = nullptr;  
		
		void changeImage(Fl_PNG_Image& im);
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

struct CountBox : Fl_Widget
{
	public:
		CountBox(int x, int y, int w, int h, const char * 	l = 0)
			:Fl_Widget{x,y,w,h,l},
			 t{new Text(Point{x, y+11+h/2}, "000")}, r{new Rectangle(Point{x,y}, w, h)}
		{
			t->set_font_size(30);
			t->set_font(Graph_lib::Font{Graph_lib::Font::Font_type::helvetica});
			t->set_color(Color::Color_type::red);
			r->set_color(Color{Color::Color_type::black});
			r->set_fill_color(Color{Color::Color_type::black});
		}
		~CountBox();
		void draw();
		void change_text(string s) {t->set_label(s);}
		void move(int nx, int ny);
	private:
		Rectangle* r;
		Text* t;
};

struct Counter : Widget
{
		public:
		Counter(Point xy, Callback cb)
			:Widget(xy, 50, 30, "", cb) //predetermined height and width
		{
		}
		void attach(Graph_lib::Window& win);
		void move(Point xy){static_cast<CountBox*>(pw)->move(xy.x, xy.y);}
		void set_value(int val);
		void increment_value(int change) {set_value(value+change);}
		
	private:
		int value = 0;
};

struct Option : Widget
{
public:
	Option(Point xy, int w, int h, Callback cb)
	:Widget(xy, w, h, "", cb)
	{

	}
	void attach(Graph_lib::Window& win);
	void change_size (int w, int h) {pw->resize(loc.x,loc.y,w,h);}
};

struct Game; //forward declaration

struct LevelWindow : Graph_lib::Window
{
	public:
		LevelWindow(Point xy, int w, int h, const char* st, Game* gm);
		~LevelWindow();
		int get_rows() const {rowIn->get_int();}
		int get_cols() const {colIn->get_int();}
		int get_mines() const {mineIn->get_int();}
		Game* game;
	
	private:
		In_box* rowIn=nullptr;
		In_box* colIn=nullptr;
		In_box* mineIn=nullptr;
		Button* sub=nullptr;
};

struct HelpWindow : Graph_lib::Window
{
	public:
		HelpWindow(Point xy, int w, int h, const char* st);
		~HelpWindow();
	
	private:
		Fl_Text_Display* helpTextD=nullptr;
		Fl_Text_Buffer* helpTextB=nullptr;
};

struct Game: Graph_lib::Window //make window later
{
	public:
		static constexpr int y_offset = 65;
		static constexpr int smile_side = 26;
		static constexpr int bar_height = 25;
	
		Game(Point xy, const string& title );
		~Game();
		
		static void cb_place_win(Fl_Widget*, Address);
		static void cb_beginner(Fl_Widget*, Address);
		static void cb_intermediate(Fl_Widget*, Address);
		static void cb_expert(Fl_Widget*, Address);
		static void cb_debug(Fl_Widget*, Address);
		static void cb_help(Fl_Widget*, Address);
		static void cb_custom(Address, Address);

	private:
		vector<vector<Tile*>> board; //pointer may be very bad (leak) but window does it
		Smile* smiley;
		Counter* mine_counter;
		Counter* timer;
		Option* menuBar;
		LevelWindow* wind=nullptr;
		HelpWindow* helpWin = nullptr;
		void placeWin(int x, int y, int w, int h, const char* st);
		void dispHelp();

		static void cb_change_time(Address pw);
		void change_time();
		bool game_started=false;
		bool game_over = false;
		int mine_total;
		int uncovered=0;
		
		static void cb_tile_click (Address, Address);
		static void cb_restart_click (Address, Address);

		void place_mines (int num, int row, int col); //pass start click & make private later
		void place_mine (int row, int col);
		void show_mines(int row, int col);  // which one to skip
		void clear_board();
		void create_board(int rows, int cols, int mines);
		void lose_game(int row, int col);
		void start_game(int row, int col);  //which one clicked to start
		void win_game();
		void restart_game();
		void set_level(int row, int col, int mines);
		void custom();
		//setting mines should increment mine count of those around
		void click (int row, int col);
		void show_scared();
		void left_click(int row, int col);
		void middle_click(int row, int col);
		void right_click(int row, int col);
		void toggle_debug(Fl_Widget* p);
};
#endif