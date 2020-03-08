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
#include "bcd.h"
#include "cdrom.h"
#include "verge.h"

void CD_Stop_Audio (void)
{
}

void CD_Resume_Audio (void)
{
}

void CD_Play_Audio (unsigned long Begin, unsigned long End)
{
}

void CD_DeInit (void)
{
  bcd_close();
}

void CD_Init (void)
{
  if (!nocdaudio) bcd_open();
}

void CD_Stop()
{
        bcd_stop();
}

void CD_Close_Door()
{
      bcd_close_door();
}

void CD_Open_Door()
{
      bcd_open_door();
}

void CD_Play(int track)
{
      if (bcd_audio_busy()) {
        bcd_stop();
        delay(500);
      }
      bcd_play_track(track);
}
