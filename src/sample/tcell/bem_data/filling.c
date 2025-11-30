#include "filling.h"

int acaplus(double* zaa, double* zab, int ndl, int ndt, int nstrtl, int nstrtt, int kmax, double eps, double znrmmat, double pACA_EPS){
  double *prow, *pcol, *pb_ref, *pa_ref;
  int *lrow_done, *lcol_done;

  double (*zaa2)[ndl];
  double (*zab2)[ndt];
  zaa2 = (double(*)[ndl])zaa;
  zab2 = (double(*)[ndt])zab;
  
  int il,it,ib;

  int INCY = 1;
  double za_ACA_EPS = 1.0e-10;

  double znrm = znrmmat * sqrt((double)ndl * (double)ndt);
  double ACA_EPS = pACA_EPS;

  int ntries = max(ndl, ndt) + 1;
  int ntries_row = 6;
  int ntries_col = 6;

  lrow_done = (int *)calloc(ndl, sizeof(int));
  lcol_done = (int *)calloc(ndt, sizeof(int));
  int k = 0;

  int j_ref = 0;
  pa_ref = (double *)malloc(ndl * sizeof(double));

  comp_col(zaa, zab, ndl, ndt, k, j_ref, pa_ref, nstrtl, nstrtt, lrow_done);

  double colnorm = cblas_dnrm2(ndl, pa_ref, INCY);
  int i_ref = minabsvalloc_d(pa_ref, ndl);
  double rownorm = fabs(pa_ref[i_ref]);
  pb_ref = (double *)malloc(ndt * sizeof(double));
  comp_row(zaa, zab, ndl, ndt, k, i_ref, pb_ref, nstrtl, nstrtt, lcol_done);
  rownorm = cblas_dnrm2(ndt, pb_ref, INCY);

  double apxnorm = 0.0;
  int lstop_aca = 0;
  
  double col_maxval, row_maxval;
  while(k < kmax && (ntries_row > 0 || ntries_col > 0) && ntries > 0){
    ntries--;
    pcol = &zaa2[k][0];
    prow = &zab2[k][0];
    col_maxval = 0.0;
    int i = maxabsvalloc_d(pa_ref, ndl);
    col_maxval = fabs(pa_ref[i]);
    row_maxval = 0.0;
    int j = maxabsvalloc_d(pb_ref, ndt);
    row_maxval = fabs(pb_ref[j]);

    double zinvmax;
    if(row_maxval > col_maxval){
      if(j != j_ref){
        comp_col(zaa, zab, ndl, ndt, k, j, pcol, nstrtl, nstrtt, lrow_done);
      }else{
        for(il=0;il<ndl;il++){
          pcol[il] = pa_ref[il];
        }
      }
      i = maxabsvalloc_d(pcol, ndl);
      col_maxval = fabs(pcol[i]);

      if(col_maxval < ACA_EPS && k >= 1){
        lstop_aca = 1;
      }else{
        comp_row(zaa, zab, ndl, ndt, k, i, prow, nstrtl, nstrtt, lcol_done);
        if(fabs(pcol[i]) > 1.0e-20){
          zinvmax = 1.0 / pcol[i];
        }else{
          k = max(k-1, 0);
          break;
        }
        for(il=0;il<ndl;il++){
          pcol[il] *= zinvmax;
        } 
      }
    }else{
      if(i != i_ref){
        comp_row(zaa, zab, ndl, ndt, k, i, prow, nstrtl, nstrtt, lcol_done);
      }else{
        for(il=0;il<ndt;il++){
          prow[il] = pb_ref[il];
        }
      }
      j = maxabsvalloc_d(prow, ndt);
      row_maxval = fabs(prow[j]);

      if(row_maxval < ACA_EPS && k >= 1){
        lstop_aca = 1;
      }else{
        comp_col(zaa, zab, ndl, ndt, k, j, pcol, nstrtl, nstrtt, lrow_done);
        if(fabs(prow[j]) > 1.0e-20){
          zinvmax = 1.0 / prow[j];
        }else{
          k = max(k-1, 0);
          break;
        }
        for(il=0;il<ndt;il++){
          prow[il] *= zinvmax;
        } 
      }
    }
    lrow_done[i] = 1;
    lcol_done[j] = 1;

    if(i != i_ref){
      zinvmax = -pcol[i_ref];
      for(il=0;il<ndt;il++){
        pb_ref[il] += prow[il] * zinvmax;
      }
      rownorm = cblas_dnrm2(ndt, pb_ref, INCY);
    }
    if(i == i_ref || rownorm < ACA_EPS){
      if(i == i_ref){
        ntries_row++;
      }
      if(ntries_row > 0){
        rownorm = 0.0;
        i = i_ref;
        while(i != (i_ref + ndl - 1) % ndl && rownorm < za_ACA_EPS && ntries_row > 0){
          if(lrow_done[i] == 0){
            comp_row(zaa, zab, ndl, ndt, k+1, i, pb_ref, nstrtl, nstrtt, lcol_done);
            rownorm = cblas_dnrm2(ndt, pb_ref, INCY);
            if(rownorm < ACA_EPS){
              lrow_done[i] = 1;
            }
            ntries_row--;
          }else{
            rownorm = 0.0;
          }
          i = (i+1) % ndl;
        }
        i_ref = (i + ndl - 1) % ndl;
      }
    }

    if(j != j_ref){
      zinvmax = -prow[j_ref];
      for(il=0;il<ndl;il++){
        pa_ref[il] += pcol[il] * zinvmax;
      }
      colnorm = cblas_dnrm2(ndl, pa_ref, INCY);
    }
    if(j == j_ref || colnorm < ACA_EPS){
      if(j == j_ref){
        ntries_col++;
      }
      if(ntries_col > 0){
        colnorm = 0.0;
        j = j_ref;
        while(j != (j_ref + ndt - 1) % ndt && colnorm < za_ACA_EPS && ntries_col > 0){
          if(lcol_done[j] == 0){
            comp_col(zaa, zab, ndl, ndt, k+1, j, pa_ref, nstrtl, nstrtt, lrow_done);
            colnorm = cblas_dnrm2(ndl, pa_ref, INCY);
            if(colnorm < ACA_EPS){
              lcol_done[j] = 1;
            }
            ntries_col--;
          }else{
            colnorm = 0.0;
          }
          j = (j+1) % ndt;
        }
        j_ref = (j + ndt - 1) % ndt;
      }
    }

    if(colnorm < ACA_EPS && rownorm < ACA_EPS && k >= 1){
      lstop_aca = 1;
      k = k + 1;
    }

    if(lstop_aca == 0){
      double blknorm = cblas_dnrm2(ndl, pcol, INCY) * cblas_dnrm2(ndt, prow, INCY);
      if(k == 0){
        apxnorm = blknorm;
      }else{
        double compared = apxnorm * eps;
        if(blknorm < compared && rownorm < compared && colnorm < compared && k >= 1){
          lstop_aca = 1;
        }
      }
    }
    if(lstop_aca == 1 && k >= 1){
      break;
    }

    k++;
  }

  if(k < 1){
    printf("alert!\n");
    printf("colnorm=%f rownorm=%f ACA_EPS=%f\n", colnorm, rownorm, ACA_EPS);
    printf("col_maxval=%f row_maxval=%f\n", col_maxval, row_maxval);
    printf("ntries_row=%d ntries_col=%d ntries=%d\n", ntries_row, ntries_col, ntries);
    printf("k=%d\n", k);
  }

  return k;

}

