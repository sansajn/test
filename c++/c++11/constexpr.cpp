#include <cstdint>

#define sfr8(adr) (*(uint8_t *)(adr))

enum timer_clock_source
{
	stopped,
	clk_io,  // cpu clock
	clk_io8
};

template <typename Trait>
struct basic_timer8
{
	uint8_t & value;

	basic_timer8() : value(sfr8(Trait::tcnt_adr)) {}
	basic_timer8(timer_clock_source c) : value(sfr8(Trait::tcnt_adr)) {clock(c);}
	void clock(timer_clock_source c) const {sfr8(Trait::tccr_adr) |= c;}
//	void enable_overflow_interrupt() const {TIMSK |= (1 << Trait::toie);}
};

struct timer0_trait
{
	static int const tcnt_adr = 0x32;
	static int const tccr_adr = 0x33;
	static int const toie = 0x33;
};

using timer0 = basic_timer8<timer0_trait>;

int main(int argc, char * argv[])
{
	timer0 t0;
	return 0;
}
