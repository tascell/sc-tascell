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

/**
 * The circular doubly linked list implementation is based upon algorithm
 * described in Knuth's The Art of Computer Programing, Vol 1,
 * section 2.2.5. 
 */

#include "list.h"

#define list_empty(list)    (list->lhead.rlink == &list->lhead)
#define list_top(list)      (list->lhead.rlink)
#define list_bottom(list)   (list->lhead.llink)

void
list_init(list_t *list)
{
	list->lhead.rlink = &list->lhead;
	list->lhead.llink = &list->lhead;
}

void  
list_insert_after(list_t *list, void *anchorp, void *linkp)
{
	link_t *anchor = (link_t *)anchorp;
	link_t *link = (link_t *)linkp; 

	if (list_empty(list)) {
		anchor = &list->lhead;
	}
	/* LLINK(P) <- X (Knuth) */
	link->llink = anchor;
	/* RLINK(P) <- RLINK(X) (Knuth) */
	link->rlink = anchor->rlink;
	/* LLINK(RLINK(X)) <- P (Knuth) */
	anchor->rlink->llink = link;
	/* RLINK(X) <- P (Knuth) */
	anchor->rlink = link;
}

void  
list_insert_before(list_t *list, void *anchorp, void *linkp)
{
	link_t *anchor = (link_t *)anchorp;
	link_t *link = (link_t *)linkp; 

	if (list_empty(list)) {
		anchor = &list->lhead;
	}
	link->rlink = anchor;
	link->llink = anchor->llink;
	anchor->llink->rlink = link;
	anchor->llink = link;
}

void  
list_append(list_t *list, void *link)
{
	list_insert_after(list, list_bottom(list), link);
}

void
list_prepend(list_t *list, void *link)
{
	list_insert_before(list, list_top(list), link);
}

void  
list_remove(list_t *list, void *linkp)
{
	link_t *link = (link_t *)linkp; 

	/* RLINK(LLINK(X)) <- RLINK(X) (Knuth) */
	link->llink->rlink = link->rlink;
	/* LLINK(RLINK(X)) <- LLINK(X) (Knuth) */
	link->rlink->llink = link->llink;
	link->rlink = link->llink = 0;
}

void *
list_pop(list_t *list) 
{
	void *link = list_last(list);
	if (link != 0)
		list_remove(list, link);
	return link;
}	

void * 
list_first(list_t *list)
{
	if (list_empty(list)) {
		return 0;
	}
	return list_top(list);
}

void * 
list_last(list_t *list)
{
	if (list_empty(list)) {
		return 0;
	}
	return list_bottom(list);
}

void * 
list_next(list_t *list, void *linkp)
{
	link_t *link = (link_t *)linkp; 
	if (link == list_bottom(list)) {
		return 0;
	}
	return link->rlink;
}

void * 
list_prev(list_t *list, void *linkp)
{
	link_t *link = (link_t *)linkp; 
	if (link == list_top(list)) {
		return 0;
	}
	return link->llink;
}

/* #define STANDALONE */
#ifdef STANDALONE

int main(void)
{
	typedef struct {
		link_t l;
		int a;
	} Item;

	Item a = { {0,0}, 1 };
	Item b = { {0,0}, 2 };
	Item c = { {0,0}, 3 };
	Item d = { {0,0}, 4 };
	Item e = { {0,0}, 5 };
	Item f = { {0,0}, 0 };

	list_t l = { {0,0} };

	Item *ptr = 0;

	list_init(&l);
	list_append(&l, &b);
	list_insert_before(&l, &b, &a);
	list_insert_after(&l, &b, &d);
	list_insert_after(&l, &b, &c);
	list_append(&l, &e);
	list_prepend(&l, &f);

	for (ptr = (Item *)list_first(&l);
		  ptr != 0;
		  ptr = (Item *)list_next(&l, ptr))
	{
		printf("%d\n", ptr->a);
	}

	list_remove(&l, &f);
	list_remove(&l, &e);
	list_remove(&l, &b);
	list_remove(&l, &c);
	for (ptr = (Item *)list_last(&l);
		  ptr != 0;
		  ptr = (Item *)list_prev(&l, ptr))
	{
		printf("%d\n", ptr->a);
	}
	list_remove(&l, &d);
	list_remove(&l, &a);
	for (ptr = (Item *)list_first(&l);
		  ptr != 0;
		  ptr = (Item *)list_next(&l, ptr))
	{
		printf("%d\n", ptr->a);
	}

	return 0;
}

#endif
