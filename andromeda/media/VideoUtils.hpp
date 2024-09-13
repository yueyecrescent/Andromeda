#ifndef ANDROMEDA_MEDIA_VIDEOUTILS
#define ANDROMEDA_MEDIA_VIDEOUTILS

#include "../../lib/avcodec/libavcodec/avcodec.h"
#include "../../lib/avcodec/libavformat/avformat.h"
#include <iostream>

namespace andromeda {
	namespace media {
		enum output_mode
		{
			OUTPUT_NONE=-1, OUTPUT_VIDEO_BUFFER, OUTPUT_VIDEO_FILE, OUTPUT_BITSTREAM_BUFFER, OUTPUT_BITSTREAM_FILE
		};

		typedef void (*packet_proc)(AVCodecContext*,AVFormatContext*,AVPacket*);
		typedef void (*frame_proc)(AVCodecContext*,AVFormatContext*,AVFrame*);

		void encode(AVCodecContext *cc,AVFrame *frm,AVFormatContext* fc,AVStream *st,AVPacket *pkt,long int* count,packet_proc p);
		void encode_flush(AVCodecContext *cc,AVFormatContext* fc,AVStream *st,AVPacket *pkt,packet_proc p);
		void decode(AVCodecContext *cc,AVPacket* pkt,AVFormatContext* fc,AVFrame *frm,frame_proc p);
		void decode_flush(AVCodecContext *cc,AVFormatContext* fc,AVFrame *frm,frame_proc p);

		inline char* getErrMsg(int errnum)
		{
			char msg[AV_ERROR_MAX_STRING_SIZE]={};
			av_make_error_string(msg,AV_ERROR_MAX_STRING_SIZE,errnum);
			return msg;
		}

		inline void logEncodingStatus(AVFrame* frm,AVPacket* pkt)
		{
			std::cout<<"Writing Frame "<<frm->pts<<" : pts = "<<(pkt->pts)<<" , dts = "<<(pkt->dts)<<" ."<<std::endl;
		}

		void addStream(AVFormatContext *fc,AVCodec* c,int num);
		int addStreamUpTo(AVFormatContext *fc,AVCodec* c,int lim);
		void initVideoStream(AVFormatContext *fc,AVCodecContext *cc,AVCodec *c,int pos);
	}
}
#endif // ANDROMEDA_MEDIA_VIDEOUTILS
