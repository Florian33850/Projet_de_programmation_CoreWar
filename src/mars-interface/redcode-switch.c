#include <stdlib.h>
#include <stdio.h>
#include "../utils/utils.h"

#include "interface.h"
#include "util.h"
#include "../exmars/pmars.h"

warrior_s* red_to_warrior(char *fname) 
{
	mars_t* mars = get_a_mars();
	warrior_s* warrior;

	warrior = (warrior_s*) malloc_s(sizeof(warrior_s));
	
	if (assemble_warrior(mars, fname, warrior)) 
	{
		free(warrior);
		return NULL;
	}

	return warrior;
}


void print_warrior(FILE* stream, warrior_s* warrior)
{
	mars_t* mars = get_a_mars();

	ADDR_T  i;

	if (warrior->offset >= 0 && warrior->offset < warrior->instLen)
		fprintf(stream, "%-6s %3s%3s  %6s\n", "", "ORG", "", "START");

	for (i = 0; i < warrior->instLen; ++i)
	{
		fprintf(stream, "%-6s ", i == warrior->offset ? "START" : "");
		FIELD_T opcode = ((FIELD_T)(warrior->instBank[i].opcode & 0xf8)) >> 3;
		FIELD_T modifier = (warrior->instBank[i].opcode & 0x07);

		fprintf(stream, "%3s", opname[opcode]);
		if (modifier != 7)
			fprintf(stream, ".%-3s", modname[modifier]);
		else
			fprintf(stream, "%-4s", "");

		char field_modifier = PM_INDIR_A(warrior->instBank[i].A_mode) ? addr_sym[INDIR_A_TO_SYM(warrior->instBank[i].A_mode)] : addr_sym[warrior->instBank[i].A_mode];
		int value = denormalize(mars, warrior->instBank[i].A_value);
		if (field_modifier == '\0')
			field_modifier = ' ';
		fprintf(stream, "%c%6d, ", field_modifier, value);

		field_modifier = PM_INDIR_A(warrior->instBank[i].B_mode) ? addr_sym[INDIR_A_TO_SYM(warrior->instBank[i].B_mode)] : addr_sym[warrior->instBank[i].B_mode];
		value = denormalize(mars, warrior->instBank[i].B_value);
		if (field_modifier == '\0')
			field_modifier = ' ';
		fprintf(stream, "%c%6d\n", field_modifier, value);
	}

	if (warrior->offset >= 0 && warrior->offset < warrior->instLen)
		fprintf(stream, "%-6s %3s%3s  %6s\n", "", "END", "", "START");
}