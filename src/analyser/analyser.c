#include <stdio.h>
#if defined(__unix__) || defined(__linux__)
	#include <dirent.h>
	#define __STDC_WANT_LIB_EXT1__ 1
	#define fopen_s(file, filename, mode) *file = fopen(filename, mode)
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64)
	#define OS_WINDOWS
	#include "../../lib/dirent/include/dirent.h"
#endif
#include "../../lib/cJSON/cJSON.h"
#include "../utils/utils.h"
#include "../mars-interface/interface.h"
#include "../mars-interface/redcode-switch.h"
#include "generator.h"

void increment_stats(int* prev, void* stats, int depth)
{
	if (depth == 1)
	{
		((float*)stats)[*prev]++;
	}
	else
	{
		if (((void**)stats)[*prev] == NULL)
		{
			if (depth == 2)
				((void**)stats)[*prev] = calloc_s((NBR_TYPE_INSTRUCTION+1) * sizeof(float));
			else
				((void**)stats)[*prev] = calloc_s((NBR_TYPE_INSTRUCTION+1) * sizeof(void*));
		}
		increment_stats(prev - 1, ((void**)stats)[*prev], depth - 1);
	}

}

void refresh_current_line(int* prev, int currentLine, int depth)
{
	for (int i = depth - 1; i >= 1; i--)
	{
		prev[i] = prev[i - 1];
	}
	prev[0] = currentLine;
}

void analyse_warrior(warrior_s* warrior, void* stats, int depth)
{
	int* prev = (int*) malloc_s(depth * sizeof(int));
	for (int i = 0; i < depth; i++)
	{
		prev[i] = NBR_TYPE_INSTRUCTION;
	}

	int len = get_instruction_number(warrior);
	for (int i = 0; i < len; i++)
	{
		int currentLine = get_instruction_at(warrior, i);
		refresh_current_line(prev, currentLine, depth);
		increment_stats(prev + depth - 1, stats, depth);
	}
	int currentLine = NBR_TYPE_INSTRUCTION;
	refresh_current_line(prev, currentLine, depth);
	increment_stats(prev + depth - 1, stats, depth);
}


cJSON* make_json(void* stats, int depth)
{
	cJSON* json = cJSON_CreateObject();
	char* buffer = (char*) malloc_s(3 * sizeof(char));
	if (depth == 1)
	{
		float sum = 0;
		for (int j = 0; j < NBR_TYPE_INSTRUCTION + 1; j++)
		{
			sum += ((float*)stats)[j];
		}
		for (int j = 0; j < NBR_TYPE_INSTRUCTION + 1; j++)
		{
			snprintf(buffer, 3, "%d", j);
			((float*)stats)[j] /= sum;
			cJSON_AddItemToObject(json, buffer, cJSON_CreateNumber(((float*)stats)[j]));
		}
	}
	else
	{
		for (int j = 0; j < NBR_TYPE_INSTRUCTION + 1; j++)
		{
			if ((((void**)stats)[j]) != NULL)
			{
				snprintf(buffer, 3, "%d", j);
				cJSON_AddItemToObject(json, buffer, make_json(((void**)stats)[j], depth - 1));
			}
		}
	}
	free(buffer);
	return json;
}

generator* start_analyser(char* path, int depth)
{
	void* stats = calloc_s((NBR_TYPE_INSTRUCTION + 1) * sizeof(void*));
	DIR* dir = opendir(path);
	char buffer[PATH_MAX];

	dirent *entry;

	// check the file extension and then analyse the warrior 
	while ((entry = readdir(dir)) != NULL)
	{
		int extension_progress = 0;
		#ifdef OS_WINDOWS 
			int d_namlen = entry->d_namlen;
		#else
			int d_namlen = get_d_namlen(entry);
		#endif

		for (size_t i = 0; i < d_namlen; i++)
		{
			switch (extension_progress)
			{
			case 0:
				if (entry->d_name[i] == '.')
					extension_progress = 1;
				break;
			case 1:
				if (entry->d_name[i] == 'r')
					extension_progress = 2;
				else
					extension_progress = -1;
				break;
			case 2:
				if (entry->d_name[i] == 'e')
					extension_progress = 3;
				else
					extension_progress = -1;
				break;
			case 3:
				if (entry->d_name[i] == 'd')
					extension_progress = 4;
				else
					extension_progress = -1;
				break;
			default:
				break;
			}
		}
		if (extension_progress != 4) continue;

		snprintf(buffer, 260, "%s/%s", path, entry->d_name);
		warrior_s* warrior = red_to_warrior(buffer);
		if (warrior != NULL)
		{
			analyse_warrior(warrior, stats, depth);
			free_warrior(warrior);
		}
	}
	closedir(dir);


	/* compute and save in stats.json the proportion of occurence of each instruction */
	FILE* file;
	snprintf(buffer, 260, "%s/stats%d.json", path, depth);
	fopen_s(&file, buffer, "w");
	if (file == NULL) exit(1);
	cJSON* json = make_json(stats, depth);
	fprintf(file, "%s", cJSON_Print(json));
	cJSON_Delete(json);
	fclose(file);

	generator* g = (generator*) malloc_s(sizeof(generator));
	g->depth = depth;
	g->stats = stats;

	return g;
}