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

// vbe2.cc

/*
	coded by Charles Rector AKA aen
	last updated: 25sep99
*/

// TODO: replace w/ the smaller interface
#include <i86.h>
#include <stdio.h>
//#include <malloc.h>

#include "verge.h"

#include "vbe2.h"
//#include "dpmi.h"
#include "linked.h"

int vbecontroller_t::instances = 0;
vbecontroller_t vbecontroller;

static int dpmi_int(char intno, RM_REGS* in)
{
	REGS r;

	r.w.ax   = 0x300;
	r.h.bl   = intno;
	r.h.bh   = 0;
	r.w.cx   = 0;
	r.x.edi  = FP_OFF(in);
	int386(0x31, &r, &r);

	return !r.w.cflag;
}

// recommend local instances only; try to keep
// low memory free when it is not needed.
class dosheap_t
{
public:
	unsigned short selector, segment;
	void* data;

	dosheap_t()
	{
	#ifndef __DJGPP__
		union REGS r;

		r.w.ax = 0x0100;
		r.w.bx = (4096/16)+1;
		int386(0x31, &r, &r);
		selector = r.w.dx;
		segment = r.w.ax;

		r.w.ax = 0x0006;
		r.w.bx = selector;
		int386(0x31, &r, &r);
		data = (void*)(((unsigned int)r.w.cx<<16)+r.w.dx);
	#endif
	}
	~dosheap_t()
	{
	#ifndef __DJGPP__
		union REGS r;

		r.w.ax = 0x0101;
		r.w.dx = selector;
		int386(0x31, &r, &r);
	#endif
	}
};

struct vbemodeext_t
	: public linked_node
{
	vbemode_t mode;
	int compare(void* c)
	{
		return (((vbemodeext_t *)c)->mode.mode_number > mode.mode_number);
	}
	vbemodeext_t(vbemode_t* newmode)
	{
		if (newmode)
			mode=*newmode;
	}
};

// determine if VESA exists; if so, F_vbeinfo holds relevant info
void vbecontroller_t::getinfo(dosheap_t& dosheap)
{
	// Get VBE info
/*
#ifdef __DJGPP__
	__dpmi_regs r;
	r.x.ax = 0x4f00;
	r.x.di = 0;
	r.x.es = __tb / 16;
	__dpmi_int(0x10, &r);
	dosmemget(__tb, sizeof(F_vbeinfo), &F_vbeinfo);
		F_vbepresent = (0x004f == r.x.ax);
#else
*/
	RM_REGS r;
	_fmemset(dosheap.data, 0, sizeof(F_vbeinfo));
	_fmemcpy(dosheap.data, "VBE2", 4); // hmm
	memset(&r, 0, sizeof(r));
	r.eax = 0x00004f00;
	r.edi = 0;
	r.es = dosheap.segment;
	dpmi_int(0x10, &r);
	_fmemcpy(&F_vbeinfo, dosheap.data, sizeof(F_vbeinfo));

	F_vbepresent = (0x004f == r.eax);
	F_vbepresent = !strncmp((char *)F_vbeinfo.signature, "VESA", 4); // hmm
//#endif
}

// determine if VESA mode 'n' exists; if so, F_vbemode holds relevant info
int vbecontroller_t::getmodeinfo(dosheap_t& dosheap, int n)
{
#ifdef __DJGPP__
	__dpmi_regs r;
	r.x.ax = 0x4f01;
	r.x.cx = n;
	r.x.es = __tb / 16;
	r.x.di = 0;
	__dpmi_int(0x10, &r);
	dosmemget(__tb, sizeof(vbemode_t), &F_vbemode);

	return (0x004f == (r.x.ax & 0xffff));
#else
	RM_REGS r;
	_fmemset(dosheap.data, 0, sizeof(vbemode_t));
	memset(&r, 0, sizeof(r));
	r.eax   = 0x00004f01;
	r.ecx   = n;
	r.edi   = 0;
	r.es    = dosheap.segment;
	dpmi_int(0x10, &r);
	_fmemcpy(&F_vbemode, dosheap.data, sizeof(vbemode_t));
	F_vbemode.mode_number = n;

	return (0x004f == (r.eax & 0xffff));
#endif
}

