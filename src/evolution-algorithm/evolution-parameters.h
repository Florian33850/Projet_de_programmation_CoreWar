#pragma once

/// <summary>
/// This structure contains all the value of a parameter that evolve during the generation, and the levels 
/// at wich it must evolve
/// </summary>
typedef struct changing_param_s
{
	int size;
	int current_lvl;
	int *level;
	float *value;
} changing_param;

/// <summary>
/// This structure contains all the parameters influencing an evolution algorithm.
/// </summary>
typedef struct evolution_params_s evolution_params;

#include <stdio.h>
#include "../analyser/generator.h"

struct evolution_params_s
{
	int generations;
	
	changing_param* battles_by_generation;
	changing_param* num_warriors;
	
	changing_param* A_value_modifier_min;
	changing_param* A_value_modifier_max;
	changing_param* B_value_modifier_min;
	changing_param* B_value_modifier_max;
	
	changing_param* survivors;
	changing_param* mutations;
	changing_param* breedings;
	changing_param* fresh;
	changing_param* old;
	
	changing_param* p_mutation_line;
	changing_param* p_mutation_field;

	changing_param* breed_length_min;
	changing_param* breed_length_max;

	generator* gen;
};

changing_param* malloc_changing_param();
evolution_params* malloc_evolution_params();

void free_changing_param(changing_param* cp);
void free_evolution_params(evolution_params* ep);

float get_current_value(changing_param* param);
void get_warrior_distribution(evolution_params* params, int* num_warriors, int* num_survivors, int* num_mutations, int* num_breedings, int* num_fresh, int* num_old);

/// <summary>
/// Load the fields of the evolution_params* "params" from a json file
/// </summary>
/// <param name="params"></param>
/// <param name="json_file"></param>
void json_parser(evolution_params* params, char* json_file);

void update_all_current_lvl(evolution_params* params, int iteration);
void update_param(changing_param* param, int iteration);