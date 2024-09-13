#ifndef ANDROMEDA_APP_MAINLOOPTHREAD
#define ANDROMEDA_APP_MAINLOOPTHREAD

#include "../util/Synchronized.hpp"
#include "../util/Thread.hpp"
#include "Application.hpp"
#include "FrameRate.hpp"

namespace andromeda {
	namespace app {
		template<typename Derived>
		class Application;
		//主循环线程，负责渲染和事件处理
		template<typename DerivedApp>
		class MainLoopThread:public andromeda::util::Thread<void(),MainLoopThread<DerivedApp> >,public andromeda::util::Synchronized
		{
			DefineThread
			friend class andromeda::util::Thread<void(),MainLoopThread<DerivedApp> >;
			friend class andromeda::app::Application<DerivedApp>;
		protected:
			DerivedApp* app;
			FrameRate update_rate;

			void initialize()
			{
				update_rate.init();
			}

			void run() //多线程执行的更新逻辑函数
			{
				app->update(update_rate.get_tpf());
				update_rate.calc();
				if(app->synchronize_fps)
					turn(app);
			}

			void terminate()
			{

			}

		public:
			MainLoopThread(DerivedApp* derived_app) :
					andromeda::util::Thread<void(),MainLoopThread<DerivedApp>>(&(derived_app->is_running),andromeda::util::ThreadWorkMode::Detach)
			{
				app=derived_app;
			}

			inline void setUpdateRateLimit(int update_fps)
			{
				update_rate.set_fps_limit(update_fps);
			}

			inline int getUpdateRate()
			{
				return update_rate.get_fps();
			}

			inline int getUpdateRateCount()
			{
				return update_rate.get_fps_count();
			}

			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp> >::start;
			using andromeda::util::Thread<void(),MainLoopThread<DerivedApp> >::stop;
			using andromeda::util::Synchronized::turn;
		};
	}
}

#endif//ANDROMEDA_APP_MAINLOOPTHREAD
