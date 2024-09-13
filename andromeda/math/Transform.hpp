#ifndef ANDROMEDA_MATH_TRANSFORM
#define ANDROMEDA_MATH_TRANSFORM

#include <math.h>

#include "Vector.hpp"
#include "Matrix.hpp"

//采用右手坐标系
namespace andromeda {
	namespace math {
		//缩放矩阵
		template<int Rank=4,typename T=float>
		inline SquareMatrix<Rank,T> scale(T scale_factor)
		{
			return SquareMatrix<Rank,T>::identify()*scale_factor;
		}

		//平移矩阵，矩阵维度比矢量大1
		template<int Rank,typename T=float>
		inline SquareMatrix<Rank+1,T> shift(Vector<Rank,T> shift_v)
		{
			SquareMatrix<Rank+1,T> shift_mat=SquareMatrix<Rank+1,T>::identify();
			for(int i=0;i<=Rank;++i)
				shift_mat[i][Rank]=shift_v[i];
			return shift_mat;
		}

		template<typename T=float>
		inline Matrix4x4<T> shift(Vector<3,T> shift_v)
		{
			return
			{
				1,0,0,shift_v[0],
				0,1,0,shift_v[1],
				0,0,1,shift_v[2],
				0,0,0,1
			};
		}

		template<typename T=float>
		inline Matrix4x4<T> Rx(T rad)
		{
			return
			{
				1,0,0,0,
				0,cos(rad),-sin(rad),0,
				0,sin(rad),cos(rad),0,
				0,0,0,1
			};
		}

		template<typename T=float>
		inline Matrix4x4<T> Ry(T rad)
		{
			return
			{
				cos(rad),0,sin(rad),0,
				0,1,0,0,
				-sin(rad),0,cos(rad),0,
				0,0,0,1
			};
		}

		template<typename T=float>
		inline Matrix4x4<T> Rz(T rad)
		{
			return
			{
				cos(rad),-sin(rad),0,0,
				sin(rad),cos(rad),0,0,
				0,0,1,0,
				0,0,0,1
			};
		}

		template<typename T=float>
		inline Matrix4x4<T> R(T rad_x,T rad_y,T rad_z)
		{
			return Rz(rad_z)*Ry(rad_y)*Rx(rad_x);
		}
	}
}

#endif//ANDROMEDA_MATH_TRANSFORM
