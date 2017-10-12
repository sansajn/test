// konecny stavovy stroj pre herne objekty
#include <iostream>
#include <cassert>

template <typename M>  // M for state map
struct state_machine
{
	using state_descriptor = typename M::state_descriptor;
	
	state_machine(M * mapper) : _mapper{mapper} {
		_cur = _mapper->initial_state();
		(*_mapper)(_cur).enter();
	}
	
	void update(float dt) {
		state_descriptor res = (*_mapper)(_cur).update(dt);
		if (res != M::invalid_descriptor)
			change_state(res);
	}
	
	void change_state(state_descriptor s) {
		(*_mapper)(_cur).exit();
		_cur = s;
		(*_mapper)(_cur).enter();
	}
	
	void set_state(state_descriptor s) {
		_cur = s;
		(*_mapper)(_cur).enter();
	}
	
	M * _mapper;
	state_descriptor _cur;
};


enum class door_states {close, openning, open, closing, invalid};

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
	door_states update(float dt) override {return door_states::open;}
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
	door_states update(float dt) override {return door_states::close;}
	void exit() override {std::cout << "door_closing::exit()" << std::endl;}
};

struct door_state_mapper  // mapper je moveable tzn. state_machine je moveable
{
	using state_descriptor = door_states;
	static state_descriptor const invalid_descriptor = door_states::invalid;
	state_descriptor initial_state() {return door_states::close;}
	
	state & operator()(state_descriptor s) {
		switch (s) {
			case door_states::close: return _close;
			case door_states::openning: return _openning;
			case door_states::open: return _open;
			case door_states::closing: return _closing;
			default: throw std::runtime_error{"bad cast"};
		}
	}
	
	door_close _close;
	door_openning _openning;
	door_open _open;
	door_closing _closing;
};

struct door_state_array_mapper
{
	using state_descriptor = int;
	
	static int const invalid_descriptor = (int)door_states::invalid;
	
	door_state_array_mapper() {
		_states[(int)door_states::close] = &_close;
		_states[(int)door_states::openning] = &_openning;
		_states[(int)door_states::open] = &_open;
		_states[(int)door_states::closing] = &_closing;
	}
	
	state_descriptor initial_state() {return (int)door_states::close;}
	
	state & operator()(state_descriptor s) {
		assert(s < (int)door_states::invalid && "out of range");
		return *_states[s];
	}
	
	door_close _close;
	door_openning _openning;
	door_open _open;
	door_closing _closing;
	state * _states[4];
};


struct door {
	door() : _state{&_state_map} {}
	void update(float dt) {_state.update(dt);}
	void open() {_state.set_state(door_states::openning);}
	
	door_state_mapper _state_map;
	state_machine<door_state_mapper> _state;
};


int main(int argc, char * argv[])
{
	door d;
	d.open();
	while (true)
		d.update(10);
	return 0;
}




