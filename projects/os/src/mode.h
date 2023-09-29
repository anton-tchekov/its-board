#ifndef __MODE_H__
#define __MODE_H__

enum
{
	MODE_LOGIN,
	MODE_SHELL,
	MODE_EDITOR,
	MODE_MANAGER,
};

void mode_set(int mode);

#endif /* __MODE_H__ */
