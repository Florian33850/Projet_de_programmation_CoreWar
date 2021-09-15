#include "evolution-parameters.h"
#if defined(__unix__) || defined(__linux__)
	#define fopen_s(file, filename, mode) *file = fopen(filename, mode)
#endif
#include "../utils/utils.h"
#include "../../lib/cJSON/cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include "../../lib/cJSON/cJSON.h"
#include "../utils/utils.h"

#include "evolution-parameters.h"
#include "../analyser/generator.h"
#include "../analyser/analyser.h"
#define MAX_CHANGING_PARAMS 100

changing_param* malloc_changing_param() {
	changing_param* cp = malloc_s(sizeof(changing_param));
	cp->level = (int*) malloc_s(MAX_CHANGING_PARAMS * sizeof(int));
	cp->value = (float*) malloc_s(MAX_CHANGING_PARAMS * sizeof(float));
	return cp;
}

evolution_params* malloc_evolution_params()
{
	evolution_params* ep = (evolution_params*) malloc_s(sizeof(evolution_params));
	ep->battles_by_generation = malloc_changing_param();
	ep->num_warriors = malloc_changing_param();

	ep->A_value_modifier_min = malloc_changing_param();
	ep->A_value_modifier_max = malloc_changing_param();
	ep->B_value_modifier_min = malloc_changing_param();
	ep->B_value_modifier_max = malloc_changing_param();

	ep->survivors = malloc_changing_param();
	ep->mutations = malloc_changing_param();
	ep->breedings = malloc_changing_param();
	ep->fresh = malloc_changing_param();
	ep->old = malloc_changing_param();

	ep->p_mutation_line = malloc_changing_param();
	ep->p_mutation_field = malloc_changing_param();

	ep->breed_length_min = malloc_changing_param();
	ep->breed_length_max = malloc_changing_param();

	return ep;
}

void free_changing_param(changing_param* cp)
{
	free(cp->level);
	free(cp->value);
	free(cp);
}

void free_evolution_params(evolution_params* ep)
{
	free_changing_param(ep->battles_by_generation);
	free_changing_param(ep->num_warriors);

	free_changing_param(ep->A_value_modifier_min);
	free_changing_param(ep->A_value_modifier_max);
	free_changing_param(ep->B_value_modifier_min);
	free_changing_param(ep->B_value_modifier_max);


	free_changing_param(ep->survivors);
	free_changing_param(ep->mutations);
	free_changing_param(ep->breedings);
	free_changing_param(ep->fresh);
	free_changing_param(ep->old);

	free_changing_param(ep->p_mutation_line);
	free_changing_param(ep->p_mutation_field);

	free_changing_param(ep->breed_length_min);
	free_changing_param(ep->breed_length_max); 

	free_generator(ep->gen);
	free(ep);
}


float get_current_value(changing_param* param)
{
	return param->value[param->current_lvl];
}

void get_warrior_distribution(evolution_params* params, int* num_warriors, int* num_survivors, int* num_mutations, int* num_breedings, int* num_fresh, int* num_old)
{
	*num_warriors = get_current_value(params->num_warriors);
	float survivors = get_current_value(params->survivors) * *num_warriors;
	float mutations = get_current_value(params->mutations) * *num_warriors;
	float breedings = get_current_value(params->breedings) * *num_warriors;
	float fresh = get_current_value(params->fresh) * *num_warriors;
	float old = get_current_value(params->old) * *num_warriors;
	*num_survivors = (int)(survivors + 0.1f);
	*num_mutations = (int)(survivors + mutations + 0.1f) - *num_survivors;
	*num_breedings = (int)(survivors + mutations + breedings + 0.1f) - *num_survivors - *num_mutations;
	*num_fresh = (int)(survivors + mutations + breedings + fresh + 0.1f) - *num_survivors - *num_mutations - *num_breedings;
	*num_old = (int)(survivors + mutations + breedings + fresh + old + 0.1f) - *num_survivors - *num_mutations - *num_breedings - *num_fresh;

	if (*num_survivors < 0 || *num_mutations < 0 || *num_breedings < 0 || *num_fresh < 0 || *num_old < 0 || num_warriors <= 0 ||
		*num_survivors + *num_mutations + *num_breedings + *num_fresh + *num_old != *num_warriors)
	{
		printf("invalid arguments !");
		exit(EXIT_FAILURE);
	}
}


