typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short	__int16_t;
typedef unsigned short __uint16_t;
typedef int	__int32_t;
typedef unsigned int __uint32_t;
typedef int	__int64_t;
typedef unsigned int __uint64_t;
typedef unsigned long __clock_t;
typedef unsigned int __cpumask_t;
typedef __int32_t __critical_t;
typedef double	__double_t;
typedef double	__float_t;
typedef __int32_t __intfptr_t;
typedef __int64_t __intmax_t;
typedef __int32_t __intptr_t;
typedef __int32_t __int_fast8_t;
typedef __int32_t __int_fast16_t;
typedef __int32_t __int_fast32_t;
typedef __int64_t __int_fast64_t;
typedef __int8_t __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;
typedef __int64_t __int_least64_t;
typedef __int32_t __ptrdiff_t;
typedef __int32_t __register_t;
typedef __int32_t __segsz_t;
typedef __uint32_t __size_t;
typedef __int32_t __ssize_t;
typedef __int32_t __time_t;
typedef __uint32_t __uintfptr_t;
typedef __uint64_t __uintmax_t;
typedef __uint32_t __uintptr_t;
typedef __uint32_t __uint_fast8_t;
typedef __uint32_t __uint_fast16_t;
typedef __uint32_t __uint_fast32_t;
typedef __uint64_t __uint_fast64_t;
typedef __uint8_t __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;
typedef __uint64_t __uint_least64_t;
typedef __uint32_t __u_register_t;
typedef __uint32_t __vm_offset_t;
typedef __int64_t __vm_ooffset_t;
typedef __uint32_t __vm_paddr_t;
typedef __uint64_t __vm_pindex_t;
typedef __uint32_t __vm_size_t;
typedef __builtin_va_list __va_list;
typedef __va_list __gnuc_va_list;
typedef __uint32_t __blksize_t;
typedef __int64_t __blkcnt_t;
typedef __int32_t __clockid_t;
typedef __uint32_t __fflags_t;
typedef __uint64_t __fsblkcnt_t;
typedef __uint64_t __fsfilcnt_t;
typedef __uint32_t __gid_t;
typedef __int64_t __id_t;
typedef __uint32_t __ino_t;
typedef long	__key_t;
typedef __int32_t __lwpid_t;
typedef __uint16_t __mode_t;
typedef int	__nl_item;
typedef __uint16_t __nlink_t;
typedef __int64_t __off_t;
typedef __int32_t __pid_t;
typedef __int64_t __rlim_t;
typedef __uint8_t __sa_family_t;
typedef __uint32_t __socklen_t;
typedef long	__suseconds_t;
typedef __int32_t __timer_t;
typedef __uint32_t __uid_t;
typedef unsigned int __useconds_t;
typedef int	__ct_rune_t;
typedef __ct_rune_t __rune_t;
typedef __ct_rune_t __wchar_t;
typedef __ct_rune_t __wint_t;
typedef __uint32_t __dev_t;
typedef __uint32_t __fixpt_t;

union unnamed0 {
  char		  __mbstate8[128];
  __int64_t	  _mbstateL;
};
typedef union unnamed0 __mbstate_t;
typedef __off_t	fpos_t;
typedef __size_t size_t;
typedef __va_list va_list;

struct __sbuf {
  unsigned char  *_base;
  int		  _size;
};

struct __sFILE {
  unsigned char  *_p;
  int		  _r;
  int		  _w;
  short		  _flags;
  short		  _file;
  struct __sbuf	  _bf;
  int		  _lbfsize;
  void           *_cookie;
  int             (*_close) (void *);
  int             (*_read) (void *, char *, int);
  		  fpos_t      (*_seek) (void *, fpos_t, int);
  int             (*_write) (void *, char const *, int);
  struct __sbuf	  _ub;
  struct __sFILEX *_extra;
  int		  _ur;
  unsigned char	  _ubuf[3];
  unsigned char	  _nbuf[1];
  struct __sbuf	  _lb;
  int		  _blksize;
  fpos_t	  _offset;
};
typedef struct __sFILE FILE;
extern FILE    *__stdinp;
extern FILE    *__stdoutp;
extern FILE    *__stderrp;

