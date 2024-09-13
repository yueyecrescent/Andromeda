#ifndef ANDROMEDA_MEDIA_MUXER
#define ANDROMEDA_MEDIA_MUXER

#include "../../lib/avcodec/libavcodec/avcodec.h"
#include "../../lib/avcodec/libavformat/avformat.h"

#include "VideoCodec.hpp"
#include "AudioCodec.hpp"

namespace andromeda {
	namespace image {
		class RasterImage;
	}
	namespace media {
		class Muxer
		{
		public:
			Muxer(VideoCodec* vc,AudioCodec* ac);
			~Muxer();

			bool construct(VideoCodec* vc,AudioCodec* ac);
			void deconstruct();

			void applySettings(void);

			void openOutputVideoFile(const char* file);
			void closeOutputVideoFile(void);

			void inputVideoFrame(andromeda::image::RasterImage& img,int last_frame_count);
			void inputVideoFrames(VideoFrameInfo* infos,int length);

			inline int getMaxNumberOfStreams()
			{
				return video_codec->properties->nb_video_stream+video_codec->properties->nb_audio_stream+video_codec->properties->nb_subtitle_stream;
			}

			inline void setVideoStreamNumber(int nb)
			{
				video_codec->properties->nb_video_stream=nb;
			}

			inline void setAudioStreamNumber(int nb)
			{
				video_codec->properties->nb_audio_stream=nb;
			}

			inline void setSubtileStreamNumber(int nb)
			{
				video_codec->properties->nb_subtitle_stream=nb;
			}

			inline void selectVideoStreamIndex(int new_idx)
			{
				thisVideoStreamIdx=new_idx;
			}

			inline void selectAudioStreamIndex(int new_idx)
			{
				thisAudioStreamIdx=video_codec->properties->nb_video_stream+new_idx;
			}

			inline void selectSubtileStreamIndex(int new_idx)
			{
				thisSubtitleStreamIdx=video_codec->properties->nb_video_stream+video_codec->properties->nb_audio_stream+new_idx;
			}
		protected:
			int thisVideoStreamIdx,thisAudioStreamIdx,thisSubtitleStreamIdx;
			VideoCodec* video_codec;
			AudioCodec* audio_codec;
			AVPacket* packet;
			AVFormatContext *format_context;
		};
	}
}

#endif // ANDROMEDA_MEDIA_MUXER
