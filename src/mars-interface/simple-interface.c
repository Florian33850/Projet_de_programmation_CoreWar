#include <stdlib.h>
#include <stdio.h>
#include "../utils/utils.h"

#include "interface.h"
#include "util.h"
#include "../exmars/pmars.h"
#include "../exmars/exhaust.h"
#include "../exmars/pspace.h"
#include "../exmars/sim.h"
#include "simple-interface.h"

#include <omp.h>

mars_t* initialize_mars(warrior_s** warriors, int num_warriors)
{
	mars_t* mars = get_a_mars();

	mars->nWarriors = num_warriors;

	for (int i = 0; i < num_warriors; i++)
	{
		warriors[i]->pSpaceIDNumber = i;
		if (i != num_warriors - 1)
			warriors[i]->nextWarrior = warriors[i + 1];
	}

	if (!sim_alloc_bufs(mars))
	{
		printf("memory alloc failed.\n");
		exit(EXIT_FAILURE);
	}

	pmars2exhaust(mars, warriors, num_warriors);

	check_sanity(mars);
	clear_results(mars);

	save_pspaces(mars);
	amalgamate_pspaces(mars);

	return mars;
}

void do_a_match(mars_t* mars, int n)
{
	int nalive;
	sim_clear_core(mars);

	mars->seed = compute_positions(mars->seed, mars);
	load_warriors(mars);
	set_starting_order(n, mars);

	nalive = sim_mw(mars, mars->startPositions, mars->deaths);
	if (nalive < 0)
		panic("simulator panic!\n");

	accumulate_results(mars);
}


void update_scores(mars_t* mars, float* scores, int num_warriors)
{
	float base = num_warriors * (num_warriors - 1);
	for (int i = 0; i < num_warriors; i++)
	{
		for (int surviving = 1; surviving < num_warriors; surviving++)
		{
			int times = mars->results[i * (num_warriors + 1) + surviving];
			#pragma omp atomic
			scores[i] += times * base / surviving;
		}
	}
}

warrior_s** get_survivors(float* scores, warrior_s** warriors, int num_warriors, int num_survivors)
{
	warrior_s** survivors = (warrior_s**) malloc_s(sizeof(warrior_s*) * num_warriors);
	int n = 0;
	while (num_survivors != 0)
	{
		int tied = 0;
		float best = 0;
		for (int i = 0; i < num_warriors; i++)
		{
			if (warriors[i] != 0)
			{
				if (scores[i] > best)
				{
					best = scores[i];
					tied = 1;
				}
				else if (scores[i] == best)
				{
					tied++;
				}
			}
		}
		for (int i = 0; i < num_warriors; i++)
		{
			if (warriors[i] != 0)
			{
				if (scores[i] == best)
				{
					if (rand_between(0, tied--) <= num_survivors)
					{
						if (n == 0)
							printf("best warrior of this battle : %s\n", get_name(warriors[i]));
						survivors[n++] = warriors[i];
						warriors[i] = 0;

						if (--num_survivors == 0) break;
					}
					if (tied == 0) break;
				}
			}
		}
	}
	free(scores);

	return survivors;
}

warrior_s** cpy_warriors(warrior_s** warriors, int num_warriors)
{
	warrior_s** w = (warrior_s**)malloc_s(sizeof(warrior_s*) * num_warriors);
	for (int i = 0; i < num_warriors; i++)
	{
		w[i] = malloc_s(sizeof(warrior_s));
		w[i]->instLen = warriors[i]->instLen;
		w[i]->offset = warriors[i]->offset;
		w[i]->instBank = warriors[i]->instBank;
	}
	return w;
}

void free_warriors(warrior_s** warriors, int num_warriors)
{
	for (int i = 0; i < num_warriors; i++)
	{
		free(warriors[i]);
	}
	free(warriors);
}

float* do_matches(int rounds, warrior_s** warriors, int num_warriors)
{
	float* scores = (float*)calloc_s(sizeof(float) * num_warriors);
	int progress_bar = rounds / 20;
	if (progress_bar == 0) progress_bar++;
	int i;
#define battle_by_thread 2
#pragma omp parallel for schedule(static) shared(scores)
	for (i = 0; i <= rounds / battle_by_thread; i++)
	{
		warrior_s** w = cpy_warriors(warriors, num_warriors);
		mars_t* mars = initialize_mars(w, num_warriors);
		int n = battle_by_thread;
		if (i == rounds / battle_by_thread)
			n = rounds % battle_by_thread;
		for (int j = 0; j < n; j++)
		{
			do_a_match(mars, i * battle_by_thread + j);
			if ((i * battle_by_thread + j) % progress_bar == 0)
				printf("#");
		}
		update_scores(mars, scores, num_warriors);
		free_warriors(w, num_warriors);
		sim_free_bufs(mars);
	}
	printf("\n");

	return scores;
}

warrior_s** do_battle(int rounds, warrior_s** warriors, int num_warriors, int num_survivors)
{
	float* scores = do_matches(rounds, warriors, num_warriors);

	warrior_s** survivors = get_survivors(scores, warriors, num_warriors, num_survivors);

	return survivors;
}

float get_final_score(int rounds, warrior_s** warriors, int num_warriors)
{
	float* scores = do_matches(rounds, warriors, num_warriors);

	for (int i = 0; i < num_warriors; i++)
	{
		scores[i] /= 500;
		printf("Score of %s : %f\n", warriors[i]->name, scores[i]);
	}

	float score = scores[0];

	for (int i = 0; i < num_warriors; i++)
	{
		free_warrior(warriors[i]);
	}
	free(warriors);
	free(scores);

	return score;
}