void		clearerr  (FILE *);

int		fclose     (FILE *);

int		feof       (FILE *);

int		ferror     (FILE *);

int		fflush     (FILE *);

int		fgetc      (FILE *);

int		fgetpos    (FILE *, fpos_t *);

char           *fgets(char *, int, FILE *);

FILE           *fopen(char const *, char const *);

int		fprintf    (FILE *, char const *,...);

int		fputc      (int, FILE *);

int		fputs      (char const *, FILE *);

size_t		fread   (void *, size_t, size_t, FILE *);

FILE           *freopen(char const *, char const *, FILE *);

int		fscanf     (FILE *, char const *,...);

int		fseek      (FILE *, long, int);

int		fsetpos    (FILE *, fpos_t const *);

long		ftell     (FILE *);

size_t		fwrite  (void const *, size_t, size_t, FILE *);

int		getc       (FILE *);

int		getchar    (void);

char           *gets(char *);

void		perror    (char const *);

int		printf     (char const *,...);

int		putc       (int, FILE *);

int		putchar    (int);

int		puts       (char const *);

int		remove     (char const *);

int		rename     (char const *, char const *);

void		rewind    (FILE *);

int		scanf      (char const *,...);

void		setbuf    (FILE *, char *);

int		setvbuf    (FILE *, char *, int, size_t);

int		sprintf    (char *, char const *,...);

int		sscanf     (char const *, char const *,...);

FILE           *tmpfile(void);

char           *tmpnam(char *);

int		ungetc     (int, FILE *);

int		vfprintf   (FILE *, char const *, __va_list);

int		vprintf    (char const *, __va_list);

int		vsprintf   (char *, char const *, __va_list);

int		snprintf   (char *, size_t, char const *,...);

int		vfscanf    (FILE *, char const *, __va_list);

int		vscanf     (char const *, __va_list);

int		vsnprintf  (char *, size_t, char const *, __va_list);

int		vsscanf    (char const *, char const *, __va_list);

char           *ctermid(char *);

FILE           *fdopen(int, char const *);

int		fileno     (FILE *);

int		pclose     (FILE *);

FILE           *popen(char const *, char const *);

int		ftrylockfile(FILE *);

void		flockfile (FILE *);

void		funlockfile(FILE *);

int		getc_unlocked(FILE *);

int		getchar_unlocked(void);

int		putc_unlocked(int, FILE *);

int		putchar_unlocked(int);

void		clearerr_unlocked(FILE *);

int		feof_unlocked(FILE *);

int		ferror_unlocked(FILE *);

int		fileno_unlocked(FILE *);

int		fseeko     (FILE *, __off_t, int);

__off_t		ftello (FILE *);

int		getw       (FILE *);

int		putw       (int, FILE *);

char           *tempnam(char const *, char const *);

int		asprintf   (char **, char const *,...);

char           *ctermid_r(char *);

char           *fgetln(FILE *, size_t *);

char const     *fmtcheck(char const *, char const *);

int		fpurge     (FILE *);

void		setbuffer (FILE *, char *, int);

int		setlinebuf (FILE *);

int		vasprintf  (char **, char const *, __va_list);
extern int const sys_nerr;
extern char const *const sys_errlist[];

FILE           *funopen(void const *, int (*) (void *, char *, int), int (*) (void *, char const *, int), fpos_t(*) (void *, fpos_t, int), int (*) (void *));

int		ftruncate  (int, __off_t);

__off_t		lseek  (int, __off_t, int);

void           *mmap(void *, size_t, int, int, int, __off_t);

int		truncate   (char const *, __off_t);

int		__srget    (FILE *);

int		__swbuf    (int, FILE *);

