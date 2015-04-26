#include "Minesweeper.h"

Game::Game (Point xy, const string& title)
	:Graph_lib::Window{xy, 0,0, title}
{
	mine_counter = new Counter{Point{0,5}, 50, y_offset-10}; //doesn't move
	attach(*mine_counter);
	timer = new Counter(Point{0, 0}, 50, y_offset-10); 
	attach(*timer);
	smiley = new Smile(Point{0, 0}, cb_restart_click);
	attach(*smiley);
	create_board(10,10,9);
	
	create_board(16, 30, 99);
	Fl::run();
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
	delete smiley; //maybe not needed
}

void Game::place_mines(int num, int row, int col)
{
	srand (time(NULL)); //starts rand in different seeds
	Tile* t = nullptr;
	int r =0;
	int c =0;
	for (int x =0; x<num; x++)
	{
		do
		{
			r = rand() % board.size();
			c = rand() % board[0].size();
			t = board[r][c];
		} while((r==row && c==col) || t->get_mine());
		place_mine(r, c);
		if (debug) t->change_state(Tile::State::unclicked);
	}
}

void Game::place_mine (int row, int col)
{
	board[row][col]->put_mine(true);
	int maxRow = board.size();
	int maxCol = board[0].size();
	int bottom= min((row+2),maxRow);
	int right = min((col+2),maxCol);
	for (int r=max(row-1,0); r<bottom; r++)
	{
		for (int c=max(col-1,0); c<right; c++)
		{
			Tile* temp = board[r][c];
			if (!(temp->get_mine()))
			{
				temp->add_adj_mines();
			}
		}
	}
}

void Game::show_mines(int row, int col) //maybe change way work (not great for debug)
{
	int rMax=board.size();
	int cMax=board[0].size();
	for (int r=0; r<rMax; r++)
	{
		for (int c = 0; c<cMax; c++)
		{
			if (r!=row || c!=col)
			{
				board[r][c]->loss();
			}
		}
	}
}

void Game::clear_board()
{
	while (board.size()>0)
	{
		int lastR = board.size()-1;
		while (board[lastR].size()>0)
		{
			int lastC = board[lastR].size()-1;
			detach(*board[lastR][lastC]);
			delete board[lastR][lastC];
			board[lastR].pop_back();
		}
		board.pop_back();
	}
}

void Game::create_board(int rows, int cols, int mines)
{
	clear_board();
	
	resize(cols*Tile::tileSide, rows*Tile::tileSide + y_offset);
	resizable(NULL);
	size_range(x_max(),y_max(),x_max(),y_max());
	
	mine_counter->set_value(mines);
	
	smiley->move(Point{(x_max()-26)/2, (y_offset-26)/2});
	
	timer->move(Point{x_max()-50, 5});
	
	for (int r=0; r<rows; r++)
	{
		board.push_back(vector<Tile*>{});
		for (int c=0; c<cols; c++)
		{
			board[r].push_back(new Tile(Point{c*Tile::tileSide,r*Tile::tileSide+y_offset},cb_tile_click));
			attach(*board[r][c]);
		}
		
	}
	mine_total=mines;
}

void Game::lose_game(int row, int col) //incomplete
{
	show_mines(row, col);
	smiley->change_image(TileImg::imgDead);
	game_over = true;
}

void Game::start_game(int row, int col) //incomplete (stuff with time & counter)
{
	place_mines(mine_total, row, col);
}

void Game::win_game() //incomplete (maybe separate into different functions)
{
	int rMax=board.size();
	int cMax=board[0].size();
	for (int r=0; r<rMax; r++)
	{
		for (int c = 0; c<cMax; c++)
		{
			if (board[r][c]->get_mine())
			{
				board[r][c]->change_state(Tile::State::flag);
			}
		}
	}
	smiley->change_image(TileImg::imgCool);
	game_over = true;
}

void Game::restart_game()
{
	if (Fl::event() == FL_PUSH)
	{
		smiley->change_image(TileImg::imgSmileDown);
	}
	else
	{
		mine_counter->set_value(mine_total);
		int rMax=board.size();
		int cMax=board[0].size();
		for (int r=0; r<rMax; r++)
		{
			for (int c = 0; c<cMax; c++)
			{
				board[r][c]->set_adj_mines(0);
				board[r][c]->put_mine(false);
				board[r][c]->change_state(Tile::State::unclicked);
				game_over = false;
				game_started = false;
				uncovered=0;
			}
		}
		smiley->change_image(TileImg::imgSmile);
	}
}

void Game::cb_tile_click (Address pt, Address pw )
{
	if (Fl::event() == FL_RELEASE)
	{
		int row= (reference_to<MyBox>(pt).y()-y_offset)/Tile::tileSide;
		int col= reference_to<MyBox>(pt).x()/Tile::tileSide;
		reference_to<Game>(pw).click(row, col);
	}
	else
	{
		reference_to<Game>(pw).show_scared();
	}
}

void Game::cb_restart_click (Address, Address pw)
{
	reference_to<Game>(pw).restart_game();
}

void Game::click (int row, int col) 
{
	if (game_over) return;
	smiley->change_image(TileImg::imgSmile);
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

void Game::show_scared()
{
	if(!game_over) smiley->change_image(TileImg::imgScared);
}

void Game::left_click(int row, int col)
{
	if(!game_started) 
	{
		start_game(row, col);
		game_started=true;
	}
	Tile* t =board[row][col];
	Tile::State curr = t->get_state();
	if (curr==Tile::State::flag || curr==Tile::State::clicked)
	{
		return; //do nothing
	}
	t->change_state(Tile::State::clicked);
	if (t->get_mine()) 
	{
		lose_game(row, col);
		return;
	}
	
	
	int maxRow = board.size();
	int maxCol = board[0].size();
	//seems very inefficient (also in place_mine(row, col))
	if (t->get_adj_mines()==0)
	{
		int bottom= min((row+2),maxRow);
		int right = min((col+2),maxCol);
		for (int r=max(row-1,0); r<bottom; r++)
		{
			for (int c=max(col-1,0); c<right; c++)
			{
				left_click(r,c); //no infinite loop because return if clicked
			}
		}
	}
	uncovered++;
	
	if (uncovered == (maxRow*maxCol - mine_total)) win_game();
}

void Game::middle_click(int row, int col) //not written
{
	int maxRow = board.size();
	int maxCol = board[0].size();
	int bottom= min((row+2),maxRow);
	int right = min((col+2),maxCol);
	for (int r=max(row-1,0); r<bottom; r++)
	{
		for (int c=max(col-1,0); c<right; c++)
		{
			left_click(r,c);
		}
	}
}

void Game::right_click(int row, int col)
{
	Tile* t =board[row][col];
	Tile::State cur = t->get_state();
	switch (cur)
	{
		case Tile::State::clicked: break;
		case Tile::State::unclicked: t->change_state(Tile::State::flag);
									 mine_counter->increment_value(-1);
									 break;
		case Tile::State::flag: t->change_state(Tile::State::question);
								mine_counter->increment_value(1);
								break;
		case Tile::State::question: t->change_state(Tile::State::unclicked);
									break;
	}
}

void Counter::draw_lines() const
{
	Shape::draw_lines();
	t->set_font_size(30);
	t->set_font(Graph_lib::Font{Graph_lib::Font::Font_type::helvetica});
	t->set_color(Color::Color_type::red);
	r->set_color(Color{Color::Color_type::black});
	r->set_fill_color(Color{Color::Color_type::black});
	
	r->draw();
	t->draw();
}

void Counter::move(Point xy)
{
	t->move(xy.x-point(0).x,xy.y-point(0).y); //maybe inefficent
	r->move(xy.x-point(0).x,xy.y-point(0).y);
	set_point(0, xy);
}

void Counter::set_value(int val)
{
	value  = val;
	ostringstream os;
	os<<val;
	string s = os.str();
	while (s.size()<3) s.insert(0,"0");
	t->set_label(s);
	//draw_lines();
}