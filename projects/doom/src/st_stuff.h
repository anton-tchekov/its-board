#ifndef __STSTUFF_H__
#define __STSTUFF_H__

#include "doomtype.h"
#include "d_event.h"

#define ST_HEIGHT	32*SCREEN_MUL
#define ST_WIDTH	SCREENWIDTH
#define ST_Y		(SCREENHEIGHT - ST_HEIGHT)

boolean ST_Responder (event_t* ev);
void ST_Ticker (void);
void ST_Drawer (boolean fullscreen, boolean refresh);
void ST_Start (void);
void ST_Init (void);

typedef enum
{
    AutomapState,
    FirstPersonState

} st_stateenum_t;


// States for the chat code.
typedef enum
{
    StartChatState,
    WaitDestState,
    GetChatState

} st_chatstateenum_t;


boolean ST_Responder(event_t* ev);

#endif
