#ifndef ANDROMEDA_MATH_MATRIX
#define ANDROMEDA_MATH_MATRIX

#include <type_traits>
#include <math.h>
#include <malloc.h>
#include <sstream>
#include "vector.h"
#include "../traits/Types.h"

namespace andromeda {
	namespace math {
		template<int Row,int Column,typename T=float>
		class Matrix
		{
		public:
			Vector<Column,T>elem[Row]; //索引行、列。储存Row个Column维行向量
			typedef T type;
			static const int row=Row;
			static const int column=Column;

			inline operator T*()
			{
				return (T*)elem;
			}

			inline operator Vector<Column,T>*()
			{
				return elem;
			}

			template<typename T2>
			inline operator Matrix<Row,Column,T2>()
			{
				Matrix<Row,Column,T2>cast_result;
				for(int i=0;i<Row;++i)
					cast_result[i]=elem[i];
				return cast_result;
			}

			//提取行向量
			inline Vector<Column,T>& operator[](int i)
			{
				return elem[i];
			}

			inline operator std::string() //转化为字符串
			{
				return toString();
			}

			inline operator char*() //转化为C字符串
			{
				return toString().c_str();
			}

			template<typename T2>
			inline Matrix<Row,Column,decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix<Row,Column,T2>&m)
			{
				return add(m);
			}

			template<typename T2>
			inline Matrix<Row,Column,decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix<Row,Column,T2>&m)
			{
				return sub(m);
			}

			template<typename T2>
			inline Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>operator *(T2 t)
			{
				return mul(t);
			}

