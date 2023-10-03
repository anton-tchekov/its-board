/**
 * @file    manager.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-10-02
 */

#include "manager.h"
#include "keyboard.h"
#include "terminal.h"
#include "mode.h"
#include <string.h>

#define HELP_Y            (TERMINAL_H - 1)
#define HELP_START        (TERMINAL_W / 2 - sizeof(help) / 2)
#define HELP_END          (HELP_START + sizeof(help) - 1)
#define FILES_Y_END       (TERMINAL_H - 2)

#define FILE_OFFSET      1
#define EXT_OFFSET      14
#define MAX_ENTRIES       (TERMINAL_H - 3)

#define NORMAL            TERMINAL_FG_WHITE | TERMINAL_BG_BLACK
#define INVERTED          TERMINAL_BG_WHITE | TERMINAL_FG_BLACK

#define MAX_FILENAME_LEN  8
#define MAX_EXTENSION_LEN 3

typedef struct
{
	uint32_t Size;
	uint8_t Type;
	char Name[8];
	char Ext[3];
} DirEnt;

typedef struct
{
	int Selected;
	int Count;
	int Offset;
	char Path[64];
	DirEnt Entries[256];
} Side;

static Side _sides[2];
static u8 _side;

static void printc(int x, int y, int c)
{
	terminal_set(x, y, c | TERMINAL_FG_WHITE | TERMINAL_BG_BLACK);
}

static void prints(int x, int y, const char *s)
{
	int c;
	while((c = *s++))
	{
		printc(x++, y, c);
	}
}

