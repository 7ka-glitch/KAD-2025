#include <iostream>
#include <Windows.h>
#include <sstream>
#include <ctime>
#include <cstring> 
#pragma warning(disable: 4996)


extern "C"
{
	// 1. lenght (длина строки)
	int lenght(char* str)
	{
		return strlen(str);
	}

	// 2. write_int (вывод числа) 
	 int write_int(int p)
	{
		std::cout << p << std::endl;
		return 0;
	}

	// 3. write_str (вывод строки)
	int write_str(char* str)
	{
		SetConsoleOutputCP(1251);
		std::cout << str << std::endl;
		return 0;
	}

	//преобразование int в char*
	char* intToStr(int num)
	{
		
		std::stringstream ss;
		ss << num;

		
		std::string s = ss.str();
		char* cstr = (char*)malloc(s.length() + 1);
		std::strcpy(cstr, s.c_str()); 

		return cstr;
	}


	int powNumber(int base, int exp)
	{
		return (int)std::pow(base, exp);
	}
}