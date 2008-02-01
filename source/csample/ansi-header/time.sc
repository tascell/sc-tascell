(%line 1 "/usr/include/time.h")
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
(%line 70 "/usr/include/machine/_types.h")
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
(%line 118 "/usr/include/machine/_types.h")
(deftype csym::__va_list csym::__builtin_va_list)
(%line 124 "/usr/include/machine/_types.h")
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
(%line 64 "/usr/include/time.h")
(deftype csym::clock_t csym::__clock_t)
(deftype csym::time_t csym::__time_t)
(deftype csym::size_t csym::__size_t)
(%line 83 "/usr/include/time.h")
(deftype csym::clockid_t csym::__clockid_t)
(deftype csym::timer_t csym::__timer_t)
(%line 1 "/usr/include/sys/timespec.h")
(%line 46 "/usr/include/sys/timespec.h")
(def (struct csym::timespec) (decl csym::tv_sec csym::time_t)
 (decl csym::tv_nsec long))
(%line 95 "/usr/include/time.h")
(def (struct csym::tm) (decl csym::tm_sec int) (decl csym::tm_min int)
 (decl csym::tm_hour int) (decl csym::tm_mday int) (decl csym::tm_mon int)
 (decl csym::tm_year int) (decl csym::tm_wday int) (decl csym::tm_yday int)
 (decl csym::tm_isdst int) (decl csym::tm_gmtoff long)
 (decl csym::tm_zone (ptr char)))
(extern-decl csym::tzname (array (ptr char)))
(decl (csym::asctime) (fn (ptr char) (ptr (const (struct csym::tm)))))
(decl (csym::clock) (fn csym::clock_t void))
(decl (csym::ctime) (fn (ptr char) (ptr (const csym::time_t))))
(decl (csym::difftime) (fn double csym::time_t csym::time_t))
(decl (csym::gmtime) (fn (ptr (struct csym::tm)) (ptr (const csym::time_t))))
(decl (csym::localtime) (fn (ptr (struct csym::tm)) (ptr (const csym::time_t))))
(decl (csym::mktime) (fn csym::time_t (ptr (struct csym::tm))))
(decl (csym::strftime)
 (fn csym::size_t (ptr char) csym::size_t (ptr (const char))
  (ptr (const (struct csym::tm)))))
(decl (csym::time) (fn csym::time_t (ptr csym::time_t)))
(decl (csym::tzset) (fn void void))
(decl (csym::clock_getres)
 (fn int csym::clockid_t (ptr (struct csym::timespec))))
(decl (csym::clock_gettime)
 (fn int csym::clockid_t (ptr (struct csym::timespec))))
(decl (csym::clock_settime)
 (fn int csym::clockid_t (ptr (const (struct csym::timespec)))))
(decl (csym::nanosleep)
 (fn int (ptr (const (struct csym::timespec))) (ptr (struct csym::timespec))))
(decl (csym::asctime_r)
 (fn (ptr char) (ptr (const (struct csym::tm))) (ptr char)))
(decl (csym::ctime_r) (fn (ptr char) (ptr (const csym::time_t)) (ptr char)))
(decl (csym::gmtime_r)
 (fn (ptr (struct csym::tm)) (ptr (const csym::time_t))
  (ptr (struct csym::tm))))
(decl (csym::localtime_r)
 (fn (ptr (struct csym::tm)) (ptr (const csym::time_t))
  (ptr (struct csym::tm))))
(decl (csym::strptime)
 (fn (ptr char) (ptr (const char)) (ptr (const char)) (ptr (struct csym::tm))))
