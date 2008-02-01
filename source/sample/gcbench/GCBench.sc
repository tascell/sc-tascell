(%line 1 "sample/gcbench/GCBench.c")
(%line 1 "/usr/include/stdio.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/machine/_types.h")
(%line 45 "/usr/include/machine/_types.h")
(deftype csym::__int8_t signed-char)
(deftype csym::__uint8_t unsigned-char)
(deftype csym::__int16_t short)
(deftype csym::__uint16_t unsigned-short)
(deftype csym::__int32_t int)
(deftype csym::__uint32_t unsigned-int)
(deftype csym::__int64_t int)
(deftype csym::__uint64_t unsigned-int)
(%line 72 "/usr/include/machine/_types.h")
(deftype csym::__clock_t unsigned-long)
(deftype csym::__cpumask_t unsigned-int)
(deftype csym::__critical_t csym::__int32_t)
(deftype csym::__double_t double)
(deftype csym::__float_t double)
(deftype csym::__intfptr_t csym::__int32_t)
(deftype csym::__intmax_t csym::__int64_t)
(deftype csym::__intptr_t csym::__int32_t)
(deftype csym::__int_fast8_t csym::__int32_t)
(deftype csym::__int_fast16_t csym::__int32_t)
(deftype csym::__int_fast32_t csym::__int32_t)
(deftype csym::__int_fast64_t csym::__int64_t)
(deftype csym::__int_least8_t csym::__int8_t)
(deftype csym::__int_least16_t csym::__int16_t)
(deftype csym::__int_least32_t csym::__int32_t)
(deftype csym::__int_least64_t csym::__int64_t)
(deftype csym::__ptrdiff_t csym::__int32_t)
(deftype csym::__register_t csym::__int32_t)
(deftype csym::__segsz_t csym::__int32_t)
(deftype csym::__size_t csym::__uint32_t)
(deftype csym::__ssize_t csym::__int32_t)
(deftype csym::__time_t csym::__int32_t)
(deftype csym::__uintfptr_t csym::__uint32_t)
(deftype csym::__uintmax_t csym::__uint64_t)
(deftype csym::__uintptr_t csym::__uint32_t)
(deftype csym::__uint_fast8_t csym::__uint32_t)
(deftype csym::__uint_fast16_t csym::__uint32_t)
(deftype csym::__uint_fast32_t csym::__uint32_t)
(deftype csym::__uint_fast64_t csym::__uint64_t)
(deftype csym::__uint_least8_t csym::__uint8_t)
(deftype csym::__uint_least16_t csym::__uint16_t)
(deftype csym::__uint_least32_t csym::__uint32_t)
(deftype csym::__uint_least64_t csym::__uint64_t)
(deftype csym::__u_register_t csym::__uint32_t)
(deftype csym::__vm_offset_t csym::__uint32_t)
(deftype csym::__vm_ooffset_t csym::__int64_t)
(deftype csym::__vm_paddr_t csym::__uint32_t)
(deftype csym::__vm_pindex_t csym::__uint64_t)
(deftype csym::__vm_size_t csym::__uint32_t)
(%line 120 "/usr/include/machine/_types.h")
(deftype csym::__va_list csym::__builtin_va_list)
(%line 126 "/usr/include/machine/_types.h")
(deftype csym::__gnuc_va_list csym::__va_list)
(%line 38 "/usr/include/sys/_types.h")
(deftype csym::__clockid_t csym::__int32_t)
(deftype csym::__fflags_t csym::__uint32_t)
(deftype csym::__fsblkcnt_t csym::__uint64_t)
(deftype csym::__fsfilcnt_t csym::__uint64_t)
(deftype csym::__gid_t csym::__uint32_t)
(deftype csym::__id_t csym::__int64_t)
(deftype csym::__ino_t csym::__uint32_t)
(deftype csym::__key_t long)
(deftype csym::__lwpid_t csym::__int32_t)
(deftype csym::__mode_t csym::__uint16_t)
(deftype csym::__nl_item int)
(deftype csym::__nlink_t csym::__uint16_t)
(deftype csym::__off_t csym::__int64_t)
(deftype csym::__pid_t csym::__int32_t)
(deftype csym::__rlim_t csym::__int64_t)
(deftype csym::__sa_family_t csym::__uint8_t)
(deftype csym::__socklen_t csym::__uint32_t)
(deftype csym::__suseconds_t long)
(deftype csym::__timer_t csym::__int32_t)
(deftype csym::__uid_t csym::__uint32_t)
(deftype csym::__useconds_t unsigned-int)
(%line 78 "/usr/include/sys/_types.h")
(deftype csym::__ct_rune_t int)
(deftype csym::__rune_t csym::__ct_rune_t)
(deftype csym::__wchar_t csym::__ct_rune_t)
(deftype csym::__wint_t csym::__ct_rune_t)
(deftype csym::__dev_t csym::__uint32_t)
(deftype csym::__fixpt_t csym::__uint32_t)
(%line 91 "/usr/include/sys/_types.h")
(def (union csym::unnamed0) (decl csym::__mbstate8 (array char 128))
 (decl csym::_MBSTATE@l csym::__int64_t))
(deftype csym::__mbstate_t (union csym::unnamed0))
(%line 47 "/usr/include/stdio.h")
(deftype csym::fpos_t csym::__off_t)
(deftype csym::size_t csym::__size_t)
(deftype csym::va_list csym::__va_list)
(%line 70 "/usr/include/stdio.h")
(def (struct csym::__sbuf) (decl csym::_base (ptr unsigned-char))
 (decl csym::_size int))
(%line 102 "/usr/include/stdio.h")
(def (struct csym::__S@file) (decl csym::_p (ptr unsigned-char))
 (decl csym::_r int) (decl csym::_w int) (decl csym::_flags short)
 (decl csym::_file short) (decl csym::_bf (struct csym::__sbuf))
 (decl csym::_lbfsize int) (decl csym::_cookie (ptr void))
 (decl csym::_close (ptr (fn int (ptr void))))
 (decl csym::_read (ptr (fn int (ptr void) (ptr char) int)))
 (decl csym::_seek (ptr (fn csym::fpos_t (ptr void) csym::fpos_t int)))
 (decl csym::_write (ptr (fn int (ptr void) (ptr (const char)) int)))
 (decl csym::_ub (struct csym::__sbuf))
 (decl csym::_extra (ptr (struct csym::__S@filex)))
 (decl csym::_ur int) (decl csym::_ubuf (array unsigned-char 3))
 (decl csym::_nbuf (array unsigned-char 1))
 (decl csym::_lb (struct csym::__sbuf)) (decl csym::_blksize int)
 (decl csym::_offset csym::fpos_t))
(deftype csym::@FILE (struct csym::__S@file))
(extern-decl csym::__stdinp (ptr csym::@FILE))
(extern-decl csym::__stdoutp (ptr csym::@FILE))
(extern-decl csym::__stderrp (ptr csym::@FILE))
(%line 208 "/usr/include/stdio.h")
(%line 212 "/usr/include/stdio.h")
(decl (csym::clearerr) (fn void (ptr csym::@FILE)))
(decl (csym::fclose) (fn int (ptr csym::@FILE)))
(decl (csym::feof) (fn int (ptr csym::@FILE)))
(decl (csym::ferror) (fn int (ptr csym::@FILE)))
(decl (csym::fflush) (fn int (ptr csym::@FILE)))
(decl (csym::fgetc) (fn int (ptr csym::@FILE)))
(decl (csym::fgetpos) (fn int (ptr csym::@FILE) (ptr csym::fpos_t)))
(decl (csym::fgets) (fn (ptr char) (ptr char) int (ptr csym::@FILE)))
(decl (csym::fopen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char))))
(decl (csym::fprintf)
 (fn int (ptr csym::@FILE) (ptr (const char)) va-arg))
(decl (csym::fputc) (fn int int (ptr csym::@FILE)))
(decl (csym::fputs) (fn int (ptr (const char)) (ptr csym::@FILE)))
(decl (csym::fread)
 (fn csym::size_t (ptr void) csym::size_t csym::size_t
  (ptr csym::@FILE)))
(decl (csym::freopen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char))
  (ptr csym::@FILE)))
(decl (csym::fscanf)
 (fn int (ptr csym::@FILE) (ptr (const char)) va-arg))
(decl (csym::fseek) (fn int (ptr csym::@FILE) long int))
(decl (csym::fsetpos)
 (fn int (ptr csym::@FILE) (ptr (const csym::fpos_t))))
(decl (csym::ftell) (fn long (ptr csym::@FILE)))
(decl (csym::fwrite)
 (fn csym::size_t (ptr (const void)) csym::size_t csym::size_t
  (ptr csym::@FILE)))
(decl (csym::getc) (fn int (ptr csym::@FILE)))
(decl (csym::getchar) (fn int void))
(decl (csym::gets) (fn (ptr char) (ptr char)))
(decl (csym::perror) (fn void (ptr (const char))))
(decl (csym::printf) (fn int (ptr (const char)) va-arg))
(decl (csym::putc) (fn int int (ptr csym::@FILE)))
(decl (csym::putchar) (fn int int))
(decl (csym::puts) (fn int (ptr (const char))))
(decl (csym::remove) (fn int (ptr (const char))))
(decl (csym::rename) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::rewind) (fn void (ptr csym::@FILE)))
(decl (csym::scanf) (fn int (ptr (const char)) va-arg))
(decl (csym::setbuf) (fn void (ptr csym::@FILE) (ptr char)))
(decl (csym::setvbuf)
 (fn int (ptr csym::@FILE) (ptr char) int csym::size_t))
(decl (csym::sprintf) (fn int (ptr char) (ptr (const char)) va-arg))
(decl (csym::sscanf)
 (fn int (ptr (const char)) (ptr (const char)) va-arg))
(decl (csym::tmpfile) (fn (ptr csym::@FILE) void))
(decl (csym::tmpnam) (fn (ptr char) (ptr char)))
(decl (csym::ungetc) (fn int int (ptr csym::@FILE)))
(decl (csym::vfprintf)
 (fn int (ptr csym::@FILE) (ptr (const char)) csym::__va_list))
(decl (csym::vprintf) (fn int (ptr (const char)) csym::__va_list))
(decl (csym::vsprintf)
 (fn int (ptr char) (ptr (const char)) csym::__va_list))
(decl (csym::snprintf)
 (fn int (ptr char) csym::size_t (ptr (const char)) va-arg))
