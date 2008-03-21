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
(%defconstant csym::__@TIME__ "22:25:33")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::__extension__ (c-exp ""))
(%defmacro csym::__attribute__ (csym::arg0) (common-lisp:list 'c-exp ""))
(%defconstant csym::__inline (c-exp ""))
(%undef csym::i386)
(%undef csym::unix)
(%undef csym::__@GNUG__)
(%defconstant csym::_@LIMITS_H_ (c-exp ""))
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
(%defconstant csym::_@POSIX_ARG_MAX 4096)
(%defconstant csym::_@POSIX_CHILD_MAX 25)
(%defconstant csym::_@POSIX_LINK_MAX 8)
(%defconstant csym::_@POSIX_MAX_CANON 255)
(%defconstant csym::_@POSIX_MAX_INPUT 255)
(%defconstant csym::_@POSIX_NAME_MAX 14)
(%defconstant csym::_@POSIX_NGROUPS_MAX 8)
(%defconstant csym::_@POSIX_OPEN_MAX 20)
(%defconstant csym::_@POSIX_PATH_MAX 256)
(%defconstant csym::_@POSIX_PIPE_BUF 512)
(%defconstant csym::_@POSIX_SSIZE_MAX 32767)
(%defconstant csym::_@POSIX_STREAM_MAX 8)
(%defconstant csym::_@POSIX_TZNAME_MAX 6)
(%defconstant csym::@BC_BASE_MAX 99)
(%defconstant csym::@BC_DIM_MAX 2048)
(%defconstant csym::@BC_SCALE_MAX 99)
(%defconstant csym::@BC_STRING_MAX 1000)
(%defconstant csym::@COLL_WEIGHTS_MAX 0)
(%defconstant csym::@EXPR_NEST_MAX 32)
(%defconstant csym::@LINE_MAX 2048)
(%defconstant csym::@RE_DUP_MAX 255)
(%defconstant csym::_@POSIX2_BC_BASE_MAX 99)
(%defconstant csym::_@POSIX2_BC_DIM_MAX 2048)
(%defconstant csym::_@POSIX2_BC_SCALE_MAX 99)
(%defconstant csym::_@POSIX2_BC_STRING_MAX 1000)
(%defconstant csym::_@POSIX2_EQUIV_CLASS_MAX 2)
(%defconstant csym::_@POSIX2_EXPR_NEST_MAX 32)
(%defconstant csym::_@POSIX2_LINE_MAX 2048)
(%defconstant csym::_@POSIX2_RE_DUP_MAX 255)
(%defconstant csym::_@POSIX_AIO_LISTIO_MAX 16)
(%defconstant csym::_@POSIX_AIO_MAX 1)
(%defconstant csym::_@POSIX_DELAYTIMER_MAX 32)
(%defconstant csym::_@POSIX_MQ_OPEN_MAX 8)
(%defconstant csym::_@POSIX_MQ_PRIO_MAX 32)
(%defconstant csym::_@POSIX_RTSIG_MAX 8)
(%defconstant csym::_@POSIX_SEM_NSEMS_MAX 256)
(%defconstant csym::_@POSIX_SEM_VALUE_MAX 32767)
(%defconstant csym::_@POSIX_SIGQUEUE_MAX 32)
(%defconstant csym::_@POSIX_TIMER_MAX 32)
(%defconstant csym::_@POSIX_THREAD_DESTRUCTOR_ITERATIONS 4)
(%defconstant csym::_@POSIX_THREAD_KEYS_MAX 128)
(%defconstant csym::_@POSIX_THREAD_THREADS_MAX 64)
(%defconstant csym::_@POSIX_HOST_NAME_MAX 255)
(%defconstant csym::_@POSIX_LOGIN_NAME_MAX 9)
(%defconstant csym::_@POSIX_SS_REPL_MAX 4)
(%defconstant csym::_@POSIX_SYMLINK_MAX 255)
(%defconstant csym::_@POSIX_SYMLOOP_MAX 8)
(%defconstant csym::_@POSIX_TRACE_EVENT_NAME_MAX 30)
(%defconstant csym::_@POSIX_TRACE_NAME_MAX 8)
(%defconstant csym::_@POSIX_TRACE_SYS_MAX 8)
(%defconstant csym::_@POSIX_TRACE_USER_EVENT_MAX 32)
(%defconstant csym::_@POSIX_TTY_NAME_MAX 9)
(%defconstant csym::_@POSIX2_CHARCLASS_NAME_MAX 14)
(%defconstant csym::_@POSIX2_COLL_WEIGHTS_MAX 2)
(%defconstant csym::_@POSIX_RE_DUP_MAX csym::_@POSIX2_RE_DUP_MAX)
(%defconstant csym::_@XOPEN_IOV_MAX 16)
(%defconstant csym::_@XOPEN_NAME_MAX 255)
(%defconstant csym::_@XOPEN_PATH_MAX 1024)
(%defconstant csym::@PASS_MAX 128)
(%defconstant csym::@NL_ARGMAX 99)
(%defconstant csym::@NL_LANGMAX 31)
(%defconstant csym::@NL_MSGMAX 32767)
(%defconstant csym::@NL_NMAX 1)
(%defconstant csym::@NL_SETMAX 255)
(%defconstant csym::@NL_TEXTMAX 2048)
(%defconstant csym::@MB_LEN_MAX 6)
(%defconstant csym::_@SYS_LIMITS_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__LIMITS_H_ (c-exp ""))
(%defconstant csym::__@CHAR_BIT 8)
(%defconstant csym::__@SCHAR_MAX 127)
(%defconstant csym::__@SCHAR_MIN (- (- 127) 1))
(%defconstant csym::__@UCHAR_MAX 255)
(%defconstant csym::__@USHRT_MAX 65535)
(%defconstant csym::__@SHRT_MAX 32767)
(%defconstant csym::__@SHRT_MIN (- (- 32767) 1))
(%defconstant csym::__@UINT_MAX (c2sc::cast c2sc::unsigned-int 4294967295))
(%defconstant csym::__@INT_MAX 2147483647)
(%defconstant csym::__@INT_MIN (- (- 2147483647) 1))
(%defconstant csym::__@ULONG_MAX (c2sc::cast c2sc::unsigned-long 4294967295))
(%defconstant csym::__@LONG_MAX (c2sc::cast c2sc::long 2147483647))
(%defconstant csym::__@LONG_MIN (- (- (c2sc::cast c2sc::long 2147483647)) 1))
(%defconstant csym::__@ULLONG_MAX
 (c2sc::cast c2sc::unsigned-long-long 18446744073709551615))
(%defconstant csym::__@LLONG_MAX
 (c2sc::cast c2sc::long-long 9223372036854775807))
(%defconstant csym::__@LLONG_MIN
 (- (- (c2sc::cast c2sc::long-long 9223372036854775807)) 1))
(%defconstant csym::__@SSIZE_MAX csym::__@INT_MAX)
(%defconstant csym::__@SIZE_T_MAX csym::__@UINT_MAX)
(%defconstant csym::__@OFF_MAX csym::__@LLONG_MAX)
(%defconstant csym::__@OFF_MIN csym::__@LLONG_MIN)
(%defconstant csym::__@UQUAD_MAX csym::__@ULLONG_MAX)
(%defconstant csym::__@QUAD_MAX csym::__@LLONG_MAX)
(%defconstant csym::__@QUAD_MIN csym::__@LLONG_MIN)
(%defconstant csym::__@LONG_BIT 32)
(%defconstant csym::__@WORD_BIT 32)
(%defconstant csym::@CHAR_BIT csym::__@CHAR_BIT)
(%defconstant csym::@SCHAR_MAX csym::__@SCHAR_MAX)
(%defconstant csym::@SCHAR_MIN csym::__@SCHAR_MIN)
(%defconstant csym::@UCHAR_MAX csym::__@UCHAR_MAX)
(%defconstant csym::@CHAR_MAX csym::@SCHAR_MAX)
(%defconstant csym::@CHAR_MIN csym::@SCHAR_MIN)
(%defconstant csym::@USHRT_MAX csym::__@USHRT_MAX)
(%defconstant csym::@SHRT_MAX csym::__@SHRT_MAX)
(%defconstant csym::@SHRT_MIN csym::__@SHRT_MIN)
(%defconstant csym::@UINT_MAX csym::__@UINT_MAX)
(%defconstant csym::@INT_MAX csym::__@INT_MAX)
(%defconstant csym::@INT_MIN csym::__@INT_MIN)
(%defconstant csym::@ULONG_MAX csym::__@ULONG_MAX)
(%defconstant csym::@LONG_MAX csym::__@LONG_MAX)
(%defconstant csym::@LONG_MIN csym::__@LONG_MIN)
(%defconstant csym::@ULLONG_MAX csym::__@ULLONG_MAX)
(%defconstant csym::@LLONG_MAX csym::__@LLONG_MAX)
(%defconstant csym::@LLONG_MIN csym::__@LLONG_MIN)
(%defconstant csym::@SSIZE_MAX csym::__@SSIZE_MAX)
(%defconstant csym::@SIZE_T_MAX csym::__@SIZE_T_MAX)
(%defconstant csym::@OFF_MAX csym::__@OFF_MAX)
(%defconstant csym::@OFF_MIN csym::__@OFF_MIN)
(%defconstant csym::@GID_MAX csym::@UINT_MAX)
(%defconstant csym::@UID_MAX csym::@UINT_MAX)
(%defconstant csym::@UQUAD_MAX csym::__@UQUAD_MAX)
(%defconstant csym::@QUAD_MAX csym::__@QUAD_MAX)
(%defconstant csym::@QUAD_MIN csym::__@QUAD_MIN)
(%defconstant csym::@LONG_BIT csym::__@LONG_BIT)
(%defconstant csym::@WORD_BIT csym::__@WORD_BIT)
(%defconstant csym::_@SYS_SYSLIMITS_H_ (c-exp ""))
(%defconstant csym::@ARG_MAX 65536)
(%defconstant csym::@CHILD_MAX 40)
(%defconstant csym::@LINK_MAX 32767)
(%defconstant csym::@MAX_CANON 255)
(%defconstant csym::@MAX_INPUT 255)
(%defconstant csym::@NAME_MAX 255)
(%defconstant csym::@NGROUPS_MAX 16)
(%defconstant csym::@OPEN_MAX 64)
(%defconstant csym::@PATH_MAX 1024)
(%defconstant csym::@PIPE_BUF 512)
(%defconstant csym::@IOV_MAX 1024)
