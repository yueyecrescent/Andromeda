#ifndef ANDROMEDA_MATH_QUATERNION
#define ANDROMEDA_MATH_QUATERNION

#include "Vector.hpp"

namespace andromeda {
	namespace math {
		template<typename T=float>
		class Quaternion
		{
		public:
			T x,y,z,w;

			Quaternion(T x,T y,T z,T w)
			{
				this->x=x;
				this->y=y;
				this->z=z;
				this->w=w;
			}

			Quaternion(Vector<3,T> v,T w)
			{
				this->x=v[0];
				this->y=v[1];
				this->z=v[2];
				this->w=w;
			}

			Quaternion conj()
			{
				return Quaternion(-x,-y,-z,w);
			}

			template<typename T2>
			Quaternion<decltype(std::declval<T>()+std::declval<T2>())> operator+(Quaternion<T2> q)
			{
				return Quaternion<decltype(std::declval<T>()+std::declval<T2>())>(x+q.x,y+q.y,z+q.z,w+q.w);
			}

			template<typename T2>
			Quaternion<decltype(std::declval<T>()-std::declval<T2>())> operator-(Quaternion<T2> q)
			{
				return Quaternion<decltype(std::declval<T>()-std::declval<T2>())>(x-q.x,y-q.y,z-q.z,w-q.w);
			}

			template<typename T2>
			Quaternion<decltype(std::declval<T>()*std::declval<T2>())> operator*(Quaternion<T2> q)
			{
				return Quaternion<decltype(std::declval<T>()*std::declval<T2>())>(y*q.z-q.y*z+w*q.x+q.w*x,z*q.x-q.z*x+w*q.y+q.w*y,x*q.y-q.x*y+w*q.z+q.w*z,w*q.w-x*q.x-y*q.y-z*q.z);
			}

			static Vector<3,T> rotate(Vector<3,T> v,Vector<3,T> axis,T rad)
			{
				auto p=Quaternion(v,0);
				auto q=Quaternion(axis*sin(rad/2.0f),cos(rad/2.0f));
				auto r=q*p*q.conj();
				return {r.x,r.y,r.z};
			}
		};
	}
}

#endif//ANDROMEDA_MATH_QUATERNION
