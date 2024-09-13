#ifndef ANDROMEDA_MACROS_DEBUG
#define ANDROMEDA_MACROS_DEBUG
//定义调试相关的宏，例如输出报错信息的方式

#define DEBUG_CMD

#ifdef DEBUG_CMD

#include <iostream>
#include "../util/StringUtils.hpp"

#define PRINT_MESSAGE(...) {std::cout<<(andromeda::util::str_join(__VA_ARGS__))<<std::endl;}

#endif

#endif//ANDROMEDA_MACROS_DEBUG
