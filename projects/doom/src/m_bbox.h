#ifndef __M_BBOX__
#define __M_BBOX__

#include "doomtype.h"
#include "m_fixed.h"

enum
{
    BOXTOP,
    BOXBOTTOM,
    BOXLEFT,
    BOXRIGHT
};	// bbox coordinates

// Bounding box functions.
void M_ClearBox (fixed_t*	box);

void
M_AddToBox
( fixed_t*	box,
  fixed_t	x,
  fixed_t	y );

#endif

