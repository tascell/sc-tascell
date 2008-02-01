#define L 0
#define R 1
#define T 2
#define B 3

#define X  2
#define O  1
#define MT 0

/* set HASH = 0 to turn off, else 1 */
#define  HASH 1


#define MAX_GAME_LENGTH 32768

#define INF 32700
#define MATE 32000

#define REPWIN 32001
#define STRAIGHTWIN 32000

/*  if you get compiler error just touch depth.h */
#include "depth.h"

#ifndef MAX_DEPTH
#define MAX_DEPTH 100
#endif


#ifndef BD_SIZE
#define BD_SIZE 6
#endif

#if (BD_SIZE <= 4)
#define ROW_SIZE 4
#else
#if (BD_SIZE <= 8)
#define ROW_SIZE 8
#else
#if (BD_SIZE <= 16)
#define ROW_SIZE 16
#else
#define ROW_SIZE 32
#endif
#endif
#endif

#ifndef REPEAT_HASH_SIZE
#define REPEAT_HASH_SIZE 1024
#endif

#define TT_SIZE 0x80000
#define TT_MASK (TT_SIZE-1)

#define READ_BD(p,x,y) (p->bd[x][y])
#define SET_BD(p,x,y,w) {p->bd[x][y] = w;}
#define CLR_BD(p,x,y) {p->bd[x][y] = MT;}

#define CNT_RANK(p,x) (p->nrank[x][X] + p->nrank[x][O])
#define CNT_FILE(p,x) (p->nfile[x][X] + p->nfile[x][O])


typedef unsigned char BDTYPE;
typedef unsigned long long u64;

typedef struct ptag
{
   BDTYPE  bd[BD_SIZE][ROW_SIZE];
   u64     key;
   int     ply_of_game;
   int     alpha;
   int     beta;
   char    nrank[BD_SIZE][3], nfile[BD_SIZE][3];
   short   npc[3];
   short   ctr_pts[3];

      /* note: his[0] is preveious position, his[1] is 2 back */

   struct ptag  *his[2];         /* pointer to last 2 positions */
      
 
} position;


#define LOWER_BOUND 0
#define EXACT_SCORE 1
#define UPPER_BOUND 2

typedef struct
{
  u64         verify;
  int          score:16;
  int        quality:16;
  int          bound:8;
  int           move;

} transposition;



/* Color to move is next macro */
#define CTM(p) ( ((p->ply_of_game & 1)+1) ^3)












