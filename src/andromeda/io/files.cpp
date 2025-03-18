#include <andromeda/io/files.h>

#include <fstream>
#include <iostream>
#include <string.h>
#include <rapidcsv/rapidcsv.h>

#ifdef __cpp_lib_filesystem
#include <filesystem>
using namespace std::filesystem;
#elif __cplusplus >= 201103L
#include <experimental/filesystem>
using namespace std::experimental::filesystem;
#endif

unsigned char* andromeda::io::readBytesFromFile(std::string filename,size_t* data_length,long long int read_length,size_t reserve_length)
{
	std::ifstream file(u8path(filename).generic_u8string(),std::ifstream::ate|std::ios::binary);
	if(file.is_open())
	{
		size_t file_length=file.tellg();
		file.seekg(std::ios::beg);
		if(read_length<0)
			read_length=file_length;
		*data_length=(read_length+reserve_length);
		unsigned char* data=(unsigned char*)malloc(sizeof(unsigned char)**data_length);
		file.read((char*)data,read_length);
		file.close();
		return data;
	}
	else
		std::cerr<<"Open file "<<filename<<" failed."<<std::endl;
	return nullptr;
}

const char* andromeda::io::readStringFromFile(std::string filename,size_t* str_length,long long int read_length,size_t reserve_length)
{
	char* str=(char*)andromeda::io::readBytesFromFile(filename,str_length,read_length,reserve_length+1);
	--*(str_length);
	str[*str_length]='\0';
	return (const char*)str;
}

void andromeda::io::writeBytesToFile(std::string filename,void* data,size_t length)
{
	std::ofstream file(u8path(filename).generic_u8string(),std::ios::out|std::ios::binary);
	file.write((const char*)data,length);
	file.close();
}

void andromeda::io::appendBytesToFile(std::string filename,void* data,size_t length)
{
	std::ofstream file(u8path(filename).generic_u8string(),std::ios::app|std::ios::binary);
	file.write((const char*)data,length);
	file.close();
}

std::string andromeda::io::getFileDir(std::string file_path)
{
	static const char _path_separators[3]={WIN32_PATH_SEPARATOR,UNIX_PATH_SEPARATOR,'\0'};
	std::string f_path=file_path;
	return f_path.substr(0,f_path.find_last_of(_path_separators));
}

rapidcsv::Document andromeda::io::readCsv(std::string filename,char comma,CsvOption option)
{
	return rapidcsv::Document(filename,rapidcsv::LabelParams(option&ROW_HEADER?0:-1,option&COLUMN_HEADER?0:-1),rapidcsv::SeparatorParams(comma));
}
