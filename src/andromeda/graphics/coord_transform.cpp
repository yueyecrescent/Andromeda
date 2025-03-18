#include <andromeda/graphics/coord_transform.h>

namespace andromeda {
	namespace graphics {
		void gui2uv(float input_x1,float input_y1,float input_x2,float input_y2,float& output_x1,float& output_y1,float& output_x2,float& output_y2,Texture2D* texture)
		{
			if(texture)
			{
				input_x1/=texture->getWidth();
				input_y1/=texture->getHeight();
				input_x2/=texture->getWidth();
				input_y2/=texture->getHeight();
			}
			output_x1=input_x1;
			output_y1=1-input_y2;
			output_x2=input_x2;
			output_y2=1-input_y1;
		}
	}
}

