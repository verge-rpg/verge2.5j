/*
VERGE 2.5+j (AKA V2k+j) -  A video game creation engine
Copyright (C) 1998-2000  Benjamin Eirich (AKA vecna), et al
Please see authors.txt for a complete list of contributing authors.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef _MMIO_H_
#define _MMIO_H_

#include <stdio.h>
#include <stdlib.h>
#include "mmtypes.h"

#ifdef MM_BIG_ENDIAN

#define _mm_HI_SLONG(x) ((SLONG *)&x)
#define _mm_LO_SLONG(x) ((SLONG *)&x + 1)
#define _mm_HI_ULONG(x) ((ULONG *)&x)
#define _mm_LO_ULONG(x) ((ULONG *)&x + 1)

#else

#define _mm_HI_SLONG(x) ((SLONG *)&x + 1)
#define _mm_LO_SLONG(x) ((SLONG *)&x)
#define _mm_HI_ULONG(x) ((ULONG *)&x + 1)
#define _mm_LO_ULONG(x) ((ULONG *)&x)

#endif

#ifdef __cplusplus
extern "C" {
#endif


// LOG.C Prototypes
// ================

#define LOG_SILENT   0
#define LOG_VERBOSE  1

extern int  log_init(CHAR *logfile, BOOL val);
extern void log_exit(void);
extern void log_verbose(void);
extern void log_silent(void);
extern void printlog(CHAR *fmt, ... );
extern void printlogv(CHAR *fmt, ... );

#ifdef __WATCOMC__
#pragma aux log_init    parm nomemory modify nomemory; 
#pragma aux log_exit    parm nomemory modify nomemory; 
#pragma aux log_verbose parm nomemory modify nomemory; 
#pragma aux log_silent  parm nomemory modify nomemory; 
#pragma aux printlog    parm nomemory modify nomemory; 
#pragma aux printlogv   parm nomemory modify nomemory; 
#endif



// MikMod's new error handling routines
// ====================================

// Specific Errors [referenced by _mm_errno]

enum
{   MMERR_OPENING_FILE = 1,
    MMERR_OUT_OF_MEMORY,
    MMERR_END_OF_FILE,
    MMERR_DISK_FULL,
    MMERR_SAMPLE_TOO_BIG,
    MMERR_OUT_OF_HANDLES,
    MMERR_ALLOCATING_DMA,
    MMERR_UNKNOWN_WAVE_TYPE,
    MMERR_NOT_A_STREAM,
    MMERR_LOADING_PATTERN,
    MMERR_LOADING_TRACK,
    MMERR_LOADING_HEADER,
    MMERR_LOADING_SAMPLEINFO,
    MMERR_NOT_A_MODULE,
    MMERR_DETECTING_DEVICE,
    MMERR_INVALID_DEVICE,
    MMERR_INITIALIZING_MIXER,
    MMERR_INITIALIZING_DRIVER
};

// Memory allocation with error handling - MMALLOC.C
// =================================================

extern void *_mm_malloc(size_t size);
extern void *_mm_calloc(size_t nitems, size_t size);

extern void (*_mm_errorhandler)(void);
extern int   _mm_errno;
extern BOOL  _mm_critical;
extern CHAR  *_mm_errmsg[];

extern void _mm_RegisterErrorHandler(void (*proc)(void));
extern BOOL _mm_FileExists(CHAR *fname);

extern void StringWrite(CHAR *s, FILE *fp);
extern CHAR *StringRead(FILE *fp);


//  MikMod/DivEnt style file input / output -
//    Solves several portability issues.
//    Notibly little vs. big endian machine complications.

#define _mm_write_SBYTE(x,y)    fputc((int)x,y)
#define _mm_write_UBYTE(x,y)    fputc((int)x,y)

#define _mm_read_SBYTE(x) (SBYTE)fgetc(x)
#define _mm_read_UBYTE(x) (UBYTE)fgetc(x)

#define _mm_write_SBYTES(x,y,z)  fwrite((void *)x,1,y,z)
#define _mm_write_UBYTES(x,y,z)  fwrite((void *)x,1,y,z)
#define _mm_read_SBYTES(x,y,z)   fread((void *)x,1,y,z)
#define _mm_read_UBYTES(x,y,z)   fread((void *)x,1,y,z)

#define _mm_rewind(x) _mm_fseek(x,0,SEEK_SET)


extern int  _mm_fseek(FILE *stream, long offset, int whence);
extern long _mm_iobase_get(void);
extern void _mm_iobase_set(long iobase);
extern void _mm_iobase_setcur(FILE *fp);
extern void _mm_iobase_revert(void);
extern long _mm_ftell(FILE *stream);
extern long _mm_flength(FILE *stream);
extern FILE *_mm_fopen(CHAR *fname, CHAR *attrib);
extern void _mm_fputs(FILE *fp, CHAR *data);
extern BOOL _mm_copyfile(FILE *fpi, FILE *fpo, ULONG len);
extern void _mm_write_string(CHAR *data, FILE *fp);
extern int  _mm_read_string (CHAR *buffer, int number, FILE *fp);


//extern SBYTE _mm_read_SBYTE (FILE *fp);
//extern UBYTE _mm_read_UBYTE (FILE *fp);

extern SWORD _mm_read_M_SWORD (FILE *fp);
extern SWORD _mm_read_I_SWORD (FILE *fp);

extern UWORD _mm_read_M_UWORD (FILE *fp);
extern UWORD _mm_read_I_UWORD (FILE *fp);

extern SLONG _mm_read_M_SLONG (FILE *fp);
extern SLONG _mm_read_I_SLONG (FILE *fp);

extern ULONG _mm_read_M_ULONG (FILE *fp);
extern ULONG _mm_read_I_ULONG (FILE *fp);


//extern int _mm_read_SBYTES    (SBYTE *buffer, int number, FILE *fp);
//extern int _mm_read_UBYTES    (UBYTE *buffer, int number, FILE *fp);

extern int _mm_read_M_SWORDS  (SWORD *buffer, int number, FILE *fp);
extern int _mm_read_I_SWORDS  (SWORD *buffer, int number, FILE *fp);

extern int _mm_read_M_UWORDS  (UWORD *buffer, int number, FILE *fp);
extern int _mm_read_I_UWORDS  (UWORD *buffer, int number, FILE *fp);

extern int _mm_read_M_SLONGS  (SLONG *buffer, int number, FILE *fp);
extern int _mm_read_I_SLONGS  (SLONG *buffer, int number, FILE *fp);

extern int _mm_read_M_ULONGS  (ULONG *buffer, int number, FILE *fp);
extern int _mm_read_I_ULONGS  (ULONG *buffer, int number, FILE *fp);


//extern void _mm_write_SBYTE     (SBYTE data, FILE *fp);
//extern void _mm_write_UBYTE     (UBYTE data, FILE *fp);

extern void _mm_write_M_SWORD   (SWORD data, FILE *fp);
extern void _mm_write_I_SWORD   (SWORD data, FILE *fp);

extern void _mm_write_M_UWORD   (UWORD data, FILE *fp);
extern void _mm_write_I_UWORD   (UWORD data, FILE *fp);

extern void _mm_write_M_SLONG   (SLONG data, FILE *fp);
extern void _mm_write_I_SLONG   (SLONG data, FILE *fp);

extern void _mm_write_M_ULONG   (ULONG data, FILE *fp);
extern void _mm_write_I_ULONG   (ULONG data, FILE *fp);

//extern void _mm_write_SBYTES    (SBYTE *data, int number, FILE *fp);
//extern void _mm_write_UBYTES    (UBYTE *data, int number, FILE *fp);

extern void _mm_write_M_SWORDS  (SWORD *data, int number, FILE *fp);
extern void _mm_write_I_SWORDS  (SWORD *data, int number, FILE *fp);

extern void _mm_write_M_UWORDS  (UWORD *data, int number, FILE *fp);
extern void _mm_write_I_UWORDS  (UWORD *data, int number, FILE *fp);

extern void _mm_write_M_SLONGS  (SLONG *data, int number, FILE *fp);
extern void _mm_write_I_SLONGS  (SLONG *data, int number, FILE *fp);

extern void _mm_write_M_ULONGS  (ULONG *data, int number, FILE *fp);
extern void _mm_write_I_ULONGS  (ULONG *data, int number, FILE *fp);

#ifdef __WATCOMC__
#pragma aux _mm_fseek      parm nomemory modify nomemory
#pragma aux _mm_ftell      parm nomemory modify nomemory
#pragma aux _mm_flength    parm nomemory modify nomemory
#pragma aux _mm_fopen      parm nomemory modify nomemory
#pragma aux _mm_fputs      parm nomemory modify nomemory
#pragma aux _mm_copyfile   parm nomemory modify nomemory
#pragma aux _mm_iobase_get parm nomemory modify nomemory
#pragma aux _mm_iobase_set parm nomemory modify nomemory
#pragma aux _mm_iobase_setcur parm nomemory modify nomemory
#pragma aux _mm_iobase_revert parm nomemory modify nomemory
#pragma aux _mm_write_string  parm nomemory modify nomemory
#pragma aux _mm_read_string   parm nomemory modify nomemory

#pragma aux _mm_read_M_SWORD parm nomemory modify nomemory; 
#pragma aux _mm_read_I_SWORD parm nomemory modify nomemory; 
#pragma aux _mm_read_M_UWORD parm nomemory modify nomemory; 
#pragma aux _mm_read_I_UWORD parm nomemory modify nomemory; 
#pragma aux _mm_read_M_SLONG parm nomemory modify nomemory; 
#pragma aux _mm_read_I_SLONG parm nomemory modify nomemory; 
#pragma aux _mm_read_M_ULONG parm nomemory modify nomemory; 
#pragma aux _mm_read_I_ULONG parm nomemory modify nomemory; 

#pragma aux _mm_read_M_SWORDS parm nomemory modify nomemory; 
#pragma aux _mm_read_I_SWORDS parm nomemory modify nomemory; 
#pragma aux _mm_read_M_UWORDS parm nomemory modify nomemory; 
#pragma aux _mm_read_I_UWORDS parm nomemory modify nomemory; 
#pragma aux _mm_read_M_SLONGS parm nomemory modify nomemory; 
#pragma aux _mm_read_I_SLONGS parm nomemory modify nomemory; 
#pragma aux _mm_read_M_ULONGS parm nomemory modify nomemory; 
#pragma aux _mm_read_I_ULONGS parm nomemory modify nomemory; 

#pragma aux _mm_write_M_SWORD parm nomemory modify nomemory; 
#pragma aux _mm_write_I_SWORD parm nomemory modify nomemory; 
#pragma aux _mm_write_M_UWORD parm nomemory modify nomemory; 
#pragma aux _mm_write_I_UWORD parm nomemory modify nomemory; 
#pragma aux _mm_write_M_SLONG parm nomemory modify nomemory; 
#pragma aux _mm_write_I_SLONG parm nomemory modify nomemory; 
#pragma aux _mm_write_M_ULONG parm nomemory modify nomemory; 
#pragma aux _mm_write_I_ULONG parm nomemory modify nomemory; 

#pragma aux _mm_write_M_SWORDS parm nomemory modify nomemory; 
#pragma aux _mm_write_I_SWORDS parm nomemory modify nomemory; 
#pragma aux _mm_write_M_UWORDS parm nomemory modify nomemory; 
#pragma aux _mm_write_I_UWORDS parm nomemory modify nomemory; 
#pragma aux _mm_write_M_SLONGS parm nomemory modify nomemory; 
#pragma aux _mm_write_I_SLONGS parm nomemory modify nomemory; 
#pragma aux _mm_write_M_ULONGS parm nomemory modify nomemory; 
#pragma aux _mm_write_I_ULONGS parm nomemory modify nomemory; 
#endif


#ifndef __WATCOMC__
#ifndef __GNUC__
#ifndef WIN32
extern CHAR *strdup(CHAR *str);
#endif
#endif
#endif

#ifdef __cplusplus
};
#endif

#endif
