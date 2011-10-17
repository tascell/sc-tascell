/****************************************************************************/
/* TREELOAD.C: routines to create tree.  Public routines: maketree().       */
/* Copyright (c) 1999 by Joshua E. Barnes, Tokyo, JAPAN.                    */
/****************************************************************************/

#include "stdinc.h"
#include "mathfns.h"
#include "vectmath.h"
#include "treedefs.h"
#include <sys/time.h>

/*
 * Local routines and variables to perform tree construction.
 */

local void newtree(void);                      /* flush existing tree      */
local int  makecell(void);                     /* create an empty cell     */
local void expandbox(bodyptr, int);            /* set size of root cell    */
local void loadbody(int);                      /* load body into tree      */
local int  subindex(bodyptr, cellptr);          /* compute subcell index    */
local void hackcofm(int, real, int);           /* find centers of mass     */
local void setrcrit(cellptr, vector, real);     /* set cell's crit. radius  */
local void threadtree(nindex, nindex);          /* set next and more links  */
local void hackquad(int);                      /* compute quad moments     */

local bool bh86, sw94;                          /* use alternate criteria   */
local nodeptr freecell = NULL;                  /* list of free cells       */

#define MAXLEVEL  32                            /* max height of tree       */

local int cellhist[MAXLEVEL];                   /* count cells by level     */
local int subnhist[MAXLEVEL];                   /* count subnodes by level  */


extern double nwtreetime, expandtime, crtreetime, hackcmtime, threadtime;
double elapsed_time(struct timeval tp[2]);


/*
 * MAKETREE: initialize tree structure for hierarchical force calculation.
 */

void maketree(bodyptr btab, int nbody)
{
    double cpustart;
    bodyptr p;
    int i;
    struct timeval tp[2];

    cpustart = cputime();                       /* record time at start     */
    
    gettimeofday(tp, 0);
    newtree();                                  /* flush existing tree, etc */
    gettimeofday(tp + 1, 0);
    nwtreetime += elapsed_time(tp);

    root = makecell();                          /* allocate the root cell   */
    cellptr rp = celltab + root;
    CLRV(Pos(rp));                            /* initialize the midpoint  */
    
    gettimeofday(tp, 0);
    expandbox(btab, nbody);                     /* and expand cell to fit   */
    gettimeofday(tp + 1, 0);
    expandtime += elapsed_time(tp);

    gettimeofday(tp, 0);
    for (i = 0; i < nbody; i++)                 /* loop over all bodies     */
        loadbody(i);                            /* insert each into tree    */
    gettimeofday(tp + 1, 0);
    crtreetime += elapsed_time(tp);

    bh86 = scanopt(options, "bh86");            /* set flags for alternate  */
    sw94 = scanopt(options, "sw94");            /* ...cell opening criteria */
    if (bh86 && sw94)                           /* can't have both at once  */
        error("maketree: incompatible options bh86 and sw94\n");
    tdepth = 0;                                 /* init count of levels     */
    for (i = 0; i < MAXLEVEL; i++)              /* and init tree histograms */
        cellhist[i] = subnhist[i] = 0;
    
    gettimeofday(tp, 0);
    hackcofm(root, rsize, 0);                   /* find c-of-m coords, etc  */
    gettimeofday(tp + 1, 0);
    hackcmtime += elapsed_time(tp);

    gettimeofday(tp, 0);
    threadtree((nindex){CELL, root}, (nindex){CELL, CNULL});           /* add next and more links  */
    gettimeofday(tp + 1, 0);
    threadtime += elapsed_time(tp);

    if (usequad)                                /* if including quad terms  */
        hackquad(root);                         /* find quadrupole moments  */
    cputree = cputime() - cpustart;             /* store elapsed CPU time   */
}

/*
 * NEWTREE: reclaim cells in tree, prepare to build new one.
 */

local void newtree(void)
{
    static bool called = FALSE;
    
    if (!called) {
        celltab = allocate(sizeof(cell) * nbody);
        called = TRUE;
    }
    root = CNULL;                                /* flush existing tree      */
    ncell = 0;                                  /* reset cell count         */
}

/*
 * MAKECELL: return pointer to free cell.
 */

local int makecell(void)
{
    int i;
    cellptr c;
    
    if (ncell > nbody)
        error("makecell: cell array overflow: ncell=%d, nbody=%d", ncell, nbody);
    
    c = celltab + ncell;
    Type(c) = CELL;                             /* initialize node type     */
    Update(c) = FALSE;                          /* and force update flag    */
    for (i = 0; i < NSUB; i++) {
        Subp(c)[i].type  = CELL;                  /* loop over subcells       */
        Subp(c)[i].index = CNULL;                      /* and empty each one       */
    }
    ncell++;                                    /* count one more cell      */
    return ncell - 1;                                 /* return pointer to cell   */
}

/*
 * EXPANDBOX: find range of coordinate values (with respect to root)
 * and expand root cell to fit.  The size is doubled at each step to
 * take advantage of exact representation of powers of two.
 */

local void expandbox(bodyptr btab, int nbody)
{
    real dmax, d;
    bodyptr p;
    int k;
    cellptr rp = celltab + root;

    dmax = 0.0;                                 /* keep track of max value  */
    for (p = btab; p < btab+nbody; p++)         /* loop over all bodies     */
        for (k = 0; k < NDIM; k++) {            /* and over all dimensions  */
            d = rabs(Pos(p)[k] - Pos(rp)[k]); /* find distance to midpnt  */
            if (d > dmax)                       /* if bigger than old one   */
                dmax = d;                       /* store new max value      */
        }
    while (rsize < 2 * dmax)                    /* loop until value fits    */
        rsize = 2 * rsize;                      /* doubling box each time   */
}

