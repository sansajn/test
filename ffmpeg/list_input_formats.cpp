// vypise zoznam podporovanych vstupnych formatou
#include <iostream>

extern "C" {

#include <libavformat/avformat.h>

}  // extern "C"

using std::cout;

int main(int argc, char * argv[])
{
	av_register_all();

	// iterate over input formats
	cout << "input formats:\n";
	AVInputFormat * infmt = nullptr;
	while ((infmt = av_iformat_next(infmt)))
	{
		cout << "  " << infmt->name << "\n";
	}

	return 0;
}
