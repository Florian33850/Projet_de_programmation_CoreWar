#pragma once

/// <summary>
/// This structure contains all the value of a parameter that evolve during the generation, and the levels 
/// at which it must evolve
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

void free_changing_param(changing_param* param);
void free_evolution_params(evolution_params* params);

/// <summary>
/// Fetches the value that should be used in the current generation of the evolution algorithm
/// </summary>
/// <param name="param">Value to get</param>
/// <returns>The value corresponding to the current generation</returns>
float get_current_value(changing_param* param);

/// <summary>
/// Same as get_current_value, but adapted to return the current number of the distribution of warrior	
/// </summary>
/// <param name="params">Evolution parameters of the algorithm</param>
/// <param name="num_warriors">Pointer to where the number of total warriors of this generation should be stored</param>
/// <param name="num_survivors">Pointer to where the number of survivors of this generation should be stored</param>
/// <param name="num_mutations">Pointer to where the number of mutated warriors of this generation should be stored</param>
/// <param name="num_breedings">Pointer to where the number of fused of this generation should be stored</param>
/// <param name="num_fresh">Pointer to where the number of fresh warriors of this generation should be stored</param>
/// <param name="num_old">Pointer to where the number of old warriors of this generation should be stored</param>
void get_warrior_distribution(evolution_params* params, int* num_warriors, int* num_survivors, int* num_mutations, int* num_breedings, int* num_fresh, int* num_old);

/// <summary>
/// Parses the evolution parameters
/// </summary>
/// <param name="json_path">Path to the json of the parameters</param>
/// <returns>Returms the corresponding structure</returns>
evolution_params* json_parser(char* json_path);

/// <summary>
/// Update a given parameter to the current generation
/// </summary>
/// <param name="param">Parameter to update</param>
/// <param name="generation">Current generation</param>
void update_param(changing_param* param, int generation);

/// <summary>
/// Update all the parameters to the current generation
/// </summary>
/// <param name="params">Parameters to update</param>
/// <param name="generation">Current generation</param>
void update_all_current_lvl(evolution_params* params, int generation);