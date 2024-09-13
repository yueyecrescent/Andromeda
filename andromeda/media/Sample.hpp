#ifndef ANDROMEDA_MEDIA_SAMPLE
#define ANDROMEDA_MEDIA_SAMPLE

namespace andromeda {
	namespace media {
		class Sample
		{
		public:
			signed short int l,r;

			Sample(signed short int lv=0,signed short int rv=0);

			inline bool operator==(Sample& s)
			{
				return (l==s.l)&&(r==s.r);
			}

			inline bool operator!=(Sample& s)
			{
				return (l!=s.l)||(r!=s.r);
			}
		};
	}
}

#endif // ANDROMEDA_MEDIA_SAMPLE
