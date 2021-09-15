#include <stdlib.h>
#include <stdio.h>
#if defined(__linux__) || defined(__unix__)
	#include <dirent.h>
    #define dirent struct dirent
	#define fopen_s(file, filename, mode) *file = fopen(filename, mode)
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
	#include "../../lib/dirent/include/dirent.h"
#endif		

void* malloc_s(size_t size)
{
    void* p = malloc(size);
    if (p == NULL) 
        exit(1);
    return p;
}

void* realloc_s(void* ptr, size_t size)
{
    void* p = realloc(ptr, size);
    if (p == NULL) 
        exit(1);
    return p;
}

void* calloc_s(size_t size)
{
    void* p = calloc(size, 1);
    if (p == NULL)
        exit(1);
    return p;
}

int rand_between(int a, int b)
{
    return a + rand() % (b-a);
}

float rand_float()
{
    return rand() / (float)RAND_MAX;
}

char* bufferize_file(char* file_path)
{
    FILE* f;
    fopen_s(&f, file_path, "r");

    if (f == NULL) 
    {
        return NULL;
    }
    int i = 0;
    char* buffer = (char*)malloc_s(sizeof(char) * 3000000);
    char c = ' ';
    while (c != EOF)
    {
        c = fgetc(f);
        buffer[i] = c;
        i++;
    }

    fclose(f);
    return buffer;
}

#if defined(__unix__) || defined(__linux__)
    int get_d_namlen(dirent *d)
    {
        int len = 0;
        char c = d->d_name[len];
        while (c != '\0')
        {
            len++;
            c = d->d_name[len];
        }
        return len;
    }
#endif //__unix__ || __linux__

