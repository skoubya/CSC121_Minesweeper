#include "Minesweeper.h"

int MyBox::handle(int event)
{
	if (event==FL_PUSH || event==FL_RELEASE) 
	{
		do_callback();
		clear_changed();
		return 1;
	}
	return 0;
}

void Tile::change_state(State s) 
{
	switch(s)
	{
		case State::unclicked: changeImage(*unclicked_img);
							   break;
		case State::clicked: changeImage(*clicked_img);
							 break;
		case State::flag: changeImage(TileImg::imgFlag);
						  break;
		case State::question: changeImage(TileImg::imgQuestionMark);
							  break;
	}
	current_state = s;
}

void Tile::attach(Graph_lib::Window& win)
{
	pw = new MyBox(loc.x, loc.y, width, height, label.c_str());
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
    own = &win;
	pw->align(FL_ALIGN_IMAGE_BACKDROP);
	change_state(State::unclicked);
}

void Tile::put_mine(bool put)
{
	mine= put;
	if(mine) clicked_img=&TileImg::imgRedBomb;
	else clicked_img=&TileImg::imgBlank;
	
	if (debug)
	{
		if (mine) unclicked_img = &TileImg::imgBombDebug;
		else unclicked_img = &TileImg::imgUnclicked;
	}
}

void Tile::add_adj_mines()
{
	set_adj_mines(adj_mines+1);
}

void Tile::set_adj_mines(int numMines)
{
	adj_mines=numMines;
	switch (adj_mines)
	{
		case 0: clicked_img = &TileImg::imgBlank;
				break;
		case 1: clicked_img = &TileImg::img1;
				if(debug) unclicked_img = &TileImg::img1Debug;
				break;
		case 2: clicked_img = &TileImg::img2;
				if(debug) unclicked_img = &TileImg::img2Debug;
				break;
		case 3: clicked_img = &TileImg::img3;
				if(debug) unclicked_img = &TileImg::img3Debug;
				break;
		case 4: clicked_img = &TileImg::img4;
				if(debug) unclicked_img = &TileImg::img4Debug;
				break;
		case 5: clicked_img = &TileImg::img5;
				if(debug) unclicked_img = &TileImg::img5Debug;
				break;
		case 6: clicked_img = &TileImg::img6;
				if(debug) unclicked_img = &TileImg::img6Debug;
				break;
		case 7: clicked_img = &TileImg::img7;
				if(debug) unclicked_img = &TileImg::img7Debug;
				break;
		case 8: clicked_img = &TileImg::img8;
				if(debug) unclicked_img = &TileImg::img8Debug;
				break;
	}
}

void Tile::loss()
{
	if(mine && current_state!= State::flag)
	{
		clicked_img = &TileImg::imgBomb;
		change_state(State::clicked);
	}
	else if (!mine && current_state== State::flag)
	{
		clicked_img = &TileImg::imgXBomb;
		change_state(State::clicked);
	}
}

void Tile::changeImage(Fl_PNG_Image& im)
{
	pw->image(im);
	pw->redraw(); //maybe get rid of later
}

void Smile::change_image(Fl_PNG_Image& im)
{
	pw->image(im);
	pw->redraw(); //maybe get rid of later
}

void Smile::attach(Graph_lib::Window& win)
{
	pw = new MyBox(loc.x, loc.y, width, height, label.c_str());
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
    own = &win;
	pw->align(FL_ALIGN_IMAGE_BACKDROP);
	change_image(TileImg::imgSmile);
}

CountBox::~CountBox()
{
	delete r;
	delete t;
}

void CountBox::draw()
{	
	r->draw();
	t->draw();
}

void CountBox::move(int nx, int ny)
{
	Point p =r->point(0);
	t->move(nx-p.x,ny-p.y); //maybe inefficent
	r->move(nx-p.x,ny-p.y);
	Fl_Widget::position(nx,ny);
	redraw();
}

void Counter::attach(Graph_lib::Window& win)
{
	pw = new CountBox(loc.x, loc.y, width, height, label.c_str());
    pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
    own = &win;
}

void Counter::set_value(int val)
{
	if (val>999) val=999;  //keep 3 digits
	else if (val< -99) val=-99;
	value  = val;
	ostringstream os;
	os<<val;
	string s = os.str();
	while (s.size()<3) 
	{
		if(s[0]=='-') s.insert(1,"0");
		else s.insert(0,"0");
	}
	static_cast<CountBox*>(pw)->change_text(s);
	pw->redraw();
}