#ifndef ANDROMEDA_MATH_TRANSFORM
#define ANDROMEDA_MATH_TRANSFORM

#include <math.h>

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

#ifdef far
#undef far
#define __SHADOWED_DEF_MARK_far
#endif

#ifdef near
#undef near
#define __SHADOWED_DEF_MARK_near
#endif

//采用右手坐标系
namespace andromeda {
	namespace math {
		//缩放矩阵
		template<int Rank=4,typename T=float>
		inline SquareMatrix<Rank,T> scale(T scale_factor)
		{
			return andromeda::traits::cast<SquareMatrix<Rank,T> >(SquareMatrix<Rank,T>::identity()*scale_factor);
		}

		//平移矩阵，矩阵维度比矢量大1
		template<int Rank,typename T=float>
		inline SquareMatrix<Rank+1,T> shift(Vector<Rank,T>& shift_v)
		{
			SquareMatrix<Rank+1,T> shift_mat=SquareMatrix<Rank+1,T>::identify();
			for(int i=0;i<=Rank;++i)
				shift_mat[i][Rank]=shift_v[i];
			return shift_mat;
		}

		template<typename T=float>
		inline Matrix4x4<T> shift(Vector<3,T> shift_v)
		{
			T m_elem[]=
			{
				1,0,0,shift_v[0],
				0,1,0,shift_v[1],
				0,0,1,shift_v[2],
				0,0,0,1
			};
			return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
		}

		template<typename T=float>
		inline Matrix4x4<T> Rx(T rad)
		{
			T m_elem[]=
			{
				1,0,0,0,
				0,cos(rad),-sin(rad),0,
				0,sin(rad),cos(rad),0,
				0,0,0,1
			};
			return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
		}

		template<typename T=float>
		inline Matrix4x4<T> Ry(T rad)
		{
			T m_elem[]=
			{
				cos(rad),0,sin(rad),0,
				0,1,0,0,
				-sin(rad),0,cos(rad),0,
				0,0,0,1
			};
			return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
		}

		template<typename T=float>
		inline Matrix4x4<T> Rz(T rad)
		{
			T m_elem[]=
			{
				cos(rad),-sin(rad),0,0,
				sin(rad),cos(rad),0,0,
				0,0,1,0,
				0,0,0,1
			};
			return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
		}

		//外在旋转
		template<typename T=float>
		inline Matrix4x4<T> Rxyz(T rad_x,T rad_y,T rad_z)
		{
			return Rx(rad_z)*Ry(rad_y)*Rz(rad_x);
		}

		//内在旋转
		template<typename T=float>
		inline Matrix4x4<T> Rzyx(T pitch,T yaw,T roll)
		{
			return Rz(roll)*Ry(yaw)*Rx(pitch);
		}

		//绕轴转动，四元数转换为旋转矩阵
		template<typename T=float>
		inline Matrix4x4<T> R(Vector<3,T> axis,T rad)
		{
			return Quaternion<T>::R(axis,rad);
		}

		//OpenGL的投影矩阵。NDC为左手坐标系，世界坐标系为右手坐标系，near和far应该取负数，向z轴负方向看去
		template<typename T=float>
		inline Matrix4x4<T>frustum(T near,T far,T fov,T left,T right,T top,T bottom)
		{
			T m_elem[]={
				2*near/(right-left),0,(right+left)/(right-left),0,
				0,2*near/(top-bottom),(top+bottom)/(top-bottom),0,
				0,0,-(far+near)/(far-near),-2*near*far/(far-near),
				0,0,-1,0
			};
			return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
		}

		//OpenGL的投影矩阵。NDC为左手坐标系，世界坐标系为右手坐标系，znear和zfar取正
		template<typename T=float>
		inline Matrix4x4<T> frustum(T znear,T zfar,T fov,T aspect_ratio)
		{
			T m_elem[]={
				1/(tan(fov/2)*aspect_ratio),0,0,0,
				0,1/tan(fov/2),0,0,
				0,0,-(zfar+znear)/(zfar-znear),-2*znear*zfar/(zfar-znear),
				0,0,-1,0
			};
			return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
		}
	}
}

#ifdef __SHADOWED_DEF_MARK_far
#define far
#undef __SHADOWED_DEF_MARK_far
#endif

#ifdef __SHADOWED_DEF_MARK_near
#define near
#undef __SHADOWED_DEF_MARK_near
#endif

#endif//ANDROMEDA_MATH_TRANSFORM