// search for a mode that matches the given criteria (within pregenerated mode list)
// returns the mode number on success; -1 otherwise
int vbecontroller_t::findmode(int xres, int yres, int bpp)
{
	int n = 0;
	F_modelist.go_head();
	do	{
		vbemode_t* mode = &((vbemodeext_t *)F_modelist.current())->mode;
		if (xres == mode->xres && yres == mode->yres && bpp == mode->bits_per_pixel)
		{
			V_memcpy(&F_vbemode, mode, sizeof(vbemode_t));
			return n;
		}
		F_modelist.go_next();
		n++;
	} while (F_modelist.current() != F_modelist.head());
	return -1;
}

// empties the mode list, destroying all contents
void vbecontroller_t::clearmodelist()
	{
		while (F_modelist.head()) {
			vbemodeext_t* v = (vbemodeext_t *)F_modelist.head();
			F_modelist.unlink((linked_node *)v);
			delete v;
		}
	}

void* vbecontroller_t::lfbmap(void* raw)
	{
		unsigned int memory = 1024*1024*8;
		unsigned int mapped = NULL;

	#ifdef __DJGPP__
		union REGS r;
		r.d.eax = 0x0800;
		r.d.ebx = ((unsigned int)(raw)) >> 16;
		r.d.ecx = ((unsigned int)(raw)) & 0xffff;
		r.x.si  = memory >> 16;
		r.x.di  = memory & 0xffff;
		int386(0x31, &r, &r);

		mapped = r.x.cx + (r.x.bx<<16);
		mapped += __djgpp_conventional_base;
	#else
		union REGS r;
		r.x.eax = 0x0800;
		r.x.ebx = ((unsigned int)(raw)) >> 16;
		r.x.ecx = ((unsigned int)(raw)) & 0xffff;
		r.w.si  = (unsigned short)(memory >> 16);
		r.w.di  = (unsigned short)(memory & 0xffff);
		int386(0x31, &r, &r);

		mapped = r.w.cx + (r.w.bx<<16);
	#endif

		return (void*)mapped;
	}
	void vbecontroller_t::lfbrelease(void* mapped)
	{
		union REGS r;

	#ifdef __DJGPP__
		r.d.eax = 0x801;
		r.d.ebx = ((unsigned int)(mapped)) >> 16;
		r.d.ecx = ((unsigned int)(mapped)) & 0xffff;
	#else
		r.x.eax = 0x801;
		r.x.ebx = ((unsigned int)(mapped)) >> 16;
		r.x.ecx = ((unsigned int)(mapped)) & 0xffff;
	#endif

		int386(0x31, &r, &r);
	}

// constructor
vbecontroller_t::vbecontroller_t()
	: lfb(NULL)
	{
		if (instances++)
			Sys_Error("vbecontroller_t: attempted to instantiate more than one VBE controller object.");

		dosheap_t dosheap;

	// will determine existence of VESA, among other things
		getinfo(dosheap);
	// now attempt to gather modes; if VESA doesn't exist, this will have no effect
		findallmodes(dosheap);
	}
// destructor
vbecontroller_t::~vbecontroller_t()
	{
		if (lfb)
			lfbrelease(lfb);
		lfb = NULL;
	}

// (re)generates the mode list; needs a dosheap_t instance to work w/
void vbecontroller_t::findallmodes(dosheap_t& dosheap)
	{
	// ignore requests when VESA is not available
		if (!F_vbepresent) return;

		clearmodelist();
		for (int n=0; n<0x3ff; n++)
		{
			if (getmodeinfo(dosheap, n))
				F_modelist.insert_tail((linked_node*)new vbemodeext_t(&F_vbemode));
		}
	}

void* vbecontroller_t::setmode(int xres, int yres, int bpp)
	{
	// release previous lfb, if any
		if (lfb)
			lfbrelease(lfb);
		lfb = NULL;

	// not found
		if (-1 == findmode(xres, yres, bpp))
			return NULL;
	// not LFB
		if (!(F_vbemode.mode_attributes & 0x80))
			return NULL;

	// whee!
		union REGS r;
		r.w.ax = 0x4f02;
		r.w.bx = (unsigned short)(F_vbemode.mode_number | 0x4000);
		int386(0x10, &r, &r);

	// now map it in
		lfb = lfbmap((void *)F_vbemode.phys_base_ptr);

		return lfb;
	}