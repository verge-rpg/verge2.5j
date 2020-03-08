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

// ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
// ³                          The VERGE Engine                           ³
// ³              Copyright (C)1998 BJ Eirich (aka vecna)                ³
// ³                 Font Loading / Text Output module                   ³
// ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

/*
	mod log:

	<aen>
	30	December	1999	Major revamp.
*/

#include "verge.h"

class font_t
{
private:
	string_t	F_filename;
// font cell dimensions; currently all fonts are monospace
	int		F_cellwidth, F_cellheight;

// total sets within this font
	int		F_subsetcount;
// currently selected subset
	int		F_selected;

// holds font image data
	memorystream_t F_data;

// helpers
	int loadheader(VFILE* vfp);
public:
	int vacant() const { return (F_data.getsize() < 1); }

	void loadbiosfont();
// vacates this font; frees image data, empties filename string
	void clear()
	{
		F_filename="";
		F_data.clear();
	}

// accessors
	const string_t& getfilename() const { return F_filename; }
	int getheight() const { return F_cellheight; }
	int getwidth() const { return F_cellwidth; }
	int getsubsetcount() const { return F_subsetcount; }

// constructors
	font_t()
	{ }
	font_t(const char* filename)
	{
		loadfromfile(filename);
	}

// primary interface

	int loadfromfile(const char* filename);

	int selected() const { return F_selected; }
	void select(int subset)
	{
		if (subset < 0 || subset >= F_subsetcount)
			return;
		F_selected = subset;
	}

	void paintchar(const char ch);
	void paintstring(const char* zstr, int imbed = 0);
};

#define MAX_FONTS 10
class fontcontroller_t
{
private:
// actual fonts
	font_t F_fonts[MAX_FONTS];
// for invalid requests; BIOS font
	static font_t F_dummy;

public:
// default constructor
	fontcontroller_t()
	{
		if (F_dummy.vacant())
			F_dummy.loadbiosfont();
	}
	void load(int slot, const char* filename)
	{
	// invalid slot
		if (slot < 0 || slot >= MAX_FONTS)
			return;
	// invalid name
		if (!filename)
			return;

		F_fonts[slot].loadfromfile(filename);
	}

// accessors
	font_t& getfont(int slot)
	{
	// invalid requests get the dummy
		if (slot < 0 || slot >= MAX_FONTS)
			return F_dummy;
	// valid requests are fulfilled
		return F_fonts[slot];
	}
};

// DATA ////////////////////////////////////////////////////////////////////////////////////////////

static font_t fontcontroller_t::F_dummy;
fontcontroller_t fontcontroller;

// global font coordinates & misc
static int font_x, font_y, font_alignx;

// CODE ////////////////////////////////////////////////////////////////////////////////////////////

void font_t::paintchar(const char ch)
{
// validate character range
	if (ch < 32 && ch >= 127)
		return;

// convert to font bay character offset
	int offset	= (F_selected*96*F_cellwidth*F_cellheight) + ((ch-32)*F_cellwidth*F_cellheight);
// move there
	F_data.setposition(offset*(hicolor?2:1));

// paint the character
	LFB_Blit(font_x, font_y, F_cellwidth, F_cellheight,
		(unsigned char*)F_data.getposdata(), 1, 0);
}

void font_t::paintstring(const char* zstr, int imbed)
{
	while (*zstr)
	{
		const char ch = *zstr++;

	// i don't likes it. implement font selection some other way.
		switch (ch)
		{
			case 126: select(0); continue;
			case 128: select(1); continue;
			case 129: select(2); continue;
			case 130: select(3); continue;
		}

	// allow tab and newlines codes to have meaning?
		if (imbed)
		{
			if ('\t' == ch)
			{
				int chx = (font_x - font_alignx)/F_cellwidth;
				font_x += (4 - (chx % 4))*F_cellwidth;
				continue;
			}
			else if ('\n' == ch)
			{
				font_y += F_cellheight;
				font_x  = font_alignx;
				continue;
			}
		}

		paintchar(ch);
		font_x += F_cellwidth;
	}
}

void font_t::loadbiosfont()
{
	int		n, mask, count;
	char*	bf;
	char*	ptr;

// header info
	F_cellwidth = 8;
	F_cellheight = 8;
	F_subsetcount = 1;
	F_selected = 0;
// get enough room for BIOS font
	if (!F_data.setsize(8*8*96))
	{
		Sys_Error("font_t::loadbiosfont: memory exhausted");
	}

// address of BIOS font in system memory
	bf = (char *) 0xffa6e + (8*32);
// bend rules a little; alias start of data for writing
	ptr = (char *)F_data.getdata();

// expand the bit-packed 8x8 BIOS font into our data pointer
	for (n = 0; n < 96; n += 1)
	{
		mask = 0x80;
		count = 8*8;
		do
		{
			if (*bf & mask)
				*ptr = 31;
			else
				*ptr = 0;

			ptr += 1;
			mask >>= 1;
			if (!mask)
			{
				mask = 0x80;
				bf += 1;
			}
			count -= 1;
		}
		while (count);
	}
}

