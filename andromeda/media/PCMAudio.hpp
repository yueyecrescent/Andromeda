#ifndef ANDROMEDA_MEDIA_PCMAUDIO
#define ANDROMEDA_MEDIA_PCMAUDIO

#include "Sample.hpp"

namespace andromeda {
	namespace media {
		class PCMAudio
		{
		protected:
			Sample* sample_data;

		public:
			int bit_rate;
			int sample_rate;
			int sample_size;
			int nb_channels;
			int sample_count;
			long int data_size;

			PCMAudio(int br=320000,int sr=0,int sb=0,int nbc=0,long int data_size_=0,Sample* data=nullptr);

			inline Sample* getSampleDataPtr()
			{
				return sample_data;
			}

			inline void setSampleData(Sample* data)
			{
				sample_data=data;
			}

			inline Sample& getSample(int sp_idx)
			{
				return *(sample_data+sp_idx);
			}

			void mulVolumeFactor(float factor);
			short int** departChannels();

			static PCMAudio& mix(PCMAudio& aud1,PCMAudio& aud2,long int pos_off);
			static PCMAudio& mulSpeedFactor(PCMAudio& audio,float factor);
			static PCMAudio* readPCMAudio(const char* file);
			void writePCMAudio(const char* dest_file); //有问题，没写完
		};
	}
}

#endif // ANDROMEDA_MEDIA_PCMAUDIO
