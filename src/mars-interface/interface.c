#include <stdlib.h>
#include <string.h>
#include "../utils/utils.h"
#if defined(_WIN32)

#else
#define sprintf_s(string, size, format, inc) snprintf(string, size, format, inc)
#endif
#include "interface.h"
#include "../exmars/pmars.h"

int get_instruction_number(warrior_s* warrior) 
{ 
    return warrior->instLen; 
}

int get_instruction_at(warrior_s* warrior, int i) 
{ 
    return ((FIELD_T)(warrior->instBank[i].opcode & 0xf8)) >> 3; 
}

int get_modifier_at(warrior_s* warrior, int i)
{
    return (FIELD_T)(warrior->instBank[i].opcode & 7);
}

int get_A_mode_at(warrior_s* warrior, int i)
{
    return (FIELD_T)(warrior->instBank[i].A_mode);
}

int get_B_mode_at(warrior_s* warrior, int i)
{
    return (FIELD_T)(warrior->instBank[i].B_mode);
}

int get_A_value_at(warrior_s* warrior, int i)
{
    return (ADDR_T)(warrior->instBank[i].A_value);
}

int get_B_value_at(warrior_s* warrior, int i)
{
    return (ADDR_T)(warrior->instBank[i].B_value);
}

char* get_name(warrior_s* warrior)
{
    return warrior->name;
}

int id = 0;
void set_unique_name(warrior_s* warrior)
{
    char* name = (char*) malloc_s(10 * sizeof(char));
    sprintf_s(name, 10, "%d", id++);
    free(warrior->name);
    warrior->name = name;
}

int get_start_position(warrior_s* warrior)
{
    return warrior->offset;
}

void set_start_position(warrior_s* warrior, int new_position)
{
    warrior->offset = new_position;
}

void new_instruction_set(warrior_s* warrior, int len)
{
    warrior->instLen = len;
    warrior->instBank = (mem_struct*) malloc_s(len * sizeof(mem_struct));
}

void edit_instruction_set(warrior_s* warrior, int len)
{
    warrior->instLen = len;
    warrior->instBank = realloc_s(warrior->instBank, len * sizeof(mem_struct));
}

void put_instruction_at(warrior_s* warrior, int i, int instr, int modifier, int A_mode, int B_mode, int A_value, int B_value)
{
    if (instr != -1)
    {
        if (modifier != -1)
            warrior->instBank[i].opcode = (FIELD_T)(instr << 3) + modifier;
        else
            warrior->instBank[i].opcode = (FIELD_T)(instr << 3) + warrior->instBank[i].opcode & 7;
    }
    else if (modifier != -1)
        warrior->instBank[i].opcode = (FIELD_T)(warrior->instBank[i].opcode & 0xf8) + modifier;
    if (A_mode != -1)
        warrior->instBank[i].A_mode = A_mode;
    if (B_mode != -1)
        warrior->instBank[i].B_mode = B_mode;
    if (A_value != -1)
        warrior->instBank[i].A_value = A_value;
    if (B_value != -1)
        warrior->instBank[i].B_value = B_value;
}

warrior_s* get_empty_warrior()
{
	warrior_s* warrior = (warrior_s*) malloc_s(sizeof(warrior_s));
    warrior->name = pstrdup("");
    warrior->version = 0;
    warrior->offset = 0;

    return warrior;
}

warrior_s* copy_warrior(warrior_s* warrior)
{
    warrior_s* new_warrior = (warrior_s*)malloc_s(sizeof(warrior_s));

    new_instruction_set(new_warrior, warrior->instLen);
    new_warrior->offset = warrior->offset;
    
    for (int i = 0; i < warrior->instLen; i++)
    {
        new_warrior->instBank[i].A_value = warrior->instBank[i].A_value;
        new_warrior->instBank[i].B_value = warrior->instBank[i].B_value;
        new_warrior->instBank[i].opcode = warrior->instBank[i].opcode;
        new_warrior->instBank[i].A_mode = warrior->instBank[i].A_mode;
        new_warrior->instBank[i].B_mode = warrior->instBank[i].B_mode;
        new_warrior->instBank[i].debuginfo = warrior->instBank[i].debuginfo;
    }

    int i = 3;
    int n = id;
    while (n >= 10)
    {
        n /= 10;
        i++;
    }
    int name_length = strlen(warrior->name) + i;
    char* new_name = (char*) malloc_s(name_length * sizeof(char));
    snprintf(new_name, name_length, "%s_%d", warrior->name, id++);
    new_warrior->name = new_name;

    return new_warrior;
}

void free_warrior(warrior_s* warrior)
{
    free(warrior->name);
    free(warrior->instBank);
    free(warrior);
}

char get_special_tag(warrior_s* warrior)
{
    return warrior->version;
}

void set_special_tag(warrior_s* warrior, char tag)
{
    warrior->version = tag;
}