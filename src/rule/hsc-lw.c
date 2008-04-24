typedef char *(*nestfn_t) (char *, void *);
typedef struct
{
  nestfn_t fun;
  void *fr;
} closure_t;
typedef double Align_t;

char *lw_call (char *esp);

struct func_arg
{
  void *(*func) (char *, void *);
  void *arg;
};

void *thread_origin (void *farg);

void __cygwin_lock_init (void **);

void __cygwin_lock_init_recursive (void **);

void __cygwin_lock_fini (void **);

void __cygwin_lock_lock (void **);

int __cygwin_lock_trylock (void **);

void __cygwin_lock_unlock (void **);

struct unnamed1
{
  int __count;

  union unnamed2
  {
    unsigned int __wch;
    unsigned char __wchb[4];
  };
  union unnamed2 __value;
};

struct _Bigint
{
  struct _Bigint *_next;
  int _k;
  int _maxwds;
  int _sign;
  int _wds;
  unsigned long _x[1];
};

struct __tm
{
  int __tm_sec;
  int __tm_min;
  int __tm_hour;
  int __tm_mday;
  int __tm_mon;
  int __tm_year;
  int __tm_wday;
  int __tm_yday;
  int __tm_isdst;
};

struct _on_exit_args
{
  void *_fnargs[32];
  void *_dso_handle[32];
  unsigned long _fntypes;
  unsigned long _is_cxa;
};

struct _atexit
{
  struct _atexit *_next;
  int _ind;
  void (*_fns[32]) (char *, void);
  struct _on_exit_args _on_exit_args;
};

struct __sbuf
{
  unsigned char *_base;
  int _size;
};

struct __sFILE
{
  unsigned char *_p;
  int _r;
  int _w;
  short _flags;
  short _file;
  struct __sbuf _bf;
  int _lbfsize;
  void *_cookie;
  int (*_read) (char *, struct _reent *, void *, char *, int);
  int (*_write) (char *, struct _reent *, void *, char const *, int);
  long (*_seek) (char *, struct _reent *, void *, long, int);
  int (*_close) (char *, struct _reent *, void *);
  struct __sbuf _ub;
  unsigned char *_up;
  int _ur;
  unsigned char _ubuf[3];
  unsigned char _nbuf[1];
  struct __sbuf _lb;
  int _blksize;
  int _offset;
  struct _reent *_data;
  void *_lock;
};

struct __sFILE64
{
  unsigned char *_p;
  int _r;
  int _w;
  short _flags;
  short _file;
  struct __sbuf _bf;
  int _lbfsize;
  struct _reent *_data;
  void *_cookie;
  int (*_read) (char *, struct _reent *, void *, char *, int);
  int (*_write) (char *, struct _reent *, void *, char const *, int);
  long (*_seek) (char *, struct _reent *, void *, long, int);
  int (*_close) (char *, struct _reent *, void *);
  struct __sbuf _ub;
  unsigned char *_up;
  int _ur;
  unsigned char _ubuf[3];
  unsigned char _nbuf[1];
  struct __sbuf _lb;
  int _blksize;
  int _flags2;
  long long _offset;
  long long (*_seek64) (char *, struct _reent *, void *, long long, int);
  void *_lock;
};

struct _glue
{
  struct _glue *_next;
  int _niobs;
  struct __sFILE64 *_iobs;
};

struct _rand48
{
  unsigned short _seed[3];
  unsigned short _mult[3];
  unsigned short _add;
};

struct _reent
{
  int _errno;
  struct __sFILE64 *_stdin;
  struct __sFILE64 *_stdout;
  struct __sFILE64 *_stderr;
  int _inc;
  char _emergency[25];
  int _current_category;
  char const *_current_locale;
  int __sdidinit;
  void (*__cleanup) (char *, struct _reent *);
  struct _Bigint *_result;
  int _result_k;
  struct _Bigint *_p5s;
  struct _Bigint **_freelist;
  int _cvtlen;
  char *_cvtbuf;

  union unnamed3
  {

    struct unnamed4
    {
      unsigned int _unused_rand;
      char *_strtok_last;
      char _asctime_buf[26];
      struct __tm _localtime_buf;
      int _gamma_signgam;
      unsigned long long _rand_next;
      struct _rand48 _r48;
      struct unnamed1 _mblen_state;
      struct unnamed1 _mbtowc_state;
      struct unnamed1 _wctomb_state;
      char _l64a_buf[8];
      char _signal_buf[24];
      int _getdate_err;
      struct unnamed1 _mbrlen_state;
      struct unnamed1 _mbrtowc_state;
      struct unnamed1 _mbsrtowcs_state;
      struct unnamed1 _wcrtomb_state;
      struct unnamed1 _wcsrtombs_state;
    };
    struct unnamed4 _reent;

    struct unnamed5
    {
      unsigned char *_nextf[30];
      unsigned int _nmalloc[30];
    };
    struct unnamed5 _unused;
  };
  union unnamed3 _new;
  struct _atexit *_atexit;
  struct _atexit _atexit0;
  void (**_sig_func) (char *, int);
  struct _glue __sglue;
  struct __sFILE64 __sf[3];
};
extern struct _reent *_impure_ptr;
extern struct _reent *const _global_impure_ptr;

void _reclaim_reent (struct _reent *);

struct _reent *__getreent (void);

struct timespec
{
  long tv_sec;
  long tv_nsec;
};

struct itimerspec
{
  struct timespec it_interval;
  struct timespec it_value;
};

struct _types_fd_set
{
  long fds_bits[(64 + (sizeof (long) * 8 - 1)) / (sizeof (long) * 8)];
};

struct flock
{
  short l_type;
  short l_whence;
  long long l_start;
  long long l_len;
  int l_pid;
};

struct __pthread_t
{
  char __dummy;
};

struct __pthread_mutex_t
{
  char __dummy;
};

struct __pthread_key_t
{
  char __dummy;
};

struct __pthread_attr_t
{
  char __dummy;
};

struct __pthread_mutexattr_t
{
  char __dummy;
};

struct __pthread_condattr_t
{
  char __dummy;
};