(decl (csym::vfscanf)
 (fn int (ptr csym::@FILE) (ptr (const char)) csym::__va_list))
(decl (csym::vscanf) (fn int (ptr (const char)) csym::__va_list))
(decl (csym::vsnprintf)
 (fn int (ptr char) csym::size_t (ptr (const char)) csym::__va_list))
(decl (csym::vsscanf)
 (fn int (ptr (const char)) (ptr (const char)) csym::__va_list))
(%line 279 "/usr/include/stdio.h")
(decl (csym::ctermid) (fn (ptr char) (ptr char)))
(decl (csym::fdopen) (fn (ptr csym::@FILE) int (ptr (const char))))
(decl (csym::fileno) (fn int (ptr csym::@FILE)))
(decl (csym::pclose) (fn int (ptr csym::@FILE)))
(decl (csym::popen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char))))
(decl (csym::ftrylockfile) (fn int (ptr csym::@FILE)))
(decl (csym::flockfile) (fn void (ptr csym::@FILE)))
(decl (csym::funlockfile) (fn void (ptr csym::@FILE)))
(%line 298 "/usr/include/stdio.h")
(decl (csym::getc_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::getchar_unlocked) (fn int void))
(decl (csym::putc_unlocked) (fn int int (ptr csym::@FILE)))
(decl (csym::putchar_unlocked) (fn int int))
(decl (csym::clearerr_unlocked) (fn void (ptr csym::@FILE)))
(decl (csym::feof_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::ferror_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::fileno_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::fseeko) (fn int (ptr csym::@FILE) csym::__off_t int))
(decl (csym::ftello) (fn csym::__off_t (ptr csym::@FILE)))
(decl (csym::getw) (fn int (ptr csym::@FILE)))
(decl (csym::putw) (fn int int (ptr csym::@FILE)))
(decl (csym::tempnam)
 (fn (ptr char) (ptr (const char)) (ptr (const char))))
(%line 328 "/usr/include/stdio.h")
(decl (csym::asprintf)
 (fn int (ptr (ptr char)) (ptr (const char)) va-arg))
(decl (csym::ctermid_r) (fn (ptr char) (ptr char)))
(decl (csym::fgetln)
 (fn (ptr char) (ptr csym::@FILE) (ptr csym::size_t)))
(%line 336 "/usr/include/stdio.h")
(decl (csym::fmtcheck)
 (fn (ptr (const char)) (ptr (const char)) (ptr (const char))))
(decl (csym::fpurge) (fn int (ptr csym::@FILE)))
(decl (csym::setbuffer) (fn void (ptr csym::@FILE) (ptr char) int))
(decl (csym::setlinebuf) (fn int (ptr csym::@FILE)))
(decl (csym::vasprintf)
 (fn int (ptr (ptr char)) (ptr (const char)) csym::__va_list))
(%line 348 "/usr/include/stdio.h")
(extern-decl csym::sys_nerr (const int))
(extern-decl csym::sys_errlist (array (const (ptr (const char)))))
(%line 354 "/usr/include/stdio.h")
(decl (csym::funopen)
 (fn (ptr csym::@FILE) (ptr (const void))
  (ptr (fn int (ptr void) (ptr char) int))
  (ptr (fn int (ptr void) (ptr (const char)) int))
  (ptr (fn csym::fpos_t (ptr void) csym::fpos_t int))
  (ptr (fn int (ptr void)))))
(%line 367 "/usr/include/stdio.h")
(decl (csym::ftruncate) (fn int int csym::__off_t))
(decl (csym::lseek) (fn csym::__off_t int csym::__off_t int))
(decl (csym::mmap)
 (fn (ptr void) (ptr void) csym::size_t int int int csym::__off_t))
