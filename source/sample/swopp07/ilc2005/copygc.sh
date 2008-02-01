






(deftype csym::__int8_t signed-char)
(deftype csym::__uint8_t unsigned-char)
(deftype csym::__int16_t short)
(deftype csym::__uint16_t unsigned-short)
(deftype csym::__int32_t int)
(deftype csym::__uint32_t unsigned-int)
(deftype csym::__int64_t int)
(deftype csym::__uint64_t unsigned-int)

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

(deftype csym::__va_list csym::__builtin_va_list)

(deftype csym::__gnuc_va_list csym::__va_list)

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

(deftype csym::__ct_rune_t int)
(deftype csym::__rune_t csym::__ct_rune_t)
(deftype csym::__wchar_t csym::__ct_rune_t)
(deftype csym::__wint_t csym::__ct_rune_t)
(deftype csym::__dev_t csym::__uint32_t)
(deftype csym::__fixpt_t csym::__uint32_t)

(def (union csym::unnamed0) (decl csym::__mbstate8 (array char 128))
 (decl csym::_MBSTATE@l csym::__int64_t))
(deftype csym::__mbstate_t (union csym::unnamed0))

(deftype csym::ptrdiff_t csym::__ptrdiff_t)
(deftype csym::rune_t csym::__rune_t)
(deftype csym::size_t csym::__size_t)
(deftype csym::wchar_t csym::__wchar_t)

(deftype csym::align_t double)

(def (struct csym::unnamed1) (decl csym::size csym::size_t)
 (decl csym::asize csym::size_t) (decl csym::fli_len int)
 (decl csym::fli (ptr csym::size_t)))
(deftype csym::descriptor (struct csym::unnamed1))
(deftype csym::desc_t (ptr (struct csym::unnamed1)))

(def (struct csym::unnamed2) (decl csym::gcv int) (decl csym::gctype int)
 (decl csym::tosize csym::size_t) (decl csym::stack_size csym::size_t)
 (decl csym::limited_stack_max csym::size_t))
(deftype csym::gc_params (struct csym::unnamed2))