struct __pthread_cond_t
{
  char __dummy;
};

struct unnamed6
{
  struct __pthread_mutex_t *mutex;
  int state;
};

struct __pthread_rwlock_t
{
  char __dummy;
};

struct __pthread_rwlockattr_t
{
  char __dummy;
};

struct __sFILE64 *tmpfile (void);

char *tmpnam (char *);

int fclose (struct __sFILE64 *);

int fflush (struct __sFILE64 *);

struct __sFILE64 *freopen (char const *, char const *, struct __sFILE64 *);

void setbuf (struct __sFILE64 *, char *);

int setvbuf (struct __sFILE64 *, char *, int, unsigned int);

int fprintf (struct __sFILE64 *, char const *, ...);

int fscanf (struct __sFILE64 *, char const *, ...);

int printf (char const *, ...);

int scanf (char const *, ...);

int sscanf (char const *, char const *, ...);

int vfprintf (struct __sFILE64 *, char const *, __builtin_va_list);

int vprintf (char const *, __builtin_va_list);

int vsprintf (char *, char const *, __builtin_va_list);

int fgetc (struct __sFILE64 *);

char *fgets (char *, int, struct __sFILE64 *);

int fputc (int, struct __sFILE64 *);

int fputs (char const *, struct __sFILE64 *);

int getc (struct __sFILE64 *);

int getchar (void);

char *gets (char *);

int putc (int, struct __sFILE64 *);

int putchar (int);

int puts (char const *);

int ungetc (int, struct __sFILE64 *);

unsigned int fread (void *, unsigned int, unsigned int, struct __sFILE64 *);

unsigned int fwrite (void const *, unsigned int, unsigned int,
                     struct __sFILE64 *);

int fgetpos (struct __sFILE64 *, long long *);

int fseek (struct __sFILE64 *, long, int);

int fsetpos (struct __sFILE64 *, long long const *);

long ftell (struct __sFILE64 *);

void rewind (struct __sFILE64 *);

void clearerr (struct __sFILE64 *);

int feof (struct __sFILE64 *);

int ferror (struct __sFILE64 *);

void perror (char const *);

struct __sFILE64 *fopen (char const *_name, char const *_type);

int sprintf (char *, char const *, ...);

int remove (char const *);

int rename (char const *, char const *);

int fseeko (struct __sFILE64 *, long long, int);

long long ftello (struct __sFILE64 *);

int asiprintf (char **, char const *, ...);

int asprintf (char **, char const *, ...);

int diprintf (int, char const *, ...);

int dprintf (int, char const *, ...);

int fcloseall (void);

int fiprintf (struct __sFILE64 *, char const *, ...);

int fiscanf (struct __sFILE64 *, char const *, ...);

int iprintf (char const *, ...);

int iscanf (char const *, ...);

int siprintf (char *, char const *, ...);

int siscanf (char const *, char const *, ...);

int snprintf (char *, unsigned int, char const *, ...);

int sniprintf (char *, unsigned int, char const *, ...);

char *tempnam (char const *, char const *);

int vasiprintf (char **, char const *, __builtin_va_list);

int vasprintf (char **, char const *, __builtin_va_list);

int vdiprintf (int, char const *, __builtin_va_list);

int vdprintf (int, char const *, __builtin_va_list);

int vsniprintf (char *, unsigned int, char const *, __builtin_va_list);

int vsnprintf (char *, unsigned int, char const *, __builtin_va_list);

int vfiprintf (struct __sFILE64 *, char const *, __builtin_va_list);

int vfiscanf (struct __sFILE64 *, char const *, __builtin_va_list);

int vfscanf (struct __sFILE64 *, char const *, __builtin_va_list);

int viprintf (char const *, __builtin_va_list);

int vsiprintf (char *, char const *, __builtin_va_list);

int viscanf (char const *, __builtin_va_list);

int vscanf (char const *, __builtin_va_list);

int vsiscanf (char const *, char const *, __builtin_va_list);

int vsscanf (char const *, char const *, __builtin_va_list);

struct __sFILE64 *fdopen (int, char const *);

int fileno (struct __sFILE64 *);

int getw (struct __sFILE64 *);

int pclose (struct __sFILE64 *);

struct __sFILE64 *popen (char const *, char const *);

int putw (int, struct __sFILE64 *);

void setbuffer (struct __sFILE64 *, char *, int);

int setlinebuf (struct __sFILE64 *);

int getc_unlocked (struct __sFILE64 *);

int getchar_unlocked (void);

void flockfile (struct __sFILE64 *);

int ftrylockfile (struct __sFILE64 *);

void funlockfile (struct __sFILE64 *);

int putc_unlocked (int, struct __sFILE64 *);

int putchar_unlocked (int);

int _asiprintf_r (struct _reent *, char **, char const *, ...);

int _asprintf_r (struct _reent *, char **, char const *, ...);

int _diprintf_r (struct _reent *, int, char const *, ...);

int _dprintf_r (struct _reent *, int, char const *, ...);

int _fclose_r (struct _reent *, struct __sFILE64 *);

int _fcloseall_r (struct _reent *);

struct __sFILE64 *_fdopen_r (struct _reent *, int, char const *);

int _fflush_r (struct _reent *, struct __sFILE64 *);

char *_fgets_r (struct _reent *, char *, int, struct __sFILE64 *);

int _fiprintf_r (struct _reent *, struct __sFILE64 *, char const *, ...);

int _fiscanf_r (struct _reent *, struct __sFILE64 *, char const *, ...);

struct __sFILE64 *_fopen_r (struct _reent *, char const *, char const *);

int _fprintf_r (struct _reent *, struct __sFILE64 *, char const *, ...);

int _fputc_r (struct _reent *, int, struct __sFILE64 *);

int _fputs_r (struct _reent *, char const *, struct __sFILE64 *);

unsigned int _fread_r (struct _reent *, void *, unsigned int, unsigned int,
                       struct __sFILE64 *);

int _fscanf_r (struct _reent *, struct __sFILE64 *, char const *, ...);

int _fseek_r (struct _reent *, struct __sFILE64 *, long, int);

