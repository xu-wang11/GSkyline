#ifndef MY_PRINTF_H_
#define MY_PRINTF_H_
#define buf_size 4096
#include <string>
/*
 * define my printf function with buffer to avoid too much printf
 * added by wx
 * 
 */
class MyPrintf{
public:
	static char print_str[buf_size];
	static int cur_index;
	static void my_printf(const char * str);
	static void my_printf(std::string str);
	static void fflush();
};
#endif