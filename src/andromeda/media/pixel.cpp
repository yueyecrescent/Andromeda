#include <andromeda/media/pixel.h>

#include <andromeda/media/color_rgba.h>

using namespace andromeda::media;

Pixel::Pixel(int i_r,int i_g,int i_b,int i_a) :
		r(getPixelInt(i_r)), g(getPixelInt(i_g)), b(getPixelInt(i_b)), a(getPixelInt(i_a))
{
}

Pixel::Pixel(float f_r,float f_g,float f_b,float f_a) :
		r(getPixelFloat(f_r)), g(getPixelFloat(f_g)), b(getPixelFloat(f_b)), a(getPixelFloat(f_a))
{
}

Pixel::Pixel(ColorRGBA color) :
		r(getPixelFloat(color.r)), g(getPixelFloat(color.g)), b(getPixelFloat(color.b)), a(getPixelFloat(color.a))
{
}

Pixel::operator ColorRGBA()
{
	return ColorRGBA(r,g,b,a);
}

ColorRGBA Pixel::toColorRGBA()
{
	return ColorRGBA(r,g,b,a);
}

Pixel& Pixel::setRGBA(int _r,int _g,int _b,int _a) //设置-1表示不变，0-255则改变为设置的值，超出这个范围的就取0或255
{
	if(_r!=-1)
		r=getPixelInt(_r);
	if(_g!=-1)
		g=getPixelInt(_g);
	if(_b!=-1)
		b=getPixelInt(_b);
	if(_a!=-1)
		a=getPixelInt(_a);
	return *this;
}