long _ftell_r (struct _reent *, struct __sFILE64 *);

unsigned int _fwrite_r (struct _reent *, void const *, unsigned int,
                        unsigned int, struct __sFILE64 *);

int _getc_r (struct _reent *, struct __sFILE64 *);

int _getc_unlocked_r (struct _reent *, struct __sFILE64 *);

int _getchar_r (struct _reent *);

int _getchar_unlocked_r (struct _reent *);

char *_gets_r (struct _reent *, char *);

int _iprintf_r (struct _reent *, char const *, ...);

int _iscanf_r (struct _reent *, char const *, ...);

int _mkstemp_r (struct _reent *, char *);

char *_mktemp_r (struct _reent *, char *);

void _perror_r (struct _reent *, char const *);

int _printf_r (struct _reent *, char const *, ...);

int _putc_r (struct _reent *, int, struct __sFILE64 *);

int _putc_unlocked_r (struct _reent *, int, struct __sFILE64 *);

int _putchar_unlocked_r (struct _reent *, int);

int _putchar_r (struct _reent *, int);

int _puts_r (struct _reent *, char const *);

int _remove_r (struct _reent *, char const *);

int _rename_r (struct _reent *, char const *, char const *);

int _scanf_r (struct _reent *, char const *, ...);

int _siprintf_r (struct _reent *, char *, char const *, ...);

int _siscanf_r (struct _reent *, char const *, char const *, ...);

int _sniprintf_r (struct _reent *, char *, unsigned int, char const *, ...);

int _snprintf_r (struct _reent *, char *, unsigned int, char const *, ...);

int _sprintf_r (struct _reent *, char *, char const *, ...);

int _sscanf_r (struct _reent *, char const *, char const *, ...);

char *_tempnam_r (struct _reent *, char const *, char const *);

struct __sFILE64 *_tmpfile_r (struct _reent *);

char *_tmpnam_r (struct _reent *, char *);

int _ungetc_r (struct _reent *, int, struct __sFILE64 *);

int _vasiprintf_r (struct _reent *, char **, char const *, __builtin_va_list);

int _vasprintf_r (struct _reent *, char **, char const *, __builtin_va_list);

int _vdiprintf_r (struct _reent *, int, char const *, __builtin_va_list);

int _vdprintf_r (struct _reent *, int, char const *, __builtin_va_list);

int _vfiprintf_r (struct _reent *, struct __sFILE64 *, char const *,
                  __builtin_va_list);

int _vfprintf_r (struct _reent *, struct __sFILE64 *, char const *,
                 __builtin_va_list);

int _viprintf_r (struct _reent *, char const *, __builtin_va_list);

int _vprintf_r (struct _reent *, char const *, __builtin_va_list);

int _vsiprintf_r (struct _reent *, char *, char const *, __builtin_va_list);

int _vsprintf_r (struct _reent *, char *, char const *, __builtin_va_list);

int _vsniprintf_r (struct _reent *, char *, unsigned int, char const *,
                   __builtin_va_list);

int _vsnprintf_r (struct _reent *, char *, unsigned int, char const *,
                  __builtin_va_list);

int _vfiscanf_r (struct _reent *, struct __sFILE64 *, char const *,
                 __builtin_va_list);

int _vfscanf_r (struct _reent *, struct __sFILE64 *, char const *,
                __builtin_va_list);

int _viscanf_r (struct _reent *, char const *, __builtin_va_list);

int _vscanf_r (struct _reent *, char const *, __builtin_va_list);

int _vsscanf_r (struct _reent *, char const *, char const *,
                __builtin_va_list);

int _vsiscanf_r (struct _reent *, char const *, char const *,
                 __builtin_va_list);

int __getdelim (char **, unsigned int *, int, struct __sFILE64 *);

int __getline (char **, unsigned int *, struct __sFILE64 *);

int __srget_r (struct _reent *, struct __sFILE64 *);

int __swbuf_r (struct _reent *, int, struct __sFILE64 *);

struct __sFILE64 *funopen (void const *_cookie,
                           int (*readfn) (void *, char *, int),
                           int (*writefn) (void *, char const *, int),
                           long long (*seekfn) (void *, long long, int),
                           int (*closefn) (void *));

extern int
__sgetc_r (struct _reent *__ptr, struct __sFILE64 *__p)
{
  int __c = --(*__p)._r < 0 ? __srget_r (__ptr, __p) : (int) *((*__p)._p)++;
  if ((*__p)._flags & 16384 && __c == '\r')
    {
      int __c2 =
        --(*__p)._r < 0 ? __srget_r (__ptr, __p) : (int) *((*__p)._p)++;
      if (__c2 == '\n')
        __c = __c2;
      else
        ungetc (__c2, __p);
    }
  return __c;
}

char *mkdtemp (char *);

char const *getprogname (void);

void setprogname (char const *);

char *realpath (char const *, char *);

int unsetenv (char const *);

int random (void);

long srandom (unsigned int);

char *ptsname (int);

int grantpt (int);

int unlockpt (int);

struct unnamed7
{
  int quot;
  int rem;
};

struct unnamed8
{
  long quot;
  long rem;
};

struct unnamed9
{
  long long quot;
  long long rem;
};
extern int __mb_cur_max;

void abort (void);

int abs (int);

int atexit (void (*__func) (void));

double atof (char const *__nptr);

float atoff (char const *__nptr);

int atoi (char const *__nptr);

int _atoi_r (struct _reent *, char const *);

long atol (char const *__nptr);

long _atol_r (struct _reent *, char const *);

void *bsearch (void const *__key, void const *__base, unsigned int __nmemb,
               unsigned int __size, int (*_compar) (void const *,
                                                    void const *));

void *calloc (unsigned int __nmemb, unsigned int __size);

struct unnamed7 div (int __numer, int __denom);

void exit (int __status);

void free (void *);

char *getenv (char const *__string);

char *_getenv_r (struct _reent *, char const *);

char *_findenv (char const *, int *);

char *_findenv_r (struct _reent *, char const *, int *);

long labs (long);

