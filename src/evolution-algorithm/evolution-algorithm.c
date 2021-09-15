#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../utils/utils.h"
#if defined(__unix__) || defined(__linux__)
	#define fopen_s(file, filename, mode) *file = fopen(filename, mode)
    #define sprintf_s(string, size, format, inc) snprintf(string, size, format, inc)
#endif
#include "evolution-parameters.h"
#include "../analyser/generator.h"
#include "../mars-interface/interface.h"
#include "../mars-interface/simple-interface.h"
#include "../mars-interface/redcode-switch.h"

void mutate_length(warrior_s* warrior, evolution_params* params)
{
    int old_length = get_instruction_number(warrior);
    int new_length;
    int random = rand_between(0, 2);
    if (random == 0)
    {
        new_length = old_length + rand_between(1, 5);
        if (new_length > MAXWARRIORLENGTH)
            new_length = MAXWARRIORLENGTH;
        edit_instruction_set(warrior, new_length);
        for (int line = old_length; line < new_length; line++)
            put_instruction_at(warrior, line, rand_between(0, 19), rand_between(0, 8), rand_between(0, 9), rand_between(0, 9),
                (CORESIZE + rand_between((int)get_current_value(params->A_value_modifier_min), (int)get_current_value(params->A_value_modifier_max) + 1)) % CORESIZE,
                (CORESIZE + rand_between((int)get_current_value(params->B_value_modifier_min), (int)get_current_value(params->B_value_modifier_max) + 1)) % CORESIZE);
    }
    else
    {
        new_length = old_length - rand_between(1, 5);
        int start = get_start_position(warrior);
        if (new_length <= start)
            new_length = start + 1;
        edit_instruction_set(warrior, new_length);
    }
}

warrior_s* mutate(warrior_s* warrior, evolution_params* params)
{
    warrior_s* w = copy_warrior(warrior);

    int instruction_number = get_instruction_number(w);
    float randomF;
    for(int line = 0; line < instruction_number; line++)
    {
        int instr = -1;
        int modifier = -1;
        int A_mode = -1;
        int B_mode = -1;
        int A_value = get_A_value_at(warrior, line);
        int B_value = get_B_value_at(warrior, line);
        /* Probability of modifying an instruction line */
        randomF = rand_float();
        if (randomF <= get_current_value(params->p_mutation_line))
        {
            /* Probability of modifying an instruction */
            randomF = rand_float();
            if (randomF <= get_current_value(params->p_mutation_field))
            {
                instr = rand_between(0, 19);

            }
            /* Probability of modifying a modifier */
            randomF = rand_float();
            if (randomF <= get_current_value(params->p_mutation_field))
            {
                modifier = rand_between(0, 8);

            }
            /* Probability of modifying addressing mode A */
            randomF = rand_float();
            if (randomF <= get_current_value(params->p_mutation_field))
            {
                A_mode = rand_between(0, 9);

            }
            /* Probability of modifying addressing mode B */
            randomF = rand_float();
            if (randomF <= get_current_value(params->p_mutation_field))
            {
                B_mode = rand_between(0, 9);

            }
            /* Probability of modifying A_value */
            randomF = rand_float();
            if (randomF <= get_current_value(params->p_mutation_field))
            {
                A_value = (CORESIZE + A_value + rand_between((int)get_current_value(params->A_value_modifier_min), (int)get_current_value(params->A_value_modifier_max) + 1)) % CORESIZE;

            }
            /* Probability of modifying B_value */
            randomF = rand_float();
            if (randomF <= get_current_value(params->p_mutation_field))
            {
                B_value = (CORESIZE + B_value + rand_between((int)get_current_value(params->B_value_modifier_min), (int)get_current_value(params->B_value_modifier_max) + 1)) % CORESIZE;
            }
            put_instruction_at(w, line, instr, modifier, A_mode, B_mode, A_value, B_value);
        }
    }

    randomF = rand_float();
    if (randomF <= get_current_value(params->p_mutation_line))
    {
        set_start_position(w, rand_between(0, get_instruction_number(w)));
    }

    randomF = rand_float();
    if (randomF <= get_current_value(params->p_mutation_line))
    {
        mutate_length(w, params);
    }
    return w;
}

warrior_s* breed(warrior_s* warrior1, warrior_s* warrior2, evolution_params* params)
{
    warrior_s* w = copy_warrior(warrior1);
    int instr_num_w1 = get_instruction_number(warrior1);
    int instr_num_w2 = get_instruction_number(warrior2);
    int breed_length = rand_between(get_current_value(params->breed_length_min), get_current_value(params->breed_length_max) + 1);

    if (breed_length > instr_num_w1 || breed_length > instr_num_w2) return w;
    int start_w1 = rand_between(0, instr_num_w1 - breed_length + 1);
    int start_w2 = rand_between(0, instr_num_w2 - breed_length + 1);
    for (int line = 0; line < breed_length; line++)
    {
        /* Probability of replacing the current warrior instruction line with that of warrior2 */
        put_instruction_at(w, start_w1 + line, get_instruction_at(warrior2, start_w2 + line), get_modifier_at(warrior2, start_w2 + line), 
            get_A_mode_at(warrior2, start_w2 + line), get_B_mode_at(warrior2, start_w2 + line), get_A_value_at(warrior2, start_w2 + line), get_B_value_at(warrior2, start_w2 + line));
    }
    return w;
}