static int	__sputc(int _c, FILE * _p)
{
  if (--(*_p)._w >= 0 || (*_p)._w >= (*_p)._lbfsize && (char)_c != '\n')
    return *((*_p)._p)++ = _c;
  else
    return __swbuf(_c, _p);
}
extern int	__isthreaded;
typedef __rune_t rune_t;
typedef __wchar_t wchar_t;

struct unnamed1 {
  int		  quot;
  int		  rem;
};
typedef struct unnamed1 div_t;

struct unnamed2 {
  long		  quot;
  long		  rem;
};
typedef struct unnamed2 ldiv_t;
extern int	__mb_cur_max;

void		abort     (void);

int		abs        (int);

int		atexit     (void (*) (void));

double		atof    (char const *);

int		atoi       (char const *);

long		atol      (char const *);

void           *bsearch(void const *, void const *, size_t, size_t, int (*) (void const *, void const *));

void           *calloc(size_t, size_t);

div_t		div      (int, int);

void		exit      (int);

void		free      (void *);

char           *getenv(char const *);

long		labs      (long);

ldiv_t		ldiv    (long, long);

void           *malloc(size_t);

int		mblen      (char const *, size_t);

size_t		mbstowcs(wchar_t *, char const *, size_t);

int		mbtowc     (wchar_t *, char const *, size_t);

void		qsort     (void *, size_t, size_t, int (*) (void const *, void const *));

int		rand       (void);

void           *realloc(void *, size_t);

void		srand     (unsigned int);

double		strtod  (char const *, char **);

float		strtof   (char const *, char **);

long		strtol    (char const *, char **, int);

long double	strtold(char const *, char **);

unsigned long	strtoul(char const *, char **, int);

int		system     (char const *);

int		wctomb     (char *, wchar_t);

size_t		wcstombs(char *, wchar_t const *, size_t);

struct unnamed3 {
  long long	  quot;
  long long	  rem;
};
typedef struct unnamed3 lldiv_t;

long long	atoll (char const *);

long long	llabs (long long);

lldiv_t		lldiv  (long long, long long);

long long	strtoll(char const *, char **, int);

unsigned long long strtoull(char const *, char **, int);

void		_Exit     (int);

int		rand_r     (unsigned int *);

int		setenv     (char const *, char const *, int);

void		unsetenv  (char const *);

long		a64l      (char const *);

double		drand48 (void);

double		erand48 (unsigned short[3]);

int		getsubopt  (char **, char *const *, char **);

int		grantpt    (int);

char           *initstate(unsigned long, char *, long);

long		jrand48   (unsigned short[3]);

char           *l64a(long);

void		lcong48   (unsigned short[7]);

long		lrand48   (void);

int		mkstemp    (char *);

char           *mktemp(char *);

long		mrand48   (void);

long		nrand48   (unsigned short[3]);

int		posix_openpt(int);

char           *ptsname(int);

int		putenv     (char const *);

long		random    (void);

char           *realpath(char const *, char[]);

unsigned short *seed48(unsigned short[3]);

int		setkey     (char const *);

char           *setstate(char *);

void		srand48   (long);

void		srandom   (unsigned long);

int		unlockpt   (int);
extern char const *_malloc_options;
extern void     (*_malloc_message) (char const *, char const *, char const *, char const *);

__uint32_t	arc4random(void);

void		arc4random_addrandom(unsigned char *dat, int datlen);

void		arc4random_stir(void);

char           *getbsize(int *, long *);

char           *cgetcap(char *, char const *, int);

int		cgetclose  (void);

int		cgetent    (char **, char **, char const *);

int		cgetfirst  (char **, char **);

int		cgetmatch  (char const *, char const *);

int		cgetnext   (char **, char **);

int		cgetnum    (char *, char const *, long *);

int		cgetset    (char const *);

int		cgetstr    (char *, char const *, char **);

int		cgetustr   (char *, char const *, char **);

