#include <stdio.h>
#include <stdlib.h>
#include "../../lib/cJSON/cJSON.h"
#include "../utils/utils.h"
#if defined(__unix__) || defined(__linux__)
	#define fopen_s(file, filename, mode) *file = fopen(filename, mode)
#endif
#include "../mars-interface/interface.h"

#include "generator.h"
#include "analyser.h"
#include "../evolution-algorithm/evolution-parameters.h"

void free_stats(void* stats, int depth)
{
	if (depth != 1)
	{
		for (int i = 0; i < NBR_TYPE_INSTRUCTION + 1; i++)
		{
			if (((void**)stats)[i] != NULL)
				free_stats(((void**)stats)[i], depth - 1);
		}
	}
	free(stats);
}

void free_generator(generator* gen)
{
	free_stats(gen->stats, gen->depth);
	free(gen);
}

void decode_json(void* stats, cJSON* json, int depth)
{
	char* buffer = (char*) malloc_s(3 * sizeof(char));
	if (depth == 1)
	{
		for (int j = 0; j < NBR_TYPE_INSTRUCTION + 1; j++)
		{
			snprintf(buffer, 3, "%d", j);
			((float*)stats)[j] = (float) cJSON_GetNumberValue(cJSON_GetObjectItem(json, buffer));
		}
	}
	else
	{
		for (int j = 0; j < NBR_TYPE_INSTRUCTION + 1; j++)
		{
			snprintf(buffer, 3, "%d", j);
			cJSON* child = cJSON_GetObjectItem(json, buffer);
			if (child != NULL)
			{
				if (depth == 2)
					((void**)stats)[j] = calloc_s((NBR_TYPE_INSTRUCTION + 1) * sizeof(float));
				else
					((void**)stats)[j] = calloc_s((NBR_TYPE_INSTRUCTION + 1) * sizeof(void*));
				decode_json(((void**)stats)[j], child, depth - 1);
			}
			
		}
	}
	free(buffer);
}

generator* decode_stats(char* path, int depth)
{
	char* file_path_buffer = (char*) malloc_s(sizeof(char)*260);
	snprintf(file_path_buffer, 260, "%s/stats%d.json", path, depth);
	char* json_buffer = bufferize_file(file_path_buffer);
	free(file_path_buffer);
	if (json_buffer == NULL)
	{
		return NULL;
	}
	cJSON* json = cJSON_Parse(json_buffer);
	free(json_buffer);
	void* stats = calloc_s((NBR_TYPE_INSTRUCTION + 1) * sizeof(void*));
	decode_json(stats, json, depth);

	generator* g = (generator*) malloc_s(sizeof(generator));
	g->depth = depth;
	g->stats = stats;
	cJSON_Delete(json);

	return g;
}

warrior_s* make_warrior(evolution_params* params)
{
	generator* gen = params->gen;
	int* instrs = (int*) malloc_s(sizeof(int) * MAXWARRIORLENGTH);
	int len = 0;
	int* prev = (int*) malloc_s(sizeof(int)*(gen->depth - 1));
	for (int i = 0; i < gen->depth - 1; i++)
	{
		prev[i] = NBR_TYPE_INSTRUCTION;
	}

	while (len < MAXWARRIORLENGTH)
	{
		void* stats = gen->stats;

		for (int i = gen->depth - 2; i >= 0; i--)
		{
			stats = ((void**)stats)[prev[i]];
		}
		float r = rand_float();
		int new_instruction = -1;
		while (r >= 0 && new_instruction != NBR_TYPE_INSTRUCTION)
		{
			new_instruction++;
			r -= ((float*)stats)[new_instruction];
		}

		if (new_instruction == NBR_TYPE_INSTRUCTION)
		{
			break;
		}

		instrs[len++] = new_instruction;
		refresh_current_line(prev, new_instruction, gen->depth - 1);
	}

	free(prev);
	if (len == 0)
	{
		free(instrs);
		return make_warrior(params);
	}

	warrior_s* warrior = get_empty_warrior();
	new_instruction_set(warrior, len);
	for (int i = 0; i < len; i++)
	{
		put_instruction_at(warrior, i, instrs[i], rand_between(0, 8), rand_between(0, 9), rand_between(0, 9), 
			(CORESIZE + rand_between((int)get_current_value(params->A_value_modifier_min), (int)get_current_value(params->A_value_modifier_max) + 1)) % CORESIZE, 
			(CORESIZE + rand_between((int)get_current_value(params->B_value_modifier_min), (int)get_current_value(params->B_value_modifier_max) + 1)) % CORESIZE);
	}

	set_start_position(warrior, rand_between(0, len));

	set_unique_name(warrior);

	free(instrs);
	
	return warrior;
}