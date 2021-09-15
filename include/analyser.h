#pragma once

#include "../mars-interface/interface.h"
#include "../mars-interface/redcode-switch.h"
#include "../../lib/cJSON/cJSON.h"
#include "generator.h"

void increment_stats(int* prev, void* stats, int depth);

void refresh_current_line(int* prev, int currentLine, int depth);

void analyse_warrior(warrior_s* warrior, void* stats, int depth);

cJSON* make_json_file(void* stats, int depth);

/// <summary>
/// Function that counts the occurence of each redcode instruction depending on its position
/// </summary>
/// <param name="warrior">A warrior to analyse</param>
/// <param name="stats">An integer array counting the instructions</param>
generator* start_analyser(char* path, int depth);