int		daemon     (int, int);

char           *devname(int, int);

char           *devname_r(int, int, char *, int);

int		getloadavg (double[], int);

char const     *getprogname(void);

int		heapsort   (void *, size_t, size_t, int (*) (void const *, void const *));

int		l64a_r     (long, char *, int);

int		mergesort  (void *, size_t, size_t, int (*) (void const *, void const *));

void		qsort_r   (void *, size_t, size_t, void *, int (*) (void *, void const *, void const *));

int		radixsort  (unsigned char const **, int, unsigned char const *, unsigned int);

void           *reallocf(void *, size_t);

int		rpmatch    (char const *);

void		setprogname(char const *);

int		sradixsort (unsigned char const **, int, unsigned char const *, unsigned int);

void		sranddev  (void);

void		srandomdev(void);

long long	strtonum(char const *, long long, long long, char const **);

__int64_t	strtoq(char const *, char **, int);

__uint64_t	strtouq(char const *, char **, int);
extern char    *suboptarg;
typedef __suseconds_t suseconds_t;
typedef __time_t time_t;

struct timeval {
  long		  tv_sec;
  suseconds_t	  tv_usec;
};

static __uint64_t __bswap64(__uint64_t _x)
{
  return _x >> 56 | _x >> 40 & 65280 | _x >> 24 & 16711680 | _x >> 8 & 4278190080 | _x << 8 & (__uint64_t) 255 << 32 | _x << 24 & (__uint64_t) 255 << 40 | _x << 40 & (__uint64_t) 255 << 48 | _x << 56;
}
typedef struct pthread *pthread_t;
typedef struct pthread_attr *pthread_attr_t;
typedef struct pthread_mutex *pthread_mutex_t;
typedef struct pthread_mutex_attr *pthread_mutexattr_t;
typedef struct pthread_cond *pthread_cond_t;
typedef struct pthread_cond_attr *pthread_condattr_t;
typedef int	pthread_key_t;
typedef struct pthread_once pthread_once_t;
typedef struct pthread_rwlock *pthread_rwlock_t;
typedef struct pthread_rwlockattr *pthread_rwlockattr_t;
typedef struct pthread_barrier *pthread_barrier_t;
typedef struct pthread_barrierattr *pthread_barrierattr_t;
typedef struct pthread_spinlock *pthread_spinlock_t;
typedef void   *pthread_addr_t;
typedef void   *(*pthread_startroutine_t) (void *);

struct pthread_once {
  int		  state;
  pthread_mutex_t mutex;
};
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef __int8_t int8_t;
typedef __int16_t int16_t;
typedef __int32_t int32_t;
typedef __int64_t int64_t;
typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
typedef __intptr_t intptr_t;
typedef __uintptr_t uintptr_t;
typedef __uint8_t u_int8_t;
typedef __uint16_t u_int16_t;
typedef __uint32_t u_int32_t;
typedef __uint64_t u_int64_t;
typedef __uint64_t u_quad_t;
typedef __int64_t quad_t;
typedef quad_t *qaddr_t;
typedef char   *caddr_t;
typedef char const *c_caddr_t;
typedef char volatile *v_caddr_t;
typedef __blksize_t blksize_t;
typedef __blkcnt_t blkcnt_t;
typedef __clock_t clock_t;
typedef __clockid_t clockid_t;
typedef __critical_t critical_t;
typedef __int64_t daddr_t;
typedef __dev_t	dev_t;
typedef __fflags_t fflags_t;
typedef __fixpt_t fixpt_t;
typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;
typedef __gid_t	gid_t;
typedef __uint32_t in_addr_t;
typedef __uint16_t in_port_t;
typedef __id_t	id_t;
typedef __ino_t	ino_t;
typedef __key_t	key_t;
typedef __lwpid_t lwpid_t;
typedef __mode_t mode_t;
typedef __nlink_t nlink_t;
typedef __off_t	off_t;
typedef __pid_t	pid_t;
typedef __register_t register_t;
typedef __rlim_t rlim_t;
typedef __segsz_t segsz_t;
typedef __ssize_t ssize_t;
typedef __timer_t timer_t;
typedef __u_register_t u_register_t;
typedef __uid_t	uid_t;
typedef __useconds_t useconds_t;
typedef __vm_offset_t vm_offset_t;
typedef __vm_ooffset_t vm_ooffset_t;
typedef __vm_paddr_t vm_paddr_t;
typedef __vm_pindex_t vm_pindex_t;
typedef __vm_size_t vm_size_t;