/*
 * LOADBODY: descend tree and insert body p in appropriate place.
 */

local void loadbody(int i)
{
    bodyptr p = bodytab + i;
    cellptr q = celltab + root;
    int qind = subindex(p, q);
    real qsize = rsize;
    while (Subp(q)[qind].index != CNULL)
    {
        if (Subp(q)[qind].type == BODY) {
            bodyptr subq = bodytab + Subp(q)[qind].index;
            real dist2;
            vector distv;
            DOTPSUBV(dist2, distv, Pos(p), Pos(subq));
            if (dist2 == 0.0)
                error("loadbody: two bodies have same position\n");
            int c = makecell();
            cellptr cp = celltab + c;
            int k = 0;
            for (; k < NDIM; k++)
                Pos(cp)[k] = Pos(q)[k] +
                    (Pos(p)[k] < Pos(q)[k] ? - qsize : qsize) / 4;
            Subp(cp)[subindex(subq, cp)] = Subp(q)[qind];
            Subp(q)[qind] = (nindex){ CELL, c };
        }
        q = celltab + Subp(q)[qind].index;
        qind = subindex(p, q);
        qsize /= 2;
    }
    Subp(q)[qind] = (nindex){ BODY, i };
}

/*
 * SUBINDEX: compute subcell index for body p in cell q.
 */

local int subindex(bodyptr p, cellptr q)
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

local void hackcofm(int pi, real psize, int lev)
{
    cellptr p = celltab + pi;
    vector cmpos, tmpv;
    int i, k;
    real dpq;

    tdepth = MAX(tdepth, lev);                  /* remember maximum level   */
    cellhist[lev]++;                            /* count cells by level     */
    Mass(p) = 0.0;                              /* init cell's total mass   */
    CLRV(cmpos);                                /* and center of mass pos   */
    for (i = 0; i < NSUB; i++) {                /* loop over the subnodes   */
        nindex qi = Subp(p)[i];
        if (qi.index != CNULL) {
            subnhist[lev]++;                    /* count existing subnodes  */
            if (qi.type == CELL)                /* and if node is a cell    */
                hackcofm(qi.index, psize/2, lev+1);
                                                /* then do the same for it  */
            nodeptr q = (qi.type == CELL) ? (nodeptr)(celltab + qi.index)
                                          : (nodeptr)(bodytab + qi.index);
            Update(p) |= Update(q);             /* propagate update request */
            Mass(p) += Mass(q);                 /* accumulate total mass    */
            MULVS(tmpv, Pos(q), Mass(q));       /* weight position by mass  */
            ADDV(cmpos, cmpos, tmpv);           /* and sum c-of-m position  */
        }
    }
    if (Mass(p) > 0.0) {                        /* usually, cell has mass   */
        DIVVS(cmpos, cmpos, Mass(p));           /* so find c-of-m position  */
    } else {                                    /* but if no mass inside    */
        SETV(cmpos, Pos(p));                    /* use geo. center for now  */
    }
    for (k = 0; k < NDIM; k++)                  /* check c-of-m of cell     */
        if (cmpos[k] < Pos(p)[k] - psize/2 ||   /* if actually outside cell */
              Pos(p)[k] + psize/2 <= cmpos[k])
            error("hackcofm: tree structure error\n");
#if !defined(QUICKSCAN)
    setrcrit(p, cmpos, psize);                  /* set critical radius      */
#endif
    SETV(Pos(p), cmpos);                        /* and center-of-mass pos   */
}

#if !defined(QUICKSCAN)

/*
 * SETRCRIT: assign critical radius for cell p, using center-of-mass
 * position cmpos and cell size psize.
 */

local void setrcrit(cellptr p, vector cmpos, real psize)
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

local void threadtree(nindex pi, nindex ni)
{
    nodeptr p, n;
    nindex desc[NSUB+1];
    
    if (pi.index != CNULL)
        p = (pi.type == CELL) ? (nodeptr)(celltab + pi.index)
                              : (nodeptr)(bodytab + pi.index);
    else
        p = NULL;
    if (ni.index != CNULL)
        n = (ni.type == CELL) ? (nodeptr)(celltab + ni.index)
                              : (nodeptr)(bodytab + ni.index);
    else
        n = NULL;
    
    Next(p) = ni;
    if (Type(p) == CELL) {
        int i;
        int ndesc = 0;
        for (i = 0; i < NSUB; i++) {
            nindex subpi = Subp(p)[i];
            if (subpi.index != CNULL)
                desc[ndesc++] = subpi;
        }
        More(p) = desc[0];
        desc[ndesc] = ni;
        for (i = 0; i < ndesc; i++)
            threadtree(desc[i], desc[i+1]);
    }
}

/*
 * HACKQUAD: descend tree, evaluating quadrupole moments.  Note that this
 * routine is coded so that the Subp() and Quad() components of a cell can
 * share the same memory locations.
 */

local void hackquad(int pi)
{
    cellptr p = celltab + pi;
    nindex desc[NSUB];
    int i;
    int ndesc = 0;
    for (i = 0; i < NSUB; i++)
        if (Subp(p)[i].index != CNULL)
            desc[ndesc++] = Subp(p)[i];
    CLRM(Quad(p));
    for (i = 0; i < ndesc; i++) {
        nodeptr q = (desc[i].type == CELL) ? (nodeptr)(celltab + desc[i].index)
                                           : (nodeptr)(bodytab + desc[i].index);
        if (Type(q) == CELL)
            hackquad(desc[i].index);
        vector dr;
        real drsq;
        matrix drdr, Idrsq, tmpm;
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
