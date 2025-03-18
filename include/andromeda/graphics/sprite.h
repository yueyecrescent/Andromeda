#ifndef ANDROMEDA_GRAPHICS_SPRITE
#define ANDROMEDA_GRAPHICS_SPRITE

#include <opengl/glad/glad.h>
#include <map>
#include "texture_2d.h"
#include "coord_transform.h"
#include "renderable.h"
#include "../io/files.h"
#include "../util/array_list.h"

namespace andromeda {
	namespace graphics {
		//精灵裁剪UV坐标，左下角为原点，向上向右为正
		typedef struct SpriteUvCoord
		{
			float u1,v1,u2,v2; //uv1为左下角，uv2为右上角
		} SpriteUvIndex;

		//精灵裁剪像素坐标，左上角为原点，向右向下为正
		typedef struct SpritePixelCoord
		{
			float u1,v1,u2,v2; //uv1为左下角，uv2为右上角
		} SpritePixelCoord;

		typedef struct SpriteFrame
		{
			SpriteUvCoord uv_index;
			float time; //持续时间
		} SpriteFrame;

		//自动裁剪的精灵表
		template<typename IndexType,typename Derived>
		class SpriteSheet
		{
		protected:
			Texture2D sheet_texture;
			andromeda::util::ArrayList<SpriteUvCoord> splited_area;
		public:
			SpriteSheet()=default;

			SpriteSheet(Texture2D texture,andromeda::util::ArrayList<SpriteUvCoord> splited_area) :
					sheet_texture(texture), splited_area(splited_area)
			{

			}

			__attribute__((always_inline)) inline operator GLuint()
			{
				return sheet_texture.texture_id;
			}

			__attribute__((always_inline)) inline operator Texture2D()
			{
				return sheet_texture;
			}

			inline SpriteUvCoord clip(IndexType index)
			{
				return (Derived*)this->clip(index);
			}

			//GUI坐标系下的裁剪
			inline SpriteUvCoord clipInGuiCoord(int x,int y,int width,int height)
			{
				SpriteUvCoord area;
				gui2uv(x,y,x+width,y+height,area.u1,area.v1,area.u2,area.v2,&sheet_texture);
				return area;
			}

			inline SpriteUvCoord clipInNormalizedCoord(float x,float y,float width,float height)
			{
				SpriteUvCoord area;
				gui2uv(x,y,x+width,y+height,area.u1,area.v1,area.u2,area.v2,nullptr);
				return area;
			}

			inline SpriteUvCoord clipInUvCoord(float x,float y,float width,float height)
			{
				return SpriteUvCoord{x,y,x+width,y+height};
			}

			inline SpriteUvCoord clip(int index)
			{
				return splited_area[index];
			}
		};

		//裁剪索引，左上角为原点，向右向下为正
		typedef struct SpriteSheetIndex
		{
			int x,y;
		} SpriteSheetIndex;

		//等分裁剪，每个裁剪区域相同大小
		class EqualDivisionSpriteSheet:public SpriteSheet<SpriteSheetIndex,EqualDivisionSpriteSheet>
		{
		private:
			using SpriteSheet<SpriteSheetIndex,EqualDivisionSpriteSheet>::sheet_texture;
			//多少行多少列
			int column,row;
			//float均为归一化坐标，裁剪区域在精灵表中的坐标，左上角为原点，向右向下为正
			float clip_stride_x,clip_stride_y; //裁剪步长，两个相邻精灵图像之间的UV坐标差
			float clip_offset_x,clip_offset_y; //有效区域左上角顶点位移
			float clip_end_offset_x,clip_end_offset_y; //有效区域右下角顶点位移

			float sprite_width,sprite_height; //裁剪单个精灵图像的UV坐标宽高

			__attribute__((always_inline)) inline void claclSpriteSize()
			{
				sprite_width=(clip_end_offset_x-clip_offset_x-clip_stride_x*(column-1))/column;
				sprite_height=(clip_end_offset_y-clip_offset_y-clip_stride_y*(row-1))/row;
			}