			template<int Row2,typename T2>
			inline Matrix<Row,Row2,decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix<Column,Row2,T2>&m)
			{
				return mul(m);
			}

			template<typename T2>
			inline Vector<Row,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<Column,T2>&v)
			{
				return mul(v);
			}

			template<typename T2>
			inline Matrix<Row,Column,decltype(std::declval<T>()/std::declval<T2>())>operator /(T2 t)
			{
				return div(t);
			}

			//右值处理
			template<typename T2>
			inline Matrix<Row,Column,decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix<Row,Column,T2>&&m)
			{
				return add(andromeda::traits::cast<Matrix<Row,Column,T2> >(m));
			}

			template<typename T2>
			inline Matrix<Row,Column,decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix<Row,Column,T2>&&m)
			{
				return sub(andromeda::traits::cast<Matrix<Row,Column,T2> >(m));
			}

			template<int Row2,typename T2>
			inline Matrix<Row,Row2,decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix<Column,Row2,T2>&&m)
			{
				return mul(andromeda::traits::cast<Matrix<Row,Column,T2> >(m));
			}

			template<typename T2>
			inline Vector<Row,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<Column,T2>&&v)
			{
				return mul(andromeda::traits::cast<Vector<Column,T2> >(v));
			}

			//提取列向量
			Vector<Row,T>extractColumn(int column)
			{
				Vector<Row,T>row_vec;
				for(int i=0;i<Row;++i)
					row_vec[i]=elem[i][column];
				return row_vec;
			}

			//提取行向量
			inline Vector<Column,T>extractRow(int row)
			{
				return elem[row];
			}

			Matrix<Column,Row,T>transpose() //转置
			{
				Matrix<Column,Row,T>transpose_result;
				for(int i=0;i<Column;++i)
					for(int j=0;j<Row;++j)
						transpose_result[i][j]=elem[j][i];
				return transpose_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>dot(Matrix<Row,Column,T2>&m) //按位相乘
			{
				Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>dot_result;
				for(int i=0;i<Row;++i)
					for(int j=0;j<Column;++j)
						dot_result[i][j]=elem[i][j]*m.elem[i][j];
				return dot_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>mul(T2 t) //数乘
			{
				Matrix<Row,Column,decltype(std::declval<T>()*std::declval<T2>())>mul_result;
				for(int i=0;i<Row;++i)
					mul_result[i]=elem[i]*t;
				return mul_result;
			}

			template<int Row2,typename T2>
			Matrix<Row,Row2,decltype(std::declval<T>()*std::declval<T2>())>mul(Matrix<Column,Row2,T2>&m)
			{
				Matrix<Row,Row2,decltype(std::declval<T>()*std::declval<T2>())>mul_result;
				for(int i=0;i<Row;++i)
				{
					Vector<Column,T>row_vec=extractRow(i);
					for(int j=0;j<Row2;++j)
						mul_result[i][j]=row_vec.dot(m.extractColumn(j));
				}
				return mul_result;
			}

			template<typename T2>
			Vector<Row,decltype(std::declval<T>()*std::declval<T2>())>mul(Vector<Column,T2>&v)
			{
				Vector<Row,decltype(std::declval<T>()*std::declval<T2>())>mul_result;
				for(int i=0;i<Row;++i)
				{
					Vector<Column,T>row_vec=extractRow(i);
					mul_result[i]=row_vec.dot(v);
				}
				return mul_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()/std::declval<T2>())>div(T2 t) //除法
			{
				Matrix<Row,Column,decltype(std::declval<T>()/std::declval<T2>())>div_result;
				for(int i=0;i<Row;++i)
					div_result[i]=elem[i]/t;
				return div_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()+std::declval<T2>())>add(Matrix<Row,Column,T2>&m)
			{
				Matrix<Row,Column,decltype(std::declval<T>()+std::declval<T2>())>add_result;
				for(int i=0;i<Row;++i)
					add_result[i]=elem[i]+m.elem[i];
				return add_result;
			}

			template<typename T2>
			Matrix<Row,Column,decltype(std::declval<T>()-std::declval<T2>())>sub(Matrix<Row,Column,T2>&m)
			{
				Matrix<Row,Column,decltype(std::declval<T>()-std::declval<T2>())>sub_result;
				for(int i=0;i<Row;++i)
					sub_result[i]=elem[i]-m.elem[i];
				return sub_result;
			}

			static Matrix<Row,Column,T>zero()
			{
				Matrix<Row,Column,T>zero_mat;
				memset(&zero_mat.elem,0,Row*Column*sizeof(T));
				return zero_mat;
			}

			std::string toString()
			{
				std::ostringstream oss;
				for(int i=0;i<Row;++i)
				{
					oss<<elem[i][0];
					for(int j=1;j<Column;++j)
						oss<<' '<<elem[i][j];
					oss<<'\n';
				}
				return oss.str();
			}
		};

		//方阵特化
		template<int Rank,typename T>
		class SquareMatrix:public Matrix<Rank,Rank,T>
		{
		public:
			using Matrix<Rank,Rank,T>::elem; //索引行、列。储存Rank个Rank维行向量
			using Matrix<Rank,Rank,T>::type;
			using Matrix<Rank,Rank,T>::row;
			using Matrix<Rank,Rank,T>::column;
			using Matrix<Rank,Rank,T>::operator+;
			using Matrix<Rank,Rank,T>::operator-;
			using Matrix<Rank,Rank,T>::operator*;
			using Matrix<Rank,Rank,T>::operator/;

			template<typename T2>
			inline SquareMatrix<Rank,decltype(std::declval<T>()+std::declval<T2>())>operator +(SquareMatrix<Rank,T2>&m)
			{
				return add(m);
			}

			template<typename T2>
			inline SquareMatrix<Rank,decltype(std::declval<T>()-std::declval<T2>())>operator -(SquareMatrix<Rank,T2>&m)
			{
				return sub(m);
			}

			template<typename T2>
			inline SquareMatrix<Rank,decltype(std::declval<T>()*std::declval<T2>())>operator *(SquareMatrix<Rank,T2>&m)
			{
				return mul(m);
			}

			template<typename T2>
			inline Vector<Rank,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<Rank,T2>&v)
			{
				return mul(v);
			}

			//右值处理
			template<typename T2>
			inline SquareMatrix<Rank,decltype(std::declval<T>()+std::declval<T2>())>operator +(SquareMatrix<Rank,T2>&&m)
			{
				return add(andromeda::traits::cast<SquareMatrix<Rank,T2> >(m));
			}

			template<typename T2>
			inline SquareMatrix<Rank,decltype(std::declval<T>()-std::declval<T2>())>operator -(SquareMatrix<Rank,T2>&&m)
			{
				return sub(andromeda::traits::cast<SquareMatrix<Rank,T2> >(m));
			}

			template<typename T2>
			inline SquareMatrix<Rank,decltype(std::declval<T>()*std::declval<T2>())>operator *(SquareMatrix<Rank,T2>&&m)
			{
				return mul(andromeda::traits::cast<SquareMatrix<Rank,T2> >(m));
			}

			template<typename T2>
			inline Vector<Rank,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<Rank,T2>&&v)
			{
				return mul(andromeda::traits::cast<Vector<Rank,T2> >(v));
			}

			//特化类独有
			inline constexpr int rank()
			{
				return Rank;
			}

			auto det() -> decltype(std::declval<T>()*std::declval<T>()); //行列式懒得写了高阶的用不上

			auto trace() -> decltype(std::declval<T>()+std::declval<T>()) //迹
			{
				decltype(std::declval<T>()+std::declval<T>()) tr=0;
				for(int i=0;i<Rank;++i)
					tr+=elem[i][i];
				return tr;
			}

			static SquareMatrix<Rank,T> identity()
			{
				SquareMatrix<Rank,T> id=andromeda::traits::cast<SquareMatrix<Rank,T> >(Matrix<Rank,Rank,T>::zero());
				for(int i=0;i<Rank;++i)
					id[i][i]=1;
				return id;
			}
		};

		//3x3方阵特化
		template<typename T>
		class Matrix3x3:public SquareMatrix<3,T>
		{
		public:
			using Matrix<3,3,T>::elem;
			using Matrix<3,3,T>::type;
			using Matrix<3,3,T>::row;
			using Matrix<3,3,T>::column;
			using Matrix<3,3,T>::operator+;
			using Matrix<3,3,T>::operator-;
			using Matrix<3,3,T>::operator*;
			using Matrix<3,3,T>::operator/;

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix3x3<T2>&m)
			{
				return add(m);
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix3x3<T2>&m)
			{
				return sub(m);
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix3x3<T2>&m)
			{
				return mul(m);
			}

			template<typename T2>
			inline Vector<3,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<3,T2>&v)
			{
				return mul(v);
			}

			//右值处理
			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix3x3<T2>&&m)
			{
				return add(andromeda::traits::cast<Matrix3x3<T2> >(m));
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix3x3<T2>&&m)
			{
				return sub(andromeda::traits::cast<Matrix3x3<T2> >(m));
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix3x3<T2>&&m)
			{
				return mul(andromeda::traits::cast<Matrix3x3<T2> >(m));
			}

			template<typename T2>
			inline Vector<3,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<3,T2>&&v)
			{
				return mul(andromeda::traits::cast<Vector<3,T2> >(v));
			}

			template<typename T2>
			inline operator Matrix<3,3,T2>()
			{
				T2 m_elem[]=
				{
					(T2)elem[0][0],(T2)elem[0][1],(T2)elem[0][2],
					(T2)elem[1][0],(T2)elem[1][1],(T2)elem[1][2],
					(T2)elem[2][0],(T2)elem[2][1],(T2)elem[2][2]
				};
				return andromeda::traits::cast<Matrix<3,3,T2> >(m_elem);
			}

			inline Vector<3,T>extractColumn(int column)
			{
				T v_elem[]=
				{
					elem[0][column],elem[1][column],elem[2][column]
				};
				return andromeda::traits::cast<Vector<3,T> >(v_elem);
			}

			inline Vector<3,T>extractRow(int row)
			{
				return elem[row];
			}

			inline Matrix3x3<T>transpose() //转置
			{
				T m_elem[]=
				{
					elem[0][0],elem[1][0],elem[2][0],
					elem[0][1],elem[1][1],elem[2][1],
					elem[0][2],elem[1][2],elem[2][2]
				};
				return andromeda::traits::cast<Matrix3x3<T> >(m_elem);
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()*std::declval<T2>())>dot(Matrix<3,3,T2>&m) //按位相乘
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]*m.elem[0][0],elem[0][1]*m.elem[0][1],elem[0][2]*m.elem[0][2],
					elem[1][0]*m.elem[1][0],elem[1][1]*m.elem[1][1],elem[1][2]*m.elem[1][2],
					elem[2][0]*m.elem[2][0],elem[2][1]*m.elem[2][1],elem[2][2]*m.elem[2][2]
				};
				return andromeda::traits::cast<Matrix3x3<R> >(m_elem);
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()*std::declval<T2>())>mul(T2 t) //数乘
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]*t,elem[0][1]*t,elem[0][2]*t,
					elem[1][0]*t,elem[1][1]*t,elem[1][2]*t,
					elem[2][0]*t,elem[2][1]*t,elem[2][2]*t
				};
				return andromeda::traits::cast<Matrix3x3<R> >(m_elem);
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()*std::declval<T2>())>mul(Matrix<3,3,T2>&m)
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]*m.elem[0][0]+elem[0][1]*m.elem[1][0]+elem[0][2]*m.elem[2][0],elem[0][0]*m.elem[0][1]+elem[0][1]*m.elem[1][1]+elem[0][2]*m.elem[2][1],elem[0][0]*m.elem[0][2]+elem[0][1]*m.elem[1][2]+elem[0][2]*m.elem[2][2],
					elem[1][0]*m.elem[0][0]+elem[1][1]*m.elem[1][0]+elem[1][2]*m.elem[2][0],elem[1][0]*m.elem[0][1]+elem[1][1]*m.elem[1][1]+elem[1][2]*m.elem[2][1],elem[1][0]*m.elem[0][2]+elem[1][1]*m.elem[1][2]+elem[1][2]*m.elem[2][2],
					elem[2][0]*m.elem[0][0]+elem[2][1]*m.elem[1][0]+elem[2][2]*m.elem[2][0],elem[2][0]*m.elem[0][1]+elem[2][1]*m.elem[1][1]+elem[2][2]*m.elem[2][1],elem[2][0]*m.elem[0][2]+elem[2][1]*m.elem[1][2]+elem[2][2]*m.elem[2][2]
				};
				return andromeda::traits::cast<Matrix3x3<R> >(m_elem);
			}

			template<typename T2>
			inline Vector<3,decltype(std::declval<T>()*std::declval<T2>())>mul(Vector<3,T2>&v)
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]*v[0]+elem[0][1]*v[1]+elem[0][2]*v[2],
					elem[1][0]*v[0]+elem[1][1]*v[1]+elem[1][2]*v[2],
					elem[2][0]*v[0]+elem[2][1]*v[1]+elem[2][2]*v[2]
				};
				return andromeda::traits::cast<Vector<3,R> >(m_elem);
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()/std::declval<T2>())>div(T2 t) //除法
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]/t,elem[0][1]/t,elem[0][2]/t,
					elem[1][0]/t,elem[1][1]/t,elem[1][2]/t,
					elem[2][0]/t,elem[2][1]/t,elem[2][2]/t
				};
				return andromeda::traits::cast<Matrix3x3<R> >(m_elem);
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()+std::declval<T2>())>add(Matrix<3,3,T2>&m)
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]+m.elem[0][0],elem[0][1]+m.elem[0][1],elem[0][2]+m.elem[0][2],
					elem[1][0]+m.elem[1][0],elem[1][1]+m.elem[1][1],elem[1][2]+m.elem[1][2],
					elem[2][0]+m.elem[2][0],elem[2][1]+m.elem[2][1],elem[2][2]+m.elem[2][2]
				};
				return andromeda::traits::cast<Matrix3x3<R> >(m_elem);
			}

			template<typename T2>
			inline Matrix3x3<decltype(std::declval<T>()-std::declval<T2>())>sub(Matrix<3,3,T2>&m)
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]-m.elem[0][0],elem[0][1]-m.elem[0][1],elem[0][2]-m.elem[0][2],
					elem[1][0]-m.elem[1][0],elem[1][1]-m.elem[1][1],elem[1][2]-m.elem[1][2],
					elem[2][0]-m.elem[2][0],elem[2][1]-m.elem[2][1],elem[2][2]-m.elem[2][2]
				};
				return andromeda::traits::cast<Matrix3x3<R> >(m_elem);
			}

			inline static Matrix3x3<T> zero()
			{
				T m_elem[]=
				{
					0,0,0,
					0,0,0,
					0,0,0
				};
				return andromeda::traits::cast<Matrix3x3<T> >(m_elem);
			}

			inline auto det() -> decltype(std::declval<T>()*std::declval<T>())
			{
				return elem[0][0]*(elem[1][1]*elem[2][2]-elem[1][2]*elem[2][1])+elem[0][1]*(elem[1][2]*elem[2][0]-elem[1][0]*elem[2][2])+elem[0][2]*(elem[1][0]*elem[2][1]-elem[1][1]*elem[2][0]);
			}

			inline auto trace() -> decltype(std::declval<T>()+std::declval<T>()) //迹
			{
				return elem[0][0]+elem[1][1]+elem[2][2];
			}

			inline static Matrix3x3<T> identity()
			{
				T m_elem[]=
				{
					1,0,0,
					0,1,0,
					0,0,1
				};
				return andromeda::traits::cast<Matrix3x3<T> >(m_elem);
			}
		};

		using Matrix3x3f=Matrix3x3<float>;

		//4x4方阵特化
		template<typename T>
		class Matrix4x4:public SquareMatrix<4,T>
		{
		public:
			using Matrix<4,4,T>::elem;
			using Matrix<4,4,T>::type;
			using Matrix<4,4,T>::row;
			using Matrix<4,4,T>::column;
			using Matrix<4,4,T>::operator+;
			using Matrix<4,4,T>::operator-;
			using Matrix<4,4,T>::operator*;
			using Matrix<4,4,T>::operator/;

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix4x4<T2>&m)
			{
				return add(m);
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix4x4<T2>&m)
			{
				return sub(m);
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix4x4<T2>&m)
			{
				return mul(m);
			}

			template<typename T2>
			inline Vector<4,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<4,T2>&v)
			{
				return mul(v);
			}

			template<typename T2>
			inline Vector<3,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<3,T2>&v)
			{
				return mul(v);
			}

			//右值处理
			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()+std::declval<T2>())>operator +(Matrix4x4<T2>&&m)
			{
				return add(andromeda::traits::cast<Matrix4x4<T2> >(m));
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()-std::declval<T2>())>operator -(Matrix4x4<T2>&&m)
			{
				return sub(andromeda::traits::cast<Matrix4x4<T2> >(m));
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()*std::declval<T2>())>operator *(Matrix4x4<T2>&&m)
			{
				return mul(andromeda::traits::cast<Matrix4x4<T2> >(m));
			}

			template<typename T2>
			inline Vector<4,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<4,T2>&&v)
			{
				return mul(andromeda::traits::cast<Vector<4,T2> >(v));
			}

			template<typename T2>
			inline Vector<3,decltype(std::declval<T>()*std::declval<T2>())>operator *(Vector<3,T2>&&v)
			{
				return mul(andromeda::traits::cast<Vector<3,T2> >(v));
			}

			template<typename T2>
			inline operator Matrix<4,4,T2>()
			{
				T2 m_elem[]=
				{
					(T2)elem[0][0],(T2)elem[0][1],(T2)elem[0][2],(T2)elem[0][3],
					(T2)elem[1][0],(T2)elem[1][1],(T2)elem[1][2],(T2)elem[1][3],
					(T2)elem[2][0],(T2)elem[2][1],(T2)elem[2][2],(T2)elem[2][3],
					(T2)elem[3][0],(T2)elem[3][1],(T2)elem[3][2],(T2)elem[3][3],
				};
				return andromeda::traits::cast<Matrix<4,4,T2> >(m_elem);
			}

			inline Vector<4,T> extractColumn(int column)
			{
				T m_elem[]=
				{
					elem[0][column],elem[1][column],elem[2][column],elem[3][column]
				};
				return andromeda::traits::cast<Vector<4,T> >(m_elem);
			}

			inline Matrix4x4<T>transpose() //转置
			{
				T m_elem[]=
				{
					elem[0][0],elem[1][0],elem[2][0],elem[3][0],
					elem[0][1],elem[1][1],elem[2][1],elem[3][1],
					elem[0][2],elem[1][2],elem[2][2],elem[3][2],
					elem[0][3],elem[1][3],elem[2][3],elem[3][3]
				};
				return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()*std::declval<T2>())>dot(Matrix<4,4,T2>&m) //按位相乘
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]*m.elem[0][0],elem[0][1]*m.elem[0][1],elem[0][2]*m.elem[0][2],elem[0][3]*m.elem[0][3],
					elem[1][0]*m.elem[1][0],elem[1][1]*m.elem[1][1],elem[1][2]*m.elem[1][2],elem[1][3]*m.elem[1][3],
					elem[2][0]*m.elem[2][0],elem[2][1]*m.elem[2][1],elem[2][2]*m.elem[2][2],elem[2][3]*m.elem[2][3],
					elem[3][0]*m.elem[3][0],elem[3][1]*m.elem[3][1],elem[3][2]*m.elem[3][2],elem[3][3]*m.elem[3][3]
				};
				return andromeda::traits::cast<Matrix4x4<R> >(m_elem);
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()*std::declval<T2>())>mul(T2 t) //数乘
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]*t,elem[0][1]*t,elem[0][2]*t,elem[0][3]*t,
					elem[1][0]*t,elem[1][1]*t,elem[1][2]*t,elem[1][3]*t,
					elem[2][0]*t,elem[2][1]*t,elem[2][2]*t,elem[2][3]*t,
					elem[3][0]*t,elem[3][1]*t,elem[3][2]*t,elem[3][3]*t
				};
				return andromeda::traits::cast<Matrix4x4<R> >(m_elem);
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()*std::declval<T2>())>mul(Matrix<4,4,T2>&m)
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]*m.elem[0][0]+elem[0][1]*m.elem[1][0]+elem[0][2]*m.elem[2][0]+elem[0][3]*m.elem[3][0], elem[0][0]*m.elem[0][1]+elem[0][1]*m.elem[1][1]+elem[0][2]*m.elem[2][1]+elem[0][3]*m.elem[3][1], elem[0][0]*m.elem[0][2]+elem[0][1]*m.elem[1][2]+elem[0][2]*m.elem[2][2]+elem[0][3]*m.elem[3][2], elem[0][0]*m.elem[0][3]+elem[0][1]*m.elem[1][3]+elem[0][2]*m.elem[2][3]+elem[0][3]*m.elem[3][3],
					elem[1][0]*m.elem[0][0]+elem[1][1]*m.elem[1][0]+elem[1][2]*m.elem[2][0]+elem[1][3]*m.elem[3][0], elem[1][0]*m.elem[0][1]+elem[1][1]*m.elem[1][1]+elem[1][2]*m.elem[2][1]+elem[1][3]*m.elem[3][1], elem[1][0]*m.elem[0][2]+elem[1][1]*m.elem[1][2]+elem[1][2]*m.elem[2][2]+elem[1][3]*m.elem[3][2], elem[1][0]*m.elem[0][3]+elem[1][1]*m.elem[1][3]+elem[1][2]*m.elem[2][3]+elem[1][3]*m.elem[3][3],
					elem[2][0]*m.elem[0][0]+elem[2][1]*m.elem[1][0]+elem[2][2]*m.elem[2][0]+elem[2][3]*m.elem[3][0], elem[2][0]*m.elem[0][1]+elem[2][1]*m.elem[1][1]+elem[2][2]*m.elem[2][1]+elem[2][3]*m.elem[3][1], elem[2][0]*m.elem[0][2]+elem[2][1]*m.elem[1][2]+elem[2][2]*m.elem[2][2]+elem[2][3]*m.elem[3][2], elem[2][0]*m.elem[0][3]+elem[2][1]*m.elem[1][3]+elem[2][2]*m.elem[2][3]+elem[2][3]*m.elem[3][3],
					elem[3][0]*m.elem[0][0]+elem[3][1]*m.elem[1][0]+elem[3][2]*m.elem[2][0]+elem[3][3]*m.elem[3][0], elem[3][0]*m.elem[0][1]+elem[3][1]*m.elem[1][1]+elem[3][2]*m.elem[2][1]+elem[3][3]*m.elem[3][1], elem[3][0]*m.elem[0][2]+elem[3][1]*m.elem[1][2]+elem[3][2]*m.elem[2][2]+elem[3][3]*m.elem[3][2], elem[3][0]*m.elem[0][3]+elem[3][1]*m.elem[1][3]+elem[3][2]*m.elem[2][3]+elem[3][3]*m.elem[3][3],
				};
				return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
			}

			template<typename T2>
			inline Vector<4,decltype(std::declval<T>()*std::declval<T2>())>mul(Vector<4,T2>&v)
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R v_elem[]=
				{
					elem[0][0]*v[0]+elem[0][1]*v[1]+elem[0][2]*v[2]+elem[0][3]*v[3],
					elem[1][0]*v[0]+elem[1][1]*v[1]+elem[1][2]*v[2]+elem[1][3]*v[3],
					elem[2][0]*v[0]+elem[2][1]*v[1]+elem[2][2]*v[2]+elem[2][3]*v[3],
					elem[3][0]*v[0]+elem[3][1]*v[1]+elem[3][2]*v[2]+elem[3][3]*v[3]
				};
				return andromeda::traits::cast<Vector<4,R> >(v_elem);
			}

			template<typename T2>
			inline Vector<3,decltype(std::declval<T>()*std::declval<T2>())>mul(Vector<3,T2>&v)//差的维度补值为1
			{
				typedef decltype(std::declval<T>()*std::declval<T2>()) R;
				R v_elem[]=
				{
					elem[0][0]*v[0]+elem[0][1]*v[1]+elem[0][2]*v[2]+elem[0][3],
					elem[1][0]*v[0]+elem[1][1]*v[1]+elem[1][2]*v[2]+elem[1][3],
					elem[2][0]*v[0]+elem[2][1]*v[1]+elem[2][2]*v[2]+elem[2][3],
				};
				return andromeda::traits::cast<Vector<3,R> >(v_elem);
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()/std::declval<T2>())>div(T2 t) //除法
			{
				typedef decltype(std::declval<T>()/std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]/t,elem[0][1]/t,elem[0][2]/t,elem[0][3]/t,
					elem[1][0]/t,elem[1][1]/t,elem[1][2]/t,elem[1][3]/t,
					elem[2][0]/t,elem[2][1]/t,elem[2][2]/t,elem[2][3]/t,
					elem[3][0]/t,elem[3][1]/t,elem[3][2]/t,elem[3][3]/t
				};
				return andromeda::traits::cast<Matrix4x4<R> >(m_elem);
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()+std::declval<T2>())>add(Matrix<4,4,T2>&m)
			{
				typedef decltype(std::declval<T>()+std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]+m.elem[0][0],elem[0][1]+m.elem[0][1],elem[0][2]+m.elem[0][2],elem[0][3]+m.elem[0][3],
					elem[1][0]+m.elem[1][0],elem[1][1]+m.elem[1][1],elem[1][2]+m.elem[1][2],elem[1][3]+m.elem[1][3],
					elem[2][0]+m.elem[2][0],elem[2][1]+m.elem[2][1],elem[2][2]+m.elem[2][2],elem[2][3]+m.elem[2][3],
					elem[3][0]+m.elem[3][0],elem[3][1]+m.elem[3][1],elem[3][2]+m.elem[3][2],elem[3][3]+m.elem[3][3]
				};
				return andromeda::traits::cast<Matrix4x4<R> >(m_elem);
			}

			template<typename T2>
			inline Matrix4x4<decltype(std::declval<T>()-std::declval<T2>())>sub(Matrix<4,4,T2>&m)
			{
				typedef decltype(std::declval<T>()-std::declval<T2>()) R;
				R m_elem[]=
				{
					elem[0][0]-m.elem[0][0],elem[0][1]-m.elem[0][1],elem[0][2]-m.elem[0][2],elem[0][3]-m.elem[0][3],
					elem[1][0]-m.elem[1][0],elem[1][1]-m.elem[1][1],elem[1][2]-m.elem[1][2],elem[1][3]-m.elem[1][3],
					elem[2][0]-m.elem[2][0],elem[2][1]-m.elem[2][1],elem[2][2]-m.elem[2][2],elem[2][3]-m.elem[2][3],
					elem[3][0]-m.elem[3][0],elem[3][1]-m.elem[3][1],elem[3][2]-m.elem[3][2],elem[3][3]-m.elem[3][3]
				};
				return andromeda::traits::cast<Matrix4x4<R> >(m_elem);
			}

			inline static Matrix4x4<T> zero()
			{
				T m_elem[]=
				{
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0
				};
				return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
			}

			inline auto det() -> decltype(std::declval<T>()*std::declval<T>())
			{
				return elem[0][0]*(elem[1][1]*elem[2][2]*elem[3][3]-elem[3][1]*elem[2][2]*elem[1][3])+elem[0][1]*(elem[1][2]*elem[2][3]*elem[3][0]-elem[1][0]*elem[2][3]*elem[3][2])+elem[0][2]*(elem[1][3]*elem[2][0]*elem[3][1]-elem[1][1]*elem[2][0]*elem[3][3])+elem[0][3]*(elem[1][0]*elem[2][1]*elem[3][2]-elem[1][2]*elem[2][1]*elem[3][0]);
			}

			inline auto trace() -> decltype(std::declval<T>()+std::declval<T>()) //迹
			{
				return elem[0][0]+elem[1][1]+elem[2][2]+elem[3][3];
			}

			inline static Matrix4x4<T> identity()
			{
				T m_elem[]=
				{
					1,0,0,0,
					0,1,0,0,
					0,0,1,0,
					0,0,0,1
				};
				return andromeda::traits::cast<Matrix4x4<T> >(m_elem);
			}
		};

		using Matrix4x4f=Matrix4x4<float>;
	}
}
#endif//ANDROMEDA_MATH_MATRIX