struct __sigset {
  __uint32_t	  __bits[4];
};
typedef struct __sigset __sigset_t;

struct timespec {
  time_t	  tv_sec;
  long		  tv_nsec;
};
typedef unsigned long __fd_mask;
typedef __fd_mask fd_mask;
typedef __sigset_t sigset_t;

struct fd_set {
  __fd_mask	  __fds_bits[((unsigned int)1024 + (sizeof(__fd_mask) * 8 - 1)) / (sizeof(__fd_mask) * 8)];
};
typedef struct fd_set fd_set;

int		pselect    (int, fd_set *, fd_set *, fd_set *, struct timespec const *, sigset_t const *);

int		select     (int, fd_set *, fd_set *, fd_set *, struct timeval *);

struct timezone {
  int		  tz_minuteswest;
  int		  tz_dsttime;
};

struct bintime {
  time_t	  sec;
  uint64_t	  frac;
};

static void	bintime_addx(struct bintime *bt, uint64_t x)
{
  uint64_t	  u;

  u = (*bt).frac;
  (*bt).frac += x;
  if (u > (*bt).frac)
    ((*bt).sec)++;
}

static void	bintime_add(struct bintime *bt, struct bintime const *bt2)
{
  uint64_t	  u;

  u = (*bt).frac;
  (*bt).frac += (*bt2).frac;
  if (u > (*bt).frac)
    ((*bt).sec)++;
  (*bt).sec += (*bt2).sec;
}

static void	bintime_sub(struct bintime *bt, struct bintime const *bt2)
{
  uint64_t	  u;

  u = (*bt).frac;
  (*bt).frac -= (*bt2).frac;
  if (u < (*bt).frac)
    ((*bt).sec)--;
  (*bt).sec -= (*bt2).sec;
}

static void	bintime2timespec(struct bintime const *bt, struct timespec *ts)
{
  (*ts).tv_sec = (*bt).sec;
  (*ts).tv_nsec = (uint64_t) 1000000000 *(uint32_t) ((*bt).frac >> 32) >> 32;
}

static void	timespec2bintime(struct timespec const *ts, struct bintime *bt)
{
  (*bt).sec = (*ts).tv_sec;
  (*bt).frac = (*ts).tv_nsec * (uint64_t) ((long long)18446744073);
}

static void	bintime2timeval(struct bintime const *bt, struct timeval *tv)
{
  (*tv).tv_sec = (*bt).sec;
  (*tv).tv_usec = (uint64_t) 1000000 *(uint32_t) ((*bt).frac >> 32) >> 32;
}

static void	timeval2bintime(struct timeval const *tv, struct bintime *bt)
{
  (*bt).sec = (*tv).tv_sec;
  (*bt).frac = (*tv).tv_usec * (uint64_t) ((long long)18446744073709);
}

struct itimerval {
  struct timeval  it_interval;
  struct timeval  it_value;
};

struct clockinfo {
  int		  hz;
  int		  tick;
  int		  spare;
  int		  stathz;
  int		  profhz;
};

struct tm {
  int		  tm_sec;
  int		  tm_min;
  int		  tm_hour;
  int		  tm_mday;
  int		  tm_mon;
  int		  tm_year;
  int		  tm_wday;
  int		  tm_yday;
  int		  tm_isdst;
  long		  tm_gmtoff;
  char           *tm_zone;
};
extern char    *tzname[];

