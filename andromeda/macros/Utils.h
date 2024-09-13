#ifndef ANDROMEDA_MACROS_UTILS
#define ANDROMEDA_MACROS_UTILS

#define get_str_len(str) ((sizeof(str))/(sizeof(str[0])))
#define bitwise_copy(dest_obj,src_obj) {\
							void* new_obj=malloc(sizeof(src_obj));\
						  }

#endif // ANDROMEDA_MACROS_UTILS
