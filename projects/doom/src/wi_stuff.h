#ifndef __WI_STUFF__
#define __WI_STUFF__

#include "doomdef.h"

typedef enum
{
	NoState = -1,
	StatCount,
	ShowNextLoc
} stateenum_t;

void WI_Ticker(void);
void WI_Drawer(void);
void WI_Start(wbstartstruct_t *wbstartstruct);

#endif
