#ifndef ANDROMEDA_IMAGE_COLORRGBAVECTOR
#define ANDROMEDA_IMAGE_COLORRGBAVECTOR

#include "ColorRGBA.hpp"
#include "Pixel.hpp"

namespace andromeda {
	namespace image {
		class ColorRGBAVector
		{
		public:
			float rdefg,gdefb,length,alpha;

			ColorRGBAVector(float rdg=0,float gdb=0,float len=0,float a=0);
			ColorRGBAVector(ColorRGBA color);
			ColorRGBAVector(Pixel pixel);

			void operator=(ColorRGBA color);
			ColorRGBA toColorRGBA();
			Pixel toPixel();
		};
	}
}

#endif //ANDROMEDA_IMAGE_COLORRGBAVECTOR
