#pragma once
#include <stdlib.h>
#if defined(__unix__) || defined(__linux__)
	#include <dirent.h>
    #define dirent struct dirent
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
	#include "../../lib/dirent/include/dirent.h"
#endif		

void* malloc_s(size_t size);
void* realloc_s(void* ptr, size_t size);

void* calloc_s(size_t size);

int rand_between(int a, int b);
float rand_float();

char* bufferize_file(char* json_file);

#if defined(__unix__) || defined(__linux__)
	int get_d_namlen(dirent *d);
#endif // __unix__ || __linux__
