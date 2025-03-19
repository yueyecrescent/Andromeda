#ifndef ANDROMEDA_GRAPHICS_GEOMETRY
#define ANDROMEDA_GRAPHICS_GEOMETRY

#include "renderable.h"

namespace andromeda {
	namespace graphics {
		//多边形默认深度z坐标为0，点在xy平面上
		class Polygon:public Renderable
		{
		private:
			using Renderable::shader;
			using Renderable::vertex_attribs;
			using Renderable::geo_strategy;
			andromeda::util::ArrayList<andromeda::math::Vector2f> vertices;
			andromeda::util::ArrayList<float> vertex_extra_data;

		public:
			using Renderable::vertex_data;
			using Renderable::element_data;
			using Renderable::textures;

			template<typename ...Ts>
			__attribute__((always_inline)) inline void addVertex(andromeda::math::Vector2f pos,Ts ... extra_data)
			{
				vertices.add(pos);
				vertex_extra_data.add(extra_data...);
			}

			void generateVertexData();

			static Polygon Quad();
		};
	}
}

#endif//ANDROMEDA_GRAPHICS_GEOMETRY
