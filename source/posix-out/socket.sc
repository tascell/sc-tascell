(%line 1 "csample/posix-header/socket.h")
(%line 1 "/usr/include/sys/cdefs.h")
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
(%line 1 "/usr/include/sys/_iovec.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 39 "/usr/include/sys/_iovec.h")
(deftype csym::size_t csym::__size_t)
(def (struct csym::iovec) (decl csym::iov_base (ptr void))
 (decl csym::iov_len csym::size_t))
(%line 1 "/usr/include/machine/param.h")
(%line 52 "csample/posix-header/socket.h")
(deftype csym::gid_t csym::__gid_t)
(deftype csym::off_t csym::__off_t)
(deftype c2sc::$abstract csym::__pid_t)
(deftype csym::sa_family_t csym::__sa_family_t)
(deftype csym::socklen_t csym::__socklen_t)
(deftype csym::ssize_t csym::__ssize_t)
(deftype csym::uid_t csym::__uid_t)
(%line 141 "csample/posix-header/socket.h")
(def (struct csym::linger) (decl csym::l_onoff int) (decl csym::l_linger int))
(def (struct csym::accept_filter_arg) (decl csym::af_name (array char 16))
 (decl csym::af_arg (array char (- 256 16))))
(%line 215 "csample/posix-header/socket.h")
(def (struct csym::sockaddr) (decl csym::sa_len unsigned-char)
 (decl csym::sa_family csym::sa_family_t) (decl csym::sa_data (array char 14)))
(%line 227 "csample/posix-header/socket.h")
(def (struct csym::sockproto) (decl csym::sp_family unsigned-short)
 (decl csym::sp_protocol unsigned-short))
(%line 243 "csample/posix-header/socket.h")
(def (struct csym::sockaddr_storage) (decl csym::ss_len unsigned-char)
 (decl csym::ss_family csym::sa_family_t)
 (decl csym::__ss_pad1
  (array char
   (- (- (sizeof csym::__int64_t) (sizeof unsigned-char))
    (sizeof csym::sa_family_t))))
 (decl csym::__ss_align csym::__int64_t)
 (decl csym::__ss_pad2
  (array char
   (-
    (-
     (- (- (cast unsigned-int 128) (sizeof unsigned-char))
      (sizeof csym::sa_family_t))
     (- (- (sizeof csym::__int64_t) (sizeof unsigned-char))
      (sizeof csym::sa_family_t)))
    (sizeof csym::__int64_t)))))
(%line 376 "csample/posix-header/socket.h")
(def (struct csym::msghdr) (decl csym::msg_name (ptr void))
 (decl csym::msg_namelen csym::socklen_t)
 (decl csym::msg_iov (ptr (struct csym::iovec))) (decl csym::msg_iovlen int)
 (decl csym::msg_control (ptr void))
 (decl csym::msg_controllen csym::socklen_t) (decl csym::msg_flags int))
(%line 406 "csample/posix-header/socket.h")
(def (struct csym::cmsghdr) (decl csym::cmsg_len csym::socklen_t)
 (decl csym::cmsg_level int) (decl csym::cmsg_type int))
(%line 428 "csample/posix-header/socket.h")
(def (struct csym::cmsgcred) (decl csym::cmcred_pid long)
 (decl csym::cmcred_uid csym::uid_t) (decl csym::cmcred_euid csym::uid_t)
 (decl csym::cmcred_gid csym::gid_t) (decl csym::cmcred_ngroups short)
 (decl csym::cmcred_groups (array csym::gid_t 16)))
(%line 481 "csample/posix-header/socket.h")
(def (struct csym::osockaddr) (decl csym::sa_family unsigned-short)
 (decl csym::sa_data (array char 14)))
(%line 489 "csample/posix-header/socket.h")
(def (struct csym::omsghdr) (decl csym::msg_name (ptr char))
 (decl csym::msg_namelen int) (decl csym::msg_iov (ptr (struct csym::iovec)))
 (decl csym::msg_iovlen int) (decl csym::msg_accrights (ptr char))
 (decl csym::msg_accrightslen int))
(%line 510 "csample/posix-header/socket.h")
(def (struct csym::sf_hdtr) (decl csym::headers (ptr (struct csym::iovec)))
 (decl csym::hdr_cnt int) (decl csym::trailers (ptr (struct csym::iovec)))
 (decl csym::trl_cnt int))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 527 "csample/posix-header/socket.h")
