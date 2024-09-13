#include "Files.hpp"

#include <fstream>
#include <iostream>
#include <string.h>

using namespace andromeda;

unsigned char* readBytesFromFile(const char* filename,size_t* data_length,long long int read_length,size_t reserve_length)
{
	std::ifstream file(filename,std::ios::in|std::ios::binary);
	file.seekg(std::ios::end);
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

const char* readStringFromFile(const char* filename,size_t* str_length,long long int read_length,size_t reserve_length)
{
	char* str=(char*)readBytesFromFile(filename,str_length,read_length,reserve_length+1);
	--*(str_length);
	str[*str_length]='\0';
	return (const char*)str;
}

void writeBytesToFile(const char* filename,void* data,long int length)
{
	std::ofstream file(filename,std::ios::out|std::ios::binary);
	file.write((const char*)data,length);
	file.close();
}

void appendBytesToFile(const char* filename,void* data,long int length)
{
	std::ofstream file(filename,std::ios::app|std::ios::binary);
	file.write((const char*)data,length);
	file.close();
}

const char* getFileDir(const char* file_path)
{
	std::string f_path=file_path;
	return f_path.substr(0,f_path.find_last_of(PATH_SEPARATOR)).c_str();
}
