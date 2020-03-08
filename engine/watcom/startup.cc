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

// startup.c
// Copyright (C) 1998 BJ Eirich
// This shouldn't really be platform-dependent, but it's put in the platform
// dependent files because of Windows. :P


#include <stdarg.h> // va_*()
#include <conio.h>	//getch()
#include <time.h>
#include <stdlib.h>

#define MAIN_H
#include "verge.h"

extern void vmain(int argc, char *argv[]);
extern void Log(const char* message);
extern void Logp(const char* message);

// ================================= Code ====================================

void V_memset (void *dest, int fill, int count)
{
	int             i;

	/*
	if ( (((long)dest | count) & 3) == 0)
	{
		count >>= 2;
		fill = fill | (fill<<8) | (fill<<16) | (fill<<24);
		for (i=0 ; i<count ; i++)
			((int *)dest)[i] = fill;
	}
	else
	*/
		for (i=0 ; i<count ; i++)
			((byte *)dest)[i] = (byte)fill;
}

void V_memcpy (void *dest, const void *src, int count)
{
	int             i;

	if (( ( (long)dest | (long)src | count) & 3) == 0 )
	{
		count>>=2;
		for (i=0 ; i<count ; i++)
			((int *)dest)[i] = ((int *)src)[i];
	}
	else
		for (i=0 ; i<count ; i++)
			((byte *)dest)[i] = ((byte *)src)[i];
}

int V_memcmp (const void *m1, const void *m2, int count)
{
	while(count)
	{
		count--;
		if (((byte *)m1)[count] != ((byte *)m2)[count])
			return -1;
	}
	return 0;
}

void V_strcpy (char *dest, const char *src)
{
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest++ = 0;
}

void V_strncpy(char* dest, const char* src, int count)
{
	while (*src && count--)
	{
		*dest++ = *src++;
	}
	if (count)
		*dest++ = 0;
}

int V_strlen (const char *str)
{
	int             count;

	count = 0;
	while (str[count])
		count++;

	return count;
}

void V_strcat (char *dest, char *src)
{
	dest += V_strlen(dest);
	V_strcpy (dest, src);
}

int V_strcmp(const char* s1, const char* s2)
{
	while (1)
	{
		if (*s1 < *s2)
			return -1;              // strings not equal
		if (*s1 > *s2)
			return +1;
		if (!*s1)
			return 0;               // strings are equal
		s1++;
		s2++;
	}

	//return 666;
}

int V_atoi (const char *str)
{
	int             val;
	int             sign;
	int             c;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else
		sign = 1;

	val = 0;

//
// check for hex
//
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X') )
	{
		str += 2;
		while (1)
		{
			c = *str++;
			if (c >= '0' && c <= '9')
				val = (val<<4) + c - '0';
			else if (c >= 'a' && c <= 'f')
				val = (val<<4) + c - 'a' + 10;
			else if (c >= 'A' && c <= 'F')
				val = (val<<4) + c - 'A' + 10;
			else
				return val*sign;
		}
	}

//
// check for character
//
	if (str[0] == '\'')
	{
		return sign * str[1];
	}

//
// assume decimal
//
	while (1)
	{
		c = *str++;
		if (c <'0' || c > '9')
			return val*sign;
		val = val*10 + c - '0';
	}

	//return 0;
}


float V_atof (const char *str)
{
	double			val;
	int             sign;
	int             c;
	int             decimal, total;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else
		sign = 1;

	val = 0;

//
// check for hex
//
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X') )
	{
		str += 2;
		while (1)
		{
			c = *str++;
			if (c >= '0' && c <= '9')
				val = (val*16) + c - '0';
			else if (c >= 'a' && c <= 'f')
				val = (val*16) + c - 'a' + 10;
			else if (c >= 'A' && c <= 'F')
				val = (val*16) + c - 'A' + 10;
			else
				return val*sign;
		}
	}

//
// check for character
//
	if (str[0] == '\'')
	{
		return sign * str[1];
	}

//
// assume decimal
//
	decimal = -1;
	total = 0;
	while (1)
	{
		c = *str++;
		if (c == '.')
		{
			decimal = total;
			continue;
		}
		if (c <'0' || c > '9')
			break;
		val = val*10 + c - '0';
		total++;
	}

	if (decimal == -1)
		return val*sign;
	while (total > decimal)
	{
		val /= 10;
		total--;
	}

	return val*sign;
}

char* va(char* format, ...)
{
	va_list argptr;
	static char string[1024];

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);

	return string;
}

void Sys_Error(const char* format, ...)
{
	va_list argptr;
	static char string[1024];

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);

	// <aen> why can't I nest another va() in here?
	Logp("Sys: Exiting with message: ");
	Log(string);

	ShutdownTimer();
	ShutdownKeyboard();
        //CD_Deinit();
	MD_PlayStop();
	MD_Exit();

	if (ShutdownVideo)
		ShutdownVideo(1);

	printf(string);
	fflush(stdout);

	exit(-1);
}

int sgn(int x)
{
  if (x>0)
  	return 1;
  else if (x<0)
  	return -1;
  return 0;
}

void Conlib_Ver_Watcom()
{
	Console_Printf("{|||||||||||||||||}");
	Console_Printf(va("VERGE v.%s Copyright (C)1998 vecna", VERSION));
	Console_Printf("All rights reserved. Watcom/DOS build.");
	Console_Printf(va("Timestamp %s at %s.",__DATE__, __TIME__));
	Console_Printf("Options: -oneatx -zp4 -5 -j -wx -d0");
	Console_Printf("{|||||||||||||||||}");
	Console_Printf("");
}

void CheckMessages()
{
  // Win95 can bite me.
}

//#include "dpmi.h"
#include "vbe2.h"
int main(int argc, char *argv[]) {
	if (1 == argc) {
		printf("VERGE v.%s build %s at %s. \n",VERSION,__DATE__,__TIME__);
		printf("Copyright (C)1998 vecna \n");
		delay(500);
	}

	//printf("entity struct size=%d\n", sizeof(entity)/256);
	//fflush(stdout);

	srand(time(NULL));

	if (!vbecontroller.vesaexists())
		printf("VESA 2.0 or better does not exist. Extended resolutions will be unavailable.\n");
	else {
		printf("VESA 2.0 support available!\n");
		printf("Searching for suitable modes...\n");
	}
	delay(500);

	vmain(argc, argv);
	return 0;
}
