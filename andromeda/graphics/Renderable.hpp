#ifndef ANDROMEDA_GRAPHICS_RENDERABLE
#define ANDROMEDA_GRAPHICS_RENDERABLE

#include <map>
#include "../../lib/opengl/glad/glad.h"

#include "../util/ArrayList.hpp"
#include "Texture2D.hpp"
#include "VertexAttribute.hpp"
#include "ShaderProgram.hpp"
#include "../math/Vector.hpp"
#include "../math/Matrix.hpp"
#include "../util/ArrayList.hpp"

#define DIRECT_INSTANCE_CAPCITY 1024

namespace andromeda {
	namespace graphics {
		class Renderable;

		//同一个对象的不同实例
		class RenderableInstance
		{
			friend class Renderable;
		private:
			andromeda::math::Matrix4x4f linear_transform;
		public:
			RenderableInstance()=default;
		};

		//绘制策略为NONE则不绘制；NORMAL表示普通绘制，INSTANCED表示实例化绘制
		enum struct DrawStrategy:GLuint
		{
			NONE,NORMAL,INSTANCED
		};

		//只包含渲染物体本身的信息（顶点数据、着色器、纹理等），不包括位置、变换等（这些属于实例的信息，不同实例可以共享同一个Renderable对象），等价于Mesh
		class Renderable
		{
		private:
			GLuint vao=0;
			GLuint vbo=0;
			GLuint ebo=0;
			DrawStrategy draw_strategy=DrawStrategy::NONE;
			andromeda::util::ArrayList<RenderableInstance> direct_instance_list;//不具名的实例对象，通常是创建后不需要操作的对象
			std::map<const char*,RenderableInstance> named_instance_map;//具名的实例对象

			VertexAttribute* vertex_attribs=nullptr;
			ShaderProgram* shader=nullptr;
		protected:
			void pretransformVertex();

		public:
			andromeda::util::ArrayList<float> vertex_data; //顶点数据的指针，采用局部坐标系
			andromeda::util::ArrayList<unsigned int> element_data; //索引数据的指针，可选
			andromeda::util::ArrayList<Texture2D> textures; //索引对应不同纹理单元，默认只有一个纹理单元0。纹理均为0级纹理，minimap靠程序自动生成
			andromeda::math::Matrix4x4f vertex_pretransform=andromeda::math::Matrix4x4f::identity();//加载模型后需要进行的变换
			GLuint geo_strategy=GL_TRIANGLE_STRIP; //绘制的几何形状
			GLuint data_strategy=GL_STATIC_DRAW;//数据策略，静态数据、动态改变数据或流式数据

			Renderable();

			//传入id返回对应的具名实例(如果该具名实例不存在则新建)，或传入nullptr得到一个不可被索引的不具名实例
			RenderableInstance& instance(const char* id=nullptr);

			inline operator GLuint()
			{
				return vao;
			}

			inline size_t getVertexCount()
			{
				return vertex_data.getLength()/vertex_attribs->getVertexSize();
			}

			inline size_t getElementCount()
			{
				return element_data.getLength();
			}

			inline void use()
			{
				glBindVertexArray(vao);
			}

			void setVertexAttributeShader(VertexAttribute* vertex_attribs=nullptr,ShaderProgram* shader=nullptr);

			//设置绘制策略，需要附带指定顶点属性和着色器，否则使用默认的PT或PTI着色器程序
			inline void setDrawStrategy(DrawStrategy strategy,VertexAttribute* vertex_attribs=nullptr,ShaderProgram* shader=nullptr)
			{
				draw_strategy=strategy;
				setVertexAttributeShader(vertex_attribs,shader);
			}

			__attribute__((always_inline)) inline void draw() //bufferVertexData()后才可以调用，否则报错
			{
				shader->use();
				use();
				switch(draw_strategy)
				{
				case DrawStrategy::INSTANCED:
				{
					size_t instance_count=direct_instance_list.getLength()+named_instance_map.size();
					if(ebo)
						glDrawElementsInstanced(geo_strategy,getElementCount(),GL_UNSIGNED_INT,0,instance_count);
					else
						glDrawArraysInstanced(geo_strategy,0,getVertexCount(),instance_count);
				}
					break;
				case DrawStrategy::NORMAL:
					if(ebo)
						glDrawElements(geo_strategy,getElementCount(),GL_UNSIGNED_INT,0);
					else
						glDrawArrays(geo_strategy,0,getVertexCount());
					break;
				}
			}

			void loadTextures(bool generate_minimap=true,bool release_image=true); //加载所有纹理到对应的纹理单元
			void useTextures(); //使用所有纹理（在不同纹理单元中）

			void bufferVertexData(); //加载顶点数据以及索引数据（如果有的话）进OpenGL，并设置好vertex_attribs对应的顶点属性格式

			inline Renderable& setVertexData(float* vertex_data,size_t vertex_count,unsigned int* element_data=nullptr,size_t element_count=0)
			{
				this->vertex_data=andromeda::util::ArrayList<float>(vertex_data,vertex_count);
				this->element_data=andromeda::util::ArrayList<unsigned int>(element_data,element_count);
				return *this;
			}

			template<typename ...Textures>
			inline void addTextures(Textures ... _textures) //必须全部传入Texture2D类型的对象
			{
				textures.add(_textures...);
			}
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_RENDERABLE