#include <vector>
#include <iostream>

using std::vector;

enum class door_states {closed, opening, opened, closing, invalid};

struct state
{
	virtual void enter() {}
	virtual door_states update(float dt) {return door_states::invalid;}
	virtual void exit() {}
};

struct door_close : public state
{
	void enter() override {std::cout << "door_close::enter()" << std::endl;}
	door_states update(float dt) override {return door_states::invalid;}
	void exit() override {std::cout << "door_close::exit()" << std::endl;}
};

struct door_openning : public state
{
	void enter() override {std::cout << "door_openning::enter()" << std::endl;}
	door_states update(float dt) override {return door_states::opened;}
	void exit() override {std::cout << "door_openning::exit()" << std::endl;}
};

struct door_open : public state
{
	void enter() override {std::cout << "door_open::enter()" << std::endl;}
	door_states update(float dt) override {return door_states::closing;}
	void exit() override {std::cout << "door_open::exit()" << std::endl;}
};

struct door_closing : public state
{
	void enter() override {std::cout << "door_closing::enter()" << std::endl;}
	door_states update(float dt) override {return door_states::closed;}
	void exit() override {std::cout << "door_closing::exit()" << std::endl;}
};

template <typename Impl, typename Desc>
struct state_machine
{
	using state_descriptor = Desc;
	
	state_machine() {}
	
	void update(float dt) {
		state_descriptor s = impl()->to_ref(_cur).update(dt);
		if (s != Impl::invalid_descriptor)
			change_state(s);
	}
	
	void change_state(state_descriptor s) {
		impl()->to_ref(_cur).exit();
		_cur = s;
		impl()->to_ref(_cur).enter();
	}
	
	void set_state(state_descriptor s) {
		_cur = s;
		impl()->to_ref(_cur).enter();
	}

	Impl * impl() {return static_cast<Impl *>(this);}
	
	state_descriptor _cur;
};

struct door;

struct door_state_machine : public state_machine<door_state_machine, door_states>
{
	using state_descriptor = door_states;
	
	static state_descriptor const invalid_descriptor = door_states::invalid;
	door_state_machine() {}
	
	door_state_machine(door * d) : _door{d} {
		fill_states();
		set_state(door_states::closed);  // _states must be initialized before
	}
	
	state & to_ref(state_descriptor s) {return *_states[(int)s];}

	door_state_machine(door_state_machine const & other)
		: _door{other._door}
		, _closed{other._closed}
		, _opening{other._opening}
		, _opened{other._opened}
		, _closing{other._closing}
	{
		fill_states();
	}

	void fill_states() {
		_states[(int)door_states::closed] = &_closed;
		_states[(int)door_states::opening] = &_opening;
		_states[(int)door_states::opened] = &_opened;
		_states[(int)door_states::closing] = &_closing;
	}
	
	door * _door;
	door_close _closed;
	door_openning _opening;
	door_open _opened;
	door_closing _closing;
	state * _states[4];
};

struct door {
	door() : _state{this} {}
	door(door && other) : _state{other._state} {}
	void update(float dt) {_state.update(dt);}
	void open() {_state.set_state(door_states::opening);}
	door(door const &) = delete;
	door & operator=(door const &) = delete;
	door_state_machine _state;
};


int main(int argc, char * argv[])
{
	vector<door> _doors;
	for (int i = 0; i < 15; ++i)
		_doors.push_back(door{});

	door & d = _doors.front();
	d.open();
	while (true)
		d.update(10);
	return 0;
}