void fill_sub_leafmtx(struct leafmtx *st_lf, double znrmmat){
  // double start, end;
  // start = get_wall_time();
  double eps = 1.0e-8;
  double ACA_EPS = 0.9 * eps;
  int kparam = 50;
  int ip,il,it;

  int ndl = st_lf->ndl;
  int ndt = st_lf->ndt;
  int ns = ndl * ndt;
  int nstrtl = st_lf->nstrtl;
  int nstrtt = st_lf->nstrtt;
  int ltmtx = st_lf->ltmtx;
  
  if(ltmtx == 1){
    st_lf->a1 = (double*)malloc(sizeof(double) * ndt * kparam);
    st_lf->a2 = (double*)malloc(sizeof(double) * ndl * kparam);
    if(!st_lf->a1 || !st_lf->a2){
      printf("allocate a1 or a2 failed!\n");
      exit(99);
    }

    int kt = acaplus(st_lf->a2, st_lf->a1, ndl, ndt, nstrtl, nstrtt, kparam, eps, znrmmat, ACA_EPS);
    // printf("#fill kt=%d, nstrtl=%d, nstrtt=%d, ndl=%d, ndt=%d\n", kt, nstrtl, nstrtt, ndl, ndt);
    st_lf->kt = kt;
    
    if(kt > kparam){ //Fortran: kt > kparam-1. kt is the rank.
      printf("WARNING: Insufficient k: kt=%d, kparam=%d, nstrtl=%d, nstrtt=%d, ndl=%d, ndt=%d\n", kt, kparam, nstrtl, nstrtt, ndl, ndt);
    }

    st_lf->a1 = (double *)realloc(st_lf->a1, kt * ndt * sizeof(double));//check if realloc is right
    st_lf->a2 = (double *)realloc(st_lf->a2, kt * ndl * sizeof(double));

  }else if(ltmtx == 2){
    st_lf->a1 = (double *)malloc(sizeof(double) * ns);

    double (*tempa1)[ndt];
    tempa1 = (double(*)[ndt])st_lf->a1;

    for(il=0;il<ndl;il++){
      int ill = il + nstrtl;
      for(it=0;it<ndt;it++){
        int itt = it + nstrtt;
        tempa1[il][it] = entry_ij(ill, itt);
      }
    }
  }
  // end = get_wall_time();
  // printf("filling time spend: %f", end - start);
}

