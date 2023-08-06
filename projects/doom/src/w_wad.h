#ifndef __W_WAD__
#define __W_WAD__

typedef struct
{
	char identification[4];
	int numlumps;
	int infotableofs;
} wadinfo_t;

typedef struct
{
	int filepos;
	int size;
	char name[8];
} filelump_t;

typedef struct
{
	char name[8];
	int handle;
	int position;
	int size;
} lumpinfo_t;

extern	void**		lumpcache;
extern	lumpinfo_t*	lumpinfo;
extern	int		numlumps;

void    W_InitMultipleFiles (char** filenames);
void    W_Reload (void);

int W_CheckNumForName (char* name);
int W_GetNumForName (char* name);

int W_LumpLength (int lump);
void W_ReadLump (int lump, void *dest);

void *W_CacheLumpNum (int lump, int tag);
void *W_CacheLumpName (char* name, int tag);

#endif