warrior_s** evolution_algorithmV1(warrior_s** warriors, evolution_params* params)
{
    if (params->generations <= 0) return warriors;
    int iteration = 0;
    int max_old = 10;
    warrior_s** old_warriors = malloc_s(sizeof(warrior_s*) * max_old);
    int old = 0;

    int num_warriors, num_survivors, num_mutations, num_breedings, num_fresh, num_old;
    
    while (++iteration <= params->generations)
    {
        printf("***** Generation %i *****\n", iteration);

        // Update all the value of the field of params depending on the current generation 
        update_all_current_lvl(params, iteration);
        
        get_warrior_distribution(params, &num_warriors, &num_survivors, &num_mutations, &num_breedings, &num_fresh, &num_old);

        //Battle between the warriors of the pool
        warrior_s** new_warriors = do_battle((int)get_current_value(params->battles_by_generation), warriors, num_warriors, num_survivors);
        for (int i = 0; i < num_warriors; i++)
            if (warriors[i] != NULL)
            {
                switch (get_special_tag(warriors[i]))
                {
                case 0:
                    free_warrior(warriors[i]);
                    break;
                case 1:
                    if (old == max_old)
                    {
                        max_old += 10;
                        old_warriors = realloc_s(old_warriors, sizeof(warrior_s*) * max_old);
                    }
                    set_special_tag(warriors[i], 2);
                    old_warriors[old++] = warriors[i];
                    break;
                }
            }
        free(warriors);
        warriors = new_warriors;
        for (int i = 0; i < num_survivors; i++)
            set_special_tag(warriors[i], 1);

        //Modify the warriors according to the proportion of breedings, mutations and fresh if it isn't the last generation
        if (iteration != params->generations)
        {
            int i = num_survivors;
            for (int j = 0; j < num_mutations; j++)
            {
                warriors[i] = mutate(warriors[j * num_survivors / num_mutations], params);
                i++;
            }
            for (int j = 0; j < num_breedings; j++)
            {
                int target1 = j * num_survivors / num_breedings;
                int target2 = rand_between(0, num_survivors - 1);
                if (target2 >= target1) target2++;
                warriors[i] = breed(warriors[target1], warriors[target2], params);
                i++;
            }
            for (int j = 0; j < num_fresh; j++)
            {
                warriors[i] = make_warrior(params);
                i++;
            }
            for (int j = 0; j < num_old; j++)
            {
                if (old == 0)
                    warriors[i] = make_warrior(params);
                else
                    warriors[i] = old_warriors[rand_between(0, old)];
                i++;
            }
        }
        if (iteration != params->generations && iteration == params->generations / 2)
        {
            for (int i = 0; i < num_survivors; i++)
            {
                printf("%d : %s\n", i + 1, get_name(warriors[i]));

                char* buffer = (char*)malloc_s(sizeof(char) * 45);
                snprintf(buffer, 45, "../../data/Warriors/Generated/mid_%d.red", i + 1);
                FILE* file;
                fopen_s(&file, buffer, "w");
                if (file == NULL) exit(1);
                print_warrior(file, warriors[i]);
                fclose(file);
            }
        }
    }
    warriors = (warrior_s**) realloc_s(warriors, num_survivors * sizeof(warrior_s*));

    return warriors;
}

void print_results(char* json_file, float score)
{
    FILE* file_results;
    cJSON* json_parse1 = cJSON_Parse(bufferize_file(json_file));
    int version = cJSON_GetObjectItemCaseSensitive(json_parse1, "version")->valueint;
    char* results_file = (char*)malloc_s(30 * sizeof(char));
    sprintf_s(results_file, 30 * sizeof(char), "../Results/version%d.json", version);
    cJSON* json_parse2 = cJSON_Parse(bufferize_file(results_file));

    fopen_s(&file_results, results_file, "w");
    if (file_results == NULL) {
        printf("Erreur lors de l'ouverture d'un fichier");
        exit(1);
    }

    if (json_parse2 == NULL)
    {
        cJSON* time_tested_json = cJSON_GetObjectItemCaseSensitive(json_parse1, "time_tested");
        cJSON* score_json = cJSON_GetObjectItemCaseSensitive(json_parse1, "score");
        if(time_tested_json != NULL && score_json != NULL)
        {
            int time_tested = time_tested_json->valueint;
            int new_time_tested = time_tested + 1;
            float new_score = (score_json->valuedouble * time_tested + score) / new_time_tested;
            cJSON_SetIntValue(time_tested_json, new_time_tested);
            cJSON_SetNumberValue(score_json, new_score);
        }
        else
        {
            if(time_tested_json != NULL)
                cJSON_SetIntValue(time_tested_json, 1);
            else
                cJSON_AddItemToObject(json_parse1, "time_tested", cJSON_CreateNumber(1));
            if (score_json != NULL)
                cJSON_SetNumberValue(score_json, score);
            else
                cJSON_AddItemToObject(json_parse1, "score", cJSON_CreateNumber(score));
        }
        fprintf(file_results, "%s\n", cJSON_Print(json_parse1));
    }
    else
    {
        cJSON* time_tested_json = cJSON_GetObjectItemCaseSensitive(json_parse2, "time_tested");
        cJSON* score_json = cJSON_GetObjectItemCaseSensitive(json_parse2, "score");
        int time_tested = time_tested_json->valueint;
        int new_time_tested = time_tested+1;
        float new_score = (score_json->valuedouble * time_tested + score)/new_time_tested;
        cJSON_SetIntValue(time_tested_json, new_time_tested);
        cJSON_SetNumberValue(score_json, new_score);
        fprintf(file_results, "%s\n", cJSON_Print(json_parse2));
    }

    fclose(file_results);
}