char           *asctime(struct tm const *);

clock_t		clock  (void);

char           *ctime(time_t const *);

double		difftime(time_t, time_t);

struct tm      *gmtime(time_t const *);

struct tm      *localtime(time_t const *);

time_t		mktime  (struct tm *);

size_t		strftime(char *, size_t, char const *, struct tm const *);

time_t		time    (time_t *);

void		tzset     (void);

int		clock_getres(clockid_t, struct timespec *);

int		clock_gettime(clockid_t, struct timespec *);

int		clock_settime(clockid_t, struct timespec const *);

int		nanosleep  (struct timespec const *, struct timespec *);

char           *asctime_r(struct tm const *, char *);

char           *ctime_r(time_t const *, char *);

struct tm      *gmtime_r(time_t const *, struct tm *);

struct tm      *localtime_r(time_t const *, struct tm *);

char           *strptime(char const *, char const *, struct tm *);

char           *timezone(int, int);

void		tzsetwall (void);

time_t		timelocal(struct tm *const);

time_t		timegm  (struct tm *const);

int		adjtime    (struct timeval const *, struct timeval *);

int		futimes    (int, struct timeval const *);

int		getitimer  (int, struct itimerval *);

int		gettimeofday(struct timeval *, struct timezone *);

int		lutimes    (char const *, struct timeval const *);

int		setitimer  (int, struct itimerval const *, struct itimerval *);

int		settimeofday(struct timeval const *, struct timezone const *);

int		utimes     (char const *, struct timeval const *);

int		bcmp       (void const *, void const *, size_t);

void		bcopy     (void const *, void *, size_t);

void		bzero     (void *, size_t);

int		ffs        (int);

int		ffsl       (long);

int		fls        (int);

int		flsl       (long);

char           *index(char const *, int);

char           *rindex(char const *, int);

int		strcasecmp (char const *, char const *);

int		strncasecmp(char const *, char const *, size_t);

void           *memccpy(void *, void const *, int, size_t);

void           *memchr(void const *, int, size_t);

int		memcmp     (void const *, void const *, size_t);

void           *memcpy(void *, void const *, size_t);

void           *memmem(void const *, size_t, void const *, size_t);

void           *memmove(void *, void const *, size_t);

void           *memset(void *, int, size_t);

char           *stpcpy(char *, char const *);

char           *strcasestr(char const *, char const *);

char           *strcat(char *, char const *);

char           *strchr(char const *, int);

int		strcmp     (char const *, char const *);

int		strcoll    (char const *, char const *);

char           *strcpy(char *, char const *);

size_t		strcspn (char const *, char const *);

char           *strdup(char const *);

char           *strerror(int);

int		strerror_r (int, char *, size_t);

size_t		strlcat (char *, char const *, size_t);

size_t		strlcpy (char *, char const *, size_t);

size_t		strlen  (char const *);

void		strmode   (int, char *);

char           *strncat(char *, char const *, size_t);

int		strncmp    (char const *, char const *, size_t);

char           *strncpy(char *, char const *, size_t);

char           *strnstr(char const *, char const *, size_t);

char           *strpbrk(char const *, char const *);

char           *strrchr(char const *, int);

char           *strsep(char **, char const *);

char           *strsignal(int);

size_t		strspn  (char const *, char const *);

char           *strstr(char const *, char const *);

char           *strtok(char *, char const *);

char           *strtok_r(char *, char const *, char **);

size_t		strxfrm (char *, char const *, size_t);

void		swab      (void const *, void *, ssize_t);
typedef void    (*sht) (void);
typedef double	align_t;
//typedef unsigned int size_t;

struct desc_struct {
  size_t	  size;
  size_t	  asize;
  int		  fli_len;
  size_t         *fli;
};
typedef struct desc_struct descriptor;
typedef descriptor *desc_t;

