// stavovy stroj pre herne objekty
#include <vector>
#include <iostream>

using std::vector;

template <typename Impl, typename Obj, typename Desc>
class state_machine
{
public:
	using state_descriptor = Desc;

	state_machine(state_descriptor s) {
		_cur = s;
		_next = Impl::invalid_descriptor;
	}

	void update(float dt, Obj * obj) {
		if (_cur != _next) {
			if (_next != Impl::invalid_descriptor) {
				change_state(_next, obj);
			}
			else {  // after creation
				_next = _cur;
				impl()->to_ref(_cur).enter(obj);
			}
		}

		state_descriptor s = impl()->to_ref(_cur).update(dt, obj);
		if (s != Impl::invalid_descriptor)
			_next = s;
	}

	void enqueue_state(state_descriptor s) {_next = s;}
	state_descriptor current_state() const {return _cur;}

private:
	void change_state(state_descriptor s, Obj * obj) {
		impl()->to_ref(_cur).exit(obj);
		_cur = s;
		impl()->to_ref(_cur).enter(obj);
	}

	Impl * impl() {return static_cast<Impl *>(this);}

	state_descriptor _cur, _next;
};



enum class door_states {close, opening, open, closing, invalid};

struct door;

struct state
{
	virtual void enter(door *) {}
	virtual door_states update(float dt, door *) {return door_states::invalid;}
	virtual void exit(door *) {}
};

struct door_close : public state
{
	void enter(door *) override {std::cout << "door_close::enter()" << std::endl;}
	door_states update(float dt, door *) override {return door_states::invalid;}
	void exit(door *) override {std::cout << "door_close::exit()" << std::endl;}
};

struct door_opening : public state
{
	void enter(door *) override {std::cout << "door_opening::enter()" << std::endl;}
	door_states update(float dt, door *) override {return door_states::open;}
	void exit(door *) override {std::cout << "door_opening::exit()" << std::endl;}
};

struct door_open : public state
{
	void enter(door *) override {std::cout << "door_open::enter()" << std::endl;}
	door_states update(float dt, door *) override {return door_states::closing;}
	void exit(door *) override {std::cout << "door_open::exit()" << std::endl;}
};

struct door_closing : public state
{
	void enter(door *) override {std::cout << "door_closing::enter()" << std::endl;}
	door_states update(float dt, door *) override {return door_states::close;}
	void exit(door *) override {std::cout << "door_closing::exit()" << std::endl;}
};

struct door;

class door_state_machine
	: public state_machine<door_state_machine, door, door_states>
{
public:
	using state_descriptor = door_states;
	static state_descriptor const invalid_descriptor = door_states::invalid;

	door_state_machine() : state_machine{door_states::close} {
		fill_states();
	}

	void enter_open_sequence() {
		if (current_state() == door_states::close || current_state() == door_states::invalid)  // TODO: tuto dvojznacnost treba poriesit (pociatocny stav)
			enqueue_state(door_states::opening);
	}

	state & to_ref(state_descriptor s) {return *_states[(int)s];}

private:
	void fill_states() {
		_states[(int)door_states::close] = &_closed;
		_states[(int)door_states::opening] = &_opening;
		_states[(int)door_states::open] = &_opened;
		_states[(int)door_states::closing] = &_closing;
	}

	door_close _closed;
	door_opening _opening;
	door_open _opened;
	door_closing _closing;
	state * _states[4];
};

struct door {
	door() {}
	door(door && other) : _state{other._state} {}
	void update(float dt) {_state.update(dt, this);}
	void open() {_state.enter_open_sequence();}
	door(door const &) = delete;
	door & operator=(door const &) = delete;
	door_state_machine _state;
};


int main(int argc, char * argv[])
{
	vector<door> _doors;
	for (int i = 0; i < 3; ++i)
		_doors.push_back(door{});

	door & d = _doors.front();
	d.update(10);
	d.open();
	while (true)
		d.update(10);

	return 0;
}