void comp_row(double* zaa, double* zab, int ndl, int ndt, int k, int il, double* row, int nstrtl, int nstrtt, int* lrow_done){
  int it;
  for(it=0;it<ndt;it++){
    if(lrow_done[it] == 0){
      int ill = il + nstrtl;
      int itt = it + nstrtt;
      row[it] = entry_ij(ill, itt);
    }
  }

  if(k == 0){
    return;
  }

  adotsub_dsm(row, zab, zaa, il, ndt, k, ndt, ndl);

  for(it=0;it<ndt;it++){
    if(lrow_done[it] != 0){
      row[it] = 0.0;
    }
  }
}

void comp_col(double* zaa, double* zab, int ndl, int ndt, int k, int it, double* col, int nstrtl, int nstrtt, int* lrow_done){
  int il;

  for(il=0;il<ndl;il++){
    if(lrow_done[il] == 0){
      int ill = il + nstrtl;
      int itt = it + nstrtt;
      col[il] = entry_ij(ill, itt);
    }
  }

  if(k == 0){
    return;
  }

  adotsub_dsm(col, zaa, zab, it, ndl, k, ndl, ndt);

  for(il=0;il<ndl;il++){
    if(lrow_done[il] != 0){
      col[il] = 0.0;
    }
  }
}

int minabsvalloc_d(double* za, int nd){
  int il = 0;
  double zz = fabs(za[0]);
  int it;
  for(it=1;it<nd;it++){
    if(fabs(za[it]) < zz){
      il = it;
      zz = fabs(za[it]);
    }
  }
  return il;
}

int maxabsvalloc_d(double* za, int nd){
  int il = 0;
  double zz = 0.0;
  int it;
  for(it=0;it<nd;it++){
    if(fabs(za[it]) > zz){
      il = it;
      zz = fabs(za[it]);
    }
  }
  return il;
}

double entry_ij(int i, int j){
  int il;
  int n[3];
  double xf[3], yf[3], zf[3];
  double xp, yp, zp;

  xp = zgmid[i][0];
  yp = zgmid[i][1];
  zp = zgmid[i][2];
  
  for(il=0;il<3;il++){
    n[il] = f2n[j][il];
  }
  for(il=0;il<3;il++){
    xf[il] = bgmid[n[il]][0];
    yf[il] = bgmid[n[il]][1];
    zf[il] = bgmid[n[il]][2];
  }

  double result = face_integral2(xf, yf, zf, xp, yp, zp);

  return result;
}

