#ifndef ANDROMEDA_IO_IOUTILS
#define ANDROMEDA_IO_IOUTILS

#include <fstream>
#include <string.h>

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

namespace andromeda {
	namespace io {
		extern unsigned char* readBytesFromFile(const char* filename,size_t *data_length=nullptr,long long int read_length=-1,size_t reserve_length=0); //data_length记录返回数组长度，read_length<0则读取全部内容，reserve_length为内存保留字节数
		extern const char* readStringFromFile(const char* filename,size_t *str_length=nullptr,long long int read_length=-1,size_t reserve_length=0); //str_length记录返回字符串长度不含'\0'，read_length=<0则读取全部内容，reserve_length为内存保留字节数
		extern void writeBytesToFile(const char* filename,void *data,size_t length);
		extern void appendBytesToFile(const char* filename,void *data,size_t length);

		extern const char* getFileDir(const char* file_path);

		inline void clearFile(const char* file_name)
		{
			std::ofstream f(file_name,std::ios::out|std::ios::binary);
			f.write(nullptr,0);
			f.close();
		}
	}
}

#endif // ANDROMEDA_IO_IOUTILS
