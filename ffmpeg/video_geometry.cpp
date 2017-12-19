// shows video geometry (libav* 2.7.6, ubuntu 15.10, 16.04)
#include <string>
#include <utility>
#include <mutex>
#include <iostream>
#include <cassert>

extern "C" {

#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <libavcodec/avcodec.h>

}  // to use in c++

using std::cout;
using std::string;
using std::pair;
using std::once_flag;
using std::call_once;
using std::make_pair;

once_flag __av_init_flag;


pair<int, int> media_geometry(string const & fname)
{
	pair<int, int> result{0, 0};

	call_once(__av_init_flag, av_register_all);

	// open file
	AVFormatContext * fmt_ctx = nullptr;
	int ret = avformat_open_input(&fmt_ctx, fname.c_str(), nullptr, nullptr);
	assert(ret >= 0);

	// retrieve stream information
	ret = avformat_find_stream_info(fmt_ctx, nullptr);
	assert(ret >= 0);

	// find stream
	int stream_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
	assert(stream_idx >= 0);
	AVStream * video_stream = fmt_ctx->streams[stream_idx];

	AVCodecContext * dec_ctx = video_stream->codec;  // stream decoder

	result = make_pair(dec_ctx->width, dec_ctx->height);

	avformat_close_input(&fmt_ctx);

	return result;
}

int main(int argc, char * argv[])
{
	string input_file = "test.mkv";
	if (argc > 1)
		input_file = argv[1];

	for (int i = 0; i < 100; ++i)
	{
		int w, h;
		std::tie(w, h) = media_geometry(input_file);
		cout << "width:" << w << ", height:" << h << "\n";
	}

	cout << "press enter to end";
	getchar();

	return 0;
}

