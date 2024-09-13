#ifndef ANDROMEDA_UTIL_THREADTURN
#define ANDROMEDA_UTIL_THREADTURN

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace andromeda {
	namespace util {
		class Synchronized //线程轮换执行的辅助类，A线程开始执行时B线程暂停，A线程暂停时B恢复执行，只允许两个线程存在轮换关系
		{
		private:
			std::atomic_bool is_running;
			std::mutex mutex;
			std::condition_variable condition;

		public:
			void turn(Synchronized* turn); //传入另一个线程的ThreadTurn对象，阻塞调用该函数的线程，恢复执行传入的线程
		};
	}
}

#endif//ANDROMEDA_UTIL_THREADTURN