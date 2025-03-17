#ifndef ANDROMEDA_GRAPHICS_QUAD
#define ANDROMEDA_GRAPHICS_QUAD

#include "renderable.h"

namespace andromeda {
	namespace graphics {
		class Quad:public Renderable
		{
		public:
			andromeda::math::Vector3f point;//固着坐标系原点在世界中的绝对坐标
			andromeda::math::Vector3f normal;

			andromeda::math::Vector3f vertex[4];//相对于原点的坐标。


		};
	}
}

#endif//ANDROMEDA_GRAPHICS_QUAD
