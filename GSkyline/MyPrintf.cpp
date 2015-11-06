#include <stdio.h>
#include <string>
#include "MyPrintf.h"

void MyPrintf::my_printf(const char * str)
{
	int len = strlen(str);
	if (len + cur_index >= buf_size)
	{
		print_str[cur_index] = 0;
		printf("%s", print_str);
		//fwrite(print_str, cur_index, 1, output);
		cur_index = 0;
	}
	while (str){
		print_str[cur_index] = *str;
		str++;
		cur_index++;
	}
	//cur_index = cur_index + len;
}

void MyPrintf::my_printf(std::string str){
	MyPrintf::my_printf(str.data());
}

void MyPrintf::fflush()
{
	print_str[cur_index] = 0;
	printf("%s", print_str);
	//printf(print_str, cur_index, 1, output);
	cur_index = 0;
}