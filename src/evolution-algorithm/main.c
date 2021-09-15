#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/utils.h"
#if defined(__unix__) || defined(__linux__)
	#define fopen_s(file, filename, mode) *file = fopen(filename, mode)
#endif
#include "evolution-algorithm.h"
#include "scoring.h"
#include "evolution-parameters.h"
#include "../analyser/generator.h"
#include "../mars-interface/interface.h"
#include "../mars-interface/redcode-switch.h"
#include "../../lib/cJSON/cJSON.h"


int main(int argc, char* argv[])
{
	time_t seed = time(NULL);
	printf("seed : %u\n", (unsigned int)seed);
	srand((unsigned int)seed);
	char* json_file = "../evolution-params.json";

	evolution_params* params = json_parser(json_file);

	int num_warriors, a, b, c, d, e;
	get_warrior_distribution(params, &num_warriors, &a, &b, &c, &d, &e);

	warrior_s** warriors = (warrior_s**) malloc_s(sizeof(warrior_s*) * num_warriors);
	/* Creation of the first generation of warriors */
	for (int i = 0; i < num_warriors; i++)
	{
		warriors[i] = make_warrior(params);
	}

	warriors = evolution_algorithmV1(warriors, params);

	get_warrior_distribution(params, &a, &num_warriors, &b, &c, &d, &e);
	/* save the warriors in file */
	for (int i = 0; i < num_warriors; i++)
	{
		printf("%d : %s\n", i + 1, get_name(warriors[i]));

		char* buffer = (char*) malloc_s(sizeof(char) * 41);
		snprintf(buffer, 41, "../../data/Warriors/Generated/%d.red", i + 1);
		FILE* file;
		fopen_s(&file, buffer, "w");
		if (file == NULL) exit(1);
		print_warrior(file, warriors[i]);
		fclose(file);
	}
	printf("generated after %.0f seconds\n", difftime(time(NULL), seed));
	float score = get_algo_score(warriors, num_warriors);
	printf("score : %f\n", score);

	print_results(json_file, score);

	free_evolution_params(params);
	
	printf("Program finished after %.0f seconds\n", difftime(time(NULL), seed));

	return 0;
}
