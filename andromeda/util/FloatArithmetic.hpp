#ifndef ANDROMEDA_UTIL_FLOATARITHMETIC
#define ANDROMEDA_UTIL_FLOATARITHMETIC

#include <malloc.h>
#include <math.h>

namespace andromeda {
	namespace util {
		inline void fa_plus(void* n1,void* n2,void* res,int el,int ml)
		{
			int n1_e,n2_e,n1_m,n2_m;
			unsigned char n1_s,n2_s;
			int e_len=ceil((double)(1+el)/8);
			int e_rsl=8*e_len-1-el;
			//n1_e=
		}

		inline void* fa_plus(void* n1,void* n2,int el,int ml)
		{
			unsigned char* ret=(unsigned char*)malloc(ceil((double)(1+el+ml)/8));
			return fa_plus(n1,n2,ret,el,ml);
			return ret;
		}

		inline double double_plus(double d1,double d2)
		{
			double ret=0;
			fa_plus(&d1,&d2,&ret,11,52);
			return ret;
		}
	}
}

#endif // ANDROMEDA_UTIL_FLOATARITHMETIC
