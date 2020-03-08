/* bcd.h -- header file for BCD, a CD-ROM audio playing library for DJGPP
   by Brennan Underwood, http://brennan.home.ml.org/ */
#ifndef _BCD_H
#define _BCD_H

#define BCD_VERSION 0x0102

/* Installation and setup functions */
/* Call this first! */
extern int bcd_open(void);
/* Call before exit. */
extern int bcd_close(void);

/* open door, unlocking first if necessary */
extern int bcd_open_door(void);
/* close door */
extern int bcd_close_door(void);

/* pass 1 to lock door, 0 to unlock */
extern int bcd_lock(int);

/* returns one of the following 3 #defined symbols */
extern int bcd_disc_changed(void);
#define BCD_DISC_CHANGED	0xff
#define BCD_DISC_NOT_CHANGED	1
#define BCD_DISC_UNKNOWN	0

/* perform a device reset */
extern int bcd_reset(void);

/* compare the returned status int to the following bits */
extern int bcd_device_status(void);
#define BCD_DOOR_OPEN		1
#define BCD_DOOR_UNLOCKED	2
#define BCD_SUPPORT_COOKED	4
#define BCD_READ_ONLY		8
#define BCD_DATA_READ_ONLY	16
#define BCD_SUPPORT_INTERLEAVE	32

/* returns 1 if audio is currently playing, 0 otherwise. -1 on error */
extern int bcd_audio_busy(void);
/* current head position in frames */
extern int bcd_audio_position(void);
/* convenience function, if audio busy, returns track# playing now */
extern int bcd_now_playing(void);

/* query MSCDEX for track list when disc changed or just starting up */
extern int bcd_get_audio_info(void);
/* get a particular track's info */
extern int bcd_get_track_address(int trackno, int *start, int *len);
/* check for track's audio/data status */
extern int bcd_track_is_audio(int trackno);
/* play a particular track from beginning to end */
extern int bcd_play_track(int tracknum);
/* play an arbitrary section of audio for an arbitrary length of time */
extern int bcd_play(int start, int len);
/* set the output volume. pass a parameter from 0-255 */
extern int bcd_set_volume(int);
/* stop and pause are equivalent */
extern int bcd_stop(void);
#define bcd_pause bcd_stop
extern int bcd_resume(void);

/* Troubleshooting */
/* Returns a human readable description of the last error encountered */
extern char *bcd_error(void);
extern int _error_code;
/* If you are mad enough play the Rach 3, I mean parse _error_code yourself */
#define BCD_DE_WRITE_PROTECT	0
#define BCD_DE_UNKNOWN_UNIT	1
#define BCD_DE_DRIVE_NOT_READY	2
#define BCD_DE_UNKNOWN_COMMAND	3
#define BCD_DE_CRC_ERROR	4
#define BCD_DE_STRUCT_LEN	5
#define BCD_DE_SEEK_ERROR	6
#define BCD_DE_UNKNOWN_MEDIA	7
#define BCD_DE_SECTOR_NOT_FOUND	8
#define BCD_DE_OUT_OF_PAPER	9
#define BCD_DE_WRITE_FAULT	10
#define BCD_DE_READ_FAULT	11
#define BCD_DE_GENERAL_FAILURE	12
#define BCD_DE_INVALID_DISK_CHANGE	15
/* set by BCD itself, for stuff like "Out of memory" */
extern char *_bcd_error;

#endif