void manager_init(void)
{
	strcpy(_sides[0].Path, "/USR/BIN/EDITOR");
	strcpy(_sides[1].Path, "/HOME/ANTON/DOWNLOAD/FILEMGR");

	_sides[1].Count = 2;
	_sides[1].Entries[0].Size = 0;
	_sides[1].Entries[0].Type = 1;
	strcpy(_sides[1].Entries[0].Name, "..");
	strcpy(_sides[1].Entries[0].Ext, "");

	_sides[1].Entries[1].Size = 0;
	_sides[1].Entries[1].Type = 0;
	strcpy(_sides[1].Entries[1].Name, "TEST");
	strcpy(_sides[1].Entries[1].Ext, "BIN");

	_sides[0].Count = 21;

	_sides[0].Entries[0].Size = 0;
	_sides[0].Entries[0].Type = 1;
	strcpy(_sides[0].Entries[0].Name, "..");
	strcpy(_sides[0].Entries[0].Ext, "");

	_sides[0].Entries[1].Size = 0;
	_sides[0].Entries[1].Type = 1;
	strcpy(_sides[0].Entries[1].Name, "SYSTEM");
	strcpy(_sides[0].Entries[1].Ext, "");

	_sides[0].Entries[2].Size = 0;
	_sides[0].Entries[2].Type = 1;
	strcpy(_sides[0].Entries[2].Name, "PROJECTS");
	strcpy(_sides[0].Entries[2].Ext, "");

	_sides[0].Entries[3].Size = 0;
	_sides[0].Entries[3].Type = 1;
	strcpy(_sides[0].Entries[3].Name, "DOWNLOAD");
	strcpy(_sides[0].Entries[3].Ext, "");

	_sides[0].Entries[4].Size = 0;
	_sides[0].Entries[4].Type = 1;
	strcpy(_sides[0].Entries[4].Name, "DOCS");
	strcpy(_sides[0].Entries[4].Ext, "");

	_sides[0].Entries[5].Size = 0;
	_sides[0].Entries[5].Type = 1;
	strcpy(_sides[0].Entries[5].Name, "IMAGES");
	strcpy(_sides[0].Entries[5].Ext, "");

	_sides[0].Entries[6].Size = 0;
	_sides[0].Entries[6].Type = 1;
	strcpy(_sides[0].Entries[6].Name, "VIDEOS");
	strcpy(_sides[0].Entries[6].Ext, "");

	_sides[0].Entries[7].Size = 0;
	_sides[0].Entries[7].Type = 0;
	strcpy(_sides[0].Entries[7].Name, "PASSWD");
	strcpy(_sides[0].Entries[7].Ext, "TXT");

	_sides[0].Entries[8].Size = 0;
	_sides[0].Entries[8].Type = 0;
	strcpy(_sides[0].Entries[8].Name, "MAIN");
	strcpy(_sides[0].Entries[8].Ext, "C");

	_sides[0].Entries[9].Size = 0;
	_sides[0].Entries[9].Type = 0;
	strcpy(_sides[0].Entries[9].Name, "SYNTAX");
	strcpy(_sides[0].Entries[9].Ext, "ASM");

	_sides[0].Entries[10].Size = 0;
	_sides[0].Entries[10].Type = 0;
	strcpy(_sides[0].Entries[10].Name, "A");
	strcpy(_sides[0].Entries[10].Ext, "OUT");

	_sides[0].Entries[11].Size = 0;
	_sides[0].Entries[11].Type = 1;
	strcpy(_sides[0].Entries[11].Name, "BOOKS");
	strcpy(_sides[0].Entries[11].Ext, "");

	_sides[0].Entries[12].Size = 0;
	_sides[0].Entries[12].Type = 1;
	strcpy(_sides[0].Entries[12].Name, "STUDIUM");
	strcpy(_sides[0].Entries[12].Ext, "");

	_sides[0].Entries[13].Size = 0;
	_sides[0].Entries[13].Type = 0;
	strcpy(_sides[0].Entries[13].Name, "KOTLIN");
	strcpy(_sides[0].Entries[13].Ext, "TXT");

	_sides[0].Entries[14].Size = 0;
	_sides[0].Entries[14].Type = 0;
	strcpy(_sides[0].Entries[14].Name, "OUT");
	strcpy(_sides[0].Entries[14].Ext, "PNG");

	_sides[0].Entries[15].Size = 0;
	_sides[0].Entries[15].Type = 1;
	strcpy(_sides[0].Entries[15].Name, "SCHULE");
	strcpy(_sides[0].Entries[15].Ext, "");

	_sides[0].Entries[16].Size = 0;
	_sides[0].Entries[16].Type = 1;
	strcpy(_sides[0].Entries[16].Name, "SOFTWARE");
	strcpy(_sides[0].Entries[16].Ext, "");

	_sides[0].Entries[17].Size = 0;
	_sides[0].Entries[17].Type = 1;
	strcpy(_sides[0].Entries[17].Name, "SCRIPTS");
	strcpy(_sides[0].Entries[17].Ext, "");

	_sides[0].Entries[18].Size = 0;
	_sides[0].Entries[18].Type = 1;
	strcpy(_sides[0].Entries[18].Name, "MUSIC");
	strcpy(_sides[0].Entries[18].Ext, "");

	_sides[0].Entries[19].Size = 0;
	_sides[0].Entries[19].Type = 1;
	strcpy(_sides[0].Entries[19].Name, "CURRENT");
	strcpy(_sides[0].Entries[19].Ext, "");

	_sides[0].Entries[20].Size = 0;
	_sides[0].Entries[20].Type = 1;
	strcpy(_sides[0].Entries[20].Name, "ATFS");
	strcpy(_sides[0].Entries[20].Ext, "");
}

static void _render_path(u8r which)
{
	Side *side = &_sides[which];
	size_t len = strlen(side->Path);
	const char *s = side->Path;
	u8r x = which * (TERMINAL_W / 2) + 2;
	int c;

	printc(x++, 0, ' ');
	if(len <= (TERMINAL_W / 2 - 6))
	{
		u8r end = x + (TERMINAL_W / 2 - 4);

		while((c = *s++))
		{
			printc(x++, 0, c);
		}

		printc(x++, 0, ' ');
		while(x < end)
		{
			printc(x++, 0, '-');
		}
	}
	else
	{
		printc(x++, 0, '.');
		printc(x++, 0, '.');

		s = s + len - (TERMINAL_W / 2 - 8);
		while((c = *s++))
		{
			printc(x++, 0, c);
		}

		printc(x, 0, ' ');
	}
}

