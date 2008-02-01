(%line 1 "csample/posix-header/errno.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 43 "csample/posix-header/errno.h")
(decl (csym::__error) (fn (ptr int) void))
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
(%defconstant csym::__@DATE__ "Feb 12 2005")
(%defconstant csym::__@TIME__ "16:14:43")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::__complex__ (c-exp ""))
(%defmacro csym::__extension__ (csym::_arg0) csym::_arg0)
(%defmacro csym::__attribute__ (csym::_arg0)
 (common-lisp:list 'c-exp "" csym::_arg0))
(%defconstant csym::__inline (c-exp ""))
(%defmacro csym::__asm (csym::_arg0) (common-lisp:list 'c-exp "" csym::_arg0))
(%undef csym::i386)
(%undef csym::unix)
(%undef csym::__@GNUG__)
(%defconstant csym::_@SYS_ERRNO_H_ (c-exp ""))
(%defconstant csym::_@SYS_CDEFS_H_ (c-exp ""))
(%defconstant csym::__@BEGIN_DECLS (c-exp ""))
(%defconstant csym::__@END_DECLS (c-exp ""))
(%defmacro csym::__@GNUC_PREREQ__ (csym::_arg0 csym::_arg1)
 (common-lisp:list 'or
                   (common-lisp:list '> 'csym::__@GNUC__ csym::_arg0)
                   (common-lisp:list 'and
                                     (common-lisp:list '==
                                                       'csym::__@GNUC__
                                                       csym::_arg0)
                                     (common-lisp:list '>=
                                                       'csym::__@GNUC_MINOR__
                                                       csym::_arg1))))
(%defmacro csym::__@P (csym::_arg0) csym::_arg0)
(%defmacro csym::__@CONCAT1 (csym::_arg0 csym::_arg1)
 '"<macro with ## operator>")
(%defmacro csym::__@CONCAT (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::__@CONCAT1 csym::_arg0 csym::_arg1))
(%defmacro csym::__@STRING (csym::_arg0)
 (common-lisp:list 'csym::_@C2SC_STRINGIFY csym::_arg0))
(%defmacro csym::__@XSTRING (csym::_arg0)
 (common-lisp:list 'csym::__@STRING csym::_arg0))
(%defconstant csym::__const (const int))
(%defconstant csym::__signed signed-int)
(%defconstant csym::__volatile (volatile int))
(%defconstant csym::__dead2 (csym::__attribute__ csym::__noreturn__))
(%defconstant csym::__pure2 (csym::__attribute__ csym::__const__))
(%defconstant csym::__unused (csym::__attribute__ csym::__unused__))
(%defconstant csym::__used (csym::__attribute__ csym::__used__))
(%defconstant csym::__packed (csym::__attribute__ csym::__packed__))
(%defmacro csym::__aligned (csym::_arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__aligned__ csym::_arg0)))
(%defmacro csym::__section (csym::_arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__section__ csym::_arg0)))
(%defconstant csym::__pure (csym::__attribute__ csym::__pure__))
(%defconstant csym::__always_inline
 (csym::__attribute__ csym::__always_inline__))
(%defmacro csym::__nonnull (csym::_arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__nonnull__ csym::_arg0)))
(%defconstant csym::__@LONG_LONG_SUPPORTED (c-exp ""))
(%defconstant csym::__restrict (c-exp ""))
(%defmacro csym::__predict_true (csym::_arg0)
 (common-lisp:list 'csym::__builtin_expect csym::_arg0 '1))
(%defmacro csym::__predict_false (csym::_arg0)
 (common-lisp:list 'csym::__builtin_expect csym::_arg0 '0))
(%defmacro csym::__offsetof (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           csym::_arg0)
                                                                          '0))
                                                       csym::_arg1))))
(%defmacro csym::__rangeof (csym::_arg0 csym::_arg1 csym::_arg2)
 (common-lisp:list '-
                   (common-lisp:list 'csym::__offsetof csym::_arg0 csym::_arg2)
                   (common-lisp:list 'csym::__offsetof
                                     csym::_arg0
                                     csym::_arg1)))
(%defmacro csym::__printflike (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__format__
                                     'csym::__printf__
                                     csym::_arg0
                                     csym::_arg1)))
(%defmacro csym::__scanflike (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__format__
                                     'csym::__scanf__
                                     csym::_arg0
                                     csym::_arg1)))
(%defmacro csym::__printf0like (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp "" csym::_arg0 csym::_arg1))
(%defmacro csym::__strong_reference (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp
                   "extern __typeof ( ~0@*~a ) ~1@*~a __attribute__ ( ( __alias__ ( _C2SC_STRINGIFY ( ~0@*~a ) ) ) ) ;"
                   csym::_arg0
                   csym::_arg1))
(%defmacro csym::__weak_reference (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".weak \" _C2SC_STRINGIFY ( ~1@*~a ) ) ; __asm__ ( \".equ \" _C2SC_STRINGIFY ( ~1@*~a ) \", \" _C2SC_STRINGIFY ( ~0@*~a ) )"
                   csym::_arg0
                   csym::_arg1))
(%defmacro csym::__warn_references (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".section .gnu.warning.\" _C2SC_STRINGIFY ( ~0@*~a ) ) ; __asm__ ( \".asciz \\\"\" ~1@*~a \"\\\"\" ) ; __asm__ ( \".previous\" )"
                   csym::_arg0
                   csym::_arg1))
(%defmacro csym::__@IDSTRING (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~1@*~a \"\\\"\" )"
                   csym::_arg0
                   csym::_arg1))
(%defmacro csym::__@FBSDID (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@RCSID (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@RCSID_SOURCE (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_source_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@SCCSID (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__sccsid_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@COPYRIGHT (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__copyright_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@DECONST (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   csym::_arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'csym::constvoid)
                                                       csym::_arg1))))
(%defmacro csym::__@DEVOLATILE (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   csym::_arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'csym::volatilevoid)
                                                       csym::_arg1))))
(%defmacro csym::__@DEQUALIFY (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   csym::_arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'csym::constvolatilevoid)
                                                       csym::_arg1))))
(%defconstant csym::__@POSIX_VISIBLE 200112)
(%defconstant csym::__@XSI_VISIBLE 600)
(%defconstant csym::__@BSD_VISIBLE 1)
(%defconstant csym::__@ISO_C_VISIBLE 1999)
(%defconstant csym::errno (mref (csym::__error)))
(%defconstant csym::@EPERM 1)
(%defconstant csym::@ENOENT 2)
(%defconstant csym::@ESRCH 3)
(%defconstant csym::@EINTR 4)
(%defconstant csym::@EIO 5)
(%defconstant csym::@ENXIO 6)
(%defconstant csym::@E2BIG 7)
(%defconstant csym::@ENOEXEC 8)
(%defconstant csym::@EBADF 9)
(%defconstant csym::@ECHILD 10)
(%defconstant csym::@EDEADLK 11)
(%defconstant csym::@ENOMEM 12)
(%defconstant csym::@EACCES 13)
(%defconstant csym::@EFAULT 14)
(%defconstant csym::@ENOTBLK 15)
(%defconstant csym::@EBUSY 16)
(%defconstant csym::@EEXIST 17)
(%defconstant csym::@EXDEV 18)
(%defconstant csym::@ENODEV 19)
(%defconstant csym::@ENOTDIR 20)
(%defconstant csym::@EISDIR 21)
(%defconstant csym::@EINVAL 22)
(%defconstant csym::@ENFILE 23)
(%defconstant csym::@EMFILE 24)
(%defconstant csym::@ENOTTY 25)
(%defconstant csym::@ETXTBSY 26)
(%defconstant csym::@EFBIG 27)
(%defconstant csym::@ENOSPC 28)
(%defconstant csym::@ESPIPE 29)
(%defconstant csym::@EROFS 30)
(%defconstant csym::@EMLINK 31)
(%defconstant csym::@EPIPE 32)
(%defconstant csym::@EDOM 33)
(%defconstant csym::@ERANGE 34)
(%defconstant csym::@EAGAIN 35)
(%defconstant csym::@EWOULDBLOCK csym::@EAGAIN)
(%defconstant csym::@EINPROGRESS 36)
(%defconstant csym::@EALREADY 37)
(%defconstant csym::@ENOTSOCK 38)
(%defconstant csym::@EDESTADDRREQ 39)
(%defconstant csym::@EMSGSIZE 40)
(%defconstant csym::@EPROTOTYPE 41)
(%defconstant csym::@ENOPROTOOPT 42)
(%defconstant csym::@EPROTONOSUPPORT 43)
(%defconstant csym::@ESOCKTNOSUPPORT 44)
(%defconstant csym::@EOPNOTSUPP 45)
(%defconstant csym::@ENOTSUP csym::@EOPNOTSUPP)
(%defconstant csym::@EPFNOSUPPORT 46)
(%defconstant csym::@EAFNOSUPPORT 47)
(%defconstant csym::@EADDRINUSE 48)
(%defconstant csym::@EADDRNOTAVAIL 49)
(%defconstant csym::@ENETDOWN 50)
(%defconstant csym::@ENETUNREACH 51)
(%defconstant csym::@ENETRESET 52)
(%defconstant csym::@ECONNABORTED 53)
(%defconstant csym::@ECONNRESET 54)
(%defconstant csym::@ENOBUFS 55)
(%defconstant csym::@EISCONN 56)
(%defconstant csym::@ENOTCONN 57)
(%defconstant csym::@ESHUTDOWN 58)
(%defconstant csym::@ETOOMANYREFS 59)
(%defconstant csym::@ETIMEDOUT 60)
(%defconstant csym::@ECONNREFUSED 61)
(%defconstant csym::@ELOOP 62)
(%defconstant csym::@ENAMETOOLONG 63)
(%defconstant csym::@EHOSTDOWN 64)
(%defconstant csym::@EHOSTUNREACH 65)
(%defconstant csym::@ENOTEMPTY 66)
(%defconstant csym::@EPROCLIM 67)
(%defconstant csym::@EUSERS 68)
(%defconstant csym::@EDQUOT 69)
(%defconstant csym::@ESTALE 70)
(%defconstant csym::@EREMOTE 71)
(%defconstant csym::@EBADRPC 72)
(%defconstant csym::@ERPCMISMATCH 73)
(%defconstant csym::@EPROGUNAVAIL 74)
(%defconstant csym::@EPROGMISMATCH 75)
(%defconstant csym::@EPROCUNAVAIL 76)
(%defconstant csym::@ENOLCK 77)
(%defconstant csym::@ENOSYS 78)
(%defconstant csym::@EFTYPE 79)
(%defconstant csym::@EAUTH 80)
(%defconstant csym::@ENEEDAUTH 81)
(%defconstant csym::@EIDRM 82)
(%defconstant csym::@ENOMSG 83)
(%defconstant csym::@EOVERFLOW 84)
(%defconstant csym::@ECANCELED 85)
(%defconstant csym::@EILSEQ 86)
(%defconstant csym::@ENOATTR 87)
(%defconstant csym::@EDOOFUS 88)
(%defconstant csym::@ELAST 88)
