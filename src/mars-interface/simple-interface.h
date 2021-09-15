#pragma once

#include "interface.h"

/// <summary>
/// Return a mars simulator initialized with warriors
/// </summary>
/// <param name="warriors">A list of warriors</param>
/// <param name="num_warriors">Number of warriors</param>
/// <returns> MARS structure initialized</returns>
mars_t* initialize_mars(warrior_s** warriors, int num_warriors);

/// <summary>
/// Compute a battle in mars with all warriors initialized in it, and store the result in the mars structure
/// </summary>
/// <param name="mars">A MARS structure</param>
/// <param name="n">TODO</param>
void do_a_match(mars_t* mars, int n);

/// <summary>
/// Get the scores of the warriors present in MARS
/// </summary>
/// <param name="mars">A MARS structure</param>
/// <param name="num_warriors">Number of warriors</param>
/// <returns>A list of scores</returns>
void update_scores(mars_t* mars, float* scores, int num_warriors);

/// <summary>
/// Sort the result of the battle, and return the num_survivors best results
/// </summary>
/// <param name="mars">A MARS structure</param>
/// <param name="warriors">A list of warriors</param>
/// <param name="num_warriors">Number of warriors in the list</param>
/// <param name="num_survivors">Number of survivors</param>
/// <returns>A list of survivors</returns>
warrior_s** get_survivors(float* scores, warrior_s** warriors, int num_warriors, int num_survivors);

warrior_s** cpy_warriors(warrior_s** warriors, int num_warriors);

void free_warriors(warrior_s** warriors, int num_warriors);

float* do_matches(int rounds, warrior_s** warriors, int num_warriors);

/// <summary>
/// Return the num_survivors best warriors
/// </summary>
/// <param name="rounds">Number of rounds to do</param>
/// <param name="warriors">A list of warriors to face off</param>
/// <param name="num_warriors">Number of warriors in the list</param>
/// <param name="num_survivors">Number of survivors</param>
/// <returns>A list of best survivors</returns>
warrior_s** do_battle(int rounds, warrior_s** warriors, int num_warriors, int num_survivors);

/// <summary>
/// TODO
/// </summary>
/// <param name="rounds">TODO</param>
/// <param name="warriors">TODO</param>
/// <param name="num_warriors">TODO</param>
/// <returns>TODO</returns>
float get_final_score(int rounds, warrior_s** warriors, int num_warriors);

char get_special_tag(warrior_s* warrior);

void set_special_tag(warrior_s* warrior, char tag);