static void _render_files(u8r which)
{
	Side *side = &_sides[which];
	u8r j;
	u8r x;
	u8r x_start = which * (TERMINAL_W / 2) + 1;
	u8r x_end = x_start + (TERMINAL_W / 2 - 2);
	u8r y = 1;
	int i = side->Offset;
	int c;
	int color;

	for(; i < side->Count && y < FILES_Y_END; ++i, ++y)
	{
		DirEnt *ent = &side->Entries[i];
		x = x_start;
		color = (which == _side && i == side->Selected) ? INVERTED : NORMAL;
		terminal_set(x++, y, (ent->Type ? '/' : ' ') | color);
		for(j = 0; j < MAX_FILENAME_LEN && (c = ent->Name[j]); ++j)
		{
			terminal_set(x++, y, c | color);
		}

		if(!ent->Type)
		{
			terminal_set(x++, y, '.' | color);
			for(j = 0; j < MAX_EXTENSION_LEN && (c = ent->Ext[j]); ++j)
			{
				terminal_set(x++, y, c | color);
			}
		}

		while(x < x_end)
		{
			terminal_set(x++, y, ' ' | color);
		}
	}

	/* Clear rest */
	for(; y < FILES_Y_END; ++y)
	{
		for(x = x_start; x < x_end; ++x)
		{
			printc(x, y, ' ');
		}
	}
}

static void _help_line(void)
{
	static const char help[] =
		"F5 Copy | F6 Move/Rename | F7 MkDir | F8 Delete";

	u8r i;
	for(i = 0; i < HELP_START; ++i) { printc(i, HELP_Y, ' '); }
	prints(HELP_START, HELP_Y, help);
	for(i = HELP_END; i < TERMINAL_W; ++i) { printc(i, HELP_Y, ' '); }
}

static void _border_vertical(void)
{
	u8r i;
	for(i = 1; i < TERMINAL_H - 2; ++i)
	{
		printc(0, i, '|');
		printc(TERMINAL_W / 2 - 1, i, '|');
		printc(TERMINAL_W / 2, i, '|');
		printc(TERMINAL_W - 1, i, '|');
	}
}

static void _border_horizontal(void)
{
	u8r i;

	/* Top */
	prints(0, 0, "+-");
	_render_path(0);
	prints(TERMINAL_W / 2 - 2, 0, "-++-");
	_render_path(1);
	prints(TERMINAL_W - 2, 0, "-+");

	/* Bottom */
	printc(0, TERMINAL_H - 2, '+');
	prints(TERMINAL_W / 2 - 1, TERMINAL_H - 2, "++");
	printc(TERMINAL_W - 1, TERMINAL_H - 2, '+');
	for(i = 0; i < TERMINAL_W / 2 - 2; ++i)
	{
		printc(i + 1, TERMINAL_H - 2, '-');
		printc(TERMINAL_W / 2 + i + 1, TERMINAL_H - 2, '-');
	}
}

void manager_open(void)
{
	_border_horizontal();
	_border_vertical();
	_render_files(0);
	_render_files(1);
	_help_line();
}

static void manager_home(void)
{
	Side *side = &_sides[_side];
	side->Selected = 0;
	side->Offset = 0;
	_render_files(_side);
}

static void manager_end(void)
{
	Side *side = &_sides[_side];
	side->Selected = side->Count - 1;
	side->Offset = side->Count - MAX_ENTRIES;
	if(side->Offset < 0)
	{
		side->Offset = 0;
	}

	_render_files(_side);
}