struct unnamed8 ldiv (long __numer, long __denom);

void *malloc (unsigned int __size);

int mblen (char const *, unsigned int);

int _mblen_r (struct _reent *, char const *, unsigned int, struct unnamed1 *);

int mbtowc (unsigned short *, char const *, unsigned int);

int _mbtowc_r (struct _reent *, unsigned short *, char const *, unsigned int,
               struct unnamed1 *);

int wctomb (char *, unsigned short);

int _wctomb_r (struct _reent *, char *, unsigned short, struct unnamed1 *);

unsigned int mbstowcs (unsigned short *, char const *, unsigned int);

unsigned int _mbstowcs_r (struct _reent *, unsigned short *, char const *,
                          unsigned int, struct unnamed1 *);

unsigned int wcstombs (char *, unsigned short const *, unsigned int);

unsigned int _wcstombs_r (struct _reent *, char *, unsigned short const *,
                          unsigned int, struct unnamed1 *);

int mkstemp (char *);

char *mktemp (char *);

void qsort (void *__base, unsigned int __nmemb, unsigned int __size,
            int (*_compar) (void const *, void const *));

int rand (void);

void *realloc (void *__r, unsigned int __size);

void srand (unsigned int __seed);

double strtod (char const *__n, char **__end_PTR);

double _strtod_r (struct _reent *, char const *, char **);

float strtof (char const *__n, char **__end_PTR);

long strtol (char const *__n, char **__end_PTR, int __base);

long _strtol_r (struct _reent *, char const *, char **, int);

unsigned long strtoul (char const *__n, char **__end_PTR, int __base);

unsigned long _strtoul_r (struct _reent *, char const *, char **, int);

int system (char const *__string);

long a64l (char const *__input);

char *l64a (long __input);

char *_l64a_r (struct _reent *, long);

int on_exit (void (*__func) (int, void *), void *__arg);

void _Exit (int __status);

int putenv (char *__string);

int _putenv_r (struct _reent *, char *);

int setenv (char const *__string, char const *__value, int __overwrite);

int _setenv_r (struct _reent *, char const *, char const *, int);

char *gcvt (double, int, char *);

char *gcvtf (float, int, char *);

char *fcvt (double, int, int *, int *);

char *fcvtf (float, int, int *, int *);

char *ecvt (double, int, int *, int *);

char *ecvtbuf (double, int, int *, int *, char *);

char *fcvtbuf (double, int, int *, int *, char *);

char *ecvtf (float, int, int *, int *);

char *dtoa (double, int, int, int *, int *, char **);

int rand_r (unsigned int *__seed);

double drand48 (void);

double _drand48_r (struct _reent *);

double erand48 (unsigned short[3]);

double _erand48_r (struct _reent *, unsigned short[3]);

long jrand48 (unsigned short[3]);

long _jrand48_r (struct _reent *, unsigned short[3]);

void lcong48 (unsigned short[7]);

void _lcong48_r (struct _reent *, unsigned short[7]);

long lrand48 (void);

long _lrand48_r (struct _reent *);

long mrand48 (void);

long _mrand48_r (struct _reent *);

long nrand48 (unsigned short[3]);

long _nrand48_r (struct _reent *, unsigned short[3]);

unsigned short *seed48 (unsigned short[3]);

unsigned short *_seed48_r (struct _reent *, unsigned short[3]);

void srand48 (long);

void _srand48_r (struct _reent *, long);

long long atoll (char const *__nptr);

long long _atoll_r (struct _reent *, char const *);

long long llabs (long long);

struct unnamed9 lldiv (long long __numer, long long __denom);

long long strtoll (char const *__n, char **__end_PTR, int __base);

long long _strtoll_r (struct _reent *, char const *, char **, int);

unsigned long long strtoull (char const *__n, char **__end_PTR, int __base);

unsigned long long _strtoull_r (struct _reent *, char const *, char **, int);

char *_dtoa_r (struct _reent *, double, int, int, int *, int *, char **);

int _system_r (struct _reent *, char const *);

void __eprintf (char const *, char const *, unsigned int, char const *);

struct timeval
{
  long tv_sec;
  long tv_usec;
};

struct timezone
{
  int tz_minuteswest;
  int tz_dsttime;
};

struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

unsigned long clock (void);

double difftime (long _time2, long _time1);

long mktime (struct tm *_timeptr);

long time (long *_timer);

char *asctime (struct tm const *_tblock);

char *ctime (long const *_time);

struct tm *gmtime (long const *_timer);

struct tm *localtime (long const *_timer);

unsigned int strftime (char *_s, unsigned int _maxsize, char const *_fmt,
                       struct tm const *_t);

char *asctime_r (struct tm const *, char *);

char *ctime_r (long const *, char *);

struct tm *gmtime_r (long const *, struct tm *);

struct tm *localtime_r (long const *, struct tm *);

char *strptime (char const *, char const *, struct tm *);

void tzset (void);

void _tzset_r (struct _reent *);

struct __tzrule_struct
{
  char ch;
  int m;
  int n;
  int d;
  int s;
  long change;
  long offset;
};

struct __tzinfo_struct
{
  int __tznorth;
  int __tzyear;
  struct __tzrule_struct __tzrule[2];
};

struct __tzinfo_struct *__gettzinfo (void);
extern long _timezone;
extern int _daylight;
extern char *_tzname[2];

int nanosleep (struct timespec const *, struct timespec *);

int clock_setres (unsigned long, struct timespec *);

int clock_getres (unsigned long, struct timespec *);

long timelocal (struct tm *);

long timegm (struct tm *);
extern int daylight;
extern long timezone;

struct _fpstate
{
  unsigned long cw;
  unsigned long sw;
  unsigned long tag;
  unsigned long ipoff;
  unsigned long cssel;
  unsigned long dataoff;
  unsigned long datasel;
  unsigned char _st[80];
  unsigned long nxst;
};

