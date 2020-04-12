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

table_type load_data_from_file(string const & fname);
void kmeans(table_type const & ds, size_t k);


double distance(vector<double> const & u, vector<double> const & v)
{
	assert(size(u) == size(v));

	double result = 0;
	for (size_t i = 0; i < size(u); ++i)
		result = pow(u[i] - v[i]);

	return result;
}

size_t euclidian_step(table_type const & ds, size_t centroid)
{
	size_t max_idx = 0;
	double max_distance = 0.0;
	for (size_t i = 0; i < size(ds); ++i)
	{
		double dist = eucl_distance(ds[centroid].first); 
		if (dist > max_distance)
		{
			max_idx = i;
			max_distance = dist;
		}
	}
	return max_idx;
}

void kmeans(table_type const & ds, size_t k)
{

}


int main(int argc, char * argv[])
{
	string const dataset_file = argc > 1 ? argv[1] : "dataset.csv";
	size_t const k = argc > 2 ? atoi(argv[2]) : 5;

	table_type dataset = load_data_from_file(dataset_file);
	cout << "records: " << size(dataset) << "\n";

	cout << "done!\n";
	return 0;
}

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
