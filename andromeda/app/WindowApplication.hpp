#ifndef ANDROMEDA_APP_WINDOWAPPLICATION
#define ANDROMEDA_APP_WINDOWAPPLICATION

#include "../macros/Debug.h"
#include "../../lib/opengl/glad/glad.h"
#include "../../lib/opengl/GLFW/glfw3.h"
#include "Window.hpp"
#include "FrameRate.hpp"
#include "MainLoopThread.hpp"
#include "../image/ColorChannel.hpp"
#include "../graphics/DoubleFramebuffer.hpp"
#include "Application.hpp"

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
			using Application<Derived>::_initialize;
			using Application<Derived>::_terminate;
			using Application<Derived>::initialize;
			using Application<Derived>::preinitialize;
			using Application<Derived>::terminate;
			using Application<Derived>::update;
			using Application<Derived>::render_update;
			using Application<Derived>::turn;

		public:

			WindowApplication(const char* window_title=nullptr,int width=800,int height=600,andromeda::image::ColorRGBA backColor_={0,0,0,0},bool isfullscreen=false,GLFWmonitor* monitor_=glfwGetPrimaryMonitor())
			{
				bool init_app=true; //如果需要的库没有加载，则不初始化该类，无法使用该类
				if(!andromeda::use_opengl)
				{
					PRINT_MESSAGE("OpenGL is not used. Please set andromeda::use_opengl to true.")
					init_app=false;
				}
				if(!andromeda::use_openal)
				{
					PRINT_MESSAGE("OpenAL is not used. Please set andromeda::use_openal to true.")
					init_app=false;
				}
				if(!init_app)
				{
					PRINT_MESSAGE("Application not initialized.")
					return;
				}
				synchronize_fps=true; //默认开启帧率同步
				preinitialize(); //可以调用glfwWindowHint()，不可设置窗口参数、调用OpenGL函数，否则空指针异常
				glfwWindowHint(GLFW_DOUBLEBUFFER,GLFW_TRUE);
				new (&window) Window(window_title?window_title:"Andromeda Application",width,height,backColor_,isfullscreen,monitor_); //初始化window
				glfwMakeContextCurrent(window);
				_load_opengl();//GLAD的加载要在glfwMakeContextCurrent()之后进行
				new (&framebuffer) andromeda::graphics::Framebuffer(width,height,backColor_);
				framebuffer.alloc();
				setBackColor(backColor_);
			}

			~WindowApplication()
			{
				glfwDestroyWindow(window);
			}

			void exit()
			{
				is_running=false;
			}

			void launch()
			{
				_initialize();
				main_loop_thread->start();
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
						render_update(render_frame_rate.get_tpf());
					framebuffer.blitToScreen();
					glfwSwapBuffers(window);
					render_frame_rate.calc();
				}
				_terminate();
			}

			void setSynchronizeFPS(bool synchronize_fps_)
			{
				synchronize_fps=synchronize_fps_;
				render_frame_rate.set_fps_limit(render_fps_limit);
				main_loop_thread->setUpdateRateLimit(update_rate_limit);
			}

			inline int getRenderFPS()
			{
				return render_frame_rate.get_fps();
			}

			inline int getRenderFPSCount() //获取当前所在帧
			{
				return render_frame_rate.get_fps_count();
			}

			inline void setRenderFPSLimit(int fps_limit)
			{
				render_fps_limit=fps_limit;
				render_frame_rate.set_fps_limit(fps_limit);
				if(synchronize_fps)
					main_loop_thread->setUpdateRateLimit(-1);
			}

			inline void setUpdateRateLimit(int ur_limit)
			{
				update_rate_limit=ur_limit;
				main_loop_thread->setUpdateRateLimit(ur_limit);
				if(synchronize_fps)
					render_frame_rate.set_fps_limit(-1); //每个线程如果开启帧率限制，均会休眠一帧剩余的时间。防止多个线程均休眠导致浪费多帧时间，一帧中只能有一个线程休眠
			}

			using Application<Derived>::getUpdateRate;
			using Application<Derived>::getUpdateRateCount; //获取当前所在帧

			inline int getWindowHeight(void)
			{
				return window.getHeight();
			}

			inline int getWindowWidth(void)
			{
				return window.getWidth();
			}

			inline bool isFullScreen(void)
			{
				return window.isWindowFullScreen();
			}

			inline void setFullScreen(bool isFullScreen)
			{
				window.setFullScreen(isFullScreen);
			}

			inline void setWindowSize(int width,int height)
			{
				window.setWindowSize(width,height);
			}

			inline void setBackColor(andromeda::image::ColorRGBA color)
			{
				window.setBackColor(color);
				framebuffer.setClearColor(color);
			}

			inline void setWindowMouseEventPassthrough(bool passthrough)//preinitialize()中调用。窗口是否鼠标事件穿透
			{
				window.setWindowMouseEventPassthrough(passthrough);
			}

			inline bool setWindowFramebufferTransparent(bool transparent=false) //preinitialize()中调用。设置窗口渲染的背景是否透明。注意即使内容透明，事件依旧不会穿透窗口透明部分，它们会被窗口捕获
			{
				return window.setWindowFramebufferTransparent(transparent); //设置成功则返回true
			}
		};
	}
}

#endif//ANDROMEDA_APP_WINDOWAPPLICATION