struct ucontext
{
  unsigned long cr2;
  unsigned long dr0;
  unsigned long dr1;
  unsigned long dr2;
  unsigned long dr3;
  unsigned long dr6;
  unsigned long dr7;
  struct _fpstate fpstate;
  unsigned long gs;
  unsigned long fs;
  unsigned long es;
  unsigned long ds;
  unsigned long edi;
  unsigned long esi;
  unsigned long ebx;
  unsigned long edx;
  unsigned long ecx;
  unsigned long eax;
  unsigned long ebp;
  unsigned long eip;
  unsigned long cs;
  unsigned long eflags;
  unsigned long esp;
  unsigned long ss;
  unsigned char _internal;
  unsigned long oldmask;
};

union sigval
{
  int sival_int;
  void *sival_ptr;
};

struct sigevent
{
  union sigval sigev_value;
  int sigev_signo;
  int sigev_notify;
  void (*sigev_notify_function) (char *, union sigval);
  struct __pthread_attr_t **sigev_notify_attributes;
};
#pragma pack(push,4)

struct _sigcommune
{
  __uint32_t _si_code;
  void *_si_read_handle;
  void *_si_write_handle;
  void *_si_process_handle;

  union unnamed10
  {
    int _si_fd;
    void *_si_pipe_fhandler;
    char *_si_str;
  };
  no_match};

struct unnamed11
{
  int si_signo;
  int si_code;
  int si_pid;
  unsigned long si_uid;
  int si_errno;

  union unnamed12
  {
    __uint32_t __pad[32];
    struct _sigcommune _si_commune;

    union unnamed13
    {

      struct unnamed14
      {

        union unnamed15
        {

          struct unnamed16
          {
            unsigned long si_tid;
            unsigned int si_overrun;
          };
            no_match union sigval si_sigval;
          union sigval si_value;
        };
        no_match};
      no_match};
      no_match struct unnamed17
    {
      int si_status;
      unsigned long si_utime;
      unsigned long si_stime;
    };
      no_match void *si_addr;
  };
  no_match};
#pragma pack(pop)
enum unnamed18
{ SI_USER = 0, SI_ASYNCIO =
    2, SI_MESGQ, SI_TIMER, SI_QUEUE, SI_KERNEL, ILL_ILLOPC, ILL_ILLOPN,
    ILL_ILLADR, ILL_ILLTRP, ILL_PRVOPC, ILL_PRVREG, ILL_COPROC, ILL_BADSTK,
    FPE_INTDIV, FPE_INTOVF, FPE_FLTDIV, FPE_FLTOVF, FPE_FLTUND, FPE_FLTRES,
    FPE_FLTINV, FPE_FLTSUB, SEGV_MAPERR, SEGV_ACCERR, BUS_ADRALN, BUS_ADRERR,
    BUS_OBJERR, CLD_EXITED, CLD_KILLED, CLD_DUMPED, CLD_TRAPPED, CLD_STOPPED,
    CLD_CONTINUED };
enum unnamed19
{ SIGEV_SIGNAL = 0, SIGEV_NONE, SIGEV_THREAD };

struct sigaction
{

  union unnamed20
  {
    void (*sa_handler) (char *, int);
    void (*sa_sigaction) (int, struct unnamed11 *, void *);
  };
    no_match unsigned long sa_mask;
  int sa_flags;
};

int sigwait (unsigned long const *, int *);

int sigwaitinfo (unsigned long const *, struct unnamed11 *);

int sighold (int);

int sigignore (int);

int sigrelse (int);

void (*sigset (int, void (*)(int))) (int);

int sigqueue (int, int, union sigval const);

int siginterrupt (int, int);

int sigprocmask (int how, unsigned long const *set, unsigned long *oset);

int pthread_sigmask (int how, unsigned long const *set, unsigned long *oset);

int kill (int, int);

int killpg (int, int);

int sigaction (int, struct sigaction const *, struct sigaction *);

int sigaddset (unsigned long *, int const);

int sigdelset (unsigned long *, int const);

int sigismember (unsigned long const *, int);

int sigfillset (unsigned long *);

int sigemptyset (unsigned long *);

int sigpending (unsigned long *);

int sigsuspend (unsigned long const *);

int sigpause (int);

int pthread_kill (struct __pthread_t *thread, int sig);

void (*_signal_r (struct _reent *, int, void (*)(int))) (int);

int _raise_r (struct _reent *, int);

void (*signal (int, void (*)(int))) (int);

int raise (int);

int clock_settime (unsigned long clock_id, struct timespec const *tp);

int clock_gettime (unsigned long clock_id, struct timespec *tp);

int clock_getres (unsigned long clock_id, struct timespec *res);

int timer_create (unsigned long clock_id, struct sigevent *evp,
                  unsigned long *timerid);

int timer_delete (unsigned long timerid);

int timer_settime (unsigned long timerid, int flags,
                   struct itimerspec const *value, struct itimerspec *ovalue);

int timer_gettime (unsigned long timerid, struct itimerspec *value);

int timer_getoverrun (unsigned long timerid);

int nanosleep (struct timespec const *rqtp, struct timespec *rmtp);

int select (int __n, struct _types_fd_set *__readfds,
            struct _types_fd_set *__writefds,
            struct _types_fd_set *__exceptfds, struct timeval *__timeout);

int pselect (int __n, struct _types_fd_set *__readfds,
             struct _types_fd_set *__writefds,
             struct _types_fd_set *__exceptfds,
             struct timespec const *__timeout, unsigned long const *__set);

int futimes (int, struct timeval const *);

int lutimes (char const *, struct timeval const *);

struct itimerval
{
  struct timeval it_interval;
  struct timeval it_value;
};

int gettimeofday (struct timeval *__p, void *__z);

int settimeofday (struct timeval const *, struct timezone const *);

int utimes (char const *__path, struct timeval const *__tvp);

int getitimer (int __which, struct itimerval *__value);

int setitimer (int __which, struct itimerval const *__value,
               struct itimerval *__ovalue);

void *memchr (void const *, int, unsigned int);

int memcmp (void const *, void const *, unsigned int);

void *memcpy (void *, void const *, unsigned int);

void *memmove (void *, void const *, unsigned int);

void *memset (void *, int, unsigned int);

char *strcat (char *, char const *);

char *strchr (char const *, int);

