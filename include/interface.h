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
int get_start_position(warrior_s* warrior);

void set_unique_name(warrior_s* warrior);
void set_start_position(warrior_s* warrior, int new_position);
void begin_at(warrior_s* warrior, int i);

void new_instruction_set(warrior_s* warrior, int len);
void modif_instruction_set(warrior_s* warrior, int len);
//-1 means that the instruction isn't changed
void put_instruction_at(warrior_s* warrior, int i, int instr, int modifier, int A_mode, int B_mode, int A_value, int B_value);

warrior_s* get_empty_warrior();
warrior_s* copy_warrior(warrior_s* warrior);
void free_warrior(warrior_s* warrior);