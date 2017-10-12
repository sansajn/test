// info o hodinach (clock)
#include <chrono>
#include <iostream>
#include <iomanip>

template <typename C>
void print_clock_data()
{
	using namespace std;
	
	cout << "- precision: ";
	typedef typename C::period P;
	if (ratio_less_equal<P, milli>::value)
	{
		typedef typename ratio_multiply<P, kilo>::type TT;
		cout << fixed << double(TT::num)/TT::den << " milliseconds" << endl;
	}
	else
		cout << fixed << double(P::num)/P::den << " seconds" << endl;

	cout << "- is_steady: " << boolalpha << C::is_steady << endl;
}

int main()
{
	using namespace std;
	
	cout << "system_clock: " << endl;
	print_clock_data<chrono::system_clock>();

	cout << "\nhigh_resolution_clock: " << endl;
	print_clock_data<chrono::high_resolution_clock>();

	cout << "\nsteady_slock: " << endl;
	print_clock_data<chrono::steady_clock>();

	return 0;
}