int strcmp (char const *, char const *);

int strcoll (char const *, char const *);

char *strcpy (char *, char const *);

unsigned int strcspn (char const *, char const *);

char *strerror (int);

unsigned int strlen (char const *);

char *strncat (char *, char const *, unsigned int);

int strncmp (char const *, char const *, unsigned int);

char *strncpy (char *, char const *, unsigned int);

char *strpbrk (char const *, char const *);

char *strrchr (char const *, int);

unsigned int strspn (char const *, char const *);

char *strstr (char const *, char const *);

char *strtok (char *, char const *);

unsigned int strxfrm (char *, char const *, unsigned int);

char *strtok_r (char *, char const *, char **);

int bcmp (void const *, void const *, unsigned int);

void bcopy (void const *, void *, unsigned int);

void bzero (void *, unsigned int);

int ffs (int);

char *index (char const *, int);

void *memccpy (void *, void const *, int, unsigned int);

void *mempcpy (void *, void const *, unsigned int);

extern void *memmem (void const *, unsigned int, void const *, unsigned int);

char *rindex (char const *, int);

int strcasecmp (char const *, char const *);

char *strdup (char const *);

char *_strdup_r (struct _reent *, char const *);

char *strndup (char const *, unsigned int);

char *_strndup_r (struct _reent *, char const *, unsigned int);

char *strerror_r (int, char *, unsigned int);

unsigned int strlcat (char *, char const *, unsigned int);

unsigned int strlcpy (char *, char const *, unsigned int);

int strncasecmp (char const *, char const *, unsigned int);

unsigned int strnlen (char const *, unsigned int);

char *strsep (char **, char const *);

char *strlwr (char *);

char *strupr (char *);

char const *strsignal (int __signo);

int strtosigno (char const *__name);

struct desc_struct
{
  size_t size;
  size_t asize;
  int fli_len;
  size_t *fli;
};

struct array_object
{
  struct desc_struct *d;
  int length;
  size_t el_size;
  size_t size;
  size_t asize;
  char body[0];
};

extern void *getmem (char *esp, closure_t * scan, size_t size);

extern void *move (void *vp);

void *
MEMCPY (void *d, void const *s, size_t sz)
{
  long *_des = (long *) d;
  long *_src = (long *) s;
  long *_til = (long *) ((char *) _src + sz);
  do
    {
      *_des++ = *_src++;
    }
  while (_src < _til);
}

static void
error (char *s)
{
  printf ("ERROR: %s\n", s);
  exit (1);
}

static void *
myalloc (size_t size)
{
  void *p;
  p = malloc (size);
  if (p == __null)
    error ("Not enough memory.");
  return p;
}

static void *
myrealloc (void *p, size_t size)
{
  void *q;
  q = realloc (p, size);
  if (q == __null)
    error ("Not enough memory.");
  return q;
}

struct gc_params_struct
{
  int gcv;
  int gctype;
  size_t tosize;
  size_t stack_size;
  size_t limited_stack_max;
};
static struct gc_params_struct params;
static int allocated_size;
static char *old_memory;
static char *new_memory;
static char *old_memory_end;
static char *new_memory_end;
static char *b;

void *
move (void *vp)
{
  char *p = vp;
  void *fwp;
  size_t tag;
  struct desc_struct *d;
  struct array_object *ao;
  size_t size;
  size_t asize;
  char *np;
  char *nb;
  if (!((unsigned int) ((char *) p - old_memory) < params.tosize))
    return vp;
  tag = 3 & *((size_t *) p);
  fwp = (void *) (tag ^ *((size_t *) p));
  if ((unsigned int) ((char *) fwp - new_memory) < params.tosize)
    return fwp;
  switch (tag)
    {
    case 0:
      d = (struct desc_struct *) fwp;
      size = (*d).size;
      asize = (*d).asize;
      break;
    case 1:
    case 3:
      ao = (struct array_object *) vp;
      size = (*ao).size;
      asize = (*ao).asize;
      break;
    default:
      error ("Illegal type ID!");
    }
  np = b;
  nb = np + asize;
  if (nb >= new_memory_end)
    error ("buffer overrun.");
  MEMCPY (np, p, size);
  b = nb;
  *((void **) p) = np;
  return np;
}

struct gc_breadth_first_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  char *s;
  char *p;
  size_t el_size;
  struct array_object *ao;
  struct desc_struct *d;
  void **link;
  int len;
  size_t tag;
  char *tmp;
  int i;
  closure_t *scan;
};

