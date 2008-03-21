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
#ifndef list_h
#define list_h

typedef struct link_t
{
	struct link_t *llink, *rlink ;
} link_t ;

typedef struct
{
	link_t  lhead ;
} list_t ;

void   list_init         ( list_t *list );
void   list_append       ( list_t *list, void *link );
void   list_prepend      ( list_t *list, void *link );
void   list_insert_after ( list_t *list, void *anchor, void *link );
void   list_insert_before( list_t *list, void *anchor, void *link );
void * list_first        ( list_t *list );
void * list_last         ( list_t *list );
void * list_next         ( list_t *list, void *link );
void * list_prev         ( list_t *list, void *link );
void   list_remove       ( list_t *list, void *link );
void   list_push         ( list_t *list, void *link );
void * list_pop          ( list_t *list );

#define list_push(list, link) list_append((list), (link))

#endif
