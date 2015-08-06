#include <iostream>

using std::cout;

template <typename Window>
class free_move
{
public:
	free_move(Window & w) : _wnd(w) {}
	virtual void controls() {cout << "free_move::controls" << std::endl;}
	
private:
	Window _wnd;
};

template <typename Window>
class map_move : public free_move<Window>
{
public:
	using base = free_move<Window>;
	
	map_move(Window & w);
};

template <typename Window>
map_move<Window>::map_move(Window & w) : base(w)
{
	base::controls();
}

class window {};

int main(int argc, char * argv[])
{
	window w;
	map_move<window> movectrl(w);
	movectrl.controls();
	
	return 0;
}