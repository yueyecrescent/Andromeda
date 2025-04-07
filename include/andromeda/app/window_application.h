#ifndef ANDROMEDA_APP_WINDOWAPPLICATION
#define ANDROMEDA_APP_WINDOWAPPLICATION

#include <opengl/glad/glad.h>
#include <opengl/GLFW/glfw3.h>
#include "window.h"
#include "frame_rate.h"
#include "../graphics/color_rgba.h"
#include "../graphics/framebuffer.h"
#include "../util/log.h"
#include "application.h"
#include "render_sys.h"

/**
 * 带有图形和音频的应用，可用于编写客户端
 * GLFW只能在主线程使用，因此主线程负责窗口事件及渲染
 */

namespace andromeda {
	extern bool use_opengl;
	extern bool use_openal;

	bool _load_opengl();

	namespace app {
		template<typename Derived>
		class WindowApplication:public andromeda::app::Application<Derived>
		{
			DefineApplication(Derived)
		private:
			using andromeda::app::Application<Derived>::is_running;
			using andromeda::app::Application<Derived>::synchronize_fps;
			using andromeda::app::Application<Derived>::main_loop_thread;
			Window window;
			FrameRate render_frame_rate; //渲染循环计数器
			andromeda::graphics::Framebuffer framebuffer; //双缓冲
			int render_fps_limit=-1;
			int update_rate_limit=-1;

		protected:
			using andromeda::app::Application<Derived>::_initialize;
			using andromeda::app::Application<Derived>::_preinitialize;
			using andromeda::app::Application<Derived>::_terminate;
			using andromeda::app::Application<Derived>::_update;
			using andromeda::app::Application<Derived>::_render_update;
			using andromeda::app::Application<Derived>::turn;

			RenderSys render_sys;

		public:
			WindowApplication(const char* window_title=nullptr,int width=800,int height=600,andromeda::graphics::ColorRGBA backColor_={0,0,0,0},bool isfullscreen=false,GLFWmonitor* monitor_=glfwGetPrimaryMonitor())
			{
				bool init_app=true; //如果需要的库没有加载，则不初始化该类，无法使用该类
				if(!andromeda::use_opengl)
				{
					LOG_ERROR("OpenGL is not used. Please set andromeda::use_opengl to true.")
					init_app=false;
				}
				if(!andromeda::use_openal)
				{
					LOG_ERROR("OpenAL is not used. Please set andromeda::use_openal to true.")
					init_app=false;
				}
				if(!init_app)
				{
					LOG_ERROR("Application not initialized.")
					return;
				}
				synchronize_fps=true; //默认开启帧率同步
				_preinitialize(); //可以调用glfwWindowHint()，不可设置窗口参数、调用OpenGL函数，否则空指针异常
				glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_TRUE);
				new (&window) Window(window_title?window_title:"Andromeda Application",width,height,backColor_,isfullscreen,monitor_); //初始化window
				glfwMakeContextCurrent(window);
				_load_opengl(); //GLAD的加载要在glfwMakeContextCurrent()之后进行
				new (&framebuffer) andromeda::graphics::Framebuffer(width,height,backColor_);
				framebuffer.alloc();
				setBackColor(backColor_);
			}

			~WindowApplication()
			{
				glfwDestroyWindow(window);
			}

			__attribute__((always_inline)) inline operator Window*()
			{
				return &window;
			}

			__attribute__((always_inline)) inline void _launchMainLoop()
			{
				render_frame_rate.init();
				while(is_running&&(!glfwWindowShouldClose(window)))
				{
					//输入处理
					glfwPollEvents();
					//开启同步帧率则优先执行更新函数
					if(synchronize_fps)
						turn(main_loop_thread);
					//渲染
					framebuffer.use();
					framebuffer.clearAll();
					if(andromeda::traits::is_class<Derived>::result&&has_func(render_update)<void,float>::check<Derived>::result) //如果子类没有render_update()则此调用将优化掉
						_render_update(render_frame_rate.get_tpf());
					framebuffer.blitToScreen();
					glfwSwapBuffers(window);
					render_frame_rate.calc();
				}
			}

			__attribute__((always_inline)) inline void setSynchronizeFPS(bool synchronize_fps_)
			{
				synchronize_fps=synchronize_fps_;
				render_frame_rate.set_fps_limit(render_fps_limit);
				main_loop_thread->setUpdateRateLimit(update_rate_limit);
			}

			__attribute__((always_inline)) inline int getRenderFPS()
			{
				return render_frame_rate.get_fps();
			}

			__attribute__((always_inline)) inline int getRenderFPSCount() //获取当前所在帧
			{
				return render_frame_rate.get_fps_count();
			}

			__attribute__((always_inline)) inline void setRenderFPSLimit(int fps_limit)
			{
				render_fps_limit=fps_limit;
				render_frame_rate.set_fps_limit(fps_limit);
				if(synchronize_fps)
					main_loop_thread->setUpdateRateLimit(-1);
			}

			__attribute__((always_inline)) inline void setUpdateRateLimit(int ur_limit)
			{
				update_rate_limit=ur_limit;
				main_loop_thread->setUpdateRateLimit(ur_limit);
				if(synchronize_fps)
					render_frame_rate.set_fps_limit(-1); //每个线程如果开启帧率限制，均会休眠一帧剩余的时间。防止多个线程均休眠导致浪费多帧时间，一帧中只能有一个线程休眠
			}

			using Application<Derived>::getUpdateRate;
			using Application<Derived>::getUpdateRateCount; //获取当前所在帧

			__attribute__((always_inline)) inline int getWindowHeight(void)
			{
				return window.getHeight();
			}

			__attribute__((always_inline)) inline int getWindowWidth(void)
			{
				return window.getWidth();
			}

			__attribute__((always_inline)) inline bool isFullScreen(void)
			{
				return window.isWindowFullScreen();
			}

			__attribute__((always_inline)) inline void setFullScreen(bool isFullScreen)
			{
				window.setFullScreen(isFullScreen);
			}

			__attribute__((always_inline)) inline void setWindowSize(int width,int height)
			{
				window.setWindowSize(width,height);
			}

			__attribute__((always_inline)) inline void setBackColor(andromeda::graphics::ColorRGBA color)
			{
				window.setBackColor(color);
				framebuffer.setClearColor(color);
			}

			__attribute__((always_inline)) inline void setWindowMouseEventPassthrough(bool passthrough) //preinitialize()中调用。窗口是否鼠标事件穿透
			{
				window.setWindowMouseEventPassthrough(passthrough);
			}

			__attribute__((always_inline)) inline void setWindowFramebufferTransparent(bool transparent) //preinitialize()中调用。设置窗口渲染的背景是否透明。注意即使内容透明，事件依旧不会穿透窗口透明部分，它们会被窗口捕获
			{
				window.setWindowFramebufferTransparent(transparent);
			}
		};
	}
}

#endif//ANDROMEDA_APP_WINDOWAPPLICATION