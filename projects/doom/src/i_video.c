#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>


#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"
#include "doomdef.h"
#include "keys.h"

int _convert_key(int sc)
{
	int rc = 0;
	switch(sc)
	{
		case USB_KEY_LEFT:   rc = KEY_LEFTARROW;  break;
		case USB_KEY_RIGHT:  rc = KEY_RIGHTARROW; break;
		case USB_KEY_DOWN:   rc = KEY_DOWNARROW;  break;
		case USB_KEY_UP:     rc = KEY_UPARROW;    break;
		case USB_KEY_ESCAPE: rc = KEY_ESCAPE;     break;
		case USB_KEY_RETURN: rc = KEY_ENTER;      break;
		case USB_KEY_TAB:    rc = KEY_TAB;        break;
		case USB_KEY_F1:     rc = KEY_F1;         break;
		case USB_KEY_F2:     rc = KEY_F2;         break;
		case USB_KEY_F3:     rc = KEY_F3;         break;
		case USB_KEY_F4:     rc = KEY_F4;         break;
		case USB_KEY_F5:     rc = KEY_F5;         break;
		case USB_KEY_F6:     rc = KEY_F6;         break;
		case USB_KEY_F7:     rc = KEY_F7;         break;
		case USB_KEY_F8:     rc = KEY_F8;         break;
		case USB_KEY_F9:     rc = KEY_F9;         break;
		case USB_KEY_F10:    rc = KEY_F10;        break;
		case USB_KEY_F11:    rc = KEY_F11;        break;
		case USB_KEY_F12:    rc = KEY_F12;        break;
		case USB_KEY_BACKSPACE:
		case USB_KEY_DELETE:
			rc = KEY_BACKSPACE;
			break;

		case USB_KEY_KP_EQUALS:
		case USB_KEY_EQUALS:  rc = KEY_EQUALS;     break;
		case USB_KEY_KP_MINUS:
		case USB_KEY_MINUS:  rc = KEY_MINUS;      break;
		case USB_KEY_L_SHIFT:
		case USB_KEY_R_SHIFT:
			rc = KEY_RSHIFT;
			break;

		case USB_KEY_L_CTRL:
		case USB_KEY_R_CTRL:
			rc = KEY_RCTRL;
			break;

		case USB_KEY_L_ALT:
		case USB_KEY_L_GUI:
		case USB_KEY_R_ALT:
		case USB_KEY_R_GUI:
			rc = KEY_RALT;
			break;

		case USB_KEY_SPACE:
			rc = ' ';
			break;

		default:
			if(sc >= USB_KEY_A && sc <= USB_KEY_Z)
			{
				rc = (sc - USB_KEY_A) + 'A';
			}
			break;
	}

	return rc;
}

void I_ShutdownGraphics(void)
{
	platform_destroy();
}

void I_FinishUpdate(void)
{
	event_t event;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			event.type = ev_keydown;
			event.data1 = _convert_key(e.key.keysym.scancode);
			D_PostEvent(&event);
			break;

		case SDL_KEYUP:
			event.type = ev_keyup;
			event.data1 = _convert_key(e.key.keysym.scancode);
			D_PostEvent(&event);
			break;
		}
	}

	platform_blit(screens[0]);
}

void I_ReadScreen(byte *scr)
{
	memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

void I_SetPalette(byte *palette)
{
	platform_palette(palette);
}
