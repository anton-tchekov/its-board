#ifndef __I_SOUND__
#define __I_SOUND__

#include "doomdef.h"
#include "doomstat.h"
#include "sounds.h"

void I_InitSound();
void I_UpdateSound(void);
void I_SubmitSound(void);
void I_ShutdownSound(void);
void I_SetChannels();
int I_GetSfxLumpNum(sfxinfo_t *sfxinfo);
int
I_StartSound
( int		id,
  int		vol,
  int		sep,
  int		pitch,
  int		priority );

void I_StopSound(int handle);
int I_SoundIsPlaying(int handle);
void
I_UpdateSoundParams
( int		handle,
  int		vol,
  int		sep,
  int		pitch );


//
//  MUSIC I/O
//
void I_InitMusic(void);
void I_ShutdownMusic(void);
// Volume.
void I_SetMusicVolume(int volume);
// PAUSE game handling.
void I_PauseSong(int handle);
void I_ResumeSong(int handle);
// Registers a song handle to song data.
int I_RegisterSong(void *data);
// Called by anything that wishes to start music.
//  plays a song, and when the song is done,
//  starts playing it again in an endless loop.
// Horrible thing to do, considering.
void
I_PlaySong
( int		handle,
  int		looping );
// Stops a song over 3 seconds.
void I_StopSong(int handle);
// See above (register), then think backwards
void I_UnRegisterSong(int handle);

#endif