(decl (csym::accept)
 (fn int int (ptr (struct csym::sockaddr)) (ptr csym::socklen_t)))
(decl (csym::bind)
 (fn int int (ptr (const (struct csym::sockaddr))) csym::socklen_t))
(decl (csym::connect)
 (fn int int (ptr (const (struct csym::sockaddr))) csym::socklen_t))
(decl (csym::getpeername)
 (fn int int (ptr (struct csym::sockaddr)) (ptr csym::socklen_t)))
(decl (csym::getsockname)
 (fn int int (ptr (struct csym::sockaddr)) (ptr csym::socklen_t)))
(decl (csym::getsockopt) (fn int int int int (ptr void) (ptr csym::socklen_t)))
(decl (csym::listen) (fn int int int))
(decl (csym::recv) (fn csym::ssize_t int (ptr void) csym::size_t int))
(decl (csym::recvfrom)
 (fn csym::ssize_t int (ptr void) csym::size_t int
  (ptr (struct csym::sockaddr)) (ptr csym::socklen_t)))
(decl (csym::recvmsg) (fn csym::ssize_t int (ptr (struct csym::msghdr)) int))
(decl (csym::send) (fn csym::ssize_t int (ptr (const void)) csym::size_t int))
(decl (csym::sendto)
 (fn csym::ssize_t int (ptr (const void)) csym::size_t int
  (ptr (const (struct csym::sockaddr))) csym::socklen_t))
(decl (csym::sendmsg)
 (fn csym::ssize_t int (ptr (const (struct csym::msghdr))) int))
(decl (csym::sendfile)
 (fn int int int csym::off_t csym::size_t (ptr (struct csym::sf_hdtr))
  (ptr csym::off_t) int))
(decl (csym::setsockopt)
 (fn int int int int (ptr (const void)) csym::socklen_t))
(decl (csym::shutdown) (fn int int int))
(decl (csym::sockatmark) (fn int int))
(decl (csym::socket) (fn int int int int))
(decl (csym::socketpair) (fn int int int int (ptr int)))
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
(%defconstant csym::__@DATE__ "Feb 13 2005")
(%defconstant csym::__@TIME__ "13:56:42")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::time_t double)
(%defconstant csym::pid_t long)
(%defconstant csym::__complex__ (c-exp ""))
(%defmacro csym::__extension__ (csym::_arg0) csym::_arg0)
(%defmacro csym::__attribute__ (csym::_arg0)
 (common-lisp:list 'c-exp "" csym::_arg0))
(%defconstant csym::__inline (c-exp ""))
(%defmacro csym::__asm (csym::_arg0) (common-lisp:list 'c-exp "" csym::_arg0))
(%undef csym::i386)
(%undef csym::unix)
(%undef csym::__@GNUG__)
(%defconstant csym::_@SYS_SOCKET_H_ (c-exp ""))
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
                                                                         (common-lisp:list
                                                                          'const
                                                                          'void))
                                                       csym::_arg1))))
(%defmacro csym::__@DEVOLATILE (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   csym::_arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'volatile
                                                                          'void))
                                                       csym::_arg1))))
(%defmacro csym::__@DEQUALIFY (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   csym::_arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'volatile
                                                                          'void))
                                                       csym::_arg1))))
