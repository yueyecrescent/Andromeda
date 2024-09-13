#ifndef ANDROMEDA_UTIL_LIMITCOUNTER
#define ANDROMEDA_UTIL_LIMITCOUNTER

namespace andromeda {
	namespace util {
		//设置起始值和终止值构成循环序列，则自增、自减运算结果值都落入该序列。只适用于基础数据类型
		template<typename T>
		class LimitCounter
		{
		private:
			T _start;
			T _end;
			T counter;
		public:
			LimitCounter(T start_value,T end_value) :
					_start(start_value), _end(end_value)
			{
				counter=start_value;
			}

			__attribute__((always_inline)) inline operator T&()
			{
				return counter;
			}

			__attribute__((always_inline)) inline T& inc() //返回自增的结果值
			{
				if(++counter>_end)
					counter=_start; //如果越过_end则回到_start
				return counter;
			}

			__attribute__((always_inline)) inline T& dec() //返回自减的结果值
			{
				if(--counter<_start)
					counter=_end;
				return counter;
			}

			T& operator++() //前置++
			{
				return counter.inc();
			}

			T& operator--()
			{
				return counter.dec();
			}

			T operator++(int) //后置++
			{
				T old_counter=counter;
				if(++counter>_end)
					counter=_start; //如果越过_end则回到_start
				return old_counter;
			}

			T operator--(int)
			{
				T old_counter=counter;
				if(--counter<_start)
					counter=_end;
				return old_counter;
			}
		};
	}
}

#endif//ANDROMEDA_UTIL_LIMITCOUNTER
