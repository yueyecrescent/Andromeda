#ifndef ANDROMEDA_IMAGE_RASTERIMAGE
#define ANDROMEDA_IMAGE_RASTERIMAGE

#include "Pixel.hpp"
#include "imageUtils.hpp"
#include <malloc.h>

namespace andromeda {
	namespace image {
		class RasterImage
		{
		public:
			operator Pixel*()
			{
				return pixels;
			}

			operator unsigned char*()
			{
				return (unsigned char*)pixels;
			}

			operator void*()
			{
				return (void*)pixels;
			}

			operator const void*()
			{
				return (const void*)pixels;
			}

			RasterImage(int img_width=0,int img_height=0,Pixel* data=nullptr);//data指定为nullptr则自动分配内存

			static RasterImage& readImage(const char* image_path);
			void writeImage(const char* image_path);

			RasterImage copyRasterImage();

			Pixel* getPixelDataPtr()
			{
				return pixels;
			}

			inline int getWidth()
			{
				return width;
			}

			inline int getHeight()
			{
				return height;
			}

			void allocate();
			inline void release()
			{
				free((void*)pixels);
			}

			void setPixel(int x,int y,Pixel new_pixel)
			{
				*(Pixel*)(pixels+width*y+x)=new_pixel;
			}

			void setPixel(int x,int y,ColorRGBA new_pixel_color)
			{
				*(Pixel*)(pixels+width*y+x)=new_pixel_color.toPixel();
			}

			inline Pixel& getPixel(int x,int y)
			{
				return *(pixels+width*y+x);
			}

			//Operations
			void fillRect(int start_x,int start_y,int end_x,int end_y,Pixel pixel);
			void fillAll(Pixel pixel);

			void mix(RasterImage &img,int pos_x,int pos_y);
			void mulAlphaFactor(float factor);

			static RasterImage& cutImage(RasterImage& src_img,int start_x,int start_y,int end_x,int end_y);

			void setPixelData(Pixel* data,int img_width,int img_height,image_size_options op); //设置某个区域的图像，将覆盖区域原本的数据
		protected:
			int height=0,width=0;
			Pixel* pixels=nullptr;
		};
	}
}

#endif // ANDROMEDA_IMAGE_RASTERIMAGE
