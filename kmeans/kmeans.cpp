/*! k-means algorithm implementation based on [article](https://www.codeproject.com/Articles/5256294/Classifying-Data-Using-Artificial-Intelligence-K-M) */
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

using std::count;
using std::string;
using std::getline;
using std::vector;
using std::size;
using std::pair; 
using std::make_pair;
using std::ifstream;
using std::istringstream;
using std::invalid_argument;
using std::runtime_error;
using std::cout;

using table_type = vector<pair<vector<double>, int>>;

table_type load_data_from_file(string const & fname)
{
	ifstream fin{fname};
	if (!fin.is_open())
		throw invalid_argument{"unable to open \"" + fname + "\" file"};

	// N columns of features as floats and label as integer
	vector<pair<vector<double>, int>> data;
	size_t columns = 0;
	istringstream line_ss;

	string line;
	while (getline(fin, line))
	{
		if (columns == 0)
			columns = count(begin(line), end(line), ',') - 1;

		if (columns < 1)
			throw runtime_error{"unable to parse table record (\"" + line + "\""};  // wrong number of columns

		vector<double> row;
		row.resize(columns);

		line_ss.str(line);

		for (size_t i = 0; i < columns; ++i)
			line_ss >> row[i];

		int label;
		line_ss >> label;

		data.push_back(make_pair(row, label));
	}

	return data;
}


int main(int argc, char * argv[])
{
	table_type dataset = load_data_from_file("dataset.csv");
	cout << "records: " << size(dataset) << "\n";

	cout << "done!\n";
	return 0;
}
