#ifndef ANDROMEDA_GRAPHICS_INTERNALSHADERS
#define ANDROMEDA_GRAPHICS_INTERNALSHADERS

#include "shader_program.h"

namespace andromeda {
	namespace graphics {
		extern const char* pct_pass_vertex_shader;
		extern const char* pct_pass_vertex_instanced_shader;
		extern const char* pct_default_fragment_shader;
		extern const char* pt_pass_vertex_shader;
		extern const char* pt_pass_vertex_instanced_shader;
		extern const char* pt_default_fragment_shader;
		//用于修复窗体透明像素的着色器
		extern const char* pt_transparent_fix_fragment_shader;

		__attribute__((always_inline)) inline ShaderProgram& getPCTDefaultShaderProgram() //获取默认着色程序
		{
			static ShaderProgram pct_default_shader_program=ShaderProgram(pct_pass_vertex_shader,pct_default_fragment_shader);
			return pct_default_shader_program;
		}

		__attribute__((always_inline)) inline ShaderProgram& getPTDefaultShaderProgram() //获取默认实例化绘制着色程序
		{
			static ShaderProgram pt_default_shader_program=ShaderProgram(pt_pass_vertex_shader,pt_default_fragment_shader);
			return pt_default_shader_program;
		}

		__attribute__((always_inline)) inline ShaderProgram& getPCTIDefaultShaderProgram() //获取默认着色程序
		{
			static ShaderProgram pct_default_instanced_shader_program=ShaderProgram(pct_pass_vertex_instanced_shader,pct_default_fragment_shader);
			return pct_default_instanced_shader_program;
		}

		__attribute__((always_inline)) inline ShaderProgram& getPTIDefaultShaderProgram() //获取默认实例化绘制着色程序
		{
			static ShaderProgram pt_default_instanced_shader_program=ShaderProgram(pt_pass_vertex_instanced_shader,pt_default_fragment_shader);
			return pt_default_instanced_shader_program;
		}
	}
}

#endif //ANDROMEDA_GRAPHICS_INTERNALSHADERS
