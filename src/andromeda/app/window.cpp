#include <andromeda/app/window.h>
#include <andromeda/util/debug.h>

using namespace andromeda::app;
using namespace andromeda::media;

namespace andromeda {
	extern void _glfw_framebuffer_size_callback(GLFWwindow *window,int width,int height);
}

Window::Window(const char *title,int width,int height,ColorRGBA backColor_,bool isfullscreen,GLFWmonitor *monitor_) :
		width(width), height(height), is_fullScreen(isfullscreen), title(title), back_color(backColor_), monitor(monitor_)
{
	if(!title)
		LOG_COUT("Window name cannot be null")
	if(!(window_id=glfwCreateWindow(width,height,title,isfullscreen?monitor_:nullptr,nullptr)))
		LOG_COUT("Create Window failed")
	else
		glfwSetFramebufferSizeCallback(window_id,andromeda::_glfw_framebuffer_size_callback);
}
