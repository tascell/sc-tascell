// The routine to load a PPOpen BEM input file
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "bem_file.h"


#define BUFSIZE 10000
#define BI_BINARY_PREAMBLE "BI_BINARY"
#define BI_VTK_PREAMBLE \
  "# vtk DataFile Version 2.0\n" \
  "header : this data is results of BEM-BB\n" \
  "ASCII\n" \
  "DATASET UNSTRUCTURED_GRID"
#define BI_VTK_CELL_TYPE "5"
#define BI_VTK_PPOHBEM_SOL 0.0

/* For debugging */
void print_int64_ts (int64_t* vals, int64_t nval) {
  for (int64_t i=0; i<nval; i++) {
    printf ("%"PRId64": %"PRId64"\n", i, vals[i]); }}
void print_doubles (double* vals, int64_t nval) {
  for (int64_t i=0; i<nval; i++) {
    printf ("%"PRId64": %.16g\n", i, vals[i]); }}

/* Aux functions called by print_bem_input */
// print 1 int64_t value (val)
void print_bem_input_int64_t (FILE* fp, int64_t val,
			   enum bi_format fmt, const char* prt_string)
{
  switch (fmt) {
  case BI_TEXT:
    fprintf (fp, "%"PRId64"\n", val); break;
  case BI_BINARY:
    fwrite (&val, sizeof(int64_t), 1, fp); break;
  case BI_PRETTY:
    fprintf (fp, "%s %"PRId64"\n", prt_string ,val); break;
  default:
    fprintf (stderr, "Unknown bi_format in print_bem_input_int64_t!\n");
    return;
  }
}

// print int64_t values (vals)
// nval is # of values. Put a newline after each nsep values in text formsts.
// prt_string is printed on the head of each line (fmt==BI_VTK)
// or printed once before the values (fmt==BI_PRETTY)
void print_bem_input_int64_ts (FILE* fp, int64_t* vals, int64_t nval, int64_t nsep,
			    enum bi_format fmt, const char* prt_string)
{
  switch (fmt) {
  case BI_TEXT:
  case BI_VTK:
    for (int64_t i=0; i<nval; i++) {
      if (fmt == BI_VTK && i%nsep == 0) {
	fprintf (fp, "%"PRId64" ", nsep);
      }
      fprintf (fp, "%"PRId64"", vals[i]);
      fputc ((i%nsep == nsep-1 || i == nval-1)?'\n':' ', fp);
    }
    break;
  case BI_BINARY:
    fwrite (vals, sizeof(int64_t), nval, fp);
    break;
  case BI_PRETTY:
    fprintf (fp, "%s\n", prt_string);
    print_bem_input_int64_ts (fp, vals, nval, nsep, BI_TEXT, prt_string);
    break;
  default:
    fprintf (stderr, "Unknown bi_format in print_bem_input_int64_ts!\n");
    return;
  }
}

// print double values (vals)
// nval is # of values. Put a newline after each nsep values in text formsts.
void print_bem_input_doubles (FILE* fp, double* vals, int64_t nval, int64_t nsep,
			      enum bi_format fmt, const char* prt_string)
{
  switch (fmt) {
  case BI_TEXT:
    for (int64_t i=0; i<nval; i++) {
      fprintf (fp, "%.16g", vals[i]);
      putc ((i%nsep == nsep-1 || i == nval-1)?'\n':' ', fp);
    }
    break;
  case BI_BINARY:
    fwrite (vals, sizeof(double), nval, fp);
    break;
  case BI_PRETTY:
    fprintf (fp, "%s\n", prt_string);
    print_bem_input_doubles (fp, vals, nval, nsep, BI_TEXT, prt_string);
    break;
  default:
    fprintf (stderr, "Unknown bi_format in print_bem_input_doubles!\n");
    return;
  }
}


