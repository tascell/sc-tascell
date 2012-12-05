/****************************************************************************/
/* TREEGRAV.C: routines to compute gravity. Public routines: gravcalc().    */
/* Copyright (c) 2001 by Joshua E. Barnes, Honolulu, Hawai`i.               */
/****************************************************************************/

#include <pthread.h>
#include "stdinc.h"
#include "mathfns.h"
#include "vectmath.h"
#include "treedefs.h"

/* Local routines to perform force calculations. */

bool bh_accept(nodeptr, real, vector);



/*
 * GRAVCALC: perform force calculation on all particles.
 */

/*------
  moved to treecode.tcell
------*/


/*
 * WALKTREE: do a complete walk of the tree, building the interaction
 * list level-by-level and computing the resulting force on each body.
 */

/*------
  moved to treecode.tcell
------*/


#if defined(QUICKSCAN)

/*
 * ACCEPT: quick criterion accepts any cell not touching cell p.
 */

bool bh_accept(nodeptr c, real psize, vector pmid)
{
    real p15, dk;

    p15 = ((real) 1.5) * psize;                 /* premultiply cell size    */
    dk = Pos(c)[0] - pmid[0];                   /* find distance to midpnt  */
    if (ABS(dk) > p15)                          /* if c does not touch p    */
        return (TRUE);                          /* then accept interaction  */
    dk = Pos(c)[1] - pmid[1];                   /* find distance to midpnt  */
    if (ABS(dk) > p15)                          /* if c does not touch p    */
        return (TRUE);                          /* then accept interaction  */
    dk = Pos(c)[2] - pmid[2];                   /* find distance to midpnt  */
    if (ABS(dk) > p15)                          /* if c does not touch p    */
        return (TRUE);                          /* then accept interaction  */
    return (FALSE);                             /* else do not accept it    */
}

#else

/*
 * ACCEPT: standard criterion accepts cell if its critical radius
 * does not intersect cell p, and also imposes above condition.
 */

bool bh_accept(nodeptr c, real psize, vector pmid)
{
    real dmax, dsq, dk;
    int k;

    dmax = psize;                               /* init maximum distance    */
    dsq = 0.0;                                  /* and squared min distance */
    for (k = 0; k < NDIM; k++) {                /* loop over space dims     */
        dk = Pos(c)[k] - pmid[k];               /* form distance to midpnt  */
        if (dk < 0)                             /* and get absolute value   */
            dk = - dk;
        if (dk > dmax)                          /* keep track of max value  */
            dmax = dk;
        dk -= ((real) 0.5) * psize;             /* allow for size of cell   */
        if (dk > 0)
            dsq += dk * dk;                     /* sum min dist to cell ^2  */
    }
    return (dsq > Rcrit2(c) &&                  /* test angular criterion   */
              dmax > ((real) 1.5) * psize);     /* and adjacency criterion  */
}

#endif

/*
 * WALKSUB: test next level's active list against subnodes of p.
 */

/*------
  moved to treecode.tcell
------*/


/*
 * GRAVSUM: compute gravitational field at body p0.
 */

/*------
  moved to treecode.tcell
------*/


/*
 * SUMNODE: add up body-node interactions.
 */

/*------
  moved to treecode.tcell
------*/


/*
 * SUMCELL: add up body-cell interactions.
 */

/*------
  moved to treecode.tcell
------*/
