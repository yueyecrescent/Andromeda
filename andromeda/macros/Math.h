#ifndef ANDROMEDA_MACROS_MATH
#define ANDROMEDA_MACROS_MATH

#include <math.h>

#define M_PI 3.14159265358979323846

#define getChannelInt(i) (i<0.0?0.0:(i<255.0?i/255.0:1.0))
#define getChannelFloat(f) (f<0.0?0.0:(f<1.0?f:1.0))
#define getPixelInt(i) (i<0?0:(i<255?i:255))
#define getPixelFloat(f) (f<0?0:(f<1?f*255:255))
#define getRadianTangent(x,y) (x==0?1.57079632679489661923:(float)(atan(y/x)))
#define getLength3DVector(x,y,z) ((float)sqrt(x*x+y*y+z*z))
#define getChannelRByRadianVector(rdg,gdb,len) ((float)(len*cos(gdb)*cos(rdg)))
#define getChannelGByRadianVector(rdg,gdb,len) ((float)(len*cos(gdb)*sin(rdg)))
#define getChannelBByRadianVector(rdg,gdb,len) ((float)(len*sin(gdb)))
#define getBiggerNumber(x,y) (x>y?x:y)
#define getSmallerNumber(x,y) (x<y?x:y)
#define getAbsoluteValue(n) (n<0?-n:n)
#define getLightChannelInt(i) (i<0.0?0.0:(i<255.0?i/255.0:1.0))
#define getLightChannelFloat(f) (f<0.0?0.0:(f<1.0?f:1.0))
#define getChrominaceChannelInt(i) (i<-127.5?-0.5:(i>127.5?0.5:i/255))
#define getChrominaceChannelFloat(f) (f<-0.5?-0.5:(f>0.5?0.5:f))
#define getPixelLightInt(i) (i<0?0:(i<255?i:255))
#define getPixelLightFloat(f) (f<0?0:(f<1?f*255:255))
#define getPixelChrominaceInt(i) (i<-127.5?-127.5:(i>127.5?127.5:i))
#define getPixelChrominaceFloat(f) (f<-0.5?-0.5:(f>0.5?0.5:f))

#endif // ANDROMEDA_MACROS_MATH
