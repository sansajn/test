// constexpr map implementation from C++ Weekly Ep. 233
#include <array>
#include <utility>
#include <string>
#include <stdexcept>
#include <string_view>
#include <algorithm>

using std::array, std::pair, std::string, std::string_view, std::range_error, std::find_if;
using namespace std::string_literals;
using namespace std::string_view_literals;

template <typename Key, typename Value, std::size_t Size>
struct cmap {
	array<pair<Key, Value>, Size> data;

	constexpr Value at(Key const & key) const {
		auto const it = find_if(begin(data), end(data), [&key](auto const & v){return v.first == key;});
		if (it != end(data))
			return it->second;
		else
			throw range_error("Not Found");
	}
};

int main(int argc, char * argv[]) {
	array<pair<int, string>, 10> data = {{
		{1, "jedna"s}
	}};

	cmap<int, string, 10> m1{data};

	cmap<int, string, 10> m2 = {{{
		{1, "jedna"s}
	}}};

	constexpr cmap<int, string_view, 10> m3 = {{{
		{1, "jedna"sv},
		{2, "dva"sv},
		{3, "tri"sv}
	}}};

	return m3.at(2).size();  // 3
}