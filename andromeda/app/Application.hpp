#ifndef ANDROMEDA_APP_APPLICATION
#define ANDROMEDA_APP_APPLICATION

#include "../macros/Debug.h"
#include "FrameRate.hpp"
#include "MainLoopThread.hpp"
#include "../traits/Types.h"
#include "../util/Synchronized.hpp"

/**
 * 抽象的应用类，只包含更新逻辑，无图形渲染、音频等，可用于编写服务端
 */

#ifndef HAS_FUNC_INITIALIZE
#define HAS_FUNC_INITIALIZE
def_cls_has_func(initialize)
#endif//#HAS_FUNC_INITIALIZE#ifndef HAS_FUNC_PREINITIALIZE
#define HAS_FUNC_PREINITIALIZE
def_cls_has_func(preinitialize)
#endif//#HAS_FUNC_PREINITIALIZE#ifndef HAS_FUNC_UPDATE#define HAS_FUNC_UPDATEdef_cls_has_func(update)#endif//HAS_FUNC_UPDATE
#ifndef HAS_FUNC_RENDER_UPDATE
#define HAS_FUNC_RENDER_UPDATE
def_cls_has_func(render_update)
#endif//#HAS_FUNC_RENDER_UPDATE#ifndef HAS_FUNC_TERMINATE#define HAS_FUNC_TERMINATEdef_cls_has_func(terminate)#endif//HAS_FUNC_TERMINATE

//子类必须添加的friend class
#define DefineApplication(Derived) \
	friend class has_func(initialize)<void>;\
	friend class has_func(preinitialize)<void>;\
	friend class has_func(terminate)<void>;\
	friend class has_func(update)<void,float>;\
	friend class has_func(render_update)<void,float>;\
	friend class andromeda::app::MainLoopThread<Derived>;\
	friend class andromeda::app::Application<Derived>;

namespace andromeda {
	extern bool use_opengl;
	extern bool use_portaudio;

	namespace app {
		template<typename Derived>
		class MainLoopThread;

		template<typename Derived>
		class Application:public andromeda::util::Synchronized
		{
			DefineApplication(Derived)
		protected:
			bool is_running=false;
			std::atomic_bool synchronize_fps; //主线程(如果是窗口程序则是渲染线程)是否与该线程同步
			MainLoopThread<Derived>*main_loop_thread=nullptr;

			using andromeda::util::Synchronized::turn;
			//主线程函数，负责事件处理和更新
			//内部使用的初始化函数
			void _initialize()
			{
				is_running=true;
				initialize();
			}

			void _terminate() //内部使用的终止函数
			{
				terminate();
				is_running=false;
			}

			void initialize() //在各系统初始化完成后调用
			{
				if(andromeda::traits::is_class<Derived>::result&&has_func(initialize)<void>::check<Derived>::result)
					((Derived*)this)->initialize();
			}

			void preinitialize() //在初始化Application时调用（各系统初始化之前）
			{
				if(andromeda::traits::is_class<Derived>::result&&has_func(preinitialize)<void>::check<Derived>::result)
					((Derived*)this)->preinitialize();
			}

			void terminate()
			{
				if(andromeda::traits::is_class<Derived>::result&&has_func(terminate)<void>::check<Derived>::result)
					((Derived*)this)->terminate();
			}

			void update(float tpf)
			{
				if(andromeda::traits::is_class<Derived>::result&&has_func(update)<void,float>::check<Derived>::result)
					((Derived*)this)->update(tpf);
			}

			void render_update(float render_tpf)
			{
				if(andromeda::traits::is_class<Derived>::result&&has_func(render_update)<void,float>::check<Derived>::result)
					((Derived*)this)->render_update(render_tpf);
			}

		public:

			Application()
			{
				main_loop_thread=new MainLoopThread<Derived>((Derived*)this);
				setSynchronizeFPS(false);
				preinitialize(); //可以调用glfwWindowHint()，不可设置窗口参数、调用OpenGL函数，否则空指针异常
			}

			~Application()
			{

			}

			void exit()
			{
				is_running=false;
			}

			void launch()
			{
				_initialize();
				main_loop_thread->start();
				while(is_running)
				{
					//更新
					turn(main_loop_thread);
				}
				_terminate();
			}

			void setSynchronizeFPS(bool synchronize_fps_)
			{
				synchronize_fps=synchronize_fps_;
			}

			inline int getUpdateRate()
			{
				return main_loop_thread->getUpdateRate();
			}

			inline int getUpdateRateCount() //获取当前所在帧
			{
				return main_loop_thread->getUpdateRateCount();
			}

			inline void setUpdateRateLimit(int ur_limit)
			{
				main_loop_thread->setUpdateRateLimit(ur_limit);
			}
		};
	}
}

#endif//ANDROMEDA_APP_APPLICATION