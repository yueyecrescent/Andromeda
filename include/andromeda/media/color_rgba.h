#ifndef ANDROMEDA_MEDIA_COLORRGBA
#define ANDROMEDA_MEDIA_COLORRGBA

#include <string.h>
#include "pixel.h"
#include "color_math.h"

namespace andromeda {
	namespace media {
		class Pixel;
		class ColorRGBA
		{
		public:
			float r,g,b,a;

			operator Pixel();

			ColorRGBA(int i_r,int i_g,int i_b,int i_a=0);
			ColorRGBA(float f_r=0,float f_g=0,float f_b=0,float f_a=0);
			ColorRGBA(Pixel pixel);

			inline bool operator==(ColorRGBA destcolor)
			{
				return (r==destcolor.r&&g==destcolor.g&&b==destcolor.b&&a==destcolor.a);
			}

			inline ColorRGBA& setRGBA(ColorRGBA color)
			{
				memcpy(this,&color,sizeof(ColorRGBA));
				return *this;
			}

			inline ColorRGBA& setRGBA(ColorChannel color_ch,float color_value)
			{
				*((float*)this+color_ch)=getChannelFloat(color_value);
				return *this;
			}

			inline float getRGBA(ColorChannel color_ch)
			{
				return *((float*)this+color_ch);
			}

			ColorRGBA& setRGBA(float _r=-1,float _g=-1,float _b=-1,float _a=-1); //设置-1表示不变，0-255则改变为设置的值，超出这个范围的就取0或255

			Pixel toPixel();
			static ColorRGBA mixAlpha(ColorRGBA below,ColorRGBA above);

			inline float grayScale(float weight_r,float weight_g,float weight_b)
			{
				return (r*weight_r+g*weight_g+b*weight_b)/3.0;
			}
		};

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

#endif //ANDROMEDA_MEDIA_COLORRGBA