double face_integral2(double xs[], double ys[], double zs[], double x, double y, double z){
  int il;
  double PI = 3.1415927;
  double EPSILON_0 = 8.854188 * 1e-12;

  double r[3];
  double xi, xj, yi, dx, dy, t, l, m, d, ti, tj;
  double theta, omega, q, g, zp, zpabs;

  int i, j;
  double *u, *v, *w;
  double ox, oy, oz;

  for(il=0;il<3;il++){
    r[il] = sqrt( pow((xs[il] - x),2.0) + pow((ys[il] - y),2.0) + pow((zs[il] - z),2.0) );
  }

  u = (double*)malloc(sizeof(double)*3);
  v = (double*)malloc(sizeof(double)*3);
  w = (double*)malloc(sizeof(double)*3);

  u[0] = xs[1] - xs[0];
  u[1] = ys[1] - ys[0];
  u[2] = zs[1] - zs[0];

  v[0] = xs[2] - xs[1];
  v[1] = ys[2] - ys[1];
  v[2] = zs[2] - zs[1];
  
  cross_product(u, v, w);
  
  double dw = sqrt( dot_product(w,w,3));
  for(il=0;il<3;il++){
    w[il] = w[il] / dw;
  }
  u[0] = x - xs[0];
  u[1] = y - ys[0];
  u[2] = z - zs[0];
  zp = dot_product(u,w,3);
  
  ox = x - zp * w[0];
  oy = y - zp * w[1];
  oz = z - zp * w[2];
  zpabs = fabs(zp);

  double face_integral = 0.0;
  for(i=0;i<3;i++){
    j = (i + 1) % 3;
    u[0] = xs[j] - ox;
    u[1] = ys[j] - oy;
    u[2] = zs[j] - oz;
    xj = sqrt( dot_product(u,u,3) );

    for(il=0;il<3;il++){
      u[il] = u[il] / xj;
    }
    cross_product(w, u, v);
    xi = (xs[i] - ox) * u[0] + (ys[i] - oy) * u[1] + (zs[i] - oz) * u[2];
    yi = (xs[i] - ox) * v[0] + (ys[i] - oy) * v[1] + (zs[i] - oz) * v[2];

    dx = xj - xi;
    dy = - yi;
    t = sqrt ((dx*dx) + (dy*dy));
    l = dx / t;
    m = dy / t;
    d = (l * yi) - (m * xi);
    ti = (l * xi) + (m * yi);
    tj = l * xj;

    theta = atan2(yi, xi);
    omega = theta - atan2( r[i] * d, zpabs * ti ) + atan2( r[j] * d, zpabs * tj );
    q = log( (r[j] + tj) / (r[i] + ti) );
    g = d * q - zpabs * omega;
    face_integral = face_integral + g;
  }
  
  return fabs(face_integral) / (4.0 * PI * EPSILON_0);

}

void cross_product(double* u, double* v, double* w){
  w[0] = u[1] * v[2] - u[2] * v[1];
  w[1] = u[2] * v[0] - u[0] * v[2];
  w[2] = u[0] * v[1] - u[1] * v[0];
}

void adotsub_dsm(double* zr, double* zaa, double* zab, int it, int ndl, int ndt, int mdl, int mdt){
  int il;
  double* zau = (double*)calloc(ndl,sizeof(double));

  adot_dsm(zau,zaa,zab,it,ndl,ndt,mdl,mdt);
  for(il=0;il<ndl;il++){
    zr[il] = zr[il] - zau[il];
  }
  free(zau);
}

void adot_dsm(double* zau, double* zaa, double* zab, int im, int ndl, int ndt, int mdl, int mdt){
  int it,il;
  double (*zaa2)[mdl];
  double (*zab2)[mdt];
  zaa2 = (double(*)[mdl])zaa;
  zab2 = (double(*)[mdt])zab;
  for(it=0;it<ndt;it++){
    for(il=0;il<ndl;il++){
      zau[il] = zau[il] + zaa2[it][il] * zab2[it][im];
    }
  }
}

double dot_product(double* v, double* u, int n){
  double result = 0.0;
  int i;
  for (i = 0; i < n; i++){
    result += v[i] * u[i];
  }
  return result;
}

