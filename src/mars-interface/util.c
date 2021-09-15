#include <stdlib.h>
#include <string.h>
#include "../utils/utils.h"

#include "interface.h"
#include "../exmars/exhaust.h"


mars_t* get_a_mars()
{
	mars_t* mars = (mars_t*)malloc_s(sizeof(mars_t));
	memset(mars, 0, sizeof(mars_t));
	mars->rounds = 1;
	mars->cycles = 80000;
	mars->coresize = CORESIZE;
	mars->processes = 8000;
	mars->maxWarriorLength = MAXWARRIORLENGTH;
	mars->seed = rand();
	mars->minsep = 100;

	/* pmars */
	mars->errorcode = SUCCESS;
	mars->errorlevel = WARNING;
	mars->saveOper = 0;
	mars->errmsg[0] = '\0';
	return mars;
}