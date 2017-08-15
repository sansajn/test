// read first 10 frames from input file
#include <iostream>
#include <cassert>

extern "C" {

#include <libavformat/avformat.h>

}  // extern "C"

using std::cout;

int main(int argc, char * argv[])
{
	av_register_all();

	AVFormatContext * ctx = nullptr;
	int ret = avformat_open_input(&ctx, "lara.mp4", nullptr, nullptr);
	assert(ret == 0);

	ret = avformat_find_stream_info(ctx, nullptr);
	assert(ret >= 0);

	cout << "stream count: " << ctx->nb_streams << "\n";

	for (unsigned i = 0; i < ctx->nb_streams; ++i)
	{
		AVStream * stream = ctx->streams[i];
		cout << "#" << i << ". ";
		switch (stream->codec->codec_type)
		{
			case AVMEDIA_TYPE_VIDEO:
				cout << "video";
				break;

			case AVMEDIA_TYPE_AUDIO:
				cout << "audio";
				break;

			case AVMEDIA_TYPE_DATA:
				cout << "data";
				break;

			case AVMEDIA_TYPE_SUBTITLE:
				cout << "subtitle";
				break;

			case AVMEDIA_TYPE_ATTACHMENT:
				cout << "attachement";
				break;

			case AVMEDIA_TYPE_UNKNOWN:
				cout << "unknown";
				break;

			case AVMEDIA_TYPE_NB:
				cout << "AVMEDIA_TYPE_NB";
				break;
		}
		cout << "\n";
	}

	// read first N frames
	AVPacket packet;
	for (int i = 0; i < 10; ++i)
	{
		ret = av_read_frame(ctx, &packet);
		assert(ret == 0);

		cout << "frame " << i << ": size:" << packet.size << ", stream-index:"
			<< packet.stream_index << "\n";
	}

	return 0;
}
