#pragma once
#include "../mars-interface/interface.h"

/// <summary>
/// Compute the score of the algorithm
/// </summary>
/// <param name="warriors">List of the warriors returned by the algorithm</param>
/// <param name="num_warriors">Number of warriors returned by the algorithm</param>
/// <returns>Score of the algorithm</returns>
float get_algo_score(warrior_s** warriors, int num_warriors);