			void generateSprites();
		public:
			EqualDivisionSpriteSheet(Texture2D texture,int column=1,int row=1,float clip_stride_x=0,float clip_stride_y=0,float clip_offset_x=0,float clip_offset_y=0,float clip_end_offset_x=1,float clip_end_offset_y=1) :
					column(column), row(row), clip_stride_x(clip_stride_x), clip_stride_y(clip_stride_y), clip_offset_x(clip_offset_x), clip_offset_y(clip_offset_y), clip_end_offset_x(clip_end_offset_x), clip_end_offset_y(clip_end_offset_y)
			{
				sheet_texture=texture;
				claclSpriteSize();
				generateSprites();
			}

			EqualDivisionSpriteSheet(Texture2D texture,int column=1,int row=1,int clip_stride_x=0,int clip_stride_y=0,float clip_offset_x=0,float clip_offset_y=0,float clip_end_offset_x=1,float clip_end_offset_y=1) :
					column(column), row(row), clip_stride_x((float)clip_stride_x/texture.getWidth()), clip_stride_y((float)clip_stride_y/texture.getHeight()), clip_offset_x(clip_offset_x), clip_offset_y(clip_offset_y), clip_end_offset_x(clip_end_offset_x), clip_end_offset_y(clip_end_offset_y)
			{
				sheet_texture=texture;
				claclSpriteSize();
				generateSprites();
			}

			EqualDivisionSpriteSheet(Texture2D texture,int column=1,int row=1,int clip_stride_x=0,int clip_stride_y=0,int clip_offset_x=0,int clip_offset_y=0,int clip_end_offset_x=1,int clip_end_offset_y=1) :
					column(column), row(row), clip_stride_x((float)clip_stride_x/texture.getWidth()), clip_stride_y((float)clip_stride_y/texture.getHeight()), clip_offset_x((float)clip_offset_x/texture.getWidth()), clip_offset_y((float)clip_offset_y/texture.getHeight()), clip_end_offset_x((float)clip_end_offset_x/texture.getWidth()), clip_end_offset_y((float)clip_end_offset_y/texture.getHeight())
			{
				sheet_texture=texture;
				claclSpriteSize();
				generateSprites();
			}

			inline SpriteUvCoord clip(SpriteSheetIndex xy_index)
			{
				return clip(xy_index.x,xy_index.y);
			}

			inline SpriteUvCoord clip(int x,int y)
			{
				return SpriteUvCoord{clip_offset_x+(sprite_width+clip_stride_x)*x,1-(clip_offset_y+(sprite_height+clip_stride_y)*y+sprite_height),clip_offset_x+(sprite_width+clip_stride_x)*x+sprite_width,1-(clip_offset_y+(sprite_height+clip_stride_y)*y)};
			}
		};

		//指定裁剪区域是采用UV坐标裁剪还是像素坐标裁剪
		enum SpriteSplitMode
		{
			UV_COORD, PIXEL_COORD
		};

		extern const char* default_split_info_names[4];

		//紧凑的精灵表，裁剪区域大小、位置不固定，需要额外的CSV文件指定每个裁剪区域的位置和尺寸
		class CompactSpriteSheet:public SpriteSheet<std::string,CompactSpriteSheet>
		{
		private:
			using SpriteSheet<std::string,CompactSpriteSheet>::sheet_texture;
			using SpriteSheet<std::string,CompactSpriteSheet>::splited_area;
			std::map<std::string,int> idx_mapping;

		public:
			//info_names依次储存了CSV中切割精灵的左上角x、左上角y、宽度、高度列的名字，mode指定坐标是UV坐标（原点在左下角）还是像素坐标（原点在左上角）
			CompactSpriteSheet(Texture2D texture,const char* split_file,char comma=',',andromeda::io::CsvOption option=andromeda::io::CsvOption::BOTH_HEADER,const char** info_names=default_split_info_names,SpriteSplitMode mode=PIXEL_COORD,AreaCoordTransform coord_transform=&gui2uv);

			inline SpriteUvCoord clip(std::string split_key)
			{
				return splited_area[idx_mapping[split_key]];
			}
		};

		class Sprite:public Renderable
		{
			void* sprite_sheet;
		};
	}
}

#endif
