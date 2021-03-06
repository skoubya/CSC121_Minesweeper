#include "Minesweeper.h"

Game::Game (Point xy, const string& title)
	:Graph_lib::Window{xy, 0,0, title}
{
	mine_counter = new Counter{Point{0,5 + bar_height},[](Address, Address){}}; //doesn't move
	attach(*mine_counter);
	timer = new Counter(Point{0, 0},[](Address, Address){}); 
	attach(*timer);
	smiley = new Smile(Point{0, 0}, cb_restart_click);
	attach(*smiley);
	menuBar = new Option(Point{0,0}, 0, bar_height, [](Address, Address pw){}); 
	attach(*menuBar);
	
	create_board(9,9,10);
	
	Fl::add_timeout(1.0, cb_change_time, this);
	Fl::run();
}

Game::~Game()
{
	if (debug) cout<<"Game destructor \n";
	int rows=board.size();
	int cols=board[0].size();
	for (int r=0; r<rows; r++)
	{
		for (int c=0; c<cols; c++)
		{
			if (debug) cout<<"still good: "<<r<<'-'<<c<<endl;
			delete board[r][c];
		}
	}
	delete smiley; //maybe not needed
	if (debug) cout<<"after smiley delete \n";
	delete mine_counter;
	if (debug) cout<<"after mine_counter delete \n";
	delete timer;
	if (debug) cout<<"after timer delete \n";
	delete menuBar;
	if (debug) cout<<"after menu_bar delete \n";
	if (wind != nullptr) delete wind;
	if (debug) cout<<"after wind delete \n";
	if (helpWin != nullptr) delete helpWin;
	if (debug) cout<<"after helpWin delete \n";
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
	}
}

void Game::place_mine (int row, int col)
{
	if (debug) cout<<"In Game: "<<debug<<endl;
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

void Game::show_mines(int row, int col) 
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
	game_started = false;
	game_over = false;
	uncovered = 0;
	if (debug) cout<<"Deleting... \n";
	while (board.size()>0)
	{
		int lastR = board.size()-1;
		while (board[lastR].size()>0)
		{
			int lastC = board[lastR].size()-1;
			if (debug) cout<<'\t'<<lastR<<':'<<lastC<<endl;
			detach(*board[lastR][lastC]);
			delete board[lastR][lastC];
			board[lastR].pop_back();
		}
		board.pop_back();
	}
	if (debug) cout<<"Done \n";
}

void Game::create_board(int rows, int cols, int mines)
{
	clear_board();
	resize(0,0); //forces redraw of tiles
	resize(cols*Tile::tileSide, rows*Tile::tileSide + y_offset);
	resizable(NULL);
	size_range(x_max(),y_max(),x_max(),y_max());
	
	mine_counter->set_value(mines);
	
	smiley->move(Point{(x_max()-smile_side)/2, (y_offset-smile_side+ bar_height)/2 });
	smiley->change_image(TileImg::imgSmile);
	
	timer->move(Point{x_max()-(timer->width), 5 + bar_height});
	timer->set_value(0);
	
	menuBar->change_size(x_max(), bar_height);
	
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
	clear_damage(); //prevents repeat 
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
	int rMax=board.size();
	int cMax=board[0].size();
	for (int r=0; r<rMax; r++)
	{
		for (int c = 0; c<cMax; c++)
		{
			if (board[r][c]->get_state() == Tile::State::unclicked)
			{
				board[r][c]->change_state(Tile::State::unclicked);
			}
		}
	}
}