void print_bem_input_vtk (FILE* fp, struct bem_input* pbin)
{
  const enum bi_format fmt = BI_VTK;
  fprintf (fp, BI_VTK_PREAMBLE "\n");
  fprintf (fp, "POINTS %"PRId64" float\n", pbin->nNode);
  print_bem_input_doubles (fp, (double*)pbin->coordOfNode, 3*pbin->nNode, 3,
			   BI_TEXT, NULL);
  fprintf (fp, "CELLS %"PRId64" %"PRId64"\n", pbin->nFace, pbin->nFace*(1+pbin->nNodePerFace));
  print_bem_input_int64_ts (fp, pbin->idOfFace,
			 pbin->nNodePerFace*pbin->nFace,
			 pbin->nNodePerFace, fmt, "");
  fprintf (fp, "CELL_TYPES %"PRId64"\n", pbin->nFace);
  for (int i=0; i<pbin->nFace; i++){
    fprintf (fp, BI_VTK_CELL_TYPE "\n");
  }
  fprintf (fp, "CELL_DATA %"PRId64"\n", pbin->nFace);
  fprintf (fp, "SCALARS solve float 1\n" "LOOKUP_TABLE default\n");
  // 単に↓0.0でOK?
  for (int i=0; i<pbin->nFace; i++){
    fprintf (fp, "%lf\n", BI_VTK_PPOHBEM_SOL);
  }
  return;
}


/* Print BEM input (pbin) to (fp) in the specified format (fmt) */
void print_bem_input (FILE* fp, struct bem_input* pbin, enum bi_format fmt)
{
  // Check whether fmt is available value.
  switch (fmt) {
  case BI_TEXT:
  case BI_BINARY:
  case BI_VTK:
  case BI_PRETTY:
    break;
  default:
    fprintf (stderr, "Unknown bi_format in print_bem_input!\n");
    return;
  }
  // Use another function for BI_VTK
  if (fmt == BI_VTK) {
    print_bem_input_vtk (fp, pbin);
    return;
  }
  // Write preamble for BI_BINARY
  if (fmt == BI_BINARY) {
    fprintf (fp, BI_BINARY_PREAMBLE "\n");
  }
  // Print
  print_bem_input_int64_t (fp, pbin->nNode, fmt, "Number of nodes:");
  if ( fmt != BI_PRETTY ) {
    print_bem_input_doubles (fp, (double*)pbin->coordOfNode, 3*pbin->nNode, 3,
			     fmt, "Coordinates of the nodes:");
  }
  print_bem_input_int64_t (fp, pbin->nFace, fmt, "Number of faces:");
  print_bem_input_int64_t (fp, pbin->nNodePerFace, fmt, "Number of nodes for each face:");
  print_bem_input_int64_t (fp, pbin->nIFValue, fmt,
			"Number of int64_t values for each face:");
  print_bem_input_int64_t (fp, pbin->nDFValue, fmt,
			"Number of double values for each face:");
  if ( fmt != BI_PRETTY ) {
    print_bem_input_int64_ts (fp, pbin->idOfFace,
			   pbin->nNodePerFace*pbin->nFace,
			   pbin->nNodePerFace, fmt, "Node IDs forming faces:");
    print_bem_input_int64_ts (fp, pbin->IFValue, pbin->nFace*pbin->nIFValue, 1,
			   fmt, "Integer parameter values of faces:");
    print_bem_input_doubles (fp, pbin->DFValue, pbin->nFace*pbin->nDFValue, 1,
			     fmt, "Double parameter values of faces:");
  }
}

 
/* Aux functions called by read_bem_input */
// Read 1 int64_t value and store to (pval)
void read_bem_input_int64_t (FILE* fp, int64_t* pval, enum bi_format fmt)
{
  char line[BUFSIZE];
  switch (fmt) {
  case BI_TEXT: 
    fgets (line, BUFSIZE, fp);
    *pval = atol (line);
    break;
  case BI_BINARY:
    fread (pval, sizeof(int64_t), 1, fp);
    break;
  default:
    fprintf (stderr, "Unknown bi_format in read_bem_input_int64_t!\n");
    return;
  }
}

