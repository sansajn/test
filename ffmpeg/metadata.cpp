// shows video metadata
#include <string>
#include <iostream>
#include <cassert>

extern "C" {

#include <libavformat/avformat.h>
#include <libavutil/dict.h>

}  // to use in c++

using std::cout;
using std::string;

int main(int argc, char * argv[])
{
	string input_file = "test.mkv";
	if (argc > 1)
		input_file = argv[1];

	av_register_all();

	AVFormatContext * ctx = nullptr;
	int ret = avformat_open_input(&ctx, input_file.c_str(), nullptr, nullptr);
	assert(ret == 0);

	AVDictionaryEntry * tag = nullptr;
	while ((tag = av_dict_get(ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
		cout << tag->key << "=" << tag->value << "\n";

	avformat_close_input(&ctx);

	return 0;
}
