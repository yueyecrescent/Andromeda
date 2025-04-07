#ifndef ANDROMEDA_APP_WINDOW
#define ANDROMEDA_APP_WINDOW

#include <opengl/glad/glad.h>
#include <opengl/GLFW/glfw3.h>
#include "../graphics/color_rgba.h"

namespace andromeda {
	namespace app {
		class Window
		{
		private:
			GLFWwindow* window_id;
			GLFWmonitor* monitor;
			int width,height;
			bool is_fullScreen;
			const char* title;
			andromeda::graphics::ColorRGBA back_color;

		public:
			Window()=default; //该构造函数不会实际初始化窗口仅分配内存，需要重新调用其他构造函数以正常工作
			Window(const char* title,int width=800,int height=600,andromeda::graphics::ColorRGBA backColor_={0,0,0,0},bool isfullscreen=false,GLFWmonitor* monitor_=glfwGetPrimaryMonitor());

			inline operator GLFWwindow*()
			{
				return window_id;
			}

			inline void setBackColor(andromeda::graphics::ColorRGBA backColor_={0,0,0,0})
			{
				back_color=backColor_;
			}

			inline andromeda::graphics::ColorRGBA getBackColor()
			{
				return back_color;
			}

			inline Window& setBackColor(andromeda::graphics::ColorChannel ch,float v)
			{
				back_color.setRGBA(ch,v);
				return *this;
			}

			inline float getBackColor(andromeda::graphics::ColorChannel ch)
			{
				return back_color.getRGBA(ch);
			}

			inline int getWidth(void)
			{
				return width;
			}

			inline int getHeight(void)
			{
				return height;
			}

			inline bool isWindowFullScreen(void)
			{
				return is_fullScreen;
			}

			inline Window& setFullScreen(bool isFullScreen_)
			{
				is_fullScreen=isFullScreen_;
				glfwSetWindowMonitor(window_id,isFullScreen_?monitor:nullptr,0,0,width,height,GLFW_DONT_CARE);
				return *this;
			}

			inline Window& setWindowed(bool windowed,int pos_x,int pos_y)
			{
				this->monitor=NULL;
				glfwSetWindowMonitor(window_id,NULL,pos_x,pos_y,width,height,0);
				return *this;
			}

			inline Window& setMonitor(GLFWmonitor* monitor)
			{
				this->monitor=monitor;
				const GLFWvidmode* mode=glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(window_id,monitor,0,0,mode->width,mode->height,mode->refreshRate);
				return *this;
			}

			inline Window& setWindowSize(int width_,int height_)
			{
				width=width_;
				height=height_;
				glfwSetWindowSize(window_id,width_,height_);
				return *this;
			}

			inline bool setIsAlwaysOnTop(bool floating) //设置窗口是否总是置顶
			{
				glfwSetWindowAttrib(window_id,GLFW_FLOATING,floating);
				return glfwGetWindowAttrib(window_id,GLFW_FLOATING); //设置成功则返回true
			}

			inline void setWindowFramebufferTransparent(bool transparent=false) //设置窗口渲染的背景是否透明。注意即使背景透明，事件依旧不会穿透窗口透明部分，它们会被窗口捕获
			{
				glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,transparent);
			}

			inline bool isWindowFramebufferTransparent()
			{
				return glfwGetWindowAttrib(window_id,GLFW_TRANSPARENT_FRAMEBUFFER);
			}

			inline float setWindowOpacity(float opacity=1) //窗口整体的透明度设置，位于0-1间
			{
				glfwSetWindowOpacity(window_id,opacity);
				return glfwGetWindowOpacity(window_id); //返回当前的窗口透明度。如果设置成功则应该返回opacity
			}

#ifdef GLFW_MOUSE_PASSTHROUGH
			inline void setWindowMouseEventPassthrough(bool passthrough) //设置窗口鼠标事件是否穿透，glfw3.4版本及以上可用，整个窗口均会鼠标事件穿透
			{
				glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,passthrough);
			}
#endif
		};
	}
}

#endif//ANDROMEDA_APP_WINDOW