struct array_object {
  desc_t	  d;
  int		  length;
  size_t	  el_size;
  size_t	  size;
  size_t	  asize;
  char		  body    [0];
};

extern void    *getmem(sht scan, size_t size);

extern void    *move(void *vp);

void           *MEMCPY(void *d, void const *s, size_t sz)
{
  long           *_des = (long *)d;
  long           *_src = (long *)s;
  long           *_til = (long *)((char *)_src + sz);

  do {
    *_des++ = *_src++;
  } while (_src < _til);
}

static void	error(char *s)
{
  printf("ERROR: %s\n", s);
  exit(1);
}

static void    *myalloc(size_t size)
{
  void           *p;

  p = malloc(size);
  if (p == (void *)0)
    error("Not enough memory.");
  return p;
}

static void    *myrealloc(void *p, size_t size)
{
  void           *q;

  q = realloc(p, size);
  if (q == (void *)0)
    error("Not enough memory.");
  return q;
}

struct gc_params_struct {
  int		  gcv;
  int		  gctype;
  size_t	  tosize;
  size_t	  stack_size;
  size_t	  limited_stack_max;
};
typedef struct gc_params_struct gc_params;
static gc_params params;
static int	allocated_size;
static char    *old_memory;
static char    *new_memory;
static char    *old_memory_end;
static char    *new_memory_end;
static char    *b;

void           *move(void *vp)
{
  char           *p = vp;
  void           *fwp;
  size_t	  tag;
  desc_t	  d;
  struct array_object *ao;
  size_t	  size;
  size_t	  asize;
  char           *np;
  char           *nb;

  if (!((unsigned int)((char *)p - old_memory) < params.tosize))
    return vp;
  tag = 3 & *((size_t *) p);
  fwp = (void *)(tag ^ *((size_t *) p));
  if ((unsigned int)((char *)fwp - new_memory) < params.tosize)
    return fwp;
  switch (tag) {
  case 0:
    d = (desc_t) fwp;
    size = (*d).size;
    asize = (*d).asize;
    break;
  case 1:
  case 3:
    ao = (struct array_object *)vp;
    size = (*ao).size;
    asize = (*ao).asize;
    break;
  default:
    error("Illegal type ID!");
  }
  np = b;
  nb = np + asize;
  if (nb >= new_memory_end)
    error("buffer overrun.");
  MEMCPY(np, p, size);
  b = nb;
  *((void **)p) = np;
  return np;
}

void		gc_breadth_first(sht scan)
{
  int		  i;
  char           *tmp;
  size_t	  tag;
  int		  len;
  void          **link;
  desc_t	  d;
  struct array_object *ao;
  size_t	  el_size;
  char           *p;
  char           *s;

  if (params.gcv)
    printf("BREADTH-FIRST-GC start\n");
  b = new_memory;
  s = b;
  scan();
  while (s < b) {
    tag = 3 & *((size_t *) s);
    switch (tag) {
    case 0:
      d = (desc_t) (tag ^ *((size_t *) s));
      p = (char *)s;
      len = (*d).fli_len;
      for (i = 0; i < len; ++i) {
	link = (void **)(p + ((*d).fli)[i]);
	*link = move(*link);
      }
      s += (*d).asize;
      break;
    case 1:
      ao = (struct array_object *)s;
      p = (*ao).body;
      len = (*ao).length;
      el_size = (*ao).el_size;
      for (i = 0; i < len; ++i) {
	link = (void **)p;
	*link = move(*link);
	p += el_size;
      }
      s += (*ao).asize;
      break;
    case 3:
      ao = (struct array_object *)s;
      s += (*ao).asize;
      break;
    default:
      error("Illegal type ID!");
    }
  }
  allocated_size = b - new_memory;
  tmp = new_memory;
  new_memory = old_memory;
  old_memory = tmp;
  tmp = new_memory_end;
  new_memory_end = old_memory_end;
  old_memory_end = tmp;
  if (params.gcv)
    printf("GC complete (%d)\n", allocated_size);
}
double		gc_ttime;
struct timeval	tp1;
struct timeval	tp2;

