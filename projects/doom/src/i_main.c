#include "doomdef.h"
#include "m_argv.h"
#include "d_main.h"
#include "platform.h"

int main(int argc, char **argv)
{
	myargc = argc;
	myargv = argv;
	platform_init();
	D_DoomMain();
	platform_destroy();
	return 0;
}