int font_t::loadheader(VFILE* vfp)
{
	if (!vfp)
	// failure
		return 0;

	long ver = vgetc(vfp);
	if (ver != 1)
	{
		vclose(vfp);
		Sys_Error("font_t::loadheader: %s: incorrect version number (reported %d).\n",
			(const char*)getfilename(), ver);
	}

// set font dims
	F_cellwidth = vgetw(vfp);
	if (F_cellwidth < 1 || F_cellwidth > 128)
	{
		vclose(vfp);
		Sys_Error("font_t::loadheader: %s: bogus cell-width (reported %d).\n",
			(const char*)getfilename(), F_cellwidth);
	}
	F_cellheight = vgetw(vfp);
	if (F_cellheight < 1 || F_cellheight > 128)
	{
		vclose(vfp);
		Sys_Error("font_t::loadheader: %s: bogus cell-length (reported %d).\n",
			(const char*)getfilename(), F_cellheight);
	}

// set subsets
	F_subsetcount = vgetw(vfp);
	F_selected = 0;
	if (F_subsetcount < 1 || F_subsetcount > 4)
	{
		vclose(vfp);
		Sys_Error("font_t::loadheader: %s: bogus subset count (reported %d).\n",
			(const char*)getfilename(), F_subsetcount);
	}
// success
	return 1;
}

int font_t::loadfromfile(const char* filename)
{
// set filename; stuff relies on this for error messages
	F_filename = filename;

	VFILE* f = vopen((const char*)getfilename());
	if (!f)
	{
		Log(va("font_t::loadfromfile: %s: unable to open", (const char*)getfilename()));
	// failture
		return 0;
	}
	else
	{
	// get header info and setup some defaults
	// -- ignore return value for now; f is gauranteed to exist anyway
		loadheader(f);

	// get cumulative size of all subsets in this font & resize data to match
		F_data.setsize(F_subsetcount*96*(F_cellwidth*F_cellheight));
	// bend the rules and read in the font
		vread((char*)F_data.getdata(), F_data.getsize(), f);

		vclose(f);
	}

	if (hicolor)
	{
	// make a temp copy of 8-bit image
		memorystream_t save;
		save.write(F_data, F_data.getsize());
	// clear & resize destination for hicolor image
		F_data.clear();
		F_data.setsize(sizeof(unsigned short)*save.getsize());
	// bend the rules so we can do this a little bit faster
		unsigned char* saveptr = (unsigned char*)save.getdata();
		unsigned short* newptr = (unsigned short*)F_data.getdata();
		for (int n = 0; n < save.getsize(); n += 1)
		{
			newptr[n]
				= (unsigned short)( (saveptr[n])
				? (LoToHi(saveptr[n]))
				: ((16 == hicolor) ? (0xF81F) : (0x7C1F)) );
		}
	}
// success
	return 1;
}

// wrapper routines for font-related classes

void Font_Print(int slot, const char* zstr)
{
	fontcontroller.getfont(slot).paintstring(zstr);
}

void Font_PrintImbed(int slot, const char* zstr)
{
	fontcontroller.getfont(slot).paintstring(zstr, 1);
}

int  Font_GetWidth(int slot)
{
	return fontcontroller.getfont(slot).getwidth();
}

int  Font_GetLength(int slot)
{
	return fontcontroller.getfont(slot).getheight();
}

int  Font_Load(const char* filename)
{
	int n;
	for (n=0; n < MAX_FONTS; n++)
	{
		if (fontcontroller.getfont(n).vacant())
		// if we fail, return a bogus slot; will equate to usage of 'dummy'
			if (!fontcontroller.getfont(n).loadfromfile(filename))
				return -1;
		// we succeeded, break out
			else
				break;
	}
// return slot of font we loaded; if no free slots found,
// we'll be returning a bogus slot anyway (total slots + 1),
// which equates to usage of 'dummy'
	return n;
}

// global positioning routines
// *** in the future, make this relative
// *** to a main viewport canvas or something

void Font_GotoXY(int x, int y)
{
	font_x = font_alignx = x;
	font_y = y;
}

int Font_GetX() { return font_x; }
int Font_GetY() { return font_y; }