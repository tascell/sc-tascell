#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

double		elapsed_time(struct timeval tp[2])
{
  return (tp[1]).tv_sec - (tp[0]).tv_sec + 1.0e-6 * ((tp[1]).tv_usec - (tp[0]).tv_usec);
}

int ps[57][5] = { // ps[i] represents the shape of the \TT{i}-th (piece, direcion)
  {1,1,1,1},{7,7,7,7},
  {4,1,1,1},{7,7,6,1},
  {6,1,1,1},{5,1,1,1},{1,1,1,5},{1,1,1,6},
  {7,1,6,7},{7,7,1,6},{7,6,1,7},{6,1,7,7},
  {7,7,1,1},{1,1,5,7},{1,1,7,7},{7,5,1,1},
  {2,5,1,1},{1,1,5,2},{1,6,7,1},{1,7,6,1},
  {7,1,1,5},{5,1,1,7},{7,6,1,1},{1,1,6,7},
  {7,1,1,6},{5,1,1,6},{6,1,1,5},{6,1,1,7},
  {1,7,1,6},{7,1,5,1},{1,5,1,7},{6,1,7,1},
  {7,1,1,7},{5,1,1,5},{1,7,7,1},{1,6,6,1},
  {6,1,1,6},
  {1,6,1,1},{1,1,5,1},{1,5,1,1},{1,1,6,1},
  {7,1,6,1},{1,6,1,6},{6,1,6,1},{1,6,1,7},
  {1,4,1,1},{1,1,7,1},{1,7,1,1},{1,1,4,1},
  {7,1,7,7},{7,6,1,6},{6,1,6,7},{7,7,1,7},
  {7,1,7,1},{1,5,1,6},{1,7,1,7},{6,1,5,1}
};

int pos[13] = { 0, 2, 4, 12, 16, 20, 24, 32, 36, 37, 45, 53, 57};
// ps[i] for pos[p]<i<pos[p+1] corresponds the shape for the \TT{p}-th piece


// 探索メイン
// Try from the \TT{j1}-th piece to the \TT{j2}-th piece in \TT{a[]}.
// The \IT{i}-th piece for \IT{i}}<\RM{\TT{j0} is already used.
// \TT{b[k]} is the first empty cell in the board.
int 
search(int k, int j0, int j1, int j2, int *a, int *b)
{
  int s = 0;
  int p;

  for (p=j1; p<j2; p++) {
    int ap = a[p], i;
    for(i = pos[ap]; i < pos[ap+1]; i++)
      /* examine the "i"-th (piece, direction)
	 at the first empty location "k" */
      {
	int *pss = ps[i];
	int kk=k,l;
	/* room available? */
	for(l=0;l<4;l++) if((kk += pss[l]) >= 70 || b[kk] !=0) goto Ln;

        // set the piece \TT{p} into the board
	b[kk=k] = p+'A'; for(l=0;l<4;l++) b[kk += pss[l]] = p+'A';
	a[p] = a[j0]; a[j0] = ap;
	// 次の空きセルをみつける
	for(kk=k; kk<70; kk++) if( b[kk] == 0 ) break;
	// 全セルが埋まっていたら
	if(kk == 70)
	  s += 1;
	// そうでなければ（周辺を見て置けそうなら？）次のステップ
	else if((kk+7 >= 70 || b[kk+7] != 0) && 
		(b[kk+1] != 0 || 
		 (kk+8 >= 70 || b[kk+8] != 0) && b[kk+2] != 0))
	  ;
	else
	  s += search(kk, j0 + 1, j0 + 1, 12, a, b);
	// remove the piece p: backtrack
	ap = a[j0]; a[j0] = a[p]; a[p] = ap; 
	b[kk=k] = 0; for(l=0;l<4;l++) b[kk += pss[l]] = 0;
      Ln:
	continue;
      }
  }
  return s;
}

int main(){
  int rslt, i;
  int a[12];         // manages which piece has been used
  int b[70];         // board（6x10 だが各列右端は壁）
  struct timeval  tp[2];

  for(i=0;i<12;i++) a[i] = i;
  for(i=0;i<70;i++) b[i] = 0;
  for(i=6;i<70;i+=7) b[i] = '\n'; /* right side wall */
  gettimeofday(tp, 0);
  rslt = search(0, 0, 0, 12, a, b);  
  gettimeofday(tp + 1, 0);
  fprintf(stderr, "time: %lf\n", elapsed_time(tp));

  if (rslt > 0)
    printf("%d possible result!\n", rslt);
  else
    printf("no possible result!\n");

  return 0;
}