// Read int64_t values and store to (vals)
// nval is # of values. Gut a newline after each nsep values in text formsts.
void read_bem_input_int64_ts (FILE* fp, int64_t* vals, int64_t nval, int64_t nsep,
				enum bi_format fmt)
{
  char line[BUFSIZE];
  switch (fmt) {
  case BI_TEXT:
    {
      int64_t i=0;
      while (i<nval) {
	fgets (line, BUFSIZE, fp);
	char* pos = line;
	char* pos_nxt;
	for (int64_t j = 0; i<nval && j<nsep; i++,j++) {
	  vals[i] = strtol (pos, &pos_nxt, 10);
	  pos = pos_nxt;
	}
      }
    }
    break;
  case BI_BINARY:
    fread (vals, sizeof(int64_t), nval, fp);
    break;
  default:
    fprintf (stderr, "Unknown bi_format in read_bem_input_int64_ts!\n");
    return;
  }
}


// Read double values and store to (vals)
// nval is # of values. Gut a newline after each nsep values in text formsts.
void read_bem_input_doubles (FILE* fp, double* vals, int64_t nval, int64_t nsep,
				  enum bi_format fmt)
{
  char line[BUFSIZE];
  switch (fmt) {
  case BI_TEXT:
    {
      int64_t i=0;
      while (i<nval) {
	fgets (line, BUFSIZE, fp);
	// fputs (line, stderr);
	char* pos = line;
	char* pos_nxt;
	for (int64_t j = 0; i<nval && j<nsep; i++,j++) {
	  vals[i] = strtod (pos, &pos_nxt);
	  pos = pos_nxt;
	}
      }
    }
    break;
  case BI_BINARY:
    fread (vals, sizeof(double), nval, fp);
    break;
  default:
    fprintf (stderr, "Unknown bi_format in read_bem_input_doubles!\n");
    return;
  }
}


