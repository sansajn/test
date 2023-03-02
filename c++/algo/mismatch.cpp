// mismatch algorithm usage sample
#include <algorithm>
#include <algorithm>
#include <string>
#include <iostream>

using std::mismatch, std::distance, std::string, std::cout;

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	string const a = "abcdefg",
		b = "abcdfg";
	
	auto const mismatch_pair = mismatch(begin(a), end(a), begin(b), end(b));
	ptrdiff_t const mismatch_pos = distance(begin(a), mismatch_pair.first);  //= 4
	cout << "mismatch-pos=" << mismatch_pos << '\n';
	cout << a << '\n'
		<< string(mismatch_pos, ' ') << '^' << '\n';

	cout << '"' << a << '"' << " mismatched part " << string{mismatch_pair.first, end(a)} << "\n"
		<< '"' << b << '"' << " mismatched part " << string{mismatch_pair.second, end(b)} << "\n";
	
	cout << "done!\n";
	return 0;
}
