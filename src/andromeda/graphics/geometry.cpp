#include <andromeda/graphics/geometry.h>

using namespace andromeda::graphics;

void Polygon::generateVertexData()
{
	geo_strategy=GeoStrategy::TRIANGLE_FAN;
	size_t point_num=vertices.length();
	for(size_t i=0;i<point_num;++i)
	{
		vertex_data.add();
	}
}