(decl (csym::timezone) (fn (ptr char) int int))
(decl (csym::tzsetwall) (fn void void))
(decl (csym::timelocal) (fn csym::time_t (const (ptr (struct csym::tm)))))
(decl (csym::timegm) (fn csym::time_t (const (ptr (struct csym::tm)))))
(%defconstant csym::i386 1)
(%defconstant csym::unix 1)
(%defconstant csym::|__I386| 1)
(%defconstant csym::|__I386__| 1)
(%defconstant csym::__unix 1)
(%defconstant csym::__unix__ 1)
(%defconstant csym::__@F@ree@BSD__ 1)
(%defconstant csym::__@GNUC__ 3)
(%defconstant csym::__@GNUC_MINOR__ 3)
(%defconstant csym::__@SIZE_TYPE__ unsigned-int)
(%defconstant csym::__@PTRDIFF_TYPE__ int)
(%defconstant csym::__@WCHAR_TYPE__ int)
(%defconstant csym::__@GNUG__ 3)
(%defconstant csym::__@LINE__ (- 1234567890))
(%defconstant csym::__@FILE__ (c-exp ""))
(%defconstant csym::__@DATE__ "Dec 20 2004")
(%defconstant csym::__@TIME__ "22:37:23")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::__extension__ (c-exp ""))
(%defmacro csym::__attribute__ (csym::arg0) (common-lisp:list 'c-exp ""))
(%defconstant csym::__inline (c-exp ""))
(%undef csym::i386)
(%undef csym::unix)
(%undef csym::__@GNUG__)
(%defconstant csym::_@TIME_H_ (c-exp ""))
(%defconstant csym::_@SYS_CDEFS_H_ (c-exp ""))
(%defconstant csym::__@BEGIN_DECLS (c-exp ""))
(%defconstant csym::__@END_DECLS (c-exp ""))
(%defmacro csym::__@GNUC_PREREQ__ (csym::arg0 csym::arg1)
 (common-lisp:list 'or
                   (common-lisp:list '> 'csym::__@GNUC__ csym::arg0)
                   (common-lisp:list 'and
                                     (common-lisp:list '==
                                                       'csym::__@GNUC__
                                                       csym::arg0)
                                     (common-lisp:list '>=
                                                       'csym::__@GNUC_MINOR__
                                                       csym::arg1))))
(%defmacro csym::__@P (csym::arg0) csym::arg0)
(%defmacro csym::__@CONCAT1 (csym::arg0 csym::arg1) '"<macro with ## operator>")
(%defmacro csym::__@CONCAT (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__@CONCAT1 csym::arg0 csym::arg1))
(%defmacro csym::__@STRING (csym::arg0) '"<macro with # operator>")
(%defmacro csym::__@XSTRING (csym::arg0)
 (common-lisp:list 'csym::__@STRING csym::arg0))
(%defconstant csym::__const (const int))
(%defconstant csym::__signed signed-int)
(%defconstant csym::__volatile (volatile int))
(%defconstant csym::__dead2 (csym::__attribute__ csym::__noreturn__))
(%defconstant csym::__pure2 (csym::__attribute__ csym::__const__))
(%defconstant csym::__unused (csym::__attribute__ csym::__unused__))
(%defconstant csym::__used (csym::__attribute__ csym::__used__))
(%defconstant csym::__packed (csym::__attribute__ csym::__packed__))
(%defmacro csym::__aligned (csym::arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__aligned__ csym::arg0)))
(%defmacro csym::__section (csym::arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__section__ csym::arg0)))
(%defconstant csym::__pure (csym::__attribute__ csym::__pure__))
(%defconstant csym::__always_inline
 (csym::__attribute__ csym::__always_inline__))
(%defmacro csym::__nonnull (csym::arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__nonnull__ csym::arg0)))
(%defconstant csym::__@LONG_LONG_SUPPORTED (c-exp ""))
(%defconstant csym::__restrict (c-exp ""))
(%defmacro csym::__predict_true (csym::arg0)
 (common-lisp:list 'csym::__builtin_expect csym::arg0 '1))
(%defmacro csym::__predict_false (csym::arg0)
 (common-lisp:list 'csym::__builtin_expect csym::arg0 '0))
(%defmacro csym::__offsetof (csym::arg0 csym::arg1)
 (common-lisp:list 'cast
                   'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           csym::arg0)
                                                                          '0))
                                                       csym::arg1))))
(%defmacro csym::__rangeof (csym::arg0 csym::arg1 csym::arg2)
 (common-lisp:list '-
                   (common-lisp:list 'csym::__offsetof csym::arg0 csym::arg2)
                   (common-lisp:list 'csym::__offsetof csym::arg0 csym::arg1)))
(%defmacro csym::__printflike (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__format__
                                     'csym::__printf__
                                     csym::arg0
                                     csym::arg1)))
(%defmacro csym::__scanflike (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__format__
                                     'csym::__scanf__
                                     csym::arg0
                                     csym::arg1)))
(%defmacro csym::__printf0like (csym::arg0 csym::arg1)
 (common-lisp:list 'c-exp ""))
(%defmacro csym::__strong_reference (csym::arg0 csym::arg1)
 '"<macro with # operator>")
(%defmacro csym::__weak_reference (csym::arg0 csym::arg1)
 '"<macro with # operator>")
(%defmacro csym::__warn_references (csym::arg0 csym::arg1)
 '"<macro with # operator>")
(%defmacro csym::__@IDSTRING (csym::arg0 csym::arg1)
 (common-lisp:list 'c-exp "__asm__(\".ident\\t\\\"\" ~a  \"\\\"\")" csym::arg1))
(%defmacro csym::__@FBSDID (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@RCSID (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@RCSID_SOURCE (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_source_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@SCCSID (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__sccsid_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@COPYRIGHT (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__copyright_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@DECONST (csym::arg0 csym::arg1)
 (common-lisp:list 'cast
                   csym::arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'void))
                                                       csym::arg1))))
(%defmacro csym::__@DEVOLATILE (csym::arg0 csym::arg1)
 (common-lisp:list 'cast
                   csym::arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'volatile
                                                                          'void))
                                                       csym::arg1))))
(%defmacro csym::__@DEQUALIFY (csym::arg0 csym::arg1)
 (common-lisp:list 'cast
                   csym::arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'volatile
                                                                          'void))
                                                       csym::arg1))))
(%defconstant csym::__@POSIX_VISIBLE 200112)
(%defconstant csym::__@XSI_VISIBLE 600)
(%defconstant csym::__@BSD_VISIBLE 1)
(%defconstant csym::__@ISO_C_VISIBLE 1999)
(%defconstant csym::@NULL (cast (ptr void) 0))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
(%defconstant csym::@CLK_TCK 128)
(%defconstant csym::@CLOCKS_PER_SEC 128)
(%defconstant csym::_@CLOCK_T_DECLARED (c-exp ""))
(%defconstant csym::_@TIME_T_DECLARED (c-exp ""))
(%defconstant csym::_@SIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@CLOCKID_T_DECLARED (c-exp ""))
(%defconstant csym::_@TIMER_T_DECLARED (c-exp ""))
(%defconstant csym::_@SYS_TIMESPEC_H_ (c-exp ""))
(%defmacro csym::@TIMEVAL_TO_TIMESPEC (csym::arg0 csym::arg1)
 (common-lisp:list 'c-exp
                   "do { (~a )->tv_sec = (~a )->tv_sec; (~a )->tv_nsec = (~a )->tv_usec * 1000; } while (0)"
                   csym::arg1
                   csym::arg0
                   csym::arg1
                   csym::arg0))
(%defmacro csym::@TIMESPEC_TO_TIMEVAL (csym::arg0 csym::arg1)
 (common-lisp:list 'c-exp
                   "do { (~a )->tv_sec = (~a )->tv_sec; (~a )->tv_usec = (~a )->tv_nsec / 1000; } while (0)"
                   csym::arg0
                   csym::arg1
                   csym::arg0
                   csym::arg1))