void
gc_breadth_first (char *esp, closure_t * scan)
{
  char *argp;
  struct gc_breadth_first_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  int i;
  char *tmp;
  size_t tag;
  int len;
  void **link;
  struct desc_struct *d;
  struct array_object *ao;
  size_t el_size;
  char *p;
  char *s;
  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct gc_breadth_first_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct gc_breadth_first_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL;
        }
      goto L_CALL;
    }
  efp = (struct gc_breadth_first_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct gc_breadth_first_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  if (params.gcv)
    {
      printf ("BREADTH-FIRST-GC start\n");
    }
  b = new_memory;
  s = b;
  {
    argp =
      (char *) ((Align_t *) esp +
                (sizeof (char *) + sizeof (Align_t) + -1) / sizeof (Align_t));
    *((closure_t **) argp) = scan;
    efp->s = s;
    efp->p = p;
    efp->el_size = el_size;
    efp->ao = ao;
    efp->d = d;
    efp->link = link;
    efp->len = len;
    efp->tag = tag;
    efp->tmp = tmp;
    efp->i = i;
    efp->scan = scan;
    efp->argp = argp;
    efp->tmp_esp = argp;
    efp->call_id = 0;
    return;
  L_CALL:;
    s = efp->s;
    p = efp->p;
    el_size = efp->el_size;
    ao = efp->ao;
    d = efp->d;
    link = efp->link;
    len = efp->len;
    tag = efp->tag;
    tmp = efp->tmp;
    i = efp->i;
    scan = efp->scan;
  }
  while (s < b)
    {
      tag = 3 & *((size_t *) s);
      switch (tag)
        {
        case 0:
          d = (struct desc_struct *) (tag ^ *((size_t *) s));
          p = (char *) s;
          len = (*d).fli_len;
          {
            i = 0;
            for (; i < len; ++i)
              {
                link = (void **) (p + ((*d).fli)[i]);
                *link = move (*link);
              }
          }
          s += (*d).asize;
          break;
        case 1:
          ao = (struct array_object *) s;
          p = (*ao).body;
          len = (*ao).length;
          el_size = (*ao).el_size;
          {
            i = 0;
            for (; i < len; ++i)
              {
                link = (void **) p;
                *link = move (*link);
                p += el_size;
              }
          }
          s += (*ao).asize;
          break;
        case 3:
          ao = (struct array_object *) s;
          s += (*ao).asize;
          break;
        default:
          error ("Illegal type ID!");
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
    {
      printf ("GC complete (%d)\n", allocated_size);
    }
}
double gc_ttime;
struct timeval tp1;
struct timeval tp2;

struct gc_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  closure_t *scan;
};

void
gc (char *esp, closure_t * scan)
{
  struct gc_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct gc_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct gc_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL2;
        }
      goto L_CALL2;
    }
  efp = (struct gc_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct gc_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  gettimeofday (&tp1, 0);
  switch (params.gctype)
    {
    case 0:
      new_esp = esp;
      while (gc_breadth_first (new_esp, scan),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          efp->scan = scan;
          efp->call_id = 0;
          return;
        L_CALL2:;
          scan = efp->scan;
          new_esp = esp + 1;
        }
      break;
    }
  gettimeofday (&tp2, 0);
  gc_ttime += tp2.tv_sec - tp1.tv_sec + (tp2.tv_usec - tp1.tv_usec) * 1.0E-6;
}

void
getmem_init (struct gc_params_struct p)
{
  char *mp;
  static int called = 0;
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
    params.stack_size = params.tosize / sizeof (double);
  if (params.limited_stack_max == 0)
    params.limited_stack_max = 32;
  printf ("tosize=%d, stack=%d, limit=%d\n", params.tosize, params.stack_size,
          params.limited_stack_max);
  old_memory = myalloc (params.tosize);
  old_memory_end = old_memory + params.tosize;
  new_memory = myalloc (params.tosize);
  new_memory_end = new_memory + params.tosize;
  allocated_size = 0;
}

void
gc_init (int tp, int tosize, int stack_size, int limited_max)
{
  struct gc_params_struct p;
  p.gcv = 1;
  p.gctype = tp;
  p.tosize = tosize;
  p.stack_size = stack_size;
  p.limited_stack_max = limited_max;
  getmem_init (p);
}

void *
try_getmem (size_t size)
{
  char *p;
  if (allocated_size + size > params.tosize)
    return 0;
  p = old_memory + allocated_size;
  allocated_size += size;
  memset (p, 0, size);
  return p;
}

struct getmem_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  void *p;
  size_t size;
  closure_t *scan;
};

void *
getmem (char *esp, closure_t * scan, size_t size)
{
  struct getmem_frame *efp;
  char *new_esp;
  size_t esp_flag = (size_t) esp & 3;
  void *p;
  if (esp_flag)
    {
      esp = (char *) ((size_t) esp ^ esp_flag);
      efp = (struct getmem_frame *) esp;
      esp =
        (char *) ((Align_t *) esp +
                  (sizeof (struct getmem_frame) + sizeof (Align_t) +
                   -1) / sizeof (Align_t));
      *((char **) esp) = 0;
    LGOTO:switch ((*efp).call_id)
        {
        case 0:
          goto L_CALL3;
        }
      goto L_CALL3;
    }
  efp = (struct getmem_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct getmem_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  p = try_getmem (size);
  if (p == 0)
    {
      new_esp = esp;
      while (gc (new_esp, scan),
             __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 0))
        {
          efp->p = p;
          efp->size = size;
          efp->scan = scan;
          efp->call_id = 0;
          return (void *) 0 - 1;
        L_CALL3:;
          p = efp->p;
          size = efp->size;
          scan = efp->scan;
          new_esp = esp + 1;
        }
      p = try_getmem (size);
      if (p == 0)
        {
          error ("getmem: No enough memory.");
        }
    }
  return p;
}

int hsc_main (char *esp, closure_t *, int, struct array_object *);

struct main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  size_t strobj_asize;
  size_t strobj_size;
  int len;
  struct array_object **body;
  int i;
  size_t hsc_argv_asize;
  size_t hsc_argv_size;
  struct array_object *hsc_argv;
  int tmp4;
  void *tmp2;
  void *tmp3;
  char **argv;
  int argc;
  closure_t scan1;
};

struct scan1_in_main_frame
{
  char *tmp_esp;
  char *argp;
  int call_id;
  struct main_frame *xfp;
};