void		gc        (sht scan)
{
  gettimeofday(&tp1, 0);
  switch (params.gctype) {
  case 0:
    gc_breadth_first(scan);
    break;
  }
  gettimeofday(&tp2, 0);
  gc_ttime += tp2.tv_sec - tp1.tv_sec + (tp2.tv_usec - tp1.tv_usec) * 1.0e-6;
}

void		getmem_init(gc_params p)
{
  char           *mp;
  static int	  called = 0;

  if (called)
    return;
  else
    called = 1;
  gc_ttime = 0.0;
  params = p;
  if (params.tosize == 0)
    params.tosize = 5 * 1024 * 1024;
  params.tosize += 3;
  params.tosize -= params.tosize & 3;
  if (params.stack_size == 0)
    params.stack_size = params.tosize / sizeof(double);
  if (params.limited_stack_max == 0)
    params.limited_stack_max = 32;
  printf("tosize=%d, stack=%d, limit=%d\n", params.tosize, params.stack_size, params.limited_stack_max);
  old_memory = myalloc(params.tosize);
  old_memory_end = old_memory + params.tosize;
  new_memory = myalloc(params.tosize);
  new_memory_end = new_memory + params.tosize;
  allocated_size = 0;
}

void		gc_init   (int tp, int tosize, int stack_size, int limited_max)
{
  gc_params	  p;

  p.gcv = 1;
  p.gctype = tp;
  p.tosize = tosize;
  p.stack_size = stack_size;
  p.limited_stack_max = limited_max;
  getmem_init(p);
}

void           *try_getmem(size_t size)
{
  char           *p;

  if (allocated_size + size > params.tosize)
    return 0;
  p = old_memory + allocated_size;
  allocated_size += size;
  memset(p, 0, size);
  return p;
}

void           *getmem(sht scan, size_t size)
{
  void           *p;

  p = try_getmem(size);
  if (p == 0) {
    gc(scan);
    p = try_getmem(size);
    if (p == 0)
      error("getmem: No enough memory.");
  }
  return p;
}

int		main       (int argc, char **argv)
{
  struct array_object *hsc_argv;
  size_t	  hsc_argv_size;
  size_t	  hsc_argv_asize;
  int		  i;
  struct array_object **body;
  int		  len;
  size_t	  strobj_size;
  size_t	  strobj_asize;

  void		  scan1   (void){
    argv = move(argv);
    return;
  }
  		  hsc_argv_size = sizeof(struct array_object) + argc * sizeof(struct array_object *);

  hsc_argv_asize = sizeof(align_t) * (hsc_argv_size + sizeof(align_t) + -1) / sizeof(align_t);
  hsc_argv = (struct array_object *)getmem(scan1, hsc_argv_asize);
  hsc_argv->d = (desc_t) 1;
  hsc_argv->length = argc;
  hsc_argv->el_size = sizeof(struct array_object *);
  hsc_argv->size = hsc_argv_size;
  hsc_argv->size = hsc_argv_asize;
  body = (struct array_object **)hsc_argv->body;
  for (i = 0; i < argc; ++i) {
    len = 1 + strlen(argv[i]);
    strobj_size = sizeof(struct array_object) + len * sizeof(char);
    strobj_asize = sizeof(align_t) * (strobj_size + sizeof(align_t) + -1) / sizeof(align_t);
    body[i] = (struct array_object *)getmem(scan1, strobj_asize);
    (body[i])->d = (desc_t) 3;
    (body[i])->length = len;
    (body[i])->el_size = sizeof(char);
    (body[i])->size = strobj_size;
    (body[i])->asize = strobj_asize;
    strncpy((char *)(body[i])->body, argv[i], len);
  }
  return hsc_main(scan1, argc, hsc_argv);
}
