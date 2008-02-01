/***
*    Copyright (C) 1999,2000  Dibyendu Majumdar.
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
/* 
* alloc.c - a fast memory allocator 
*
* dibyendu majumdar                 
* adapted from code in C++ STL lib 
*
* This file implements a fast fixed size memory allocator. It is very useful
* for situations where you need to allocate and free fixed size memory chunks
* fairly frequently.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "alloc.h"

void *safe_malloc(size_t n) {
	void *ptr = calloc(1, n);
	if (ptr == 0) {
		perror("calloc");
		fprintf(stderr, "Failed to allocate memory\n");
		exit(1);
	}
	return ptr;
}
#define safe_free free

#define CHUNKSIZE       16
#define BLOCKSIZE       8192
#define ROUND(x,y)      ((((x)+((y)-1))/(y))*(y))
#define MAXALLOCATORS   100

typedef allocator *allocatorp;
static allocatorp Aa[MAXALLOCATORS];
unsigned long Maxmem;		/* TODO: Get rid of this */

/***
*	FUNCTION	: new_allocator
*
*	DESCRIPTION	: create a new allocator object
*
*	ARGS		: size_t [IN] - object size
*			: size_t [IN] - no of objects to be allocated initially
*
*	RETURNS		: ptr to newly created object
*/
allocator      *
new_allocator(size_t size, size_t n)
{
	allocator *a = (allocator *) safe_malloc(sizeof(allocator));
	a->buffer_list = 0;
	a->free_list = 0;
	a->next_avail = 0;
	a->last = 0;
	a->size = size;
	a->n = n;
	return a;
}

/***
*
*	FUNCTION	: grow_allocator
*
*	DESCRIPTION	: grows an allocator object 
*
*	ARGS		: allocator * - ptr to allocator object
*
*	RETTURNS	:
*/
void 
grow_allocator(allocator * a)
{
	size_t size = (a->size == 0 ? 1024 : a->size);
	buffer_type_t *tmp = (buffer_type_t *) safe_malloc(sizeof(buffer_type_t));
	tmp->buffer = (char *) safe_malloc(size * a->n);
	tmp->next_buffer = a->buffer_list;
	a->buffer_list = tmp;
	a->next_avail = a->buffer_list->buffer;
	a->last = a->next_avail + (size * a->n);
	Maxmem += (size*a->n);
}

/***
*
*	FUNCTION	: allocate
*
*	DESCRIPTION	: allocates memory using an allocator
*
*	ARGS		: allocator *
*
*	RETURNS		: void * - ptr to memory allocated
*/
void           *
allocate(allocator * a, size_t size)
{
	link_st           *tmp = a->free_list;

	if (a->free_list) {
		a->free_list = (link_st *) (a->free_list->next);
		return (void *) tmp;
	} else {
		if (a->size > 0)
			size = a->size;
		if (a->next_avail+size > a->last) {
			grow_allocator(a);
		} 
		{
			void *tmp = (void *) a->next_avail;
			a->next_avail += size;
			assert(a->next_avail <= a->last);
			return tmp;
		}
	}
}

/***
*
*	FUNCTION	: deallocate
*
*	DESCRIPTION	: free memory allocated by allocate
*
*	ARGS		: allocator *
*			: void * - ptr to memory to be freed
*
*	RETURNS		: 
*/
void 
deallocate(allocator * a, void *n)
{
	if (a->size == 0) return;
	((link_st *) n)->next = a->free_list;
	a->free_list = (link_st *) n;
}

/***
*	FUNCTION	: destroy_allocator
*
*	DESCRIPTION	: free memory associated with an allocator
*
*	ARGS		: allocator *
*
*	RETURNS		:
*/
void 
destroy_allocator(allocator * a)
{
	size_t size = (a->size == 0 ? 1024 : a->size);
	while (a->buffer_list) {
		buffer_type_t    *tmp = a->buffer_list;
		a->buffer_list = a->buffer_list->next_buffer;
		safe_free(tmp->buffer);
		safe_free(tmp);
		Maxmem -= (size*a->n);
	}
	safe_free(a);
}

void *
allocmem(size_t size)
{
	unsigned int i;
	size_t asize;
	void *p;

	/* fprintf(stderr, "allocating %lu bytes\n", size); */
	asize = ROUND(size, CHUNKSIZE);
	i = (asize/CHUNKSIZE)-1;
	if (i >= MAXALLOCATORS) {
		Maxmem += size;
		return safe_malloc(size);
	}
	if (Aa[i] == 0) {
		size_t blocksize = ROUND(BLOCKSIZE, asize);
		assert(blocksize >= BLOCKSIZE && blocksize <= BLOCKSIZE*2);
		Aa[i] = new_allocator(asize, blocksize/asize);
		assert(Aa[i] != 0);
		/* fprintf(stderr, "allocator # %d created\n", i); */
	}
	assert(Aa[i]->size == asize);
	p = allocate(Aa[i], 0);
	return p;
}

void
freemem(void *p, size_t size)
{
	unsigned int i;
	size_t asize;

	if (p == NULL)
		return;

	/* fprintf(stderr, "deallocating %lu bytes\n", size); */
	asize = ROUND(size, CHUNKSIZE);
	i = (asize/CHUNKSIZE)-1;
	if (i >= MAXALLOCATORS) {
		Maxmem -= size;
		safe_free(p);
		return;
	}
	if (Aa[i] == 0) {
		/* fprintf(stderr, "allocator # %d undefined\n", i); */
	}
	assert(Aa[i] != 0);
	assert(Aa[i]->size == asize);
	deallocate(Aa[i], p);
}

void *
reallocmem(void *p, size_t osize, size_t nsize)
{
	void *np;
	np = allocmem(nsize);
	if (p == NULL)
		return np;
	assert(osize < nsize);
	memcpy(np, p, osize);
	freemem(p, osize);
	return np;
}

void
destroymem(size_t size)
{
	unsigned int i;
	size_t asize;
	asize = ROUND(size, CHUNKSIZE);
	i = (asize/CHUNKSIZE)-1;
	if (i >= MAXALLOCATORS || Aa[i] == 0)
		return;
	destroy_allocator(Aa[i]);
	Aa[i] = 0;
}

void
destroyallmem(void)
{
	unsigned int i;
	for (i = 0; i < MAXALLOCATORS; i++) {
		if (Aa[i] == 0) continue;
		destroy_allocator(Aa[i]);
		Aa[i] = 0;
	}
}
