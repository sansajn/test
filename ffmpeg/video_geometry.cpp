// shows video geometry (libav* 2.7.6, ubuntu 15.10)
#include <string>
#include <iostream>
#include <cassert>

extern "C" {

#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <libavcodec/avcodec.h>

}  // to use in c++

using std::cout;
using std::string;

int open_codec_context(AVFormatContext * fmt_ctx, AVMediaType type)
{
	int stream_idx = av_find_best_stream(fmt_ctx, type, -1, -1, nullptr, 0);
	if (stream_idx < 0)
		return stream_idx;

	AVStream * st = fmt_ctx->streams[stream_idx];

	// find decoder for the stream
	AVCodecContext * dec_ctx = st->codec;
	AVCodec * dec = avcodec_find_decoder(dec_ctx->codec_id);
	assert(dec);

	int ret = avcodec_open2(dec_ctx, dec, nullptr);
	assert(ret >= 0);

	return stream_idx;
}

int main(int argc, char * argv[])
{
	string input_file = "test.mkv";
	if (argc > 1)
		input_file = argv[1];

	av_register_all();

	// open file
	AVFormatContext * fmt_ctx = nullptr;
	int ret = avformat_open_input(&fmt_ctx, input_file.c_str(), nullptr, nullptr);
	assert(ret >= 0);

	// retrieve stream information
	ret = avformat_find_stream_info(fmt_ctx, nullptr);
	assert(ret >= 0);

	int stream_idx = open_codec_context(fmt_ctx, AVMEDIA_TYPE_VIDEO);
	if (stream_idx >= 0)
	{
		AVStream * video_stream = fmt_ctx->streams[stream_idx];
		AVCodecContext * video_dec_ctx = video_stream->codec;
		cout << "w:" << video_dec_ctx->width << ", h:" << video_dec_ctx->height << "\n";
	}
	else
		cout << "no video found\n";

	avformat_close_input(&fmt_ctx);

	return 0;
}

