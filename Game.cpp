#include "Minesweeper.h"

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
	mine_total=9; //leave for now
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
			t = board[row][col];
		} while((r==row && c==col) || t->get_mine());
		place_mine(r, c);
	}
}

void Game::place_mine (int row, int col)
{
	board[row][col]->put_mine();
	int maxRow = board.size();
	int maxCol = board[0].size();
	int right= min((row+2),maxRow);
	int bottom = min((col+2),maxCol);
	for (int r=max(row-1,0); r<right; r++)
	{
		for (int c=max(col-1,0); c<bottom; c++)
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

void Game::lose_game(int row, int col) //incomplete
{
	show_mines(row, col);
}

void Game::start_game(int row, int col) //incomplete (stuff with time & counter)
{
	place_mines(mine_total, row, col);
}

void Game::win_game() //incomplete
{
	cout<<"You win\n";
}

void Game::cb_tile_click (Address pt, Address pw )
{
	int row= reference_to<MyBox>(pt).y()/Tile::tileSide;
	int col= reference_to<MyBox>(pt).x()/Tile::tileSide;
	reference_to<Game>(pw).click(row, col);
}

void Game::click (int row, int col)  //should game start on any click or just left
{
	if(!game_started) 
	{
		start_game(row, col);
		game_started=true;
	}
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

void Game::left_click(int row, int col)
{
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
		int right= min((row+2),maxRow);
		int bottom = min((col+2),maxCol);
		for (int r=max(row-1,0); r<right; r++)
		{
			for (int c=max(col-1,0); c<bottom; c++)
			{
				if (r!= row || c != col) left_click(r,c);
			}
		}
	}
	uncovered++;
	
	if (uncovered == (maxRow*maxCol - mine_total)) win_game();
}

void Game::middle_click(int row, int col) //not written
{
}

void Game::right_click(int row, int col)
{
	Tile* t =board[row][col];
	Tile::State cur = t->get_state();
	switch (cur)
	{
		case Tile::State::clicked: break;
		case Tile::State::unclicked: t->change_state(Tile::State::flag);
									 break;
		case Tile::State::flag: t->change_state(Tile::State::question);
								break;
		case Tile::State::question: t->change_state(Tile::State::unclicked);
									break;
	}
}