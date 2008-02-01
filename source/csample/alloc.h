/***
*    Copyright (C) 1995-2000  Dibyendu Majumdar.
*
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software
*    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*    Author : Dibyendu Majumdar
*    Email  : dibyendu@mazumdar.demon.co.uk
*    Website: www.mazumdar.demon.co.uk
*/
/* alloc.h - fast memory allocator      */
/* dibyendu majumdar                    */
/* adapted from code in C++ STL library */

#ifndef alloc_h
#define alloc_h

struct buffer_type_st {
	struct buffer_type_st    *next_buffer;
	char           *buffer;
};
typedef struct buffer_type_st buffer_type_t;

struct link_st {
	struct link_st           *next;
};
typedef struct link_st link_st;

struct allocator_st {
	buffer_type_t  *buffer_list;
	link_st        *free_list;
	char           *next_avail;
	char           *last;
	size_t          size;
	size_t          n;
};
typedef struct allocator_st allocator;

extern allocator *
new_allocator(size_t size, size_t n);

extern void 
grow_allocator(allocator * a);

extern void *
allocate(allocator * a, size_t size);

extern void 
deallocate(allocator * a, void *n);

extern void 
destroy_allocator(allocator * a);

extern void *
allocmem(size_t size);

extern void
freemem(void *p, size_t size);

extern void *
reallocmem(void *p, size_t osize, size_t nsize);

extern void
destroymem(size_t size);

extern void
destroyallmem(void);

extern unsigned long Maxmem;

#endif
