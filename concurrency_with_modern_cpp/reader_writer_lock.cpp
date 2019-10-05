// reader writer locks

#include <iostream>
#include <map>
#include <shared_mutex>
#include <string>
#include <thread>

using namespace std;
using std::chrono::milliseconds;

map<string, int> tele_book{
	{"Dijkstra", 1972},
	{"Scott", 1976},
	{"Ritchie", 1983}};

shared_timed_mutex tele_book_locker;

void add_to_tele_book(string const & na, int tele)
{
	lock_guard<shared_timed_mutex> writer_lock{tele_book_locker};
	cout << "\nSTARTING UPDATE " << na;
	this_thread::sleep_for(milliseconds{500});
	tele_book[na] = tele;
	cout << " ... ENDING UPDATE " << na << endl;
}

void print_number(string const & na)
{
	shared_lock<shared_timed_mutex> reader_lock{tele_book_locker};

/* note: Data race there (two threads access tele_book and one of them is writer)
operator[] can modify tele_book, we need to use find() instead. */
//	cout << na << ": " << tele_book[na];

	auto it = tele_book.find(na);
	if (it != tele_book.end())
		cout << it->first << ": " << it->second << endl;
	else
		cout << na << " not found!" << endl;
}

int main(int argc, char * argv[])
{
	cout << endl;

	thread reader1{[]{print_number("Scott");}};
	thread reader2{[]{print_number("Ritchie");}};
	thread w1{[]{add_to_tele_book("Scott", 1968);}};
	thread reader3{[]{print_number("Dijkstra");}};
	thread reader4{[]{print_number("Scott");}};
	thread w2{[]{add_to_tele_book("Bjarne", 1965);}};
	thread reader5{[]{print_number("Scott");}};
	thread reader6{[]{print_number("Ritchie");}};
	thread reader7{[]{print_number("Scott");}};
	thread reader8{[]{print_number("Bjarne");}};

	reader1.join();
	reader2.join();
	reader3.join();
	reader4.join();
	reader5.join();
	reader6.join();
	reader7.join();
	reader8.join();
	w1.join();
	w2.join();

	cout << endl;

	cout << "\nthe new telephone book" << endl;
	for (auto it : tele_book)
		cout << it.first << ": " << it.second << endl;

	cout << "done!\n";

	return 0;
}