int max(int a, int b){
  if(a >= b){
    return a;
  }
  return b;
}

int min(int a, int b){
  if(a <= b){
    return a;
  }
  return b;
}

double dist_2cluster(int st_cltl,int st_cltt){
  double zs = 0.0;
  int id;
  for(id=0;id<resultCTlist[st_cltl].ndim;id++){
    if(resultCTlist[st_cltl].bmax[id] < resultCTlist[st_cltt].bmin[id]){
      zs = zs + (resultCTlist[st_cltt].bmin[id] - resultCTlist[st_cltl].bmax[id]) * (resultCTlist[st_cltt].bmin[id] - resultCTlist[st_cltl].bmax[id]);
    }else if(resultCTlist[st_cltt].bmax[id]< resultCTlist[st_cltl].bmin[id]){
      zs = zs+ (resultCTlist[st_cltl].bmin[id] - resultCTlist[st_cltt].bmax[id]) * (resultCTlist[st_cltl].bmin[id] - resultCTlist[st_cltt].bmax[id]);
    }
  }
  return sqrt(zs);
}

int create_cluster(int ndpth,int nstrt,int nsize,int ndim,int nson){
  int st_clt;
  st_clt = countCT;
  countCT++;
  resultCTlist[st_clt].nstrt = nstrt;
  resultCTlist[st_clt].nsize = nsize;
  resultCTlist[st_clt].ndim = ndim;
  resultCTlist[st_clt].nnson = nson;
  resultCTlist[st_clt].ndpth = ndpth;

  return st_clt;
}

