#include  "Window.hpp"
#include "../macros/Debug.h"

using namespace andromeda::app;
using namespace andromeda::image;

namespace andromeda{
	extern void _glfw_framebuffer_size_callback(GLFWwindow* window,int width,int height);
}

Window::Window(const char* title,int width,int height,ColorRGBA backColor_,bool isfullscreen,GLFWmonitor* monitor_) :
		width(width), height(height), is_fullScreen(isfullscreen), title(title), back_color(backColor_), monitor(monitor_)
{
	if(!title)
		PRINT_MESSAGE("Window name cannot be null")
	if(!(window_id=glfwCreateWindow(width,height,title,isfullscreen?monitor_:nullptr,nullptr)))
		PRINT_MESSAGE("Create Window failed")
	else
		glfwSetFramebufferSizeCallback(window_id,andromeda::_glfw_framebuffer_size_callback);
}