// Read BEM input data from fp and store it to the object pointed by pbin
// fp: file pointer of the input
// fmt: file format (BI_TEXT, BI_BINARY, BI_AUTO)
// Returns the format of input file if succeeded, returns -1 if failed.
enum bi_format read_bem_input (FILE* fp, struct bem_input* pbin, enum bi_format fmt)
{
  // Check and read preamble for BI_BINARY
  if (fmt == BI_BINARY || fmt == BI_AUTO) {
    int ch = fgetc(fp);
    ungetc (ch, fp);
    if ( ch != BI_BINARY_PREAMBLE[0] ) {
      if (fmt == BI_BINARY) {
	fprintf (stderr, "This file does not look a BEM input file in the binary format in read_bem_input!\n");
	return -1;
      }
      fmt = BI_TEXT;
    } else {
      const int bufsz = sizeof(BI_BINARY_PREAMBLE "\n")+1;
      char buf[bufsz]; // BI_BINARY_PREAMBLE + \n + \0
      fgets (buf, bufsz, fp);
      if ( strncmp(buf, BI_BINARY_PREAMBLE "\n", bufsz) ) {
	fprintf (stderr, "This file does not look a BEM input file in the binary format in read_bem_input!\n");
	return -1;
      }
      if (fmt == BI_AUTO) {
	fmt = BI_BINARY;
      }
    }
  }
  // Check whether fmt is available value.
  switch (fmt) {
  case BI_TEXT:
  case BI_BINARY:
    break;
  default:
    fprintf (stderr, "Unknown bi_format in read_bem_input!\n");
    return -1;
  }
  // Get nNode
  read_bem_input_int64_t (fp, &pbin->nNode, fmt);
  // Allocate and get coordOfNode
  pbin->coordOfNode = (double(*)[3]) malloc (pbin->nNode * 3 * sizeof(double));
  read_bem_input_doubles (fp, (double*)pbin->coordOfNode, pbin->nNode * 3, 3, fmt);
  // Get nFace
  read_bem_input_int64_t (fp, &pbin->nFace, fmt);
  // Get nNodePerFace
  read_bem_input_int64_t (fp, &pbin->nNodePerFace, fmt);
  // Get nIFValue and nDFValue
  read_bem_input_int64_t (fp, &pbin->nIFValue, fmt);
  read_bem_input_int64_t (fp, &pbin->nDFValue, fmt);
  // Allocate and get idOfFace
  pbin->idOfFace = (int64_t*)malloc (pbin->nFace * pbin->nNodePerFace * sizeof(int64_t));
  read_bem_input_int64_ts (fp, pbin->idOfFace, pbin->nFace * pbin->nNodePerFace,
			     pbin->nNodePerFace, fmt);
  // Allocate and calculate coordOfFace
  pbin->coordOfFace = (double(*)[3]) malloc (pbin->nFace * 3 * sizeof(double));
  pbin->face2node = (int(*)[3]) malloc (pbin->nFace * 3 * sizeof(int));
  for(int64_t i=0; i < pbin->nFace; i++){
    const int64_t ncpf = pbin->nNodePerFace;
    double (* const con)[3] = pbin->coordOfNode;
    // Initialize coordOfFace
    pbin->coordOfFace[i][0] = pbin->coordOfFace[i][1] = pbin->coordOfFace[i][2] = 0.0;
    // Sum up all coordinates of the i-th face
    for (int64_t j = 0; j<ncpf; j++) {
      int64_t pid = pbin->idOfFace[ncpf*i+j];
      if (pid < 0 || pid >= pbin->nNode) {
	fprintf (stderr, "Illegal Node ID: %"PRId64" at %"PRId64" th ID of %"PRId64" th Face\n"
		 "(# Nodes is %"PRId64". # Faces is %"PRId64". # nodes per face is %"PRId64".)\n.",
		 pid, j, i, pbin->nNode, pbin->nFace, pbin->nNodePerFace);
	exit (99);
      }
      pbin->coordOfFace[i][0] += con[pid][0];
      pbin->coordOfFace[i][1] += con[pid][1];
      pbin->coordOfFace[i][2] += con[pid][2];
      pbin->face2node[i][j] = (int)pid;
    }
    // Divide by ncpf to calcuate the average
    pbin->coordOfFace[i][0] /= (double) ncpf;
    pbin->coordOfFace[i][1] /= (double) ncpf;
    pbin->coordOfFace[i][2] /= (double) ncpf;
  }
  // Allocate and get IFValue
  pbin->IFValue = (int64_t*) malloc (sizeof(int64_t)* pbin->nIFValue * pbin->nFace);
  read_bem_input_int64_ts (fp, pbin->IFValue, pbin->nIFValue * pbin->nFace, 1, fmt);
  // Allocate and get DFValue
  pbin->DFValue = (double*) malloc (sizeof(double)* pbin->nDFValue * pbin->nFace);
  read_bem_input_doubles (fp, pbin->DFValue, pbin->nDFValue * pbin->nFace, 1, fmt);
  return fmt;
}

enum bi_format open_and_read_bem_input (char* ifile, struct bem_input* pbin, enum bi_format fmt)
{
  enum bi_format fmt_ret;
  FILE *fpin;
  if ( !(fpin = fopen (ifile, "r")) ) {
    fprintf (stderr, "Input file %s open error!\n", ifile);
    exit (99);
  }
  fmt_ret = read_bem_input (fpin, pbin, fmt);
  fclose (fpin);
  return fmt_ret;
}

// Load test
#ifdef TEST
int main (void)
{
  struct bem_input bem_in;
  // Read text file
  FILE* fpin = fopen ("input.txt", "r");
  read_bem_input (fpin, &bem_in, BI_AUTO);
  fclose (fpin);
#if 0
  // Pretty print
  print_bem_input (stdout, &bem_in, BI_PRETTY);
  // Write as binary file
  FILE* fpout = fopen ("input.txt.bin", "w");
  print_bem_input (fpout, &bem_in, BI_BINARY);
  fclose (fpout);
  // Read the binary file
  FILE* fpin_b = fopen ("input.txt.bin", "r");
  read_bem_input (fpin_b, &bem_in, BI_AUTO);
  fclose (fpin_b);
  // Pretty print
  print_bem_input (stdout, &bem_in, BI_PRETTY);
#endif
  // Write a vti file
  FILE* fp_vtk = fopen ("input.txt.vtk", "w");
  print_bem_input (fp_vtk, &bem_in, BI_VTK);
  fclose (fp_vtk);
}
#endif