static void manager_up(void)
{
	Side *side = &_sides[_side];
	if(side->Selected == 0)
	{
		return;
	}

	--side->Selected;
	if(side->Selected < side->Offset)
	{
		--side->Offset;
	}

	_render_files(_side);
}

static void manager_down(void)
{
	Side *side = &_sides[_side];
	if(side->Selected == side->Count - 1)
	{
		return;
	}

	++side->Selected;
	if(side->Selected >= side->Offset + MAX_ENTRIES)
	{
		++side->Offset;
	}

	_render_files(_side);
}

static void _parent_dir(Side *side, u8r len)
{
	char *p = side->Path + len - 2;
	for(; *p != '/'; --p) {}
	if(p == side->Path)
	{
		*p++ = '/';
	}

	*p = '\0';
}

static void _open_dir(Side *side, DirEnt *ent, u8r len)
{
	char *s = ent->Name;
	char *p = side->Path + len;
	int c;
	u8r i;

	if(len > 1)
	{
		*p++ = '/';
	}

	i = 0;
	while(i < MAX_FILENAME_LEN && (c = s[i++]))
	{
		*p++ = c;
	}

	*p = '\0';
}

static void _open_file(void)
{
#if 0
	editor_load("SOME PATH?");
#endif
}

static void manager_enter(void)
{
	Side *side = &_sides[_side];
	u8r len = strlen(side->Path);
	DirEnt *ent = &side->Entries[side->Selected];
	if(ent->Type)
	{
		if(side->Selected == 0 && len > 1)
		{
			_parent_dir(side, len);
		}
		else if((len + strnlen(ent->Name, MAX_FILENAME_LEN))
			< (sizeof(side->Path) - 1))
		{
			_open_dir(side, ent, len);
		}
	}
	else
	{
		_open_file();
	}

	_render_path(_side);
	_render_files(_side);
}

static void manager_tab(void)
{
	_side = !_side;
	_render_files(0);
	_render_files(1);
}

#if 0
static char *cursel(void)
{
	return "1 file";
}

static void manager_select(void)
{
}

static void manager_copy(void)
{
}

static void manager_move(void)
{
}

static void fs_fail_alert(void)
{
	alert("Failed to create directory:\n"
		"%s\n",
		fatfs_status_str(ret));
}

static void create_dir_callback(int yes, const char *name)
{
	if(yes)
	{
		int ret = f_mkdir(name);
		if(ret)
		{
			fs_fail_alert(ret);
			return;
		}
	}

	mode_set(MODE_MANAGER);
}

static void manager_new_dir(void)
{
	prompt("Create Dir:", create_dir_callback);
}

static int delete_sel(void)
{
	return 1;
}

static void delete_dir_callback(int yes)
{
	if(yes)
	{
		int ret = delete_sel();
		if(ret)
		{
			fs_fail_alert(ret);
			return;
		}
	}

	mode_set(MODE_MANAGER);
}

static void manager_delete(void)
{
	confirm("Delete %s?", delete_dir_callback);
}

static void manager_new_file(void)
{
	prompt("Create File:", new_file_callback);
}
#endif

void manager_key(int key, int c)
{
	switch(key)
	{
	case KEY_HOME:         manager_home();     break;
	case KEY_END:          manager_end();      break;
	case KEY_UP:           manager_up();       break;
	case KEY_DOWN:         manager_down();     break;
	case KEY_RETURN:       manager_enter();    break;
	case KEY_TAB:          manager_tab();      break;
#if 0
	case KEY_INSERT:       manager_select();   break;
	case KEY_F5:           manager_copy();     break;
	case KEY_F6:           manager_move();     break;
	case KEY_F7:           manager_new_dir();  break;
	case KEY_F8:           manager_delete();   break;
	case MOD_CTRL | KEY_N: manager_new_file(); break;
#endif
	}

	(void)c;
}
