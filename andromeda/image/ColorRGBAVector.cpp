#include "ColorRGBAVector.hpp"

#include "../macros/Math.h"
#include "ColorRGBA.hpp"
#include "Pixel.hpp"

using namespace andromeda::image;

ColorRGBAVector::ColorRGBAVector(float rdg,float gdb,float len,float a) :
		rdefg(rdg), gdefb(gdb), length(len), alpha(a)
{
}

ColorRGBAVector::ColorRGBAVector(ColorRGBA color) :
		rdefg(getRadianTangent(color.r,color.g)), gdefb(getRadianTangent(color.g,color.b)), length(getLength3DVector(color.r,color.g,color.b)), alpha(color.a)
{
}

ColorRGBAVector::ColorRGBAVector(Pixel pixel) :
		rdefg(getRadianTangent(getChannelInt(pixel.r),getChannelInt(pixel.g))), gdefb(getRadianTangent(getChannelInt(pixel.g),getChannelInt(pixel.b))), length(getLength3DVector(getChannelInt(pixel.r),getChannelInt(pixel.g),getChannelInt(pixel.b))), alpha(getChannelInt(pixel.a))
{
}

void ColorRGBAVector::operator=(ColorRGBA color)
{
	rdefg=getRadianTangent(getChannelInt(color.r),getChannelInt(color.g));
	gdefb=getRadianTangent(getChannelInt(color.g),getChannelInt(color.b));
	length=getLength3DVector(getChannelInt(color.r),getChannelInt(color.g),getChannelInt(color.b));
	alpha=getChannelInt(color.a);
}

ColorRGBA ColorRGBAVector::toColorRGBA()
{
	return ColorRGBA(getChannelRByRadianVector(rdefg,gdefb,length),getChannelGByRadianVector(rdefg,gdefb,length),getChannelBByRadianVector(rdefg,gdefb,length),alpha);
}

inline Pixel ColorRGBAVector::toPixel()
{
	return Pixel(ColorRGBA(getChannelRByRadianVector(rdefg,gdefb,length),getChannelGByRadianVector(rdefg,gdefb,length),getChannelBByRadianVector(rdefg,gdefb,length),alpha));
}
