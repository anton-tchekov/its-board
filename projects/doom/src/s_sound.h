#ifndef __S_SOUND__
#define __S_SOUND__

void
S_Init
( int		sfxVolume,
  int		musicVolume );

void S_Start(void);

void
S_StartSound
( void*		origin,
  int		sound_id );

void
S_StartSoundAtVolume
( void*		origin,
  int		sound_id,
  int		volume );


// Stop sound for thing at <origin>
void S_StopSound(void* origin);


// Start music using <music_id> from sounds.h
void S_StartMusic(int music_id);

// Start music using <music_id> from sounds.h,
//  and set whether looping
void
S_ChangeMusic
( int		music_id,
  int		looping );

// Stops the music fer sure.
void S_StopMusic(void);

// Stop and resume music, during game PAUSE.
void S_PauseSound(void);
void S_ResumeSound(void);


//
// Updates music & sounds
//
void S_UpdateSounds(void* listener);

void S_SetMusicVolume(int volume);
void S_SetSfxVolume(int volume);


#endif

