#include <stdlib.h>
#include "../utils/utils.h"

#include "../mars-interface/interface.h"
#include "../mars-interface/simple-interface.h"
#include "../mars-interface/redcode-switch.h"
#if defined(__unix__) || defined(__linux__)
#define sprintf_s(string, size, format, inc) snprintf(string, size, format, inc )
#endif
float get_algo_score(warrior_s** warriors, int num_warriors)
{
	printf("Getting score (1/2)\n");
	if (num_warriors > 1)
	{
		warrior_s** new_warriors = do_battle(1000, warriors, num_warriors, 1);
		for (int i = 0; i < num_warriors; i++)
			if (warriors[i] != NULL)
				free_warrior(warriors[i]);
		free(warriors);
		warriors = new_warriors;
	}
	warriors = realloc_s(warriors, sizeof(warrior_s*) * 6);
	warriors[1] = red_to_warrior("../../data/Warriors/TestPool/armadillo.red");
	warriors[2] = red_to_warrior("../../data/Warriors/TestPool/quicksilver.red");
	warriors[3] = red_to_warrior("../../data/Warriors/TestPool/glyph.red");
	warriors[4] = red_to_warrior("../../data/Warriors/TestPool/Dwarf.red");
	warriors[5] = red_to_warrior("../../data/Warriors/TestPool/Imp.red");

	printf("Getting score (2/2)\n");
	float score = get_final_score(10000, warriors, 6);

	return score;
}