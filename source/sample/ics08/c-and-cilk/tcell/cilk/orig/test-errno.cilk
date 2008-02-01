/*
 * This program checks that errno works correctly.  
 *
 * On Solaris, multithreaded programs require -D_REENTRANT for errno
 * to work.  If we forget -D_REENTRANT, this program will fail.
 *
 * Thanks to Kartik K. Agaram <akkartik@cs.utexas.edu> for the program.
 */

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

cilk int main(int argc, char *argv[])
{
     int fd;

     fd = open ("tHiS.fILe.DOES.not.exIST", O_RDONLY, 0644) ;

     if (fd == -1 && errno == ENOENT) {
	  printf("OK\n");
	  return 0;
     } else {
	  printf("BAD %d %d\n", fd, errno);
	  return -1;
     }
}