(%defconstant csym::__@POSIX_VISIBLE 200112)
(%defconstant csym::__@XSI_VISIBLE 600)
(%defconstant csym::__@BSD_VISIBLE 1)
(%defconstant csym::__@ISO_C_VISIBLE 1999)
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
(%defconstant csym::_@SYS__IOVEC_H_ (c-exp ""))
(%defconstant csym::_@SIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@NO_NAMESPACE_POLLUTION (c-exp ""))
(%defconstant csym::_@ALIGNBYTES (- (sizeof int) 1))
(%defmacro csym::_@ALIGN (csym::_arg0)
 (common-lisp:list 'bit-and
                   (common-lisp:list '+
                                     (common-lisp:list 'cast
                                                       'unsigned-int
                                                       csym::_arg0)
                                     'csym::_@ALIGNBYTES)
                   (common-lisp:list 'bit-not 'csym::_@ALIGNBYTES)))
(%defconstant csym::_@MACHINE csym::i386)
(%defconstant csym::_@MACHINE_ARCH csym::i386)
(%undef csym::_@NO_NAMESPACE_POLLUTION)
(%defconstant csym::_@GID_T_DECLARED (c-exp ""))
(%defconstant csym::_@OFF_T_DECLARED (c-exp ""))
(%defconstant csym::_@PID_T_DECLARED (c-exp ""))
(%defconstant csym::_@SA_FAMILY_T_DECLARED (c-exp ""))
(%defconstant csym::_@SOCKLEN_T_DECLARED (c-exp ""))
(%defconstant csym::_@SSIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@UID_T_DECLARED (c-exp ""))
(%defconstant csym::@SOCK_STREAM 1)
(%defconstant csym::@SOCK_DGRAM 2)
(%defconstant csym::@SOCK_RAW 3)
(%defconstant csym::@SOCK_RDM 4)
(%defconstant csym::@SOCK_SEQPACKET 5)
(%defconstant csym::@SO_DEBUG 1)
(%defconstant csym::@SO_ACCEPTCONN 2)
(%defconstant csym::@SO_REUSEADDR 4)
(%defconstant csym::@SO_KEEPALIVE 8)
(%defconstant csym::@SO_DONTROUTE 16)
(%defconstant csym::@SO_BROADCAST 32)
(%defconstant csym::@SO_USELOOPBACK 64)
(%defconstant csym::@SO_LINGER 128)
(%defconstant csym::@SO_OOBINLINE 256)
(%defconstant csym::@SO_REUSEPORT 512)
(%defconstant csym::@SO_TIMESTAMP 1024)
(%defconstant csym::@SO_NOSIGPIPE 2048)
(%defconstant csym::@SO_ACCEPTFILTER 4096)
(%defconstant csym::@SO_BINTIME 8192)
(%defconstant csym::@SO_SNDBUF 4097)
(%defconstant csym::@SO_RCVBUF 4098)
(%defconstant csym::@SO_SNDLOWAT 4099)
(%defconstant csym::@SO_RCVLOWAT 4100)
(%defconstant csym::@SO_SNDTIMEO 4101)
(%defconstant csym::@SO_RCVTIMEO 4102)
(%defconstant csym::@SO_ERROR 4103)
(%defconstant csym::@SO_TYPE 4104)
(%defconstant csym::@SO_LABEL 4105)
(%defconstant csym::@SO_PEERLABEL 4112)
(%defconstant csym::@SOL_SOCKET 65535)
(%defconstant csym::@AF_UNSPEC 0)
(%defconstant csym::@AF_LOCAL csym::@AF_UNIX)
(%defconstant csym::@AF_UNIX 1)
(%defconstant csym::@AF_INET 2)
(%defconstant csym::@AF_IMPLINK 3)
(%defconstant csym::@AF_PUP 4)
(%defconstant csym::@AF_CHAOS 5)
(%defconstant csym::@AF_NETBIOS 6)
(%defconstant csym::@AF_ISO 7)
(%defconstant csym::@AF_OSI csym::@AF_ISO)
(%defconstant csym::@AF_ECMA 8)
(%defconstant csym::@AF_DATAKIT 9)
(%defconstant csym::@AF_CCITT 10)
(%defconstant csym::@AF_SNA 11)
(%defconstant csym::@AF_DEC@net 12)
(%defconstant csym::@AF_DLI 13)
(%defconstant csym::@AF_LAT 14)
(%defconstant csym::@AF_HYLINK 15)
(%defconstant csym::@AF_APPLETALK 16)
(%defconstant csym::@AF_ROUTE 17)
(%defconstant csym::@AF_LINK 18)
(%defconstant csym::pseudo_@AF_XTP 19)
(%defconstant csym::@AF_COIP 20)
(%defconstant csym::@AF_CNT 21)
(%defconstant csym::pseudo_@AF_RTIP 22)
(%defconstant csym::@AF_IPX 23)
(%defconstant csym::@AF_SIP 24)
(%defconstant csym::pseudo_@AF_PIP 25)
(%defconstant csym::@AF_ISDN 26)
(%defconstant csym::@AF_E164 csym::@AF_ISDN)
(%defconstant csym::pseudo_@AF_KEY 27)
(%defconstant csym::@AF_INET6 28)
(%defconstant csym::@AF_NATM 29)
(%defconstant csym::@AF_ATM 30)
(%defconstant csym::pseudo_@AF_HDRCMPLT 31)
(%defconstant csym::@AF_NETGRAPH 32)
(%defconstant csym::@AF_SLOW 33)
(%defconstant csym::@AF_SCLUSTER 34)
(%defconstant csym::@AF_ARP 35)
(%defconstant csym::@AF_BLUETOOTH 36)
(%defconstant csym::@AF_MAX 37)
(%defconstant csym::@SOCK_MAXADDRLEN 255)
(%defconstant csym::_@SS_MAXSIZE (cast unsigned-int 128))
(%defconstant csym::_@SS_ALIGNSIZE (sizeof csym::__int64_t))
(%defconstant csym::_@SS_PAD1SIZE
 (- (- csym::_@SS_ALIGNSIZE (sizeof unsigned-char)) (sizeof csym::sa_family_t)))
(%defconstant csym::_@SS_PAD2SIZE
 (-
  (-
   (- (- csym::_@SS_MAXSIZE (sizeof unsigned-char)) (sizeof csym::sa_family_t))
   csym::_@SS_PAD1SIZE)
  csym::_@SS_ALIGNSIZE))
(%defconstant csym::@PF_UNSPEC csym::@AF_UNSPEC)
(%defconstant csym::@PF_LOCAL csym::@AF_LOCAL)
(%defconstant csym::@PF_UNIX csym::@PF_LOCAL)
(%defconstant csym::@PF_INET csym::@AF_INET)
(%defconstant csym::@PF_IMPLINK csym::@AF_IMPLINK)
(%defconstant csym::@PF_PUP csym::@AF_PUP)
(%defconstant csym::@PF_CHAOS csym::@AF_CHAOS)
(%defconstant csym::@PF_NETBIOS csym::@AF_NETBIOS)
(%defconstant csym::@PF_ISO csym::@AF_ISO)
(%defconstant csym::@PF_OSI csym::@AF_ISO)
(%defconstant csym::@PF_ECMA csym::@AF_ECMA)
(%defconstant csym::@PF_DATAKIT csym::@AF_DATAKIT)
(%defconstant csym::@PF_CCITT csym::@AF_CCITT)
(%defconstant csym::@PF_SNA csym::@AF_SNA)
(%defconstant csym::@PF_DEC@net csym::@af_dec@NET)
(%defconstant csym::@PF_DLI csym::@AF_DLI)
(%defconstant csym::@PF_LAT csym::@AF_LAT)
(%defconstant csym::@PF_HYLINK csym::@AF_HYLINK)
(%defconstant csym::@PF_APPLETALK csym::@AF_APPLETALK)
(%defconstant csym::@PF_ROUTE csym::@AF_ROUTE)
(%defconstant csym::@PF_LINK csym::@AF_LINK)
(%defconstant csym::@PF_XTP csym::PSEUDO_@af_xtp)
(%defconstant csym::@PF_COIP csym::@AF_COIP)
(%defconstant csym::@PF_CNT csym::@AF_CNT)
(%defconstant csym::@PF_SIP csym::@AF_SIP)
(%defconstant csym::@PF_IPX csym::@AF_IPX)
(%defconstant csym::@PF_RTIP csym::PSEUDO_@af_rtip)
(%defconstant csym::@PF_PIP csym::PSEUDO_@af_pip)
(%defconstant csym::@PF_ISDN csym::@AF_ISDN)
(%defconstant csym::@PF_KEY csym::PSEUDO_@af_key)
(%defconstant csym::@PF_INET6 csym::@AF_INET6)
(%defconstant csym::@PF_NATM csym::@AF_NATM)
(%defconstant csym::@PF_ATM csym::@AF_ATM)
(%defconstant csym::@PF_NETGRAPH csym::@AF_NETGRAPH)
(%defconstant csym::@PF_SLOW csym::@AF_SLOW)
(%defconstant csym::@PF_SCLUSTER csym::@AF_SCLUSTER)
(%defconstant csym::@PF_ARP csym::@AF_ARP)
(%defconstant csym::@PF_BLUETOOTH csym::@AF_BLUETOOTH)
(%defconstant csym::@PF_MAX csym::@AF_MAX)
(%defconstant csym::@NET_MAXID csym::@AF_MAX)
(%defconstant csym::@CTL_NET_NAMES
 (c-exp
  "{ { 0, 0 }, { \"unix\", CTLTYPE_NODE }, { \"inet\", CTLTYPE_NODE }, { \"implink\", CTLTYPE_NODE }, { \"pup\", CTLTYPE_NODE }, { \"chaos\", CTLTYPE_NODE }, { \"xerox_ns\", CTLTYPE_NODE }, { \"iso\", CTLTYPE_NODE }, { \"emca\", CTLTYPE_NODE }, { \"datakit\", CTLTYPE_NODE }, { \"ccitt\", CTLTYPE_NODE }, { \"ibm_sna\", CTLTYPE_NODE }, { \"decnet\", CTLTYPE_NODE }, { \"dec_dli\", CTLTYPE_NODE }, { \"lat\", CTLTYPE_NODE }, { \"hylink\", CTLTYPE_NODE }, { \"appletalk\", CTLTYPE_NODE }, { \"route\", CTLTYPE_NODE }, { \"link_layer\", CTLTYPE_NODE }, { \"xtp\", CTLTYPE_NODE }, { \"coip\", CTLTYPE_NODE }, { \"cnt\", CTLTYPE_NODE }, { \"rtip\", CTLTYPE_NODE }, { \"ipx\", CTLTYPE_NODE }, { \"sip\", CTLTYPE_NODE }, { \"pip\", CTLTYPE_NODE }, { \"isdn\", CTLTYPE_NODE }, { \"key\", CTLTYPE_NODE }, { \"inet6\", CTLTYPE_NODE }, { \"natm\", CTLTYPE_NODE }, { \"atm\", CTLTYPE_NODE }, { \"hdrcomplete\", CTLTYPE_NODE }, { \"netgraph\", CTLTYPE_NODE }, { \"snp\", CTLTYPE_NODE }, { \"scp\", CTLTYPE_NODE }, }"))
(%defconstant csym::@NET_RT_DUMP 1)
(%defconstant csym::@NET_RT_FLAGS 2)
(%defconstant csym::@NET_RT_IFLIST 3)
(%defconstant csym::@NET_RT_IFMALIST 4)
(%defconstant csym::@NET_RT_MAXID 5)
(%defconstant csym::@CTL_NET_RT_NAMES
 (c-exp
  "{ { 0, 0 }, { \"dump\", CTLTYPE_STRUCT }, { \"flags\", CTLTYPE_STRUCT }, { \"iflist\", CTLTYPE_STRUCT }, { \"ifmalist\", CTLTYPE_STRUCT }, }"))
(%defconstant csym::@SOMAXCONN 128)
(%defconstant csym::@MSG_OOB 1)
(%defconstant csym::@MSG_PEEK 2)
(%defconstant csym::@MSG_DONTROUTE 4)
(%defconstant csym::@MSG_EOR 8)
(%defconstant csym::@MSG_TRUNC 16)
(%defconstant csym::@MSG_CTRUNC 32)
(%defconstant csym::@MSG_WAITALL 64)
(%defconstant csym::@MSG_DONTWAIT 128)
(%defconstant csym::@MSG_EOF 256)
(%defconstant csym::@MSG_NBIO 16384)
(%defconstant csym::@MSG_COMPAT 32768)
(%defconstant csym::@CMGROUP_MAX 16)
(%defmacro csym::@CMSG_DATA (csym::_arg0)
 (common-lisp:list '+
                   (common-lisp:list 'cast
                                     (common-lisp:list 'ptr 'unsigned-char)
                                     csym::_arg0)
                   (common-lisp:list 'csym::_@ALIGN
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr)))))
(%defmacro csym::@CMSG_NXTHDR (csym::_arg0 csym::_arg1)
 (common-lisp:list 'if-exp
                   (common-lisp:list '>
                                     (common-lisp:list '+
                                                       (common-lisp:list '+
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           'char)
                                                                          csym::_arg1)
                                                                         (common-lisp:list
                                                                          'csym::_@ALIGN
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            csym::_arg1)
                                                                           'csym::cmsg_len)))
                                                       (common-lisp:list
                                                        'csym::_@ALIGN
                                                        (common-lisp:list
                                                         'sizeof
                                                         (common-lisp:list
                                                          'struct
                                                          'csym::cmsghdr))))
                                     (common-lisp:list '+
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          'char)
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::msg_control))
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          csym::_arg0)
                                                                         'csym::msg_controllen)))
                   (common-lisp:list 'cast
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr))
                                     '0)
                   (common-lisp:list 'cast
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr))
                                     (common-lisp:list '+
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          'char)
                                                                         csym::_arg1)
                                                       (common-lisp:list
                                                        'csym::_@ALIGN
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg1)
                                                                          'csym::cmsg_len))))))
(%defmacro csym::@CMSG_FIRSTHDR (csym::_arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '>=
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg0)
                                                       'csym::msg_controllen)
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr)))
                   (common-lisp:list 'cast
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr))
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg0)
                                                       'csym::msg_control))
                   (common-lisp:list 'cast
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr))
                                     'csym::@NULL)))
(%defmacro csym::@CMSG_SPACE (csym::_arg0)
 (common-lisp:list '+
                   (common-lisp:list 'csym::_@ALIGN
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr)))
                   (common-lisp:list 'csym::_@ALIGN csym::_arg0)))
(%defmacro csym::@CMSG_LEN (csym::_arg0)
 (common-lisp:list '+
                   (common-lisp:list 'csym::_@ALIGN
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr)))
                   csym::_arg0))
(%defconstant csym::@SCM_RIGHTS 1)
(%defconstant csym::@SCM_TIMESTAMP 2)
(%defconstant csym::@SCM_CREDS 3)
(%defconstant csym::@SCM_BINTIME 4)
(%defconstant csym::@SHUT_RD 0)
(%defconstant csym::@SHUT_WR 1)
(%defconstant csym::@SHUT_RDWR 2)
(%defconstant csym::@SF_NODISKIO 1)