char *
scan1_in_main (char *esp, void *xfp0)
{
  char *new_esp;
  struct scan1_in_main_frame *efp;
  struct main_frame *xfp = xfp0;
  size_t esp_flag = (size_t) esp & 3;
  char *parmp = (char *) ((size_t) esp ^ esp_flag);
LGOTO:;
  efp = (struct scan1_in_main_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct scan1_in_main_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  xfp->argv = move (xfp->argv);
  return 0;
  return 0;
}

int
main (int argc, char **argv)
{
  struct main_frame *efp;
  char *new_esp;
  char estack[16384];
  char *esp = estack;
  void *tmp3;
  void *tmp2;
  int tmp4;
  struct array_object *hsc_argv;
  size_t hsc_argv_size;
  size_t hsc_argv_asize;
  int i;
  struct array_object **body;
  int len;
  size_t strobj_size;
  size_t strobj_asize;
LGOTO:;
  efp = (struct main_frame *) esp;
  esp =
    (char *) ((Align_t *) esp +
              (sizeof (struct main_frame) + sizeof (Align_t) +
               -1) / sizeof (Align_t));
  *((char **) esp) = 0;
  gc_init (0, 0, 0, 0);
  hsc_argv_size =
    sizeof (struct array_object) + argc * sizeof (struct array_object *);
  hsc_argv_asize =
    (hsc_argv_size + sizeof (double) +
     -1) / sizeof (double) * sizeof (double);
  {
    new_esp = esp;
    while (__builtin_expect
           ((tmp2 =
             getmem (new_esp, &efp->scan1, hsc_argv_asize)) == (void *) 0 - 1,
            0)
           && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 1))
      {
        char *goto_fr;
        *((char **) esp) = 0;
        efp->strobj_asize = strobj_asize;
        efp->strobj_size = strobj_size;
        efp->len = len;
        efp->body = body;
        efp->i = i;
        efp->hsc_argv_asize = hsc_argv_asize;
        efp->hsc_argv_size = hsc_argv_size;
        efp->hsc_argv = hsc_argv;
        efp->tmp4 = tmp4;
        efp->tmp2 = tmp2;
        efp->tmp3 = tmp3;
        efp->argv = argv;
        efp->argc = argc;
        efp->scan1.fun = scan1_in_main;
        efp->scan1.fr = (void *) efp;
        goto_fr = lw_call (efp->tmp_esp);
        if ((char *) goto_fr == (char *) efp)
          goto LGOTO;
        strobj_asize = efp->strobj_asize;
        strobj_size = efp->strobj_size;
        len = efp->len;
        body = efp->body;
        i = efp->i;
        hsc_argv_asize = efp->hsc_argv_asize;
        hsc_argv_size = efp->hsc_argv_size;
        hsc_argv = efp->hsc_argv;
        tmp4 = efp->tmp4;
        tmp2 = efp->tmp2;
        tmp3 = efp->tmp3;
        argv = efp->argv;
        argc = efp->argc;
        new_esp = esp + 1;
      }
    hsc_argv = (struct array_object *) tmp2;
  }
  (*hsc_argv).d = (struct desc_struct *) 1;
  (*hsc_argv).length = argc;
  (*hsc_argv).el_size = sizeof (struct array_object *);
  (*hsc_argv).size = hsc_argv_size;
  (*hsc_argv).asize = hsc_argv_asize;
  body = (struct array_object **) (*hsc_argv).body;
  {
    i = 0;
    for (; i < argc; ++i)
      {
        len = 1 + strlen (argv[i]);
        strobj_size = sizeof (struct array_object) + len * sizeof (char);
        strobj_asize =
          (strobj_size + sizeof (double) +
           -1) / sizeof (double) * sizeof (double);
        {
          new_esp = esp;
          while (__builtin_expect
                 ((tmp3 =
                   getmem (new_esp, &efp->scan1,
                           strobj_asize)) == (void *) 0 - 1, 0)
                 && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0,
                                      1))
            {
              char *goto_fr;
              *((char **) esp) = 0;
              efp->strobj_asize = strobj_asize;
              efp->strobj_size = strobj_size;
              efp->len = len;
              efp->body = body;
              efp->i = i;
              efp->hsc_argv_asize = hsc_argv_asize;
              efp->hsc_argv_size = hsc_argv_size;
              efp->hsc_argv = hsc_argv;
              efp->tmp4 = tmp4;
              efp->tmp2 = tmp2;
              efp->tmp3 = tmp3;
              efp->argv = argv;
              efp->argc = argc;
              efp->scan1.fun = scan1_in_main;
              efp->scan1.fr = (void *) efp;
              goto_fr = lw_call (efp->tmp_esp);
              if ((char *) goto_fr == (char *) efp)
                goto LGOTO;
              strobj_asize = efp->strobj_asize;
              strobj_size = efp->strobj_size;
              len = efp->len;
              body = efp->body;
              i = efp->i;
              hsc_argv_asize = efp->hsc_argv_asize;
              hsc_argv_size = efp->hsc_argv_size;
              hsc_argv = efp->hsc_argv;
              tmp4 = efp->tmp4;
              tmp2 = efp->tmp2;
              tmp3 = efp->tmp3;
              argv = efp->argv;
              argc = efp->argc;
              new_esp = esp + 1;
            }
          body[i] = (struct array_object *) tmp3;
        }
        (*body[i]).d = (struct desc_struct *) 3;
        (*body[i]).length = len;
        (*body[i]).el_size = sizeof (char);
        (*body[i]).size = strobj_size;
        (*body[i]).asize = strobj_asize;
        strncpy ((char *) (*body[i]).body, argv[i], len);
      }
  }
  {
    new_esp = esp;
    while (__builtin_expect
           ((tmp4 =
             hsc_main (new_esp, &efp->scan1, argc, hsc_argv)) == (int) 0 - 1,
            0)
           && __builtin_expect ((efp->tmp_esp = *((char **) esp)) != 0, 1))
      {
        char *goto_fr;
        *((char **) esp) = 0;
        efp->strobj_asize = strobj_asize;
        efp->strobj_size = strobj_size;
        efp->len = len;
        efp->body = body;
        efp->i = i;
        efp->hsc_argv_asize = hsc_argv_asize;
        efp->hsc_argv_size = hsc_argv_size;
        efp->hsc_argv = hsc_argv;
        efp->tmp4 = tmp4;
        efp->tmp2 = tmp2;
        efp->tmp3 = tmp3;
        efp->argv = argv;
        efp->argc = argc;
        efp->scan1.fun = scan1_in_main;
        efp->scan1.fr = (void *) efp;
        goto_fr = lw_call (efp->tmp_esp);
        if ((char *) goto_fr == (char *) efp)
          goto LGOTO;
        strobj_asize = efp->strobj_asize;
        strobj_size = efp->strobj_size;
        len = efp->len;
        body = efp->body;
        i = efp->i;
        hsc_argv_asize = efp->hsc_argv_asize;
        hsc_argv_size = efp->hsc_argv_size;
        hsc_argv = efp->hsc_argv;
        tmp4 = efp->tmp4;
        tmp2 = efp->tmp2;
        tmp3 = efp->tmp3;
        argv = efp->argv;
        argc = efp->argc;
        new_esp = esp + 1;
      }
    return tmp4;
  }
}