;;
(deftype sht (ptr (lightweight void void)))
;;
(extern-decl (csym::getmem_init) (fn void csym::gc_params))
(extern-decl (getmem csym::sht csym::desc_t) (fn (ptr void)))
(extern-decl (csym::move csym::vp) (fn (ptr void) (ptr void)))
(extern-decl (gc csym::sht) (fn void))
(extern-decl csym::gc_ttime double)
(%defconstant csym::time_t double)
(%defconstant csym::pid_t long)
(%defconstant csym::__complex__ (c-exp ""))
(%defmacro csym::__extension__ (csym::x) csym::x)
(%defmacro csym::__attribute__ (csym::x) (common-lisp:list 'c-exp "" csym::x))
(%defconstant csym::__inline (c-exp ""))
(%defmacro csym::__asm (csym::x) (common-lisp:list 'c-exp "" csym::x))
(%defconstant csym::i386 1)
(%defconstant csym::unix 1)
(%defconstant csym::|__I386| 1)
(%defconstant csym::|__I386__| 1)
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
(%defconstant csym::__@DATE__ "May 12 2005")
(%defconstant csym::__@TIME__ "13:40:08")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::@COPYGC_H (c-exp ""))
(%defconstant csym::_@STDDEF_H_ (c-exp ""))
(%defconstant csym::_@SYS_CDEFS_H_ (c-exp ""))
(%defconstant csym::__@BEGIN_DECLS (c-exp ""))
(%defconstant csym::__@END_DECLS (c-exp ""))
(%defmacro csym::__@GNUC_PREREQ__ (csym::ma csym::mi)
 (common-lisp:list 'or
                   (common-lisp:list '> '3 csym::ma)
                   (common-lisp:list 'and
                                     (common-lisp:list '== '3 csym::ma)
                                     (common-lisp:list '>= '4 csym::mi))))
(%defmacro csym::__@P (csym::protos) csym::protos)
(%defmacro csym::__@CONCAT1 (csym::x csym::y) '"<macro with ## operator>")
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
(%defmacro csym::__aligned (csym::x) (common-lisp:list 'c-exp "" csym::x))
(%defmacro csym::__section (csym::x) (common-lisp:list 'c-exp "" csym::x))
(%defconstant csym::__pure (c-exp ""))
(%defconstant csym::__always_inline (c-exp ""))
(%defmacro csym::__nonnull (csym::x) (common-lisp:list 'c-exp "" csym::x))
(%defconstant csym::__@LONG_LONG_SUPPORTED (c-exp ""))
(%defconstant csym::__restrict (c-exp ""))
(%defmacro csym::__predict_true (csym::exp)
 (common-lisp:list 'csym::__builtin_expect csym::exp '1))
(%defmacro csym::__predict_false (csym::exp)
 (common-lisp:list 'csym::__builtin_expect csym::exp '0))
(%defmacro csym::__offsetof (csym::type csym::field)
 (common-lisp:list 'cast
                   'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           csym::type)
                                                                          '0))
                                                       csym::field))))
(%defmacro csym::__rangeof (csym::type csym::start csym::end)
 (common-lisp:list '-
                   (common-lisp:list 'cast
                                     'csym::size_t
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          (common-lisp:list
                                                                           'cast
                                                                           (common-lisp:list
                                                                            'ptr
                                                                            csym::type)
                                                                           '0))
                                                                         csym::end)))
                   (common-lisp:list 'cast
                                     'csym::size_t
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list 'fref
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
 (common-lisp:list 'c-exp
                   "extern __typeof ( ~0@*~a ) ~1@*~a ;"
                   csym::sym
                   csym::aliassym))
(%defmacro csym::__weak_reference (csym::sym csym::alias)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".weak \" _C2SC_STRINGIFY ( ~1@*~a ) ) ; __asm__ ( \".equ \" _C2SC_STRINGIFY ( ~1@*~a ) \", \" _C2SC_STRINGIFY ( ~0@*~a ) )"
                   csym::sym
                   csym::alias))
(%defmacro csym::__warn_references (csym::sym csym::msg)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".section .gnu.warning.\" _C2SC_STRINGIFY ( ~0@*~a ) ) ; __asm__ ( \".asciz \\\"\" ~1@*~a \"\\\"\" ) ; __asm__ ( \".previous\" )"
                   csym::sym
                   csym::msg))
(%defmacro csym::__@IDSTRING (csym::name csym::string)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~1@*~a \"\\\"\" )"
                   csym::name
                   csym::string))
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
 (common-lisp:list 'cast
                   csym::type
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'void))
                                                       csym::var))))
(%defmacro csym::__@DEVOLATILE (csym::type csym::var)
 (common-lisp:list 'cast
                   csym::type
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'volatile
                                                                          'void))
                                                       csym::var))))
(%defmacro csym::__@DEQUALIFY (csym::type csym::var)
 (common-lisp:list 'cast
                   csym::type
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
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
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
(%defconstant csym::_@RUNE_T_DECLARED (c-exp ""))
(%defconstant csym::_@SIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@WCHAR_T_DECLARED (c-exp ""))
(%defmacro csym::offsetof (csym::type csym::member)
 (common-lisp:list 'cast
                   'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           csym::type)
                                                                          '0))
                                                       csym::member))))
(%defmacro csym::@ALIGN_UPWARD (csym::size)
 (common-lisp:list '*
                   (common-lisp:list 'sizeof 'csym::align_t)
                   (common-lisp:list '/
                                     (common-lisp:list '-
                                                       (common-lisp:list '+
                                                                         csym::size
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'csym::align_t))
                                                       '1)
                                     (common-lisp:list 'sizeof
                                                       'csym::align_t))))
(%defmacro DESC (type fli)
  `(struct
    (sizeof ,type)
    (ALIGN_UPWARD (sizeof ,type)) 
    (/ (sizeof ,fli) (sizeof size_t))
    ,fli) )

#|
 (common-lisp:list 'c-exp
                   "{ sizeof ( ~0@*~a ) , ( sizeof ( align_t ) * ( ( ( sizeof ( ~0@*~a ) ) + sizeof ( align_t ) - 1 ) / sizeof ( align_t ) ) ) , sizeof ( ~1@*~a ) / sizeof ( size_t ) , ~1@*~a }"
                   csym::type
                   csym::fli))
|#

;;;
(decl (csym::nrand48 xseed) (fn long (array unsigned-short 3)))
(decl (csym::strcmp s1 s2) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::memset b c len) (fn (ptr void) (ptr void) int size_t))
(decl (csym::memcpy dest src len) 
      (fn (ptr void) (ptr void) (ptr (const void)) size-t))
(decl (csym::atoi nptr) (fn int (ptr (const char))))
(decl (csym::printf format) (fn int (ptr (const char)) va-arg))
(decl (csym::malloc size) (fn (ptr void) size_t))
(decl (csym::alloca size) (fn (ptr void) size_t))
(decl (csym::realloc pt size) (fn (ptr void) (ptr void) size_t))
(decl (csym::free pt) (fn void (ptr void)))

(def (struct timeval)
  (def tv_sec long)
  (def tv_usec long) )

(def (struct timezone)
  (def tz_minuteswest int)
  (def tz_dsttime int) )

(decl (csym::gettimeofday tp tzp)
      (fn int (ptr (struct timeval)) (ptr (struct timezone))))

