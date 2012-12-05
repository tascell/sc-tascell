/****************************************************************************/
/* TREEDEFS.H: include file for hierarchical force calculation routines.    */
/* These definitions are needed for treeload.c and treegrav.c, but this     */
/* file does not provide definitions for other parts of the N-body code.    */
/* Copyright (c) 1999 by Joshua E. Barnes, Tokyo, JAPAN.                    */
/****************************************************************************/

#ifndef _treedefs_h
#define _treedefs_h

#if defined(USELOCK)
#include <pthread.h>
#endif

#if defined(DISTRIBUTED)
typedef struct nindex {
    short type;
    int index;
} nindex, *nindexptr;

#define IndexToPointer(x) \
  ((((nindexptr)(x))->type == BODY) ? (nodeptr)(bodytab + ((nindexptr)(x))->index) : \
                                      (nodeptr)(celltab + ((nindexptr)(x))->index))
#define PointerToIndex(x) \
  ((Type(x) == BODY) ? ((bodyptr)(x) - bodytab) : ((cellptr)(x) - celltab))

#endif  // #if defined(DISTRIBUTED)

/*
 * NODE: data common to BODY and CELL structures.
 */

typedef struct _node {
    short type;                 /* code for node type */
    bool update;                /* status in force calc */
    real mass;                  /* total mass of node */
    vector pos;                 /* position of node */
#if defined(DISTRIBUTED)
    nindex next;
#else
    struct _node *next;         /* link to next force calc */
#endif
} node, *nodeptr;

#define Type(x)   (((nodeptr) (x))->type)
#define Update(x) (((nodeptr) (x))->update)
#define Mass(x)   (((nodeptr) (x))->mass)
#define Pos(x)    (((nodeptr) (x))->pos)
#define Next(x)   (((nodeptr) (x))->next)

#define BODY 01                 /* type code for bodies */
#define CELL 02                 /* type code for cells */

/*
 * BODY: data structure used to represent particles.
 */

typedef struct {
    node bodynode;              /* data common to all nodes */
    vector vel;                 /* velocity of body */
    vector acc;                 /* acceleration of body */
    real phi;                   /* potential at body */
} body, *bodyptr;

#define Vel(x)    (((bodyptr) (x))->vel)
#define Acc(x)    (((bodyptr) (x))->acc)
#define Phi(x)    (((bodyptr) (x))->phi)

/*
 * CELL: structure used to represent internal nodes of tree.
 */

#define NSUB (1 << NDIM)        /* subcells per cell */

typedef struct {
    node cellnode;              /* data common to all nodes */
#if !defined(QUICKSCAN)
    real rcrit2;                /* critical c-of-m radius^2 */
#endif
#if defined(USELOCK)
    pthread_mutex_t mx;
#endif
#if defined(COUNTBODY)
    int nbody;
#endif
#if defined(DISTRIBUTED)
    nindex more;
#else
    nodeptr more;               /* link to first descendent */
#endif
    union {
#if defined(DISTRIBUTED)
        nindex subp[NSUB];
#else
        nodeptr subp[NSUB];     /* descendents of cell */
#endif
        matrix quad;            /* quad. moment of cell */
    } sorq;
} cell, *cellptr;

#if defined(DISTRIBUTED)
#define CNULL  (-1)
#endif

#if !defined(QUICKSCAN)
#define Rcrit2(x) (((cellptr) (x))->rcrit2)
#endif

#define Nbody(x)  ((int)((((nodeptr)(x))->mass) * (double)nbody))
#define More(x)   (((cellptr) (x))->more)
#define Subp(x)   (((cellptr) (x))->sorq.subp)
#define Quad(x)   (((cellptr) (x))->sorq.quad)

/*
 * GLOBAL: pseudo-keyword for storage class.
 */

#if !defined(global)
#  define global extern
#endif

/*
 * Parameters for tree construction and force calculation.
 */

#if !defined(QUICKSCAN)
global real theta;                      /* force accuracy parameter         */
#endif

global string options;                  /* various option keywords          */

global bool usequad;                    /* use quadrupole corrections       */

global real eps;                        /* density smoothing parameter      */

/*
 * Tree construction.
 */

/* maketree is a worker function in shared memory environments */
#if defined(DISTRIBUTED)
void maketree(bodyptr, int);            /* construct tree structure         */
#endif

#if defined(DISTRIBUTED)
global int root;                        /* pointer to root cell             */
#else
global cellptr root;                    /* pointer to root cell             */
global cellptr old_root;
#endif
global real rsize;                      /* side-length of root cell         */
global int tdepth;                      /* count of levels in tree          */
global real cputree;                    /* CPU time to build tree           */

global int nbody;                       /* number of bodies in system       */
global bodyptr bodytab;                 /* points to array of bodies        */

global int ncell;                       /* count of cells in tree           */
global cellptr celltab;

/*
 * Force calculation.
 */

/* gravcalc is changed to a worker function */
// void gravcalc(void);                    /* update force on bodies           */

global int actmax;                      /* maximum length of active list    */
global int nbbcalc;                     /* total body-body interactions     */
global int nbccalc;                     /* total body-cell interactions     */
global real cpuforce;                   /* CPU time for force calc          */

#endif /* ! _treedefs_h */
