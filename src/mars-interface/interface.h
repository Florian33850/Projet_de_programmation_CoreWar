#pragma once

#include "../exmars/exhaust.h"

#define MAXWARRIORLENGTH 100
#define CORESIZE 8000
#define NBR_TYPE_INSTRUCTION 19

#define warrior_s warrior_struct

int get_instruction_number(warrior_s* warrior);
int get_instruction_at(warrior_s* warrior, int i);
int get_modifier_at(warrior_s* warrior, int i);
int get_A_mode_at(warrior_s* warrior, int i);
int get_B_mode_at(warrior_s* warrior, int i);
int get_A_value_at(warrior_s* warrior, int i);
int get_B_value_at(warrior_s* warrior, int i);

char* get_name(warrior_s* warrior);
void set_unique_name(warrior_s* warrior);

int get_start_position(warrior_s* warrior);
void set_start_position(warrior_s* warrior, int new_position);

/// <summary>
/// Creates a new set of instruction for a warrior that doesn't have one
/// </summary>
/// <param name="warrior">Warrior that will get the set of instructions</param>
/// <param name="len">Length of the instruction set</param>
void new_instruction_set(warrior_s* warrior, int len);

/// <summary>
/// Edits the set of instruction for a warrior that doesn't have one
/// </summary>
/// <param name="warrior">Warrior that will get the set of instructions</param>
/// <param name="len">New length of the instruction set</param>
void edit_instruction_set(warrior_s* warrior, int len);

/// <summary>
/// Edits a particular instruction of the warrior
/// </summary>
/// <param name="warrior">Warrior to edit</param>
/// <param name="i">Instruction number to edit</param>
/// <param name="instr">New instruction code, -1 means no change</param>
/// <param name="modifier">New instruction modifier, -1 means no change</param>
/// <param name="A_mode">New A value modifier, -1 means no change</param>
/// <param name="B_mode">New B value modifier, -1 means no change</param>
/// <param name="A_value">New A value, -1 means no change</param>
/// <param name="B_value">New B value, -1 means no change</param>
void put_instruction_at(warrior_s* warrior, int i, int instr, int modifier, int A_mode, int B_mode, int A_value, int B_value);

/// <summary>
/// Creates a new warrior
/// </summary>
/// <returns>A fresh and empty warrior without any instruction set</returns>
warrior_s* get_empty_warrior();

/// <summary>
/// Creates a copy of a warrior, giving it a unique name depending on its parent's name
/// </summary>
/// <param name="warrior">Warrior to duplicate</param>
/// <returns>A new warrior, copy of the given one</returns>
warrior_s* copy_warrior(warrior_s* warrior);

void free_warrior(warrior_s* warrior);

char get_special_tag(warrior_s* warrior);
void set_special_tag(warrior_s* warrior, char tag);