// spracovananie touch eventou
#include <algorithm>
#include <list>
#include <stdexcept>
#include <iostream>
#include <glm/vec2.hpp>

using std::list;
using std::find_if;
using std::cout;
using glm::ivec2;

int touch_list_finger_cast(int touch_event_action);

struct touch_event
{
	enum {down, move, up, canceled};  // action states
	ivec2 position;
	int action;
	int finger_id;
};

class touch_list 
{
public:
	struct finger 
	{
		enum {down = 1, move = 2, up = 4, canceled = 8};  // finger states
		ivec2 position;
		int id;
		int state;  //!< \note kombinacia hodnot down, move, up a canceled
	};
	
	using iterator = std::list<finger>::iterator;
	
	void insert(touch_event const & te);
	iterator begin() {return _fingers.begin();}
	iterator end() {return _fingers.end();}
	
private:
	std::list<finger> _fingers;
};

void touch_list::insert(touch_event const & te)
{
	auto it = find_if(_fingers.begin(), _fingers.end(), 
		[&te](finger const & f){return f.id == te.finger_id;});
	
	if (it == _fingers.end())  // unikatny event
	{
		finger f;
		f.position = te.position;
		f.id = te.finger_id;
		f.state = touch_list_finger_cast(te.action);
		_fingers.push_back(f);
	}
	else  // uz existujuci event
	{
		if (te.action == touch_event::up)  // ak je action up, zmaz event
			_fingers.erase(it);
		else
			it->state |= touch_list_finger_cast(te.action);  // inak uprav stav
	}
}

class touch_input 
{
public:
	touch_list & fingers() {return _touches;}

	void touch_performed(int x, int y, int finger_id, int action);  //!< internl use only
	
private:
	touch_list _touches;
};

void touch_input::touch_performed(int x, int y, int finger_id, int action)
{
	touch_event te;
	te.position = ivec2{x,y};
	te.action = action;
	te.finger_id = finger_id;
	_touches.insert(te);
}

int touch_list_finger_cast(int touch_event_action)
{
	switch (touch_event_action)
	{
		case touch_event::down: return touch_list::finger::down;
		case touch_event::move: return touch_list::finger::move;
		case touch_event::up: return touch_list::finger::up;
		case touch_event::canceled: return touch_list::finger::canceled;
		default:
			throw std::logic_error{"unknown touch_event"};
	}
}

struct mouse_input {};

struct keyboard_input {};

struct user_input
{
	mouse_input mouse;
	touch_input touch;
	keyboard_input keyboard;
	// joystick
};

user_input in;

void input()
{
	for (touch_list::finger & f : in.touch.fingers())
		if (f.state & (touch_list::finger::down|touch_list::finger::move))
			cout << "finger down or move" << std::endl;
}


int main(int argc, char * argv[])
{
	in.touch.touch_performed(0,0,0,(int)touch_event::down);
	input();
	return 0;
}
