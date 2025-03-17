#ifndef ANDROMEDA_UTIL_ARRAYLIST
#define ANDROMEDA_UTIL_ARRAYLIST

#include <malloc.h>
#include <string.h>
#include <stddef.h>

namespace andromeda {
	namespace util {
		template<typename E>
		class ArrayList
		{
		protected:
			size_t available_length=0,extend_capcity=16,last_element_idx=-1;
			E* elements;
		public:
			ArrayList()=default;

			ArrayList(size_t arr_length) :
					available_length(arr_length), last_element_idx(-1), extend_capcity(arr_length), elements((E*)malloc(sizeof(E)*arr_length))
			{
			}

			ArrayList(size_t arr_length,size_t arr_extend_capcity) :
					available_length(arr_length), last_element_idx(-1), extend_capcity(arr_extend_capcity), elements((E*)malloc(sizeof(E)*arr_length))
			{
			}

			ArrayList(const ArrayList<E>& arr) :
					available_length(arr.available_length), last_element_idx(arr.last_element_idx), extend_capcity(arr.extend_capcity), elements((E*)malloc(sizeof(E)*arr.available_length))
			{
				memcpy(elements,arr.elements,sizeof(E)*arr.available_length);
			}

			ArrayList(ArrayList<E>&& arr) :
					available_length(arr.available_length), last_element_idx(arr.last_element_idx), extend_capcity(arr.extend_capcity), elements(arr.elements)
			{
				arr.elements=nullptr;
			}

			ArrayList(E* elem,size_t len):
				elements(elem),available_length(len),last_element_idx(len-1)
			{
			}

			ArrayList(E* elem,size_t len,size_t arr_extend_capcity):
				elements(elem),available_length(len),last_element_idx(len-1),extend_capcity(arr_extend_capcity)
			{
			}

			~ArrayList()
			{
				free(elements);
			}

			operator E*()
			{
				return elements;
			}

			void operator=(ArrayList& arr)
			{
				available_length=arr.available_length;
				last_element_idx=arr.last_element_idx;
				extend_capcity=arr.extend_capcity;
				elements=(E*)malloc(sizeof(E)*arr.available_length);
				memcpy(elements,arr.elements,sizeof(E)*arr.available_length);
			}

			void operator=(ArrayList&& arr)
			{
				available_length=arr.available_length;
				last_element_idx=arr.last_element_idx;
				extend_capcity=arr.extend_capcity;
				elements=arr.elements;
				arr.elements=nullptr;
			}

			template<typename ...Args>
			E& add_placement_new(Args... args)
			{
				if(last_element_idx>=available_length-1)
					extendArray(extend_capcity);
				new (elements+(++last_element_idx)) E(args...);
				return getLastElement();
			}

			void add(E& e)
			{
				if(last_element_idx>=available_length-1)
					extendArray(extend_capcity);
				*(elements+(++last_element_idx))=e;
			}

			void add(E&& e)
			{
				if(last_element_idx>=available_length-1)
					extendArray(extend_capcity);
				*(elements+(++last_element_idx))=e;
			}

			void add(E* es,size_t num)
			{
				checkExtendArray(num);
				memcpy(elements+last_element_idx,es,sizeof(E)*num);
			}

			void add(E& e1,E& e2)
			{
				add(e1);
				add(e2);
			}

			template<typename ...Es>
			void add(Es& ...es)
			{
				checkExtendArray(sizeof...(Es));
				E arr[sizeof...(Es)]={es...};
				memcpy(elements+last_element_idx+1,arr,sizeof(E)*(sizeof...(Es)));
				last_element_idx+=sizeof...(Es);
			}

			void add(ArrayList& arr_lst)
			{
				add(arr_lst.getElements(),arr_lst.getLength());
			}

			void add(ArrayList&& arr_lst)
			{
				add(arr_lst.getElements(),arr_lst.getLength());
			}

			E& remove()
			{
				return *(elements+(last_element_idx--));
			}

			E* remove(size_t num)
			{
				last_element_idx-=num;
				return elements+last_element_idx+1;
			}

			E& getLastElement()
			{
				return *(elements+last_element_idx);
			}

			E& get(size_t index)
			{
				return *(elements+index);
			}

			E& operator[](size_t index)
			{
				return *(elements+index);
			}

			inline size_t getAvailableLength() //数组可使用的总长度，包括没有赋值的元素
			{
				return available_length;
			}

			inline size_t getLastIndex()
			{
				return last_element_idx;
			}

			inline size_t getLength()//数组实际使用的长度
			{
				return last_element_idx+1;
			}

			inline size_t getExtendCapcity()
			{
				return extend_capcity;
			}

			inline E* getElements()
			{
				return elements;
			}

			inline void clear()
			{
				last_element_idx=-1;
			}

			//手动释放内存，调用后就不应该再对该对象进行任何操作，否则会出错
			inline void release()
			{
				free(elements);
			}
			//将数据类型强制转换为指定类型
			template<typename T>
			inline ArrayList<T>& cast()
			{
				return *(ArrayList<T>*)this;
			}

			inline E* toArray()
			{
				size_t len=sizeof(E)*getLength();
				E* arr=(E*)malloc(len);
				memcpy(arr,elements,len);
				return arr;
			}
		protected:

			void extendArray(size_t extend_cap)
			{
				E* new_elem=(E*)malloc(sizeof(E)*(available_length+extend_cap));
				memcpy(new_elem,elements,sizeof(E)*available_length);
				free(elements);
				available_length+=extend_cap;
				elements=new_elem;
			}

			void checkExtendArray(size_t add_num)
			{
				if(last_element_idx+add_num>=available_length)
				{
					size_t extend_times=add_num/extend_capcity+1;
					extendArray(extend_capcity*extend_times);
				}
			}
		};
	}
}

#endif // ANDROMEDA_UTIL_ARRAYLIST