static void load_field(cJSON* json_parse, changing_param* param, char* field)
{
	param->current_lvl = 0;
	cJSON* object = cJSON_GetObjectItemCaseSensitive(json_parse, field);
	if (object->child == NULL)
	{
		param->level[0] = 0;
		param->value[0] = object->valuedouble;
		param->size = 1;
	}
	else
	{
		object = object->child;
		int i = 0;
		while (object != NULL && i < MAX_CHANGING_PARAMS)
		{
			param->level[i] = cJSON_GetObjectItemCaseSensitive(object, "gen_number")->valueint;
			param->value[i] = cJSON_GetObjectItemCaseSensitive(object, "value")->valuedouble;
			++i;
			object = object->next;
		};
		param->size = i;
	}
}

void json_parser(evolution_params* params, char* json_file)
{
	char* json = bufferize_file(json_file);
	if (json == NULL)
	{
		printf("Evolution parameters not found !\n");
		exit(EXIT_FAILURE);
	}
	cJSON_Minify(json);
	cJSON* json_parse = cJSON_Parse(json);
	if (json_parse == NULL)
	{
		printf("Invalid evolution parameters file format !\n");
		exit(EXIT_FAILURE);
	}
	params->generations = cJSON_GetObjectItemCaseSensitive(json_parse, "generations")->valueint;

	load_field(json_parse, params->battles_by_generation, "battles_by_generation");
	load_field(json_parse, params->num_warriors, "num_warriors");

	load_field(json_parse, params->A_value_modifier_min, "A_value_modifier_min");
	load_field(json_parse, params->A_value_modifier_max, "A_value_modifier_max");
	load_field(json_parse, params->B_value_modifier_min, "B_value_modifier_min");
	load_field(json_parse, params->B_value_modifier_max, "B_value_modifier_max");

	load_field(json_parse, params->survivors, "survivors");
	load_field(json_parse, params->mutations, "mutations");
	load_field(json_parse, params->breedings, "breedings");
	load_field(json_parse, params->fresh, "fresh");
	load_field(json_parse, params->old, "old");

	load_field(json_parse, params->p_mutation_line, "p_mutation_line");
	load_field(json_parse, params->p_mutation_field, "p_mutation_field");

	load_field(json_parse, params->breed_length_min, "breed_length_min");
	load_field(json_parse, params->breed_length_max, "breed_length_max");

	char* path = cJSON_GetObjectItemCaseSensitive(json_parse, "generator_path")->valuestring;
	int depth = cJSON_GetObjectItemCaseSensitive(json_parse, "generator_depth")->valueint;
	generator* gen = decode_stats( path, depth);
	if(gen == NULL)
	{
		printf("The stats file for this depth is not found. Running analysis with this parameters\n");
		gen = start_analyser(path, depth);
	}
	params->gen = gen;
	free(json);
	cJSON_Delete(json_parse);
}

void update_param(changing_param* param, int iteration)
{
	if (param->current_lvl + 1 < param->size)
		if (iteration == param->level[param->current_lvl + 1])
			param->current_lvl++;
}

void update_all_current_lvl(evolution_params* params, int iteration)
{
	update_param(params->battles_by_generation, iteration);
	update_param(params->num_warriors, iteration);
	
	update_param(params->A_value_modifier_min, iteration);
	update_param(params->A_value_modifier_max, iteration);
	update_param(params->B_value_modifier_min, iteration);
	update_param(params->B_value_modifier_max, iteration);

	update_param(params->survivors, iteration);
	update_param(params->mutations, iteration);
	update_param(params->breedings, iteration);
	update_param(params->fresh, iteration);
	update_param(params->old, iteration);

	update_param(params->p_mutation_line, iteration);
	update_param(params->p_mutation_field, iteration);

	update_param(params->breed_length_min, iteration);
	update_param(params->breed_length_max, iteration);
}