int create_ctree_ssgeom(int st_clt,   //the current node
			      double (*zgmid)[3],     //coordination of objects
            int (*face2node)[3],
			      int ndpth,         //depth of the tree
			      int ndscd,
			      int nsrt,          //the start index of list
			      int nd,            //the length of list
			      int md,            //number of data
			      int ndim){
  int id,il,nson;
  double minsz = 50.0;
  double zcoef = 1.1;
  double zlmin[ndim],zlmax[ndim];
  ndpth = ndpth + 1;
  
  if(nd <= minsz){
    nson = 0;
    st_clt = create_cluster(ndpth,nsrt,nd,ndim,nson);
  }else{
    for(id=0;id<ndim;id++){
      zlmin[id] = zgmid[0][id];
      zlmax[id] = zlmin[id];
      for(il=1;il<nd;il++){
	      double zg = zgmid[il][id];
        if(zg < zlmin[id]){
          zlmin[id] = zg;
        }else if(zlmax[id] < zg){
          zlmax[id] = zg;
        }
      }
    }
    double zdiff = zlmax[0] - zlmin[0];
    int ncut = 0;
    for(id=0;id<ndim;id++){
      double zidiff = zlmax[id]-zlmin[id];
      if(zidiff > zcoef * zdiff){
        zdiff = zidiff;
        ncut = id;
      }
    }
    double zlmid = 0.5 * (zlmax[ncut] + zlmin[ncut]);
    int nl = 0;
    int nr = nd-1;
    while(nl < nr){
      while(nl < nd && zgmid[nl][ncut] <= zlmid){
        nl = nl + 1;
      }
      while(nr >= 0 && zgmid[nr][ncut] > zlmid){
        nr = nr - 1;
      }
      if(nl < nr){
        for(id=0;id<ndim;id++){
          double nh = zgmid[nl][id];
          zgmid[nl][id] = zgmid[nr][id];
          zgmid[nr][id] = nh;
        }
        for(id=0;id<ndim;id++){
          int mh = face2node[nl][id];
          face2node[nl][id] = face2node[nr][id];
          face2node[nr][id] = mh;
        }
      }
    }
    
    if(nl == nd || nl == 0){
      fprintf (stdout, "nl = %ld, nr = %ld\n", nl, nr);
      nson = 0;
      st_clt = create_cluster(ndpth,nsrt,nd,ndim,nson);
    }else{
      nson = 2;
      st_clt = create_cluster(ndpth,nsrt,nd,ndim,nson);
      int nsrt1 = nsrt;
      int nd1 = nl;
      resultCTlist[st_clt].offsets[0] = create_ctree_ssgeom(resultCTlist[st_clt].offsets[0],zgmid,face2node,
					       ndpth,ndscd,nsrt1,nd1,md,ndim);

      nsrt1 = nsrt + nl;
      nd1 = nd - nl;
      resultCTlist[st_clt].offsets[1] = create_ctree_ssgeom(resultCTlist[st_clt].offsets[1],&zgmid[nl],&face2node[nl],
					       ndpth,ndscd,nsrt1,nd1,md,ndim);
    }
  }
  resultCTlist[st_clt].ndscd = nd;
  //bounding box
  double zeps = 1.0e-5;
  if(resultCTlist[st_clt].nnson > 0){
    for(id=0;id<ndim;id++){
      resultCTlist[st_clt].bmin[id] = resultCTlist[resultCTlist[st_clt].offsets[0]].bmin[id];
      resultCTlist[st_clt].bmax[id] = resultCTlist[resultCTlist[st_clt].offsets[0]].bmax[id];
    }
    for(il=1;il<resultCTlist[st_clt].nnson;il++){
      for(id=0;id<ndim;id++){
	      if(resultCTlist[resultCTlist[st_clt].offsets[il]].bmin[id] < resultCTlist[st_clt].bmin[id]){
	        resultCTlist[st_clt].bmin[id] = resultCTlist[resultCTlist[st_clt].offsets[il]].bmin[id];
	      }
	      if(resultCTlist[st_clt].bmax[id] < resultCTlist[resultCTlist[st_clt].offsets[il]].bmax[id]){
	        resultCTlist[st_clt].bmax[id] = resultCTlist[resultCTlist[st_clt].offsets[il]].bmax[id];
	      }
      }
    }
  }else{
    for(id=0;id<ndim;id++){
      resultCTlist[st_clt].bmin[id] = zgmid[0][id];
      resultCTlist[st_clt].bmax[id] = zgmid[0][id];
    }
    for(id=0;id<ndim;id++){
      for(il=1;il<resultCTlist[st_clt].nsize;il++){
	      if(zgmid[il][id] < resultCTlist[st_clt].bmin[id]){
	        resultCTlist[st_clt].bmin[id] = zgmid[il][id];
	      }
	      if(resultCTlist[st_clt].bmax[id] < zgmid[il][id]){
	        resultCTlist[st_clt].bmax[id] = zgmid[il][id];
	      }
      }
    }
  }
  double zwdth = (resultCTlist[st_clt].bmax[0] - resultCTlist[st_clt].bmin[0]) * (resultCTlist[st_clt].bmax[0] - resultCTlist[st_clt].bmin[0]);
  for(id=1;id<ndim;id++){
    zwdth = zwdth + (resultCTlist[st_clt].bmax[id] - resultCTlist[st_clt].bmin[id]) * (resultCTlist[st_clt].bmax[id] - resultCTlist[st_clt].bmin[id]);
  }
  zwdth = sqrt(zwdth);
  for(id=0;id<ndim;id++){
    double bdiff = resultCTlist[st_clt].bmax[id] - resultCTlist[st_clt].bmin[id];
    if(bdiff < zeps * zwdth){
      resultCTlist[st_clt].bmax[id] = resultCTlist[st_clt].bmax[id] + 0.5 * (zeps * zwdth - bdiff);
      resultCTlist[st_clt].bmin[id] = resultCTlist[st_clt].bmin[id] - 0.5 * (zeps * zwdth - bdiff);
    }
  }
  zwdth = (resultCTlist[st_clt].bmax[0] - resultCTlist[st_clt].bmin[0]) * (resultCTlist[st_clt].bmax[0] - resultCTlist[st_clt].bmin[0]);
  for(id=1;id<ndim;id++){
    zwdth += (resultCTlist[st_clt].bmax[id] - resultCTlist[st_clt].bmin[id]) * (resultCTlist[st_clt].bmax[id] - resultCTlist[st_clt].bmin[id]);
  }
  resultCTlist[st_clt].zwdth = sqrt(zwdth);
  //end of bounding box
  return st_clt;
}