void Game::win_game() //incomplete (maybe separate into different functions)
{
	int rMax = board.size();
	int cMax = board[0].size();
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
		timer->set_value(0);
		damage(FL_DAMAGE_CHILD);
		int rMax = board.size();
		int cMax = board[0].size();
		for (int r = 0; r < rMax; r++)
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
	else if (Fl::event_button()!=FL_RIGHT_MOUSE)
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
		case FL_LEFT_MOUSE: if (Fl::event_buttons()==FL_BUTTON3) middle_click(row, col);
							else left_click(row, col);
							break;
		case FL_MIDDLE_MOUSE: middle_click(row, col);
							  break;
		case FL_RIGHT_MOUSE: if (Fl::event_buttons()==FL_BUTTON1) middle_click(row, col);
							 else right_click(row, col);
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

void Game::middle_click(int row, int col) //maybe inefficient
{
	if (board[row][col]->get_state() != Tile::State::clicked) return;  //do nothing
	int maxRow = board.size();
	int maxCol = board[0].size();
	int bottom= min((row+2),maxRow);
	int right = min((col+2),maxCol);
	int flags = 0;
	for (int r=max(row-1,0); r<bottom; r++)
	{
		for (int c=max(col-1,0); c<right; c++)
		{
			if (board[r][c]->get_state() == Tile::State::flag) flags++;
		}
	}
	
	if (flags == board[row][col]->get_adj_mines())
	{
		for (int r=max(row-1,0); r<bottom; r++)
		{
			for (int c=max(col-1,0); c<right; c++)
			{
				left_click(r,c);
			}
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
									 damage(FL_DAMAGE_CHILD);
									 break;
		case Tile::State::flag: t->change_state(Tile::State::question);
								mine_counter->increment_value(1);
								damage(FL_DAMAGE_CHILD);
								break;
		case Tile::State::question: t->change_state(Tile::State::unclicked);
									break;
	}
}

void Game::cb_change_time(Address pw)
{
	reference_to<Game>(pw).change_time();
	Fl::repeat_timeout(1.0, cb_change_time, pw);
}

void Game::cb_place_win(Fl_Widget*, Address pw)
{
	reference_to<Game>(pw).placeWin(100, 200, 200, 200, "Set Custom Level");
}

void Game::change_time()
{
	if (game_started && !game_over)
	{
		timer->increment_value(1);
		damage(FL_DAMAGE_CHILD);
	}
}

void Game::cb_custom(Address, Address pw)
{
	reference_to<LevelWindow>(pw).game->custom();
}

void Game::cb_beginner(Fl_Widget*, Address pw)
{
	reference_to<Game>(pw).create_board(9,9,10);
}

void Game::cb_intermediate(Fl_Widget*, Address pw)
{
	reference_to<Game>(pw).create_board(16,16,40);
}

void Game::cb_expert(Fl_Widget*, Address pw)
{
	reference_to<Game>(pw).create_board(16,30,99);
}

void Game::cb_debug(Fl_Widget* p, Address pw)
{
	reference_to<Game>(pw).toggle_debug(p);
}

void Game::toggle_debug(Fl_Widget* p)
{
	if (debug)
	{
		debug = false;
		reference_to<Fl_Menu_Bar>(p).replace(7, "Toggle debug on");
	}
	else
	{
		debug = true;
		reference_to<Fl_Menu_Bar>(p).replace(7, "Toggle debug off");
	}
	
	//refreshed images
	int rMax = board.size();
	int cMax = board[0].size();
	for (int r = 0; r < rMax; r++)
	{
		for (int c = 0; c<cMax; c++)
		{
			Tile* t= board[r][c];
			t->set_adj_mines(t->get_adj_mines());
			if (t->get_mine()) t->put_mine(true);
			t->change_state(t->get_state());
		}
	}
}

void Game::cb_help(Fl_Widget* p, Address pw)
{
	reference_to<Game>(pw).dispHelp();
}

void Game::dispHelp()
{
	if (helpWin == nullptr)
	{
		helpWin = new HelpWindow(Point{50,50}, 300, 200, "Help");
	}
	helpWin->show();
}

void Game::custom()
{
	int row = wind->get_rows();
	int col = wind->get_cols();
	int mine = wind->get_mines();
	if(row<10) row = 10;
	else if (row>31) row = 31;
	if (col<10) col = 10;
	else if (col>32) col = 32;
	if (mine<1) mine = 1;
	else if (mine>((row-1)*(col-1))) mine=(row-1)*(col-1);

	create_board(row, col, mine);
	wind->hide();
}

void Game::placeWin(int x, int y, int w, int h, const char* st)
{
	if (wind == nullptr)
	{
		wind = new LevelWindow(Point{x,y}, w, h, st, this);
	}
	wind->show();
}