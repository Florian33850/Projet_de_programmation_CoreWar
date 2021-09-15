#pragma once

#include <stdio.h>
#include "interface.h"

/// <summary>
/// Convert a .red warrior in a warrior_s warrior, the structure of pmars, and return it
/// </summary>
/// <param name="fname">fname of the red file to convert in warrior</param>
/// <returns></returns>
warrior_s* red_to_warrior(char* fname);

/// <summary>
/// Print in file all the redcode of a warrior
/// </summary>
/// <param name="stream">The target file</param>
/// <param name="warrior">The warrior to print</param>
void print_warrior(FILE* file, warrior_s* warrior);