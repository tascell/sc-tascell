/****************************************************************************/
/* TREELOAD.C: routines to create tree.  Public routines: maketree().       */
/* Copyright (c) 1999 by Joshua E. Barnes, Tokyo, JAPAN.                    */
/****************************************************************************/

#include <pthread.h>
#include "stdinc.h"
#include "mathfns.h"
#include "vectmath.h"
#include "treedefs.h"

/*
 * Local routines and variables to perform tree construction.
 */

cellptr makecell(void);                   /* create an empty cell     */
void expandbox(bodyptr, int);             /* set size of root cell    */
int subindex(bodyptr, cellptr);           /* compute subcell index    */
void setrcrit(cellptr, vector, real);     /* set cell's crit. radius  */
void hackquad(cellptr);                   /* compute quad moments     */

bool bh86, sw94;                          /* use alternate criteria   */
nodeptr freecell = NULL;                  /* list of free cells       */

#define MAXLEVEL  32                            /* max height of tree       */

int cellhist[MAXLEVEL];                   /* count cells by level     */
int subnhist[MAXLEVEL];                   /* count subnodes by level  */

/*
 * MAKETREE: initialize tree structure for hierarchical force calculation.
 */
/*------
  moved to treeload.tcell
------*/

/*
 * NEWTREE: reclaim cells in tree, prepare to build new one.
 */
/*------
  moved to treeload.tcell
------*/

/*
 * MAKECELL: return pointer to free cell.
 */
/*------
 moved to treeload.tcell
 ------*/

/*
 * EXPANDBOX: find range of coordinate values (with respect to root)
 * and expand root cell to fit.  The size is doubled at each step to
 * take advantage of exact representation of powers of two.
 */
/*------
  moved to treeload.tcell
------*/

/*
 * LOADBODY: descend tree and insert body p in appropriate place.
 */
/*------
  moved to treeload.tcell
------*/

/*
 * SUBINDEX: compute subcell index for body p in cell q.
 */

int subindex(bodyptr p, cellptr q)
{
    int ind, k;

    ind = 0;                                    /* accumulate subcell index */
    for (k = 0; k < NDIM; k++)                  /* loop over dimensions     */
        if (Pos(q)[k] <= Pos(p)[k])             /* if beyond midpoint       */
            ind += NSUB >> (k + 1);             /* then skip over subcells  */
    return (ind);
}

/*
 * HACKCOFM: descend tree finding center-of-mass coordinates;
 * also sets critical cell radii, if appropriate.
 */
/*------
  moved to treeload.tcell
------*/

#if !defined(QUICKSCAN)

/*
 * SETRCRIT: assign critical radius for cell p, using center-of-mass
 * position cmpos and cell size psize.
 */

void setrcrit(cellptr p, vector cmpos, real psize)
{
    real bmax2, d;
    int k;

    if (theta == 0.0)                           /* if exact calculation     */
        Rcrit2(p) = rsqr(2 * rsize);            /* then always open cells   */
    else if (sw94) {                            /* if using S&W's criterion */
        bmax2 = 0.0;                            /* compute max distance^2   */
        for (k = 0; k < NDIM; k++) {            /* loop over dimensions     */
            d = cmpos[k] - Pos(p)[k] + psize/2; /* get dist from corner     */
            bmax2 += rsqr(MAX(d, psize - d));   /* and sum max distance^2   */
        }
        Rcrit2(p) = bmax2 / rsqr(theta);        /* use max dist from cm     */
    } else if (bh86)                            /* if using old criterion   */
        Rcrit2(p) = rsqr(psize / theta);        /* then use size of cell    */
    else {                                      /* else use new criterion   */
        DISTV(d, cmpos, Pos(p));                /* find offset from center  */
        Rcrit2(p) = rsqr(psize / theta + d);    /* use size plus offset     */
    }
}

#endif

/*
 * THREADTREE: do a recursive treewalk starting from node p,
 * with next stop n, installing Next and More links.
 */
/*------
  moved to treeload.tcell
------*/

/*
 * HACKQUAD: descend tree, evaluating quadrupole moments.  Note that this
 * routine is coded so that the Subp() and Quad() components of a cell can
 * share the same memory locations.
 */

void hackquad(cellptr p)
{
    int ndesc, i;
    nodeptr desc[NSUB], q;
    vector dr;
    real drsq;
    matrix drdr, Idrsq, tmpm;

    ndesc = 0;                                  /* count occupied subnodes  */
    for (i = 0; i < NSUB; i++)                  /* loop over all subnodes   */
        if (Subp(p)[i] != NULL)                 /* if this one's occupied   */
            desc[ndesc++] = Subp(p)[i];         /* copy it to safety        */
    CLRM(Quad(p));                              /* init quadrupole moment   */
    for (i = 0; i < ndesc; i++) {               /* loop over real subnodes  */
        q = desc[i];                            /* access ech one in turn   */
        if (Type(q) == CELL)                    /* if it's also a cell      */
            hackquad((cellptr) q);              /* then process it first    */
        SUBV(dr, Pos(q), Pos(p));               /* find displacement vect.  */
        OUTVP(drdr, dr, dr);                    /* form outer prod. of dr   */
        DOTVP(drsq, dr, dr);                    /* and dot prod. dr * dr    */
        SETMI(Idrsq);                           /* init unit matrix         */
        MULMS(Idrsq, Idrsq, drsq);              /* and scale by dr * dr     */
        MULMS(tmpm, drdr, 3.0);                 /* scale drdr by 3          */
        SUBM(tmpm, tmpm, Idrsq);                /* now form quad. moment    */
        MULMS(tmpm, tmpm, Mass(q));             /* from cm of subnode       */
        if (Type(q) == CELL)                    /* if subnode is cell       */
            ADDM(tmpm, tmpm, Quad(q));          /* then include its moment  */
        ADDM(Quad(p), Quad(p), tmpm);           /* increment moment of cell */
    }
}