(decl (csym::truncate) (fn int (ptr (const char)) csym::__off_t))
(%line 386 "/usr/include/stdio.h")
(decl (csym::__srget) (fn int (ptr csym::@FILE)))
(decl (csym::__swbuf) (fn int int (ptr csym::@FILE)))
(%line 395 "/usr/include/stdio.h")
(static-def (csym::__sputc csym::_c csym::_p)
 (fn int int (ptr csym::@FILE))
 (if (or (>= (-- (fref (mref csym::_p) csym::_w)) 0)
         (and (>= (fref (mref csym::_p) csym::_w)
                  (fref (mref csym::_p) csym::_lbfsize))
              (!= (cast char csym::_c) #\Newline)))
     (return (= (mref (inc (fref (mref csym::_p) csym::_p))) csym::_c))
     (return (csym::__swbuf csym::_c csym::_p))))
(%line 420 "/usr/include/stdio.h")
(extern-decl csym::__isthreaded int)
(%line 454 "/usr/include/stdio.h")
(%line 1 "/usr/include/stdlib.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 46 "/usr/include/stdlib.h")
(deftype csym::rune_t csym::__rune_t)
(%line 58 "/usr/include/stdlib.h")
(deftype csym::wchar_t csym::__wchar_t)
(def (struct csym::unnamed1) (decl csym::quot int) (decl csym::rem int))
(deftype csym::div_t (struct csym::unnamed1))
(def (struct csym::unnamed2) (decl csym::quot long)
 (decl csym::rem long))
(deftype csym::ldiv_t (struct csym::unnamed2))
(extern-decl csym::__mb_cur_max int)
(decl (csym::abort) (fn void void))
(decl (csym::abs) (fn int int))
(decl (csym::atexit) (fn int (ptr (fn void void))))
(decl (csym::atof) (fn double (ptr (const char))))
(decl (csym::atoi) (fn int (ptr (const char))))
(decl (csym::atol) (fn long (ptr (const char))))
(decl (csym::bsearch)
 (fn (ptr void) (ptr (const void)) (ptr (const void)) csym::size_t
  csym::size_t (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::calloc) (fn (ptr void) csym::size_t csym::size_t))
(decl (csym::div) (fn csym::div_t int int))
(decl (csym::exit) (fn void int))
(decl (csym::free) (fn void (ptr void)))
(decl (csym::getenv) (fn (ptr char) (ptr (const char))))
(decl (csym::labs) (fn long long))
(decl (csym::ldiv) (fn csym::ldiv_t long long))
(decl (csym::malloc) (fn (ptr void) csym::size_t))
(decl (csym::mblen) (fn int (ptr (const char)) csym::size_t))
(decl (csym::mbstowcs)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const char)) csym::size_t))
(decl (csym::mbtowc)
 (fn int (ptr csym::wchar_t) (ptr (const char)) csym::size_t))
(decl (csym::qsort)
 (fn void (ptr void) csym::size_t csym::size_t
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::rand) (fn int void))
(decl (csym::realloc) (fn (ptr void) (ptr void) csym::size_t))
(decl (csym::srand) (fn void unsigned-int))
(decl (csym::strtod) (fn double (ptr (const char)) (ptr (ptr char))))
(decl (csym::strtof) (fn float (ptr (const char)) (ptr (ptr char))))
(decl (csym::strtol) (fn long (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::strtold)
 (fn long-double (ptr (const char)) (ptr (ptr char))))
(decl (csym::strtoul)
 (fn unsigned-long (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::system) (fn int (ptr (const char))))
(decl (csym::wctomb) (fn int (ptr char) csym::wchar_t))
(decl (csym::wcstombs)
 (fn csym::size_t (ptr char) (ptr (const csym::wchar_t)) csym::size_t))
(%line 130 "/usr/include/stdlib.h")
(def (struct csym::unnamed3) (decl csym::quot long-long)
 (decl csym::rem long-long))
(deftype csym::lldiv_t (struct csym::unnamed3))
(decl (csym::atoll) (fn long-long (ptr (const char))))
(decl (csym::llabs) (fn long-long long-long))
(decl (csym::lldiv) (fn csym::lldiv_t long-long long-long))
(decl (csym::strtoll)
 (fn long-long (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::strtoull)
 (fn unsigned-long-long (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::_@e@XIT) (fn void int))
(%line 161 "/usr/include/stdlib.h")
(decl (csym::rand_r) (fn int (ptr unsigned-int)))
(decl (csym::setenv) (fn int (ptr (const char)) (ptr (const char)) int))
(decl (csym::unsetenv) (fn void (ptr (const char))))
(%line 175 "/usr/include/stdlib.h")
(decl (csym::drand48) (fn double void))
(decl (csym::erand48) (fn double (array unsigned-short 3)))
(decl (csym::getsubopt)
 (fn int (ptr (ptr char)) (ptr (const (ptr char))) (ptr (ptr char))))
(decl (csym::grantpt) (fn int int))
(decl (csym::initstate) (fn (ptr char) unsigned-long (ptr char) long))
(decl (csym::jrand48) (fn long (array unsigned-short 3)))
(decl (csym::lcong48) (fn void (array unsigned-short 7)))
(decl (csym::lrand48) (fn long void))
(decl (csym::mkstemp) (fn int (ptr char)))
(decl (csym::mktemp) (fn (ptr char) (ptr char)))
(decl (csym::mrand48) (fn long void))
(decl (csym::nrand48) (fn long (array unsigned-short 3)))
(decl (csym::posix_openpt) (fn int int))
(decl (csym::ptsname) (fn (ptr char) int))
(decl (csym::putenv) (fn int (ptr (const char))))
(decl (csym::random) (fn long void))
(decl (csym::realpath) (fn (ptr char) (ptr (const char)) (array char)))
(decl (csym::seed48) (fn (ptr unsigned-short) (array unsigned-short 3)))
(decl (csym::setkey) (fn int (ptr (const char))))
(decl (csym::setstate) (fn (ptr char) (ptr char)))
(decl (csym::srand48) (fn void long))
(decl (csym::srandom) (fn void unsigned-long))
(decl (csym::unlockpt) (fn int int))
(extern-decl csym::_malloc_options (ptr (const char)))
(extern-decl csym::_malloc_message
 (ptr
  (fn void (ptr (const char)) (ptr (const char)) (ptr (const char))
   (ptr (const char)))))
(%line 235 "/usr/include/stdlib.h")
(decl (csym::arc4random) (fn csym::__uint32_t void))
(decl (csym::arc4random_addrandom csym::dat csym::datlen)
 (fn void (ptr unsigned-char) int))
(decl (csym::arc4random_stir) (fn void void))
(decl (csym::getbsize) (fn (ptr char) (ptr int) (ptr long)))
(decl (csym::cgetcap) (fn (ptr char) (ptr char) (ptr (const char)) int))
(decl (csym::cgetclose) (fn int void))
(decl (csym::cgetent)
 (fn int (ptr (ptr char)) (ptr (ptr char)) (ptr (const char))))
(decl (csym::cgetfirst) (fn int (ptr (ptr char)) (ptr (ptr char))))
(decl (csym::cgetmatch) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::cgetnext) (fn int (ptr (ptr char)) (ptr (ptr char))))
(decl (csym::cgetnum) (fn int (ptr char) (ptr (const char)) (ptr long)))
(decl (csym::cgetset) (fn int (ptr (const char))))
(decl (csym::cgetstr)
 (fn int (ptr char) (ptr (const char)) (ptr (ptr char))))
(decl (csym::cgetustr)
 (fn int (ptr char) (ptr (const char)) (ptr (ptr char))))
(decl (csym::daemon) (fn int int int))
(decl (csym::devname) (fn (ptr char) int int))
(decl (csym::devname_r) (fn (ptr char) int int (ptr char) int))
(decl (csym::getloadavg) (fn int (array double) int))
(decl (csym::getprogname) (fn (ptr (const char)) void))
(decl (csym::heapsort)
 (fn int (ptr void) csym::size_t csym::size_t
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::mergesort)
 (fn int (ptr void) csym::size_t csym::size_t
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::qsort_r)
 (fn void (ptr void) csym::size_t csym::size_t (ptr void)
  (ptr (fn int (ptr void) (ptr (const void)) (ptr (const void))))))
(decl (csym::radixsort)
 (fn int (ptr (ptr (const unsigned-char))) int
  (ptr (const unsigned-char)) unsigned-int))
(decl (csym::reallocf) (fn (ptr void) (ptr void) csym::size_t))
(decl (csym::setprogname) (fn void (ptr (const char))))
(decl (csym::sradixsort)
 (fn int (ptr (ptr (const unsigned-char))) int
  (ptr (const unsigned-char)) unsigned-int))
(decl (csym::sranddev) (fn void void))
(decl (csym::srandomdev) (fn void void))
(decl (csym::strtoq)
 (fn csym::__int64_t (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::strtouq)
 (fn csym::__uint64_t (ptr (const char)) (ptr (ptr char)) int))
(extern-decl csym::suboptarg (ptr char))
(%line 1 "/usr/include/sys/time.h")
(%line 1 "/usr/include/sys/_timeval.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 35 "/usr/include/sys/_timeval.h")
(deftype csym::suseconds_t csym::__suseconds_t)
(deftype c2sc::$abstract csym::__time_t)
(%line 47 "/usr/include/sys/_timeval.h")
(def (struct csym::timeval) (decl csym::tv_sec long)
 (decl csym::tv_usec csym::suseconds_t))
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/machine/endian.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 152 "/usr/include/machine/endian.h")
(static-def (csym::__bswap64 csym::_x)
 (fn csym::__uint64_t csym::__uint64_t)
 (return (bit-or (>> csym::_x 56) (bit-and (>> csym::_x 40) 65280)
          (bit-and (>> csym::_x 24) 16711680)
          (bit-and (>> csym::_x 8) 4278190080)
          (bit-and (<< csym::_x 8) (<< (cast csym::__uint64_t 255) 32))
          (bit-and (<< csym::_x 24)
                   (<< (cast csym::__uint64_t 255) 40))
          (bit-and (<< csym::_x 40)
                   (<< (cast csym::__uint64_t 255) 48))
          (<< csym::_x 56))))
(static-def (csym::__bswap32 csym::_x)
 (fn csym::__uint32_t csym::__uint32_t)
 (register-decl csym::__@X csym::__uint32_t)
 (return (c2sc::exps (= csym::__@X csym::_x) csym::__@X)))
(static-def (csym::__bswap16 csym::_x)
 (fn csym::__uint16_t csym::__uint16_t)
 (register-decl csym::__@X csym::__uint16_t)
 (return (c2sc::exps (= csym::__@X csym::_x) csym::__@X)))
(%line 1 "/usr/include/sys/_types.h")
(%line 48 "/usr/include/sys/types.h")
(deftype csym::u_char unsigned-char)
(deftype csym::u_short unsigned-short)
(deftype csym::u_int unsigned-int)
(deftype csym::u_long unsigned-long)
(deftype csym::ushort unsigned-short)
(deftype csym::uint unsigned-int)
(%line 62 "/usr/include/sys/types.h")
(deftype csym::int8_t csym::__int8_t)
(deftype csym::int16_t csym::__int16_t)
(deftype csym::int32_t csym::__int32_t)
(deftype csym::int64_t csym::__int64_t)
(deftype csym::uint8_t csym::__uint8_t)
(deftype csym::uint16_t csym::__uint16_t)
(deftype csym::uint32_t csym::__uint32_t)
(deftype csym::uint64_t csym::__uint64_t)
(deftype csym::intptr_t csym::__intptr_t)
(deftype csym::uintptr_t csym::__uintptr_t)
(deftype csym::u_int8_t csym::__uint8_t)
(deftype csym::u_int16_t csym::__uint16_t)
(deftype csym::u_int32_t csym::__uint32_t)
(deftype csym::u_int64_t csym::__uint64_t)
(deftype csym::u_quad_t csym::__uint64_t)
(deftype csym::quad_t csym::__int64_t)
(deftype csym::qaddr_t (ptr csym::quad_t))
(deftype csym::caddr_t (ptr char))
(deftype csym::c_caddr_t (ptr (const char)))
(deftype csym::v_caddr_t (ptr (volatile char)))
(deftype csym::clock_t csym::__clock_t)
(deftype csym::clockid_t csym::__clockid_t)
(deftype csym::critical_t csym::__critical_t)
(deftype csym::daddr_t csym::__int64_t)
(deftype csym::dev_t csym::__dev_t)
(deftype csym::fflags_t csym::__fflags_t)
(deftype csym::fixpt_t csym::__fixpt_t)
(deftype csym::fsblkcnt_t csym::__fsblkcnt_t)
(deftype csym::fsfilcnt_t csym::__fsfilcnt_t)
(deftype csym::gid_t csym::__gid_t)
(deftype csym::in_addr_t csym::__uint32_t)
(deftype csym::in_port_t csym::__uint16_t)
(deftype csym::id_t csym::__id_t)
(deftype csym::ino_t csym::__ino_t)
(deftype csym::key_t csym::__key_t)
(deftype csym::lwpid_t csym::__lwpid_t)
(deftype csym::mode_t csym::__mode_t)
(deftype csym::nlink_t csym::__nlink_t)
(deftype csym::off_t csym::__off_t)
(deftype c2sc::$abstract csym::__pid_t)
(deftype csym::register_t csym::__register_t)
(deftype csym::rlim_t csym::__rlim_t)
(deftype csym::segsz_t csym::__segsz_t)
(%line 221 "/usr/include/sys/types.h")
(deftype csym::ssize_t csym::__ssize_t)
(%line 236 "/usr/include/sys/types.h")
(deftype csym::timer_t csym::__timer_t)
(deftype csym::u_register_t csym::__u_register_t)
(deftype csym::uid_t csym::__uid_t)
(deftype csym::useconds_t csym::__useconds_t)
(deftype csym::vm_offset_t csym::__vm_offset_t)
(deftype csym::vm_ooffset_t csym::__vm_ooffset_t)
(deftype csym::vm_paddr_t csym::__vm_paddr_t)
(deftype csym::vm_pindex_t csym::__vm_pindex_t)
(deftype csym::vm_size_t csym::__vm_size_t)
(%line 1 "/usr/include/sys/select.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/_sigset.h")
(%line 51 "/usr/include/sys/_sigset.h")
(def (struct csym::__sigset)
 (decl csym::__bits (array csym::__uint32_t 4)))
(deftype csym::__sigset_t (struct csym::__sigset))
(%line 1 "/usr/include/sys/_timeval.h")
(%line 1 "/usr/include/sys/timespec.h")
(%line 46 "/usr/include/sys/timespec.h")
(def (struct csym::timespec) (decl csym::tv_sec double)
 (decl csym::tv_nsec long))
(%line 42 "/usr/include/sys/select.h")
(deftype csym::__fd_mask unsigned-long)
(deftype csym::fd_mask csym::__fd_mask)
(deftype csym::sigset_t csym::__sigset_t)
(%line 71 "/usr/include/sys/select.h")
(def (struct csym::fd_set)
 (decl csym::__fds_bits
  (array csym::__fd_mask
   (/ (+ (cast unsigned-int 1024) (- (* (sizeof csym::__fd_mask) 8) 1))
      (* (sizeof csym::__fd_mask) 8)))))
(deftype csym::fd_set (struct csym::fd_set))
(%line 97 "/usr/include/sys/select.h")
(decl (csym::pselect)
 (fn int int (ptr csym::fd_set) (ptr csym::fd_set) (ptr csym::fd_set)
  (ptr (const (struct csym::timespec))) (ptr (const csym::sigset_t))))
(decl (csym::select)
 (fn int int (ptr csym::fd_set) (ptr csym::fd_set) (ptr csym::fd_set)
  (ptr (struct csym::timeval))))
(%line 307 "/usr/include/sys/types.h")
(%line 324 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/timespec.h")
(%line 40 "/usr/include/sys/time.h")
(def (struct csym::timezone) (decl csym::tz_minuteswest int)
 (decl csym::tz_dsttime int))
(def (struct csym::bintime) (decl csym::sec double)
 (decl csym::frac csym::uint64_t))
(static-def (csym::bintime_addx csym::bt csym::x)
 (fn void (ptr (struct csym::bintime)) csym::uint64_t)
 (decl csym::u csym::uint64_t)
 (= csym::u (fref (mref csym::bt) csym::frac))
 (+= (fref (mref csym::bt) csym::frac) csym::x)
 (if (> csym::u (fref (mref csym::bt) csym::frac))
     (inc (fref (mref csym::bt) csym::sec))))
(static-def (csym::bintime_add csym::bt csym::bt2)
 (fn void (ptr (struct csym::bintime))
  (ptr (const (struct csym::bintime))))
 (decl csym::u csym::uint64_t)
 (= csym::u (fref (mref csym::bt) csym::frac))
 (+= (fref (mref csym::bt) csym::frac)
  (fref (mref csym::bt2) csym::frac))
 (if (> csym::u (fref (mref csym::bt) csym::frac))
     (inc (fref (mref csym::bt) csym::sec)))
 (+= (fref (mref csym::bt) csym::sec)
  (fref (mref csym::bt2) csym::sec)))
(static-def (csym::bintime_sub csym::bt csym::bt2)
 (fn void (ptr (struct csym::bintime))
  (ptr (const (struct csym::bintime))))
 (decl csym::u csym::uint64_t)
 (= csym::u (fref (mref csym::bt) csym::frac))
 (-= (fref (mref csym::bt) csym::frac)
  (fref (mref csym::bt2) csym::frac))
 (if (< csym::u (fref (mref csym::bt) csym::frac))
     (dec (fref (mref csym::bt) csym::sec)))
 (-= (fref (mref csym::bt) csym::sec)
  (fref (mref csym::bt2) csym::sec)))
(%line 107 "/usr/include/sys/time.h")
(static-def (csym::bintime2timespec csym::bt csym::ts)
 (fn void (ptr (const (struct csym::bintime)))
  (ptr (struct csym::timespec)))
 (= (fref (mref csym::ts) csym::tv_sec)
    (fref (mref csym::bt) csym::sec))
 (= (fref (mref csym::ts) csym::tv_nsec)
    (>>
     (* (cast csym::uint64_t 1000000000)
        (cast csym::uint32_t
         (>> (fref (mref csym::bt) csym::frac) 32)))
     32)))
(static-def (csym::timespec2bintime csym::ts csym::bt)
 (fn void (ptr (const (struct csym::timespec)))
  (ptr (struct csym::bintime)))
 (= (fref (mref csym::bt) csym::sec)
    (fref (mref csym::ts) csym::tv_sec))
 (= (fref (mref csym::bt) csym::frac)
    (* (fref (mref csym::ts) csym::tv_nsec)
       (cast csym::uint64_t (cast long-long 18446744073)))))
(static-def (csym::bintime2timeval csym::bt csym::tv)
 (fn void (ptr (const (struct csym::bintime)))
  (ptr (struct csym::timeval)))
 (= (fref (mref csym::tv) csym::tv_sec)
    (fref (mref csym::bt) csym::sec))
 (= (fref (mref csym::tv) csym::tv_usec)
    (>>
     (* (cast csym::uint64_t 1000000)
        (cast csym::uint32_t
         (>> (fref (mref csym::bt) csym::frac) 32)))
     32)))
(static-def (csym::timeval2bintime csym::tv csym::bt)
 (fn void (ptr (const (struct csym::timeval)))
  (ptr (struct csym::bintime)))
 (= (fref (mref csym::bt) csym::sec)
    (fref (mref csym::tv) csym::tv_sec))
 (= (fref (mref csym::bt) csym::frac)
    (* (fref (mref csym::tv) csym::tv_usec)
       (cast csym::uint64_t (cast long-long 18446744073709)))))
(%line 219 "/usr/include/sys/time.h")
(def (struct csym::itimerval)
 (decl csym::it_interval (struct csym::timeval))
 (decl csym::it_value (struct csym::timeval)))
(%line 227 "/usr/include/sys/time.h")
(def (struct csym::clockinfo) (decl csym::hz int) (decl csym::tick int)
 (decl csym::spare int) (decl csym::stathz int) (decl csym::profhz int))
(%line 1 "/usr/include/time.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/timespec.h")
(%line 95 "/usr/include/time.h")
(def (struct csym::tm) (decl csym::tm_sec int) (decl csym::tm_min int)
 (decl csym::tm_hour int) (decl csym::tm_mday int)
 (decl csym::tm_mon int) (decl csym::tm_year int)
 (decl csym::tm_wday int) (decl csym::tm_yday int)
 (decl csym::tm_isdst int) (decl csym::tm_gmtoff long)
 (decl csym::tm_zone (ptr char)))
(extern-decl csym::tzname (array (ptr char)))
(decl (csym::asctime) (fn (ptr char) (ptr (const (struct csym::tm)))))
(decl (csym::clock) (fn csym::clock_t void))
(decl (csym::ctime) (fn (ptr char) (ptr (const double))))
(decl (csym::difftime) (fn double double double))
(decl (csym::gmtime) (fn (ptr (struct csym::tm)) (ptr (const double))))
(decl (csym::localtime)
 (fn (ptr (struct csym::tm)) (ptr (const double))))
(decl (csym::mktime) (fn double (ptr (struct csym::tm))))
(decl (csym::strftime)
 (fn csym::size_t (ptr char) csym::size_t (ptr (const char))
  (ptr (const (struct csym::tm)))))
(decl (csym::time) (fn double (ptr double)))
(decl (csym::tzset) (fn void void))
(decl (csym::clock_getres)
 (fn int csym::clockid_t (ptr (struct csym::timespec))))
(decl (csym::clock_gettime)
 (fn int csym::clockid_t (ptr (struct csym::timespec))))
(decl (csym::clock_settime)
 (fn int csym::clockid_t (ptr (const (struct csym::timespec)))))
(decl (csym::nanosleep)
 (fn int (ptr (const (struct csym::timespec)))
  (ptr (struct csym::timespec))))
(decl (csym::asctime_r)
 (fn (ptr char) (ptr (const (struct csym::tm))) (ptr char)))
(decl (csym::ctime_r) (fn (ptr char) (ptr (const double)) (ptr char)))
(decl (csym::gmtime_r)
 (fn (ptr (struct csym::tm)) (ptr (const double))
  (ptr (struct csym::tm))))
(decl (csym::localtime_r)
 (fn (ptr (struct csym::tm)) (ptr (const double))
  (ptr (struct csym::tm))))
(decl (csym::strptime)
 (fn (ptr char) (ptr (const char)) (ptr (const char))
  (ptr (struct csym::tm))))
(decl (csym::timezone) (fn (ptr char) int int))
(decl (csym::tzsetwall) (fn void void))
(decl (csym::timelocal) (fn double (const (ptr (struct csym::tm)))))
(decl (csym::timegm) (fn double (const (ptr (struct csym::tm)))))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 305 "/usr/include/sys/time.h")
(decl (csym::adjtime)
 (fn int (ptr (const (struct csym::timeval)))
  (ptr (struct csym::timeval))))
(decl (csym::futimes) (fn int int (ptr (const (struct csym::timeval)))))
(decl (csym::getitimer) (fn int int (ptr (struct csym::itimerval))))
(decl (csym::gettimeofday)
 (fn int (ptr (struct csym::timeval)) (ptr (struct csym::timezone))))
(decl (csym::lutimes)
 (fn int (ptr (const char)) (ptr (const (struct csym::timeval)))))
(decl (csym::setitimer)
 (fn int int (ptr (const (struct csym::itimerval)))
  (ptr (struct csym::itimerval))))
(decl (csym::settimeofday)
 (fn int (ptr (const (struct csym::timeval)))
  (ptr (const (struct csym::timezone)))))
(decl (csym::utimes)
 (fn int (ptr (const char)) (ptr (const (struct csym::timeval)))))
(%line 64 "sample/gcbench/GCBench.c")
(def (csym::stats_rtclock) (fn unsigned-int void)
 (decl csym::t (struct csym::timeval))
 (decl csym::tz (struct csym::timezone))
 (if (== (csym::gettimeofday (ptr csym::t) (ptr csym::tz)) (- 1))
     (return 0))
 (return (+ (* (fref csym::t csym::tv_sec) 1000)
            (/ (fref csym::t csym::tv_usec) 1000))))
(static-def csym::K@s@TRETCH@t@REE@d@EPTH (const int) 18)
(static-def csym::K@l@ONG@l@IVED@t@REE@d@EPTH (const int) 16)
(static-def csym::K@a@RRAY@s@IZE (const int) 500000)
(static-def csym::K@m@IN@t@REE@d@EPTH (const int) 4)
(static-def csym::K@m@AX@t@REE@d@EPTH (const int) 16)
(def (struct csym::@n@ODE0_STRUCT)
 (decl csym::left (ptr (struct csym::@n@ODE0_STRUCT)))
 (decl csym::right (ptr (struct csym::@n@ODE0_STRUCT)))
 (decl csym::i int) (decl csym::j int))
(deftype csym::@n@ODE0 (struct csym::@n@ODE0_STRUCT))
(deftype csym::@n@ODE (ptr csym::@n@ODE0))
(def (csym::INIT_@n@ODE csym::me csym::l csym::r)
 (fn void csym::@n@ODE csym::@n@ODE csym::@n@ODE)
 (= (fref (mref csym::me) csym::left) csym::l)
 (= (fref (mref csym::me) csym::right) csym::r))
(def (csym::DESTROY_@n@ODE csym::me) (fn void csym::@n@ODE)
 (if (fref (mref csym::me) csym::left)
     (begin (csym::DESTROY_@n@ODE (fref (mref csym::me) csym::left))))
 (if (fref (mref csym::me) csym::right)
     (begin (csym::DESTROY_@n@ODE (fref (mref csym::me) csym::right))))
 (csym::free csym::me))
(static-def (csym::@t@REE@s@IZE csym::i) (fn int int)
 (return (- (<< 1 (+ csym::i 1)) 1)))
(static-def (csym::@n@UM@i@TERS csym::i) (fn int int)
 (return (/ (* 2 (csym::@t@REE@s@IZE csym::K@s@TRETCH@t@REE@d@EPTH))
            (csym::@t@REE@s@IZE csym::i))))
(static-def (csym::@p@OPULATE csym::I@d@EPTH csym::THIS@n@ODE)
 (fn void int csym::@n@ODE)
 (if (<= csym::I@d@EPTH 0) (begin (return))
     (begin (dec csym::I@d@EPTH)
      (= (fref (mref csym::THIS@n@ODE) csym::left)
         (csym::calloc 1 (sizeof csym::@n@ODE0)))
      (= (fref (mref csym::THIS@n@ODE) csym::right)
         (csym::calloc 1 (sizeof csym::@n@ODE0)))
      (csym::@p@OPULATE csym::I@d@EPTH
       (fref (mref csym::THIS@n@ODE) csym::left))
      (csym::@p@OPULATE csym::I@d@EPTH
       (fref (mref csym::THIS@n@ODE) csym::right)))))
(static-def (csym::@m@AKE@t@REE csym::I@d@EPTH) (fn csym::@n@ODE int)
 (decl csym::result csym::@n@ODE)
 (if (<= csym::I@d@EPTH 0)
     (begin (= csym::result (csym::calloc 1 (sizeof csym::@n@ODE0)))
      (%line 150 "sample/gcbench/GCBench.c") (return csym::result))
     (begin
      (def csym::left csym::@n@ODE
       (csym::@m@AKE@t@REE (- csym::I@d@EPTH 1)))
      (def csym::right csym::@n@ODE
       (csym::@m@AKE@t@REE (- csym::I@d@EPTH 1)))
      (= csym::result (csym::malloc (sizeof csym::@n@ODE0)))
      (%line 159 "sample/gcbench/GCBench.c")
      (csym::INIT_@n@ODE csym::result csym::left csym::right)
      (return csym::result))))
(static-def (csym::@p@RINT@d@IAGNOSTICS) (fn void))
(%line 173 "sample/gcbench/GCBench.c")
(static-def (csym::@t@IME@c@ONSTRUCTION csym::depth) (fn void int)
 (decl csym::T@s@TART long) (decl csym::T@f@INISH long)
 (def csym::I@n@UM@i@TERS int (csym::@n@UM@i@TERS csym::depth))
 (decl csym::TEMP@t@REE csym::@n@ODE) (decl csym::i int)
 (csym::printf "Creating %d trees of depth %d~|12|" csym::I@n@UM@i@TERS
  csym::depth)
 (= csym::T@s@TART (csym::stats_rtclock))
 (for ((= csym::i 0) (< csym::i csym::I@n@UM@i@TERS) (++ csym::i))
  (= csym::TEMP@t@REE (csym::calloc 1 (sizeof csym::@n@ODE0)))
  (%line 190 "sample/gcbench/GCBench.c")
  (csym::@p@OPULATE csym::depth csym::TEMP@t@REE)
  (csym::DESTROY_@n@ODE csym::TEMP@t@REE) (= csym::TEMP@t@REE 0))
 (= csym::T@f@INISH (csym::stats_rtclock))
 (csym::printf "~|11|Top down construction took %d msec~|12|"
  (- csym::T@f@INISH csym::T@s@TART))
 (= csym::T@s@TART (csym::stats_rtclock))
 (for ((= csym::i 0) (< csym::i csym::I@n@UM@i@TERS) (++ csym::i))
  (= csym::TEMP@t@REE (csym::@m@AKE@t@REE csym::depth))
  (csym::DESTROY_@n@ODE csym::TEMP@t@REE) (= csym::TEMP@t@REE 0))
 (= csym::T@f@INISH (csym::stats_rtclock))
 (csym::printf "~|11|Bottom up construction took %d msec~|12|"
  (- csym::T@f@INISH csym::T@s@TART)))
(def (csym::main) (fn int) (decl csym::root csym::@n@ODE)
 (decl csym::LONG@l@IVED@t@REE csym::@n@ODE)
 (decl csym::TEMP@t@REE csym::@n@ODE) (decl csym::T@s@TART long)
 (decl csym::T@f@INISH long) (decl csym::T@e@LAPSED long)
 (decl csym::i int) (decl csym::d int) (decl csym::array (ptr double))
 (%line 228 "sample/gcbench/GCBench.c")
 (csym::printf "Garbage Collector Test~|12|")
 (csym::printf " Live storage will peak at %d bytes.~|12|~|12|"
  (+ (* (* 2 (sizeof csym::@n@ODE0))
        (csym::@t@REE@s@IZE csym::K@l@ONG@l@IVED@t@REE@d@EPTH))
     (* (sizeof double) csym::K@a@RRAY@s@IZE)))
 (csym::printf " Stretching memory with a binary tree of depth %d~|12|"
  csym::K@s@TRETCH@t@REE@d@EPTH)
 (csym::@p@RINT@d@IAGNOSTICS) (%line 239 "sample/gcbench/GCBench.c")
 (= csym::T@s@TART (csym::stats_rtclock))
 (= csym::TEMP@t@REE
    (csym::@m@AKE@t@REE csym::K@s@TRETCH@t@REE@d@EPTH))
 (csym::DESTROY_@n@ODE csym::TEMP@t@REE) (= csym::TEMP@t@REE 0)
 (csym::printf " Creating a long-lived binary tree of depth %d~|12|"
  csym::K@l@ONG@l@IVED@t@REE@d@EPTH)
 (= csym::LONG@l@IVED@t@REE (csym::calloc 1 (sizeof csym::@n@ODE0)))
 (%line 256 "sample/gcbench/GCBench.c")
 (csym::@p@OPULATE csym::K@l@ONG@l@IVED@t@REE@d@EPTH
  csym::LONG@l@IVED@t@REE)
 (csym::printf " Creating a long-lived array of %d doubles~|12|"
  csym::K@a@RRAY@s@IZE)
 (= csym::array
    (csym::malloc (* csym::K@a@RRAY@s@IZE (sizeof double))))
 (%line 269 "sample/gcbench/GCBench.c")
 (for
  ((= csym::i 0) (< csym::i (/ csym::K@a@RRAY@s@IZE 2)) (++ csym::i))
  (= (aref csym::array csym::i) (/ 1.0d0 csym::i)))
 (csym::@p@RINT@d@IAGNOSTICS)
 (for
  ((= csym::d csym::K@m@IN@t@REE@d@EPTH)
   (<= csym::d csym::K@m@AX@t@REE@d@EPTH) (+= csym::d 2))
  (csym::@t@IME@c@ONSTRUCTION csym::d))
 (if (or (== csym::LONG@l@IVED@t@REE 0)
         (!= (aref csym::array 1000) (/ 1.0d0 1000)))
     (csym::fprintf csym::__stderrp "Failed~|12|"))
 (= csym::T@f@INISH (csym::stats_rtclock))
 (= csym::T@e@LAPSED (- csym::T@f@INISH csym::T@s@TART))
 (csym::@p@RINT@d@IAGNOSTICS)
 (csym::printf "Completed in %d msec~|12|" csym::T@e@LAPSED))
(%defconstant csym::time_t double)
(%defconstant csym::pid_t long)
(%defconstant csym::__complex__ (c-exp ""))
(%defmacro csym::__extension__ (csym::x) csym::x)
(%defmacro csym::__attribute__ (csym::x)
 (common-lisp:list 'c-exp "" csym::x))
(%defconstant csym::__inline (c-exp ""))
(%defmacro csym::__asm (csym::x) (common-lisp:list 'c-exp "" csym::x))
(%defconstant csym::i386 1)
(%defconstant csym::unix 1)
(%defconstant csym::__i386 1)
(%defconstant csym::__i386__ 1)
(%defconstant csym::__unix 1)
(%defconstant csym::__unix__ 1)
(%defconstant csym::__@F@ree@BSD__ 1)
(%defconstant csym::__@GNUC__ 3)
(%defconstant csym::__@GNUC_MINOR__ 4)
(%defconstant csym::__@SIZE_TYPE__ unsigned-int)
(%defconstant csym::__@PTRDIFF_TYPE__ int)
(%defconstant csym::__@WCHAR_TYPE__ int)
(%defconstant csym::__@GNUG__ 3)
(%defconstant csym::__@LINE__ (- 1234567890))
(%defconstant csym::__@FILE__ (c-exp ""))
(%defconstant csym::__@DATE__ "Feb  1 2006")
(%defconstant csym::__@TIME__ "19:46:20")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::_@STDIO_H_ (c-exp ""))
(%defconstant csym::_@SYS_CDEFS_H_ (c-exp ""))
(%defconstant csym::__@BEGIN_DECLS (c-exp ""))
(%defconstant csym::__@END_DECLS (c-exp ""))
(%defmacro csym::__@GNUC_PREREQ__ (csym::ma csym::mi)
 (common-lisp:list 'or (common-lisp:list '> '3 csym::ma)
                   (common-lisp:list 'and
                                     (common-lisp:list '== '3 csym::ma)
                                     (common-lisp:list
                                      '>=
                                      '4
                                      csym::mi))))
(%defmacro csym::__@P (csym::protos) csym::protos)
(%defmacro csym::__@CONCAT1 (csym::x csym::y)
 '"<macro with ## operator>")
(%defmacro csym::__@CONCAT (csym::x csym::y) 'csym::xy)
(%defmacro csym::__@STRING (csym::x)
 (common-lisp:list 'csym::_@C2SC_STRINGIFY csym::x))
(%defmacro csym::__@XSTRING (csym::x) '"x")
(%defconstant csym::__const (const int))
(%defconstant csym::__signed signed-int)
(%defconstant csym::__volatile (volatile int))
(%defconstant csym::__dead2 (c-exp ""))
(%defconstant csym::__pure2 (c-exp ""))
(%defconstant csym::__unused (c-exp ""))
(%defconstant csym::__used (c-exp ""))
(%defconstant csym::__packed (c-exp ""))
(%defmacro csym::__aligned (csym::x)
 (common-lisp:list 'c-exp "" csym::x))
(%defmacro csym::__section (csym::x)
 (common-lisp:list 'c-exp "" csym::x))
(%defconstant csym::__pure (c-exp ""))
(%defconstant csym::__always_inline (c-exp ""))
(%defmacro csym::__nonnull (csym::x)
 (common-lisp:list 'c-exp "" csym::x))
(%defconstant csym::__@LONG_LONG_SUPPORTED (c-exp ""))
(%defconstant csym::__restrict (c-exp ""))
(%defmacro csym::__predict_true (csym::exp)
 (common-lisp:list 'csym::__builtin_expect csym::exp '1))
(%defmacro csym::__predict_false (csym::exp)
 (common-lisp:list 'csym::__builtin_expect csym::exp '0))
(%defmacro csym::__offsetof (csym::type csym::field)
 (common-lisp:list 'cast 'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list
                                       'mref
                                       (common-lisp:list
                                        'cast
                                        (common-lisp:list
                                         'ptr
                                         csym::type)
                                        '0))
                                      csym::field))))
(%defmacro csym::__rangeof (csym::type csym::start csym::end)
 (common-lisp:list '-
                   (common-lisp:list 'cast 'csym::size_t
                                     (common-lisp:list
                                      'ptr
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        (common-lisp:list
                                         'cast
                                         (common-lisp:list
                                          'ptr
                                          csym::type)
                                         '0))
                                       csym::end)))
                   (common-lisp:list 'cast 'csym::size_t
                                     (common-lisp:list
                                      'ptr
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        (common-lisp:list
                                         'cast
                                         (common-lisp:list
                                          'ptr
                                          csym::type)
                                         '0))
                                       csym::start)))))
(%defmacro csym::__printflike (csym::fmtarg csym::firstvararg)
 (common-lisp:list 'c-exp "" csym::fmtarg csym::firstvararg))
(%defmacro csym::__scanflike (csym::fmtarg csym::firstvararg)
 (common-lisp:list 'c-exp "" csym::fmtarg csym::firstvararg))
(%defmacro csym::__printf0like (csym::fmtarg csym::firstvararg)
 (common-lisp:list 'c-exp "" csym::fmtarg csym::firstvararg))
(%defmacro csym::__strong_reference (csym::sym csym::aliassym)
 (common-lisp:list 'c-exp "extern __typeof ( ~0@*~a ) ~1@*~a ;"
                   csym::sym csym::aliassym))
(%defmacro csym::__weak_reference (csym::sym csym::alias)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".weak \" _C2SC_STRINGIFY ( ~1@*~a ) ) ; __asm__ ( \".equ \" _C2SC_STRINGIFY ( ~1@*~a ) \", \" _C2SC_STRINGIFY ( ~0@*~a ) )"
                   csym::sym csym::alias))
(%defmacro csym::__warn_references (csym::sym csym::msg)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".section .gnu.warning.\" _C2SC_STRINGIFY ( ~0@*~a ) ) ; __asm__ ( \".asciz \\\"\" ~1@*~a \"\\\"\" ) ; __asm__ ( \".previous\" )"
                   csym::sym csym::msg))
(%defmacro csym::__@IDSTRING (csym::name csym::string)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~1@*~a \"\\\"\" )"
                   csym::name csym::string))
(%defmacro csym::__@FBSDID (csym::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   csym::s))
(%defmacro csym::__@RCSID (csym::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   csym::s))
(%defmacro csym::__@RCSID_SOURCE (csym::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   csym::s))
(%defmacro csym::__@SCCSID (csym::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   csym::s))
(%defmacro csym::__@COPYRIGHT (csym::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   csym::s))
(%defmacro csym::__@DECONST (csym::type csym::var)
 (common-lisp:list 'cast csym::type
                   (common-lisp:list 'cast 'csym::uintptr_t
                                     (common-lisp:list
                                      'cast
                                      (common-lisp:list
                                       'ptr
                                       (common-lisp:list 'const 'void))
                                      csym::var))))
(%defmacro csym::__@DEVOLATILE (csym::type csym::var)
 (common-lisp:list 'cast csym::type
                   (common-lisp:list 'cast 'csym::uintptr_t
                                     (common-lisp:list
                                      'cast
                                      (common-lisp:list
                                       'ptr
                                       (common-lisp:list
                                        'volatile
                                        'void))
                                      csym::var))))
(%defmacro csym::__@DEQUALIFY (csym::type csym::var)
 (common-lisp:list 'cast csym::type
                   (common-lisp:list 'cast 'csym::uintptr_t
                                     (common-lisp:list
                                      'cast
                                      (common-lisp:list
                                       'ptr
                                       (common-lisp:list
                                        'const
                                        'volatile
                                        'void))
                                      csym::var))))
(%defconstant csym::__@POSIX_VISIBLE 200112)
(%defconstant csym::__@XSI_VISIBLE 600)
(%defconstant csym::__@BSD_VISIBLE 1)
(%defconstant csym::__@ISO_C_VISIBLE 1999)
(%defconstant csym::@NULL (cast (ptr void) 0))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@NO_STRICT_ALIGNMENT (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
(%defconstant csym::_@SIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@VA_LIST_DECLARED (c-exp ""))
(%defconstant csym::_@FSTDIO (c-exp ""))
(%defconstant csym::_@STDSTREAM_DECLARED (c-exp ""))
(%defconstant csym::__@SLBF 1)
(%defconstant csym::__@SNBF 2)
(%defconstant csym::__@SRD 4)
(%defconstant csym::__@SWR 8)
(%defconstant csym::__@SRW 16)
(%defconstant csym::__@SEOF 32)
(%defconstant csym::__@SERR 64)
(%defconstant csym::__@SMBF 128)
(%defconstant csym::__@SAPP 256)
(%defconstant csym::__@SSTR 512)
(%defconstant csym::__@SOPT 1024)
(%defconstant csym::__@SNPT 2048)
(%defconstant csym::__@SOFF 4096)
(%defconstant csym::__@SMOD 8192)
(%defconstant csym::__@SALC 16384)
(%defconstant csym::__@SIGN 32768)
(%defconstant csym::_@IOFBF 0)
(%defconstant csym::_@IOLBF 1)
(%defconstant csym::_@IONBF 2)
(%defconstant csym::@BUFSIZ 1024)
(%defconstant csym::@EOF (- 1))
(%defconstant csym::@FOPEN_MAX 20)
(%defconstant csym::@FILENAME_MAX 1024)
(%defconstant csym::@P_@tmpdir "/var/tmp/")
(%defconstant csym::@L_@tmpnam 1024)
(%defconstant csym::@TMP_MAX 308915776)
(%defconstant csym::@SEEK_SET 0)
(%defconstant csym::@SEEK_CUR 1)
(%defconstant csym::@SEEK_END 2)
(%defconstant csym::stdin csym::__stdinp)
(%defconstant csym::stdout csym::__stdoutp)
(%defconstant csym::stderr csym::__stderrp)
(%defconstant csym::@L_@cuserid 17)
(%defconstant csym::@L_@ctermid 1024)
(%defconstant csym::__@ATTR_FORMAT_ARG (c-exp ""))
(%defmacro csym::fropen (csym::cookie csym::fn)
 (common-lisp:list 'csym::funopen csym::cookie 'csym::nil '0 '0 '0))
(%defmacro csym::fwopen (csym::cookie csym::fn)
 (common-lisp:list 'csym::funopen csym::cookie '0 'csym::nil '0 '0))
(%defconstant csym::_@FTRUNCATE_DECLARED (c-exp ""))
(%defconstant csym::_@LSEEK_DECLARED (c-exp ""))
(%defconstant csym::_@MMAP_DECLARED (c-exp ""))
(%defconstant csym::_@TRUNCATE_DECLARED (c-exp ""))
(%defmacro csym::__sgetc (csym::p)
 (common-lisp:list 'if-exp
                   (common-lisp:list '<
                                     (common-lisp:list
                                      '--
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list 'mref csym::p)
                                       'csym::_r))
                                     '0)
                   (common-lisp:list 'csym::__srget csym::p)
                   (common-lisp:list 'cast 'int
                                     (common-lisp:list
                                      'mref
                                      (common-lisp:list
                                       'inc
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::p)
                                        'csym::_p))))))
(%defmacro csym::__sfeof (csym::p)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::_flags)
                                     '32)
                   '0))
(%defmacro csym::__sferror (csym::p)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::_flags)
                                     '64)
                   '0))
(%defmacro csym::__sclearerr (csym::p)
 (common-lisp:list 'cast 'void
                   (common-lisp:list 'bit-and=
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::_flags)
                                     (common-lisp:list
                                      'bit-not
                                      (common-lisp:list
                                       'bit-or
                                       '64
                                       '32)))))
(%defmacro csym::__sfileno (csym::p)
 (common-lisp:list 'fref (common-lisp:list 'mref csym::p) 'csym::_file))
(%defmacro csym::feof (csym::p)
 (common-lisp:list 'if-exp (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list '!=
                                     (common-lisp:list
                                      'bit-and
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list 'mref csym::p)
                                       'csym::_flags)
                                      '32)
                                     '0)
                   (common-lisp:list 'csym::feof csym::p)))
(%defmacro csym::ferror (csym::p)
 (common-lisp:list 'if-exp (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list '!=
                                     (common-lisp:list
                                      'bit-and
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list 'mref csym::p)
                                       'csym::_flags)
                                      '64)
                                     '0)
                   (common-lisp:list 'csym::ferror csym::p)))
(%defmacro csym::clearerr (csym::p)
 (common-lisp:list 'if-exp (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'cast 'void
                                     (common-lisp:list
                                      'bit-and=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list 'mref csym::p)
                                       'csym::_flags)
                                      (common-lisp:list
                                       'bit-not
                                       (common-lisp:list
                                        'bit-or
                                        '64
                                        '32))))
                   (common-lisp:list 'csym::clearerr csym::p)))
(%defmacro csym::fileno (csym::p)
 (common-lisp:list 'if-exp (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref csym::p)
                                     'csym::_file)
                   (common-lisp:list 'csym::fileno csym::p)))
(%defmacro csym::getc (csym::fp)
 (common-lisp:list 'if-exp (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list
                                      '<
                                      (common-lisp:list
                                       '--
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::fp)
                                        'csym::_r))
                                      '0)
                                     (common-lisp:list
                                      'csym::__srget
                                      csym::fp)
                                     (common-lisp:list
                                      'cast
                                      'int
                                      (common-lisp:list
                                       'mref
                                       (common-lisp:list
                                        'inc
                                        (common-lisp:list
                                         'fref
                                         (common-lisp:list
                                          'mref
                                          csym::fp)
                                         'csym::_p)))))
                   (common-lisp:list 'csym::getc csym::fp)))
(%defmacro csym::putc (csym::x csym::fp)
 (common-lisp:list 'if-exp (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sputc csym::x csym::fp)
                   (common-lisp:list 'csym::putc csym::x csym::fp)))
(%defmacro csym::getchar common-lisp:nil
 (common-lisp:list 'if-exp (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list
                                      '<
                                      (common-lisp:list
                                       '--
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         'csym::__stdinp)
                                        'csym::_r))
                                      '0)
                                     (common-lisp:list
                                      'csym::__srget
                                      'csym::__stdinp)
                                     (common-lisp:list
                                      'cast
                                      'int
                                      (common-lisp:list
                                       'mref
                                       (common-lisp:list
                                        'inc
                                        (common-lisp:list
                                         'fref
                                         (common-lisp:list
                                          'mref
                                          'csym::__stdinp)
                                         'csym::_p)))))
                   (common-lisp:list 'csym::getc 'csym::__stdinp)))
(%defmacro csym::putchar (csym::x)
 (common-lisp:list 'if-exp (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sputc csym::x
                                     'csym::__stdoutp)
                   (common-lisp:list 'csym::putc csym::x
                                     'csym::__stdoutp)))
(%defmacro csym::feof_unlocked (csym::p)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::_flags)
                                     '32)
                   '0))
(%defmacro csym::ferror_unlocked (csym::p)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::_flags)
                                     '64)
                   '0))
(%defmacro csym::clearerr_unlocked (csym::p)
 (common-lisp:list 'cast 'void
                   (common-lisp:list 'bit-and=
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::_flags)
                                     (common-lisp:list
                                      'bit-not
                                      (common-lisp:list
                                       'bit-or
                                       '64
                                       '32)))))
(%defmacro csym::fileno_unlocked (csym::p)
 (common-lisp:list 'fref (common-lisp:list 'mref csym::p) 'csym::_file))
(%defmacro csym::getc_unlocked (csym::fp)
 (common-lisp:list 'if-exp
                   (common-lisp:list '<
                                     (common-lisp:list
                                      '--
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::fp)
                                       'csym::_r))
                                     '0)
                   (common-lisp:list 'csym::__srget csym::fp)
                   (common-lisp:list 'cast 'int
                                     (common-lisp:list
                                      'mref
                                      (common-lisp:list
                                       'inc
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::fp)
                                        'csym::_p))))))
(%defmacro csym::putc_unlocked (csym::x csym::fp)
 (common-lisp:list 'csym::__sputc csym::x csym::fp))
(%defmacro csym::getchar_unlocked common-lisp:nil
 (common-lisp:list 'if-exp
                   (common-lisp:list '<
                                     (common-lisp:list
                                      '--
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        'csym::__stdinp)
                                       'csym::_r))
                                     '0)
                   (common-lisp:list 'csym::__srget 'csym::__stdinp)
                   (common-lisp:list 'cast 'int
                                     (common-lisp:list
                                      'mref
                                      (common-lisp:list
                                       'inc
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         'csym::__stdinp)
                                        'csym::_p))))))
(%defmacro csym::putchar_unlocked (csym::x)
 (common-lisp:list 'csym::__sputc csym::x 'csym::__stdoutp))
(%defconstant csym::_@STDLIB_H_ (c-exp ""))
(%defconstant csym::_@RUNE_T_DECLARED (c-exp ""))
(%defconstant csym::_@WCHAR_T_DECLARED (c-exp ""))
(%defconstant csym::@EXIT_FAILURE 1)
(%defconstant csym::@EXIT_SUCCESS 0)
(%defconstant csym::@RAND_MAX 2147483647)
(%defconstant csym::@MB_CUR_MAX csym::__mb_cur_max)
(%defconstant csym::_@MKSTEMP_DECLARED (c-exp ""))
(%defconstant csym::_@MKTEMP_DECLARED (c-exp ""))
(%defconstant csym::_@SETKEY_DECLARED (c-exp ""))
(%defmacro csym::alloca (csym::sz)
 (common-lisp:list 'csym::__builtin_alloca csym::sz))
(%defconstant csym::_@SYS_TIME_H_ (c-exp ""))
(%defconstant csym::_@SYS__TIMEVAL_H_ (c-exp ""))
(%defconstant csym::_@SUSECONDS_T_DECLARED (c-exp ""))
(%defconstant csym::_@TIME_T_DECLARED (c-exp ""))
(%defconstant csym::_@SYS_TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE_ENDIAN_H_ (c-exp ""))
(%defconstant csym::_@QUAD_HIGHWORD 1)
(%defconstant csym::_@QUAD_LOWWORD 0)
(%defconstant csym::_@LITTLE_ENDIAN 1234)
(%defconstant csym::_@BIG_ENDIAN 4321)
(%defconstant csym::_@PDP_ENDIAN 3412)
(%defconstant csym::_@BYTE_ORDER 1234)
(%defconstant csym::@LITTLE_ENDIAN 1234)
(%defconstant csym::@BIG_ENDIAN 4321)
(%defconstant csym::@PDP_ENDIAN 3412)
(%defconstant csym::@BYTE_ORDER 1234)
(%defmacro csym::__word_swap_int_var (csym::x)
 (common-lisp:list (common-lisp:list 'begin
                                     (common-lisp:list
                                      'register-def
                                      'csym::__@X
                                      'csym::__uint32_t
                                      csym::x)
                                     'common-lisp:list 'common-lisp:nil
                                     'csym::__@X)))
(%defmacro csym::__word_swap_int (csym::x)
 (common-lisp:list (common-lisp:list 'begin
                                     (common-lisp:list
                                      'register-def
                                      'csym::__@X
                                      'csym::__uint32_t
                                      csym::x)
                                     'common-lisp:list 'common-lisp:nil
                                     'csym::__@X)))
(%defmacro csym::__byte_swap_int_var (csym::x)
 (common-lisp:list (common-lisp:list 'begin
                                     (common-lisp:list
                                      'register-def
                                      'csym::__@X
                                      'csym::__uint32_t
                                      csym::x)
                                     'common-lisp:list 'common-lisp:nil
                                     'csym::__@X)))
(%defmacro csym::__byte_swap_int (csym::x)
 (common-lisp:list (common-lisp:list 'begin
                                     (common-lisp:list
                                      'register-def
                                      'csym::__@X
                                      'csym::__uint32_t
                                      csym::x)
                                     'common-lisp:list 'common-lisp:nil
                                     'csym::__@X)))
(%defmacro csym::__byte_swap_word_var (csym::x)
 (common-lisp:list (common-lisp:list 'begin
                                     (common-lisp:list
                                      'register-def
                                      'csym::__@X
                                      'csym::__uint16_t
                                      csym::x)
                                     'common-lisp:list 'common-lisp:nil
                                     'csym::__@X)))
(%defmacro csym::__byte_swap_word (csym::x)
 (common-lisp:list (common-lisp:list 'begin
                                     (common-lisp:list
                                      'register-def
                                      'csym::__@X
                                      'csym::__uint16_t
                                      csym::x)
                                     'common-lisp:list 'common-lisp:nil
                                     'csym::__@X)))
(%defmacro csym::__htonl (csym::x)
 (common-lisp:list 'csym::__bswap32 csym::x))
(%defmacro csym::__htons (csym::x)
 (common-lisp:list 'csym::__bswap16 csym::x))
(%defmacro csym::__ntohl (csym::x)
 (common-lisp:list 'csym::__bswap32 csym::x))
(%defmacro csym::__ntohs (csym::x)
 (common-lisp:list 'csym::__bswap16 csym::x))
(%defconstant csym::_@INT8_T_DECLARED (c-exp ""))
(%defconstant csym::_@INT16_T_DECLARED (c-exp ""))
(%defconstant csym::_@INT32_T_DECLARED (c-exp ""))
(%defconstant csym::_@INT64_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT8_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT16_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT32_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT64_T_DECLARED (c-exp ""))
(%defconstant csym::_@INTPTR_T_DECLARED (c-exp ""))
(%defconstant csym::_@CLOCK_T_DECLARED (c-exp ""))
(%defconstant csym::_@CLOCKID_T_DECLARED (c-exp ""))
(%defconstant csym::_@DEV_T_DECLARED (c-exp ""))
(%defconstant csym::_@FFLAGS_T_DECLARED (c-exp ""))
(%defconstant csym::_@FSBLKCNT_T_DECLARED (c-exp ""))
(%defconstant csym::_@GID_T_DECLARED (c-exp ""))
(%defconstant csym::_@IN_ADDR_T_DECLARED (c-exp ""))
(%defconstant csym::_@IN_PORT_T_DECLARED (c-exp ""))
(%defconstant csym::_@ID_T_DECLARED (c-exp ""))
(%defconstant csym::_@INO_T_DECLARED (c-exp ""))
(%defconstant csym::_@KEY_T_DECLARED (c-exp ""))
(%defconstant csym::_@LWPID_T_DECLARED (c-exp ""))
(%defconstant csym::_@MODE_T_DECLARED (c-exp ""))
(%defconstant csym::_@NLINK_T_DECLARED (c-exp ""))
(%defconstant csym::_@OFF_T_DECLARED (c-exp ""))
(%defconstant csym::_@PID_T_DECLARED (c-exp ""))
(%defconstant csym::_@RLIM_T_DECLARED (c-exp ""))
(%defconstant csym::_@SSIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@TIMER_T_DECLARED (c-exp ""))
(%defconstant csym::_@UID_T_DECLARED (c-exp ""))
(%defconstant csym::_@USECONDS_T_DECLARED (c-exp ""))
(%defconstant csym::_@SYS_SELECT_H_ (c-exp ""))
(%defconstant csym::_@SYS__SIGSET_H_ (c-exp ""))
(%defconstant csym::_@SIG_WORDS 4)
(%defconstant csym::_@SIG_MAXSIG 128)
(%defmacro csym::_@SIG_IDX (csym::sig)
 (common-lisp:list '- csym::sig '1))
(%defmacro csym::_@SIG_WORD (csym::sig)
 (common-lisp:list '>> (common-lisp:list '- csym::sig '1) '5))
(%defmacro csym::_@SIG_BIT (csym::sig)
 (common-lisp:list '<< '1
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '- csym::sig '1)
                                     '31)))
(%defmacro csym::_@SIG_VALID (csym::sig)
 (common-lisp:list 'and (common-lisp:list '<= csym::sig '128)
                   (common-lisp:list '> csym::sig '0)))
(%defconstant csym::_@SYS_TIMESPEC_H_ (c-exp ""))
(%defmacro csym::@TIMEVAL_TO_TIMESPEC (csym::tv csym::ts)
 (common-lisp:list (common-lisp:list 'do-while '0
                                     (common-lisp:list
                                      '=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::ts)
                                       'csym::tv_sec)
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::tv)
                                       'csym::tv_sec))
                                     (common-lisp:list
                                      '=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::ts)
                                       'csym::tv_nsec)
                                      (common-lisp:list
                                       '*
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::tv)
                                        'csym::tv_usec)
                                       '1000)))))
(%defmacro csym::@TIMESPEC_TO_TIMEVAL (csym::tv csym::ts)
 (common-lisp:list (common-lisp:list 'do-while '0
                                     (common-lisp:list
                                      '=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::tv)
                                       'csym::tv_sec)
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::ts)
                                       'csym::tv_sec))
                                     (common-lisp:list
                                      '=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::tv)
                                       'csym::tv_usec)
                                      (common-lisp:list
                                       '/
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::ts)
                                        'csym::tv_nsec)
                                       '1000)))))
(%defconstant csym::_@SIGSET_T_DECLARED (c-exp ""))
(%defconstant csym::@FD_SETSIZE (cast unsigned-int 1024))
(%defconstant csym::_@NFDBITS (* (sizeof csym::__fd_mask) 8))
(%defconstant csym::@NFDBITS (* (sizeof csym::__fd_mask) 8))
(%defmacro csym::_howmany (csym::x csym::y)
 (common-lisp:list '/
                   (common-lisp:list '+ csym::x
                                     (common-lisp:list '- csym::y '1))
                   csym::y))
(%defconstant csym::fds_bits csym::__fds_bits)
(%defmacro csym::__fdset_mask (csym::n)
 (common-lisp:list '<< (common-lisp:list 'cast 'csym::__fd_mask '1)
                   (common-lisp:list '% csym::n
                                     (common-lisp:list
                                      '*
                                      (common-lisp:list
                                       'sizeof
                                       'csym::__fd_mask)
                                      '8))))
(%defmacro csym::@FD_CLR (csym::n csym::p)
 (common-lisp:list 'bit-and=
                   (common-lisp:list 'aref
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::__fds_bits)
                                     (common-lisp:list
                                      '/
                                      csym::n
                                      (common-lisp:list
                                       '*
                                       (common-lisp:list
                                        'sizeof
                                        'csym::__fd_mask)
                                       '8)))
                   (common-lisp:list 'bit-not
                                     (common-lisp:list
                                      '<<
                                      (common-lisp:list
                                       'cast
                                       'csym::__fd_mask
                                       '1)
                                      (common-lisp:list
                                       '%
                                       csym::n
                                       (common-lisp:list
                                        '*
                                        (common-lisp:list
                                         'sizeof
                                         'csym::__fd_mask)
                                        '8))))))
(%defmacro csym::@FD_COPY (csym::f csym::t)
 (common-lisp:list 'cast 'void
                   (common-lisp:list '=
                                     (common-lisp:list 'mref csym::t)
                                     (common-lisp:list 'mref csym::f))))
(%defmacro csym::@FD_ISSET (csym::n csym::p)
 (common-lisp:list 'bit-and
                   (common-lisp:list 'aref
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::__fds_bits)
                                     (common-lisp:list
                                      '/
                                      csym::n
                                      (common-lisp:list
                                       '*
                                       (common-lisp:list
                                        'sizeof
                                        'csym::__fd_mask)
                                       '8)))
                   (common-lisp:list '<<
                                     (common-lisp:list
                                      'cast
                                      'csym::__fd_mask
                                      '1)
                                     (common-lisp:list
                                      '%
                                      csym::n
                                      (common-lisp:list
                                       '*
                                       (common-lisp:list
                                        'sizeof
                                        'csym::__fd_mask)
                                       '8)))))
(%defmacro csym::@FD_SET (csym::n csym::p)
 (common-lisp:list 'bit-or=
                   (common-lisp:list 'aref
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list 'mref csym::p)
                                      'csym::__fds_bits)
                                     (common-lisp:list
                                      '/
                                      csym::n
                                      (common-lisp:list
                                       '*
                                       (common-lisp:list
                                        'sizeof
                                        'csym::__fd_mask)
                                       '8)))
                   (common-lisp:list '<<
                                     (common-lisp:list
                                      'cast
                                      'csym::__fd_mask
                                      '1)
                                     (common-lisp:list
                                      '%
                                      csym::n
                                      (common-lisp:list
                                       '*
                                       (common-lisp:list
                                        'sizeof
                                        'csym::__fd_mask)
                                       '8)))))
(%defmacro csym::@FD_ZERO (csym::p)
 (common-lisp:list (common-lisp:list 'do-while '0
                                     (common-lisp:list
                                      '*
                                      'csym::fd_set
                                      'csym::_p)
                                     (common-lisp:list
                                      'decl
                                      'csym::_n
                                      'csym::__size_t)
                                     (common-lisp:list
                                      '=
                                      'csym::_p
                                      csym::p)
                                     (common-lisp:list
                                      '=
                                      'csym::_n
                                      (common-lisp:list
                                       '/
                                       (common-lisp:list
                                        '+
                                        (common-lisp:list
                                         'cast
                                         'unsigned-int
                                         '1024)
                                        (common-lisp:list
                                         '-
                                         (common-lisp:list
                                          '*
                                          (common-lisp:list
                                           'sizeof
                                           'csym::__fd_mask)
                                          '8)
                                         '1))
                                       (common-lisp:list
                                        '*
                                        (common-lisp:list
                                         'sizeof
                                         'csym::__fd_mask)
                                        '8)))
                                     (common-lisp:list
                                      'while
                                      (common-lisp:list
                                       '>
                                       'csym::_n
                                       '0)
                                      (common-lisp:list
                                       '=
                                       (common-lisp:list
                                        'aref
                                        (common-lisp:list
                                         'fref
                                         (common-lisp:list
                                          'mref
                                          'csym::_p)
                                         'csym::__fds_bits)
                                        (common-lisp:list
                                         '--
                                         'csym::_n))
                                       '0)))))
(%defconstant csym::_@SELECT_DECLARED (c-exp ""))
(%defmacro csym::major (csym::x)
 (common-lisp:list 'cast 'int
                   (common-lisp:list 'bit-and
                                     (common-lisp:list
                                      '>>
                                      (common-lisp:list
                                       'cast
                                       'csym::u_int
                                       csym::x)
                                      '8)
                                     '255)))
(%defmacro csym::minor (csym::x)
 (common-lisp:list 'cast 'int
                   (common-lisp:list 'bit-and csym::x '4294902015)))
(%defmacro csym::makedev (csym::x csym::y)
 (common-lisp:list 'cast 'csym::dev_t
                   (common-lisp:list 'bit-or
                                     (common-lisp:list '<< csym::x '8)
                                     csym::y)))
(%defconstant csym::@DST_NONE 0)
(%defconstant csym::@DST_USA 1)
(%defconstant csym::@DST_AUST 2)
(%defconstant csym::@DST_WET 3)
(%defconstant csym::@DST_MET 4)
(%defconstant csym::@DST_EET 5)
(%defconstant csym::@DST_CAN 6)
(%defmacro csym::timerclear (csym::tvp)
 (common-lisp:list '=
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref csym::tvp)
                                     'csym::tv_sec)
                   (common-lisp:list '=
                                     (common-lisp:list
                                      'fref
                                      (common-lisp:list
                                       'mref
                                       csym::tvp)
                                      'csym::tv_usec)
                                     '0)))
(%defmacro csym::timerisset (csym::tvp)
 (common-lisp:list 'or
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref csym::tvp)
                                     'csym::tv_sec)
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref csym::tvp)
                                     'csym::tv_usec)))
(%defmacro csym::timercmp (csym::tvp csym::uvp csym::cmp)
 (common-lisp:list 'c-exp
                   "( ( ( ~0@*~a ) -> tv_sec == ( ~1@*~a ) -> tv_sec ) ? ( ( ~0@*~a ) -> tv_usec ~2@*~a ( ~1@*~a ) -> tv_usec ) : ( ( ~0@*~a ) -> tv_sec ~2@*~a ( ~1@*~a ) -> tv_sec ) )"
                   csym::tvp csym::uvp csym::cmp))
(%defmacro csym::timeradd (csym::tvp csym::uvp csym::vvp)
 (common-lisp:list (common-lisp:list 'do-while '0
                                     (common-lisp:list
                                      '=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::vvp)
                                       'csym::tv_sec)
                                      (common-lisp:list
                                       '+
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::tvp)
                                        'csym::tv_sec)
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::uvp)
                                        'csym::tv_sec)))
                                     (common-lisp:list
                                      '=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::vvp)
                                       'csym::tv_usec)
                                      (common-lisp:list
                                       '+
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::tvp)
                                        'csym::tv_usec)
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::uvp)
                                        'csym::tv_usec)))
                                     (common-lisp:list
                                      'if
                                      (common-lisp:list
                                       '>=
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::vvp)
                                        'csym::tv_usec)
                                       '1000000)
                                      (common-lisp:list
                                       'begin
                                       (common-lisp:list
                                        'inc
                                        (common-lisp:list
                                         'fref
                                         (common-lisp:list
                                          'mref
                                          csym::vvp)
                                         'csym::tv_sec))
                                       (common-lisp:list
                                        '-=
                                        (common-lisp:list
                                         'fref
                                         (common-lisp:list
                                          'mref
                                          csym::vvp)
                                         'csym::tv_usec)
                                        '1000000))))))
(%defmacro csym::timersub (csym::tvp csym::uvp csym::vvp)
 (common-lisp:list (common-lisp:list 'do-while '0
                                     (common-lisp:list
                                      '=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::vvp)
                                       'csym::tv_sec)
                                      (common-lisp:list
                                       '-
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::tvp)
                                        'csym::tv_sec)
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::uvp)
                                        'csym::tv_sec)))
                                     (common-lisp:list
                                      '=
                                      (common-lisp:list
                                       'fref
                                       (common-lisp:list
                                        'mref
                                        csym::vvp)
                                       'csym::tv_usec)
                                      (common-lisp:list
                                       '-
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::tvp)
                                        'csym::tv_usec)
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::uvp)
                                        'csym::tv_usec)))
                                     (common-lisp:list
                                      'if
                                      (common-lisp:list
                                       '<
                                       (common-lisp:list
                                        'fref
                                        (common-lisp:list
                                         'mref
                                         csym::vvp)
                                        'csym::tv_usec)
                                       '0)
                                      (common-lisp:list
                                       'begin
                                       (common-lisp:list
                                        'dec
                                        (common-lisp:list
                                         'fref
                                         (common-lisp:list
                                          'mref
                                          csym::vvp)
                                         'csym::tv_sec))
                                       (common-lisp:list
                                        '+=
                                        (common-lisp:list
                                         'fref
                                         (common-lisp:list
                                          'mref
                                          csym::vvp)
                                         'csym::tv_usec)
                                        '1000000))))))
(%defconstant csym::@ITIMER_REAL 0)
(%defconstant csym::@ITIMER_VIRTUAL 1)
(%defconstant csym::@ITIMER_PROF 2)
(%defconstant csym::@CLOCK_REALTIME 0)
(%defconstant csym::@CLOCK_VIRTUAL 1)
(%defconstant csym::@CLOCK_PROF 2)
(%defconstant csym::@CLOCK_MONOTONIC 4)
(%defconstant csym::@TIMER_RELTIME 0)
(%defconstant csym::@TIMER_ABSTIME 1)
(%defconstant csym::_@TIME_H_ (c-exp ""))
(%defconstant csym::@CLK_TCK 128)
(%defconstant csym::@CLOCKS_PER_SEC 128)
(%defmacro csym::current@T@ime common-lisp:nil
 (common-lisp:list 'csym::stats_rtclock))
(%defmacro csym::elapsed@T@ime (csym::x) csym::x)
(%defmacro csym::@HOLE common-lisp:nil (common-lisp:list 'c-exp ""))
