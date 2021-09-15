#pragma once

#include "../mars-interface/interface.h"
#include "../mars-interface/redcode-switch.h"
#include "../../lib/cJSON/cJSON.h"
#include "generator.h"

/// <summary>
/// Increments a value found in the stats array depending on the values in the prev array. 
/// This recursive function depends on the value of depth.
/// </summary>
/// <param name="prev">An array containing the integer values of the current and previous 
/// instructions of a code in red code.</param>
/// <param name="stats">An array of pointers whose dimension is depth and which contains floats.</param>
/// <param name="depth">The depth of the statistics table.</param>
void increment_stats(int* prev, void* stats, int depth);

/// <summary>
/// Updates the red code instructions present in prev dependent on currentLine.
/// </summary>
/// <param name="prev">An array containing the integer values of the current and previous 
/// instructions of a code in red code.</param>
/// <param name="currentLine">The current line of instruction of a warrior</param>
/// <param name="depth">The size of the prev table.</param>
void refresh_current_line(int* prev, int currentLine, int depth);

/// <summary>
/// This function analyzes a warrior to reference the number 
/// of iterations of a series of instructions.
/// </summary>
/// <param name="warrior">A warrior to analyze</param>
/// <param name="stats">An array of pointers whose dimension is depth and which contains floats.</param>
/// <param name="depth">The depth of the statistics table.</param>
void analyse_warrior(warrior_s* warrior, void* stats, int depth);

/// <summary>
/// This function transforms an array of statistics 
/// into a cJSON variable and returns a pointer to that variable.
/// </summary>
/// <param name="stats">An array of pointers whose dimension is depth and which contains floats.</param>
/// <param name="depth">The depth of the statistics table.</param>
/// <returns>A pointer to cJSON variable</returns>
cJSON* make_json_file(void* stats, int depth);

/// <summary>
/// This function sets up a table of statistics of dimension depth depending on a pool 
/// of warriors present in the path directory. Returns a pointer to a generator structure.
/// </summary>
/// <param name="path">Path directory of warrior pool</param>
/// <param name="depth">The depth of the statistics table.</param>
/// <returns>A generator structure</returns>
generator* start_analyser(char* path, int depth);

