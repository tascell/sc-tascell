//  haku parallel distributed

#include <stdint.h>
#include <sys/time.h>

enum bi_format {
  BI_TEXT,      // oroginal text format
  BI_BINARY,    // binary format
  BI_VTK,       // vtk text format (only for print, for visualising using ParaView)
  BI_PRETTY,    // pretty print (only for print)
  BI_AUTO,      // automatic select (only for read)
};

struct bem_input {
  int64_t nNode;                      // # of nodes
  double (*coordOfNode)[3];           // coordOfnode[i][d] is d-th dimension of i-th coordinate
                                      // where 0 <= i < nNode and 0 <= d < 3
  int64_t nFace;                      // # of faces
  int64_t nNodePerFace;               // # of nodes that form each face     
  int64_t nIFValue;                   // # of integer parameter values for each face
  int64_t nDFValue;                   // # of double parameter values for each face
  int64_t *idOfFace;                  // idOfFace[i*nNodePerFace+j] is j-th node ID of i-th face
                                      // where 0 <= i < nFace and 0 <= j < jNodePerFace
  double (*coordOfFace)[3];           // coordOfFace[i][d] is d-th dimension of i-th face's center
                                      // where 0 <= i < nFace and 0 <= d < 3
  int64_t *IFValue;                   // IFValue[i*nFace+j] is i-th integer parameter value
                                      // of j-th face where 0 <= i < nFace and 0 <= j < nIFValue
  double *DFValue;                    // DFValue[i*nFace+j] is i-th double parameter value
                                      // of j-th face where 0 <= i <nFace and 0 <= j < nIDValue
  int (*face2node)[3];            // nodes which compose face
};

/* functions defined in bem_file.c */
void print_bem_input (FILE* fp, struct bem_input* pbin, enum bi_format fmt);
enum bi_format read_bem_input (FILE* fp, struct bem_input* pbin, enum bi_format fmt);
enum bi_format open_and_read_bem_input (char *ifile, struct bem_input* pbin, enum bi_format fmt);


/* functions defined in bem_aux.c */
double show_elapsed_time (struct timeval *tv1, struct timeval *tv2);
void open_write_show (const int flag, struct bem_input *pbin,
		      const enum bi_format fmt, const char* ofile);
