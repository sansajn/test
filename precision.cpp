#include <cstdio>
#include <string>
#include <algorithm>
#include <sstream>
using std::string;
using std::min;
using std::ostringstream;

string format_number(float num);

int main(int argv, char * argc[])
{
	float num = 3.141519f;
	float num2 = 3.0f;
	printf("%f | %f\n", num, num2);
	printf("%g | %g\n", num, num2);
	printf("%.2f| %.2f\n", num, num2);
	printf("%2f | %2f\n", num, num2);
	printf("%2.2f | %2.2f\n", num, num2);

	string num_as_str(format_number(num));
	string num2_as_str(format_number(num2));

	printf("%s | %s\n", num_as_str.c_str(), num2_as_str.c_str());

	return 0;
}

string format_number(float num)
{
	ostringstream ostr;
	ostr << num;
	string num_as_str = ostr.str();
	size_t dotpos = num_as_str.find('.');
	if (dotpos == string::npos)
		return num_as_str;
	size_t lastpos = min(dotpos+2, num_as_str.size()-1)+1;
	return string(num_as_str.begin(), num_as_str.begin()+lastpos);
}
