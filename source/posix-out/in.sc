(%line 1 "csample/posix-header/in.h")
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
(%line 1 "/usr/include/machine/endian.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 152 "/usr/include/machine/endian.h")
(static-def (csym::__bswap64 csym::_x) (fn csym::__uint64_t csym::__uint64_t)
 (return
  (bit-or (>> csym::_x 56) (bit-and (>> csym::_x 40) 65280)
   (bit-and (>> csym::_x 24) 16711680) (bit-and (>> csym::_x 8) 4278190080)
   (bit-and (<< csym::_x 8) (<< (cast csym::__uint64_t 255) 32))
   (bit-and (<< csym::_x 24) (<< (cast csym::__uint64_t 255) 40))
   (bit-and (<< csym::_x 40) (<< (cast csym::__uint64_t 255) 48))
   (<< csym::_x 56))))
(static-def (csym::__bswap32 csym::_x) (fn csym::__uint32_t csym::__uint32_t))
(static-def (csym::__bswap16 csym::_x) (fn csym::__uint16_t csym::__uint16_t))
(%line 50 "csample/posix-header/in.h")
(deftype csym::uint8_t csym::__uint8_t)
(deftype csym::uint16_t csym::__uint16_t)
(deftype csym::uint32_t csym::__uint32_t)
(deftype csym::in_addr_t csym::uint32_t)
(deftype csym::in_port_t csym::uint16_t)
(deftype csym::sa_family_t csym::__sa_family_t)
(def (struct csym::in_addr) (decl csym::s_addr csym::in_addr_t))
(def (struct csym::sockaddr_in) (decl csym::sin_len csym::uint8_t)
 (decl csym::sin_family csym::sa_family_t)
 (decl csym::sin_port csym::in_port_t)
 (decl csym::sin_addr (struct csym::in_addr))
 (decl csym::sin_zero (array char 8)))
(decl (csym::htonl) (fn csym::uint32_t csym::uint32_t))
(decl (csym::htons) (fn csym::uint16_t csym::uint16_t))
(decl (csym::ntohl) (fn csym::uint32_t csym::uint32_t))
(decl (csym::ntohs) (fn csym::uint16_t csym::uint16_t))
(%line 421 "csample/posix-header/in.h")
(def (struct csym::ip_mreq) (decl csym::imr_multiaddr (struct csym::in_addr))
 (decl csym::imr_interface (struct csym::in_addr)))
(%line 1 "/usr/include/netinet6/in6.h")
(%line 117 "/usr/include/netinet6/in6.h")
(def (struct csym::in6_addr)
 (def (union csym::unnamed1) (decl csym::__u6_addr8 (array csym::uint8_t 16))
  (decl csym::__u6_addr16 (array csym::uint16_t 8))
  (decl csym::__u6_addr32 (array csym::uint32_t 4)))
 (decl csym::__u6_addr (union csym::unnamed1)))
(%line 145 "/usr/include/netinet6/in6.h")
(def (struct csym::sockaddr_in6) (decl csym::sin6_len csym::uint8_t)
 (decl csym::sin6_family csym::sa_family_t)
 (decl csym::sin6_port csym::in_port_t)
 (decl csym::sin6_flowinfo csym::uint32_t)
 (decl csym::sin6_addr (struct csym::in6_addr))
 (decl csym::sin6_scope_id csym::uint32_t))
(%line 228 "/usr/include/netinet6/in6.h")
(extern-decl csym::in6addr_any (const (struct csym::in6_addr)))
(extern-decl csym::in6addr_loopback (const (struct csym::in6_addr)))
(extern-decl csym::in6addr_nodelocal_allnodes (const (struct csym::in6_addr)))
(extern-decl csym::in6addr_linklocal_allnodes (const (struct csym::in6_addr)))
(extern-decl csym::in6addr_linklocal_allrouters (const (struct csym::in6_addr)))
(%line 396 "/usr/include/netinet6/in6.h")
(def (struct csym::route_in6) (decl csym::ro_rt (ptr (struct csym::rtentry)))
 (decl csym::ro_dst (struct csym::sockaddr_in6)))
(%line 513 "/usr/include/netinet6/in6.h")
(def (struct csym::ipv6_mreq)
 (decl csym::ipv6mr_multiaddr (struct csym::in6_addr))
 (decl csym::ipv6mr_interface unsigned-int))
(%line 521 "/usr/include/netinet6/in6.h")
(def (struct csym::in6_pktinfo) (decl csym::ipi6_addr (struct csym::in6_addr))
 (decl csym::ipi6_ifindex unsigned-int))
(%line 529 "/usr/include/netinet6/in6.h")
(def (struct csym::ip6_mtuinfo)
 (decl csym::ip6m_addr (struct csym::sockaddr_in6))
 (decl csym::ip6m_mtu csym::uint32_t))
(%line 636 "/usr/include/netinet6/in6.h")
(deftype csym::size_t csym::__size_t)
(deftype csym::socklen_t csym::__socklen_t)
(extern-decl (csym::inet6_option_space) (fn int int))
(extern-decl (csym::inet6_option_init)
 (fn int (ptr void) (ptr (ptr (struct csym::cmsghdr))) int))
(extern-decl (csym::inet6_option_append)
 (fn int (ptr (struct csym::cmsghdr)) (ptr (const csym::uint8_t)) int int))
(%line 654 "/usr/include/netinet6/in6.h")
(extern-decl (csym::inet6_option_alloc)
 (fn (ptr csym::uint8_t) (ptr (struct csym::cmsghdr)) int int int))
(extern-decl (csym::inet6_option_next)
 (fn int (ptr (const (struct csym::cmsghdr))) (ptr (ptr csym::uint8_t))))
(extern-decl (csym::inet6_option_find)
 (fn int (ptr (const (struct csym::cmsghdr))) (ptr (ptr csym::uint8_t)) int))
(extern-decl (csym::inet6_rthdr_space) (fn csym::size_t int int))
(extern-decl (csym::inet6_rthdr_init)
 (fn (ptr (struct csym::cmsghdr)) (ptr void) int))
(extern-decl (csym::inet6_rthdr_add)
 (fn int (ptr (struct csym::cmsghdr)) (ptr (const (struct csym::in6_addr)))
  unsigned-int))
(%line 662 "/usr/include/netinet6/in6.h")
(extern-decl (csym::inet6_rthdr_lasthop)
 (fn int (ptr (struct csym::cmsghdr)) unsigned-int))
(%line 666 "/usr/include/netinet6/in6.h")
(extern-decl (csym::inet6_rthdr_segments)
 (fn int (ptr (const (struct csym::cmsghdr)))))
(extern-decl (csym::inet6_rthdr_getaddr)
 (fn (ptr (struct csym::in6_addr)) (ptr (struct csym::cmsghdr)) int))
(extern-decl (csym::inet6_rthdr_getflags)
 (fn int (ptr (const (struct csym::cmsghdr))) int))
(extern-decl (csym::inet6_opt_init) (fn int (ptr void) csym::socklen_t))
(extern-decl (csym::inet6_opt_append)
 (fn int (ptr void) csym::socklen_t int csym::uint8_t csym::socklen_t
  csym::uint8_t (ptr (ptr void))))
(%line 673 "/usr/include/netinet6/in6.h")
(extern-decl (csym::inet6_opt_finish) (fn int (ptr void) csym::socklen_t int))
(extern-decl (csym::inet6_opt_set_val)
 (fn int (ptr void) int (ptr void) csym::socklen_t))
(extern-decl (csym::inet6_opt_next)
 (fn int (ptr void) csym::socklen_t int (ptr csym::uint8_t)
  (ptr csym::socklen_t) (ptr (ptr void))))
(%line 678 "/usr/include/netinet6/in6.h")
(extern-decl (csym::inet6_opt_find)
 (fn int (ptr void) csym::socklen_t int csym::uint8_t (ptr csym::socklen_t)
  (ptr (ptr void))))
(%line 680 "/usr/include/netinet6/in6.h")
(extern-decl (csym::inet6_opt_get_val)
 (fn int (ptr void) int (ptr void) csym::socklen_t))
(extern-decl (csym::inet6_rth_space) (fn csym::socklen_t int int))
(extern-decl (csym::inet6_rth_init)
 (fn (ptr void) (ptr void) csym::socklen_t int int))
(extern-decl (csym::inet6_rth_add)
 (fn int (ptr void) (ptr (const (struct csym::in6_addr)))))
(extern-decl (csym::inet6_rth_reverse) (fn int (ptr (const void)) (ptr void)))
(extern-decl (csym::inet6_rth_segments) (fn int (ptr (const void))))
(extern-decl (csym::inet6_rth_getaddr)
 (fn (ptr (struct csym::in6_addr)) (ptr (const void)) int))
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
(%defconstant csym::__@TIME__ "21:56:30")
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
(%defconstant csym::_@NETINET_IN_H_ (c-exp ""))
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
(%defconstant csym::_@MACHINE_ENDIAN_H_ (c-exp ""))
(%defconstant csym::_@QUAD_HIGHWORD 1)
(%defconstant csym::_@QUAD_LOWWORD 0)
(%defconstant csym::_@LITTLE_ENDIAN 1234)
(%defconstant csym::_@BIG_ENDIAN 4321)
(%defconstant csym::_@PDP_ENDIAN 3412)
(%defconstant csym::_@BYTE_ORDER csym::_@LITTLE_ENDIAN)
(%defconstant csym::@LITTLE_ENDIAN csym::_@LITTLE_ENDIAN)
(%defconstant csym::@BIG_ENDIAN csym::_@BIG_ENDIAN)
(%defconstant csym::@PDP_ENDIAN csym::_@PDP_ENDIAN)
(%defconstant csym::@BYTE_ORDER csym::_@BYTE_ORDER)
(%defmacro csym::__word_swap_int_var (csym::_arg0)
 (common-lisp:list 'c-exp
                   "__extension__ ( { register __uint32_t __X = ( ~0@*~a ) ; __asm ( \"rorl $16, %0\" : \"+r\" ( __X ) ) ; __X ; } )"
                   csym::_arg0))
(%defmacro csym::__word_swap_int (csym::_arg0)
 (common-lisp:list 'csym::__word_swap_int_var csym::_arg0))
(%defmacro csym::__byte_swap_int_var (csym::_arg0)
 (common-lisp:list 'c-exp
                   "__extension__ ( { register __uint32_t __X = ( ~0@*~a ) ; __asm ( \"xchgb %h0, %b0\\n\\trorl $16, %0\\n\\txchgb %h0, %b0\" : \"+q\" ( __X ) ) ; __X ; } )"
                   csym::_arg0))
(%defmacro csym::__byte_swap_int (csym::_arg0)
 (common-lisp:list 'csym::__byte_swap_int_var csym::_arg0))
(%defmacro csym::__byte_swap_word_var (csym::_arg0)
 (common-lisp:list 'c-exp
                   "__extension__ ( { register __uint16_t __X = ( ~0@*~a ) ; __asm ( \"xchgb %h0, %b0\" : \"+q\" ( __X ) ) ; __X ; } )"
                   csym::_arg0))
(%defmacro csym::__byte_swap_word (csym::_arg0)
 (common-lisp:list 'csym::__byte_swap_word_var csym::_arg0))
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
(%defconstant csym::__@TIME__ "21:56:30")
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
(%defconstant csym::_@NETINET_IN_H_ (c-exp ""))
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
(%defconstant csym::_@MACHINE_ENDIAN_H_ (c-exp ""))
(%defconstant csym::_@QUAD_HIGHWORD 1)
(%defconstant csym::_@QUAD_LOWWORD 0)
(%defconstant csym::_@LITTLE_ENDIAN 1234)
(%defconstant csym::_@BIG_ENDIAN 4321)
(%defconstant csym::_@PDP_ENDIAN 3412)
(%defconstant csym::_@BYTE_ORDER csym::_@LITTLE_ENDIAN)
(%defconstant csym::@LITTLE_ENDIAN csym::_@LITTLE_ENDIAN)
(%defconstant csym::@BIG_ENDIAN csym::_@BIG_ENDIAN)
(%defconstant csym::@PDP_ENDIAN csym::_@PDP_ENDIAN)
(%defconstant csym::@BYTE_ORDER csym::_@BYTE_ORDER)
(%defmacro csym::__word_swap_int_var (csym::_arg0)
 (common-lisp:list 'c-exp
                   "__extension__ ( { register __uint32_t __X = ( ~0@*~a ) ; __asm ( \"rorl $16, %0\" : \"+r\" ( __X ) ) ; __X ; } )"
                   csym::_arg0))
(%defmacro csym::__word_swap_int (csym::_arg0)
 (common-lisp:list 'csym::__word_swap_int_var csym::_arg0))
(%defmacro csym::__byte_swap_int_var (csym::_arg0)
 (common-lisp:list 'c-exp
                   "__extension__ ( { register __uint32_t __X = ( ~0@*~a ) ; __asm ( \"xchgb %h0, %b0\\n\\trorl $16, %0\\n\\txchgb %h0, %b0\" : \"+q\" ( __X ) ) ; __X ; } )"
                   csym::_arg0))
(%defmacro csym::__byte_swap_int (csym::_arg0)
 (common-lisp:list 'csym::__byte_swap_int_var csym::_arg0))
(%defmacro csym::__byte_swap_word_var (csym::_arg0)
 (common-lisp:list 'c-exp
                   "__extension__ ( { register __uint16_t __X = ( ~0@*~a ) ; __asm ( \"xchgb %h0, %b0\" : \"+q\" ( __X ) ) ; __X ; } )"
                   csym::_arg0))
(%defmacro csym::__byte_swap_word (csym::_arg0)
 (common-lisp:list 'csym::__byte_swap_word_var csym::_arg0))
(%defmacro csym::__htonl (csym::_arg0)
 (common-lisp:list 'csym::__bswap32 csym::_arg0))
(%defmacro csym::__htons (csym::_arg0)
 (common-lisp:list 'csym::__bswap16 csym::_arg0))
(%defmacro csym::__ntohl (csym::_arg0)
 (common-lisp:list 'csym::__bswap32 csym::_arg0))
(%defmacro csym::__ntohs (csym::_arg0)
 (common-lisp:list 'csym::__bswap16 csym::_arg0))
(%defconstant csym::@IPPROTO_IP 0)
(%defconstant csym::@IPPROTO_ICMP 1)
(%defconstant csym::@IPPROTO_TCP 6)
(%defconstant csym::@IPPROTO_UDP 17)
(%defconstant csym::@INADDR_ANY (cast csym::u_int32_t 0))
(%defconstant csym::@INADDR_BROADCAST (cast csym::u_int32_t 4294967295))
(%defconstant csym::_@UINT8_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT16_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT32_T_DECLARED (c-exp ""))
(%defconstant csym::_@IN_ADDR_T_DECLARED (c-exp ""))
(%defconstant csym::_@IN_PORT_T_DECLARED (c-exp ""))
(%defconstant csym::_@SA_FAMILY_T_DECLARED (c-exp ""))
(%defconstant csym::_@STRUCT_IN_ADDR_DECLARED (c-exp ""))
(%defconstant csym::_@BYTEORDER_PROTOTYPED (c-exp ""))
(%defconstant csym::_@BYTEORDER_FUNC_DEFINED (c-exp ""))
(%defmacro csym::htonl (csym::_arg0)
 (common-lisp:list 'csym::__htonl csym::_arg0))
(%defmacro csym::htons (csym::_arg0)
 (common-lisp:list 'csym::__htons csym::_arg0))
(%defmacro csym::ntohl (csym::_arg0)
 (common-lisp:list 'csym::__ntohl csym::_arg0))
(%defmacro csym::ntohs (csym::_arg0)
 (common-lisp:list 'csym::__ntohs csym::_arg0))
(%defconstant csym::@IPPROTO_RAW 255)
(%defconstant csym::@INET_ADDRSTRLEN 16)
(%defconstant csym::@IPPROTO_HOPOPTS 0)
(%defconstant csym::@IPPROTO_IGMP 2)
(%defconstant csym::@IPPROTO_GGP 3)
(%defconstant csym::@IPPROTO_IPV4 4)
(%defconstant csym::@IPPROTO_IPIP csym::@IPPROTO_IPV4)
(%defconstant csym::@IPPROTO_ST 7)
(%defconstant csym::@IPPROTO_EGP 8)
(%defconstant csym::@IPPROTO_PIGP 9)
(%defconstant csym::@IPPROTO_RCCMON 10)
(%defconstant csym::@IPPROTO_NVPII 11)
(%defconstant csym::@IPPROTO_PUP 12)
(%defconstant csym::@IPPROTO_ARGUS 13)
(%defconstant csym::@IPPROTO_EMCON 14)
(%defconstant csym::@IPPROTO_XNET 15)
(%defconstant csym::@IPPROTO_CHAOS 16)
(%defconstant csym::@IPPROTO_MUX 18)
(%defconstant csym::@IPPROTO_MEAS 19)
(%defconstant csym::@IPPROTO_HMP 20)
(%defconstant csym::@IPPROTO_PRM 21)
(%defconstant csym::@IPPROTO_IDP 22)
(%defconstant csym::@IPPROTO_TRUNK1 23)
(%defconstant csym::@IPPROTO_TRUNK2 24)
(%defconstant csym::@IPPROTO_LEAF1 25)
(%defconstant csym::@IPPROTO_LEAF2 26)
(%defconstant csym::@IPPROTO_RDP 27)
(%defconstant csym::@IPPROTO_IRTP 28)
(%defconstant csym::@IPPROTO_TP 29)
(%defconstant csym::@IPPROTO_BLT 30)
(%defconstant csym::@IPPROTO_NSP 31)
(%defconstant csym::@IPPROTO_INP 32)
(%defconstant csym::@IPPROTO_SEP 33)
(%defconstant csym::@IPPROTO_3PC 34)
(%defconstant csym::@IPPROTO_IDPR 35)
(%defconstant csym::@IPPROTO_XTP 36)
(%defconstant csym::@IPPROTO_DDP 37)
(%defconstant csym::@IPPROTO_CMTP 38)
(%defconstant csym::@IPPROTO_TPXX 39)
(%defconstant csym::@IPPROTO_IL 40)
(%defconstant csym::@IPPROTO_IPV6 41)
(%defconstant csym::@IPPROTO_SDRP 42)
(%defconstant csym::@IPPROTO_ROUTING 43)
(%defconstant csym::@IPPROTO_FRAGMENT 44)
(%defconstant csym::@IPPROTO_IDRP 45)
(%defconstant csym::@IPPROTO_RSVP 46)
(%defconstant csym::@IPPROTO_GRE 47)
(%defconstant csym::@IPPROTO_MHRP 48)
(%defconstant csym::@IPPROTO_BHA 49)
(%defconstant csym::@IPPROTO_ESP 50)
(%defconstant csym::@IPPROTO_AH 51)
(%defconstant csym::@IPPROTO_INLSP 52)
(%defconstant csym::@IPPROTO_SWIPE 53)
(%defconstant csym::@IPPROTO_NHRP 54)
(%defconstant csym::@IPPROTO_MOBILE 55)
(%defconstant csym::@IPPROTO_TLSP 56)
(%defconstant csym::@IPPROTO_SKIP 57)
(%defconstant csym::@IPPROTO_ICMPV6 58)
(%defconstant csym::@IPPROTO_NONE 59)
(%defconstant csym::@IPPROTO_DSTOPTS 60)
(%defconstant csym::@IPPROTO_AHIP 61)
(%defconstant csym::@IPPROTO_CFTP 62)
(%defconstant csym::@IPPROTO_HELLO 63)
(%defconstant csym::@IPPROTO_SATEXPAK 64)
(%defconstant csym::@IPPROTO_KRYPTOLAN 65)
(%defconstant csym::@IPPROTO_RVD 66)
(%defconstant csym::@IPPROTO_IPPC 67)
(%defconstant csym::@IPPROTO_ADFS 68)
(%defconstant csym::@IPPROTO_SATMON 69)
(%defconstant csym::@IPPROTO_VISA 70)
(%defconstant csym::@IPPROTO_IPCV 71)
(%defconstant csym::@IPPROTO_CPNX 72)
(%defconstant csym::@IPPROTO_CPHB 73)
(%defconstant csym::@IPPROTO_WSN 74)
(%defconstant csym::@IPPROTO_PVP 75)
(%defconstant csym::@IPPROTO_BRSATMON 76)
(%defconstant csym::@IPPROTO_ND 77)
(%defconstant csym::@IPPROTO_WBMON 78)
(%defconstant csym::@IPPROTO_WBEXPAK 79)
(%defconstant csym::@IPPROTO_EON 80)
(%defconstant csym::@IPPROTO_VMTP 81)
(%defconstant csym::@IPPROTO_SVMTP 82)
(%defconstant csym::@IPPROTO_VINES 83)
(%defconstant csym::@IPPROTO_TTP 84)
(%defconstant csym::@IPPROTO_IGP 85)
(%defconstant csym::@IPPROTO_DGP 86)
(%defconstant csym::@IPPROTO_TCF 87)
(%defconstant csym::@IPPROTO_IGRP 88)
(%defconstant csym::@IPPROTO_OSPFIGP 89)
(%defconstant csym::@IPPROTO_SRPC 90)
(%defconstant csym::@IPPROTO_LARP 91)
(%defconstant csym::@IPPROTO_MTP 92)
(%defconstant csym::@IPPROTO_AX25 93)
(%defconstant csym::@IPPROTO_IPEIP 94)
(%defconstant csym::@IPPROTO_MICP 95)
(%defconstant csym::@IPPROTO_SCCSP 96)
(%defconstant csym::@IPPROTO_ETHERIP 97)
(%defconstant csym::@IPPROTO_ENCAP 98)
(%defconstant csym::@IPPROTO_APES 99)
(%defconstant csym::@IPPROTO_GMTP 100)
(%defconstant csym::@IPPROTO_IPCOMP 108)
(%defconstant csym::@IPPROTO_PIM 103)
(%defconstant csym::@IPPROTO_PGM 113)
(%defconstant csym::@IPPROTO_PFSYNC 240)
(%defconstant csym::@IPPROTO_OLD_DIVERT 254)
(%defconstant csym::@IPPROTO_MAX 256)
(%defconstant csym::@IPPROTO_DONE 257)
(%defconstant csym::@IPPROTO_DIVERT 258)
(%defconstant csym::@IPPORT_RESERVED 1024)
(%defconstant csym::@IPPORT_HIFIRSTAUTO 49152)
(%defconstant csym::@IPPORT_HILASTAUTO 65535)
(%defconstant csym::@IPPORT_RESERVEDSTART 600)
(%defconstant csym::@IPPORT_MAX 65535)
(%defmacro csym::@IN_CLASSA (csym::_arg0)
 (common-lisp:list '==
                   (common-lisp:list 'cast
                                     'csym::u_int32_t
                                     (common-lisp:list 'cast
                                                       csym::_arg0
                                                       (common-lisp:list 'ptr
                                                                         '2147483648)))
                   '0))
(%defconstant csym::@IN_CLASSA_NET 4278190080)
(%defconstant csym::@IN_CLASSA_NSHIFT 24)
(%defconstant csym::@IN_CLASSA_HOST 16777215)
(%defconstant csym::@IN_CLASSA_MAX 128)
(%defmacro csym::@IN_CLASSB (csym::_arg0)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       csym::_arg0)
                                     '3221225472)
                   '2147483648))
(%defconstant csym::@IN_CLASSB_NET 4294901760)
(%defconstant csym::@IN_CLASSB_NSHIFT 16)
(%defconstant csym::@IN_CLASSB_HOST 65535)
(%defconstant csym::@IN_CLASSB_MAX 65536)
(%defmacro csym::@IN_CLASSC (csym::_arg0)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       csym::_arg0)
                                     '3758096384)
                   '3221225472))
(%defconstant csym::@IN_CLASSC_NET 4294967040)
(%defconstant csym::@IN_CLASSC_NSHIFT 8)
(%defconstant csym::@IN_CLASSC_HOST 255)
(%defmacro csym::@IN_CLASSD (csym::_arg0)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       csym::_arg0)
                                     '4026531840)
                   '3758096384))
(%defconstant csym::@IN_CLASSD_NET 4026531840)
(%defconstant csym::@IN_CLASSD_NSHIFT 28)
(%defconstant csym::@IN_CLASSD_HOST 268435455)
(%defmacro csym::@IN_MULTICAST (csym::_arg0)
 (common-lisp:list 'csym::@IN_CLASSD csym::_arg0))
(%defmacro csym::@IN_EXPERIMENTAL (csym::_arg0)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       csym::_arg0)
                                     '4026531840)
                   '4026531840))
(%defmacro csym::@IN_BADCLASS (csym::_arg0)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       csym::_arg0)
                                     '4026531840)
                   '4026531840))
(%defconstant csym::@INADDR_LOOPBACK (cast csym::u_int32_t 2130706433))
(%defconstant csym::@INADDR_NONE 4294967295)
(%defconstant csym::@INADDR_UNSPEC_GROUP (cast csym::u_int32_t 3758096384))
(%defconstant csym::@INADDR_ALLHOSTS_GROUP (cast csym::u_int32_t 3758096385))
(%defconstant csym::@INADDR_ALLRTRS_GROUP (cast csym::u_int32_t 3758096386))
(%defconstant csym::@INADDR_PFSYNC_GROUP (cast csym::u_int32_t 3758096624))
(%defconstant csym::@INADDR_ALLMDNS_GROUP (cast csym::u_int32_t 3758096635))
(%defconstant csym::@INADDR_MAX_LOCAL_GROUP (cast csym::u_int32_t 3758096639))
(%defconstant csym::@IN_LOOPBACKNET 127)
(%defconstant csym::@IP_OPTIONS 1)
(%defconstant csym::@IP_HDRINCL 2)
(%defconstant csym::@IP_TOS 3)
(%defconstant csym::@IP_TTL 4)
(%defconstant csym::@IP_RECVOPTS 5)
(%defconstant csym::@IP_RECVRETOPTS 6)
(%defconstant csym::@IP_RECVDSTADDR 7)
(%defconstant csym::@IP_SENDSRCADDR csym::@IP_RECVDSTADDR)
(%defconstant csym::@IP_RETOPTS 8)
(%defconstant csym::@IP_MULTICAST_IF 9)
(%defconstant csym::@IP_MULTICAST_TTL 10)
(%defconstant csym::@IP_MULTICAST_LOOP 11)
(%defconstant csym::@IP_ADD_MEMBERSHIP 12)
(%defconstant csym::@IP_DROP_MEMBERSHIP 13)
(%defconstant csym::@IP_MULTICAST_VIF 14)
(%defconstant csym::@IP_RSVP_ON 15)
(%defconstant csym::@IP_RSVP_OFF 16)
(%defconstant csym::@IP_RSVP_VIF_ON 17)
(%defconstant csym::@IP_RSVP_VIF_OFF 18)
(%defconstant csym::@IP_PORTRANGE 19)
(%defconstant csym::@IP_RECVIF 20)
(%defconstant csym::@IP_IPSEC_POLICY 21)
(%defconstant csym::@IP_FAITH 22)
(%defconstant csym::@IP_ONESBCAST 23)
(%defconstant csym::@IP_FW_TABLE_ADD 40)
(%defconstant csym::@IP_FW_TABLE_DEL 41)
(%defconstant csym::@IP_FW_TABLE_FLUSH 42)
(%defconstant csym::@IP_FW_TABLE_GETSIZE 43)
(%defconstant csym::@IP_FW_TABLE_LIST 44)
(%defconstant csym::@IP_FW_ADD 50)
(%defconstant csym::@IP_FW_DEL 51)
(%defconstant csym::@IP_FW_FLUSH 52)
(%defconstant csym::@IP_FW_ZERO 53)
(%defconstant csym::@IP_FW_GET 54)
(%defconstant csym::@IP_FW_RESETLOG 55)
(%defconstant csym::@IP_DUMMYNET_CONFIGURE 60)
(%defconstant csym::@IP_DUMMYNET_DEL 61)
(%defconstant csym::@IP_DUMMYNET_FLUSH 62)
(%defconstant csym::@IP_DUMMYNET_GET 64)
(%defconstant csym::@IP_RECVTTL 65)
(%defconstant csym::@IP_DEFAULT_MULTICAST_TTL 1)
(%defconstant csym::@IP_DEFAULT_MULTICAST_LOOP 1)
(%defconstant csym::@IP_MAX_MEMBERSHIPS 20)
(%defconstant csym::@IP_PORTRANGE_DEFAULT 0)
(%defconstant csym::@IP_PORTRANGE_HIGH 1)
(%defconstant csym::@IP_PORTRANGE_LOW 2)
(%defconstant csym::@IPPROTO_MAXID (+ csym::@IPPROTO_AH 1))
(%defconstant csym::@CTL_IPPROTO_NAMES
 (c-exp
  "{ { \"ip\", CTLTYPE_NODE }, { \"icmp\", CTLTYPE_NODE }, { \"igmp\", CTLTYPE_NODE }, { \"ggp\", CTLTYPE_NODE }, { 0, 0 }, { 0, 0 }, { \"tcp\", CTLTYPE_NODE }, { 0, 0 }, { \"egp\", CTLTYPE_NODE }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { \"pup\", CTLTYPE_NODE }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { \"udp\", CTLTYPE_NODE }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { \"idp\", CTLTYPE_NODE }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { \"ipsec\", CTLTYPE_NODE }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { \"pim\", CTLTYPE_NODE }, }"))
(%defconstant csym::@IPCTL_FORWARDING 1)
(%defconstant csym::@IPCTL_SENDREDIRECTS 2)
(%defconstant csym::@IPCTL_DEFTTL 3)
(%defconstant csym::@IPCTL_RTEXPIRE 5)
(%defconstant csym::@IPCTL_RTMINEXPIRE 6)
(%defconstant csym::@IPCTL_RTMAXCACHE 7)
(%defconstant csym::@IPCTL_SOURCEROUTE 8)
(%defconstant csym::@IPCTL_DIRECTEDBROADCAST 9)
(%defconstant csym::@IPCTL_INTRQMAXLEN 10)
(%defconstant csym::@IPCTL_INTRQDROPS 11)
(%defconstant csym::@IPCTL_STATS 12)
(%defconstant csym::@IPCTL_ACCEPTSOURCEROUTE 13)
(%defconstant csym::@IPCTL_FASTFORWARDING 14)
(%defconstant csym::@IPCTL_KEEPFAITH 15)
(%defconstant csym::@IPCTL_GIF_TTL 16)
(%defconstant csym::@IPCTL_MAXID 17)
(%defconstant csym::@IPCTL_NAMES
 (c-exp
  "{ { 0, 0 }, { \"forwarding\", CTLTYPE_INT }, { \"redirect\", CTLTYPE_INT }, { \"ttl\", CTLTYPE_INT }, { \"mtu\", CTLTYPE_INT }, { \"rtexpire\", CTLTYPE_INT }, { \"rtminexpire\", CTLTYPE_INT }, { \"rtmaxcache\", CTLTYPE_INT }, { \"sourceroute\", CTLTYPE_INT }, { \"directed-broadcast\", CTLTYPE_INT }, { \"intr-queue-maxlen\", CTLTYPE_INT }, { \"intr-queue-drops\", CTLTYPE_INT }, { \"stats\", CTLTYPE_STRUCT }, { \"accept_sourceroute\", CTLTYPE_INT }, { \"fastforwarding\", CTLTYPE_INT }, }"))
(%defconstant csym::__@KAME_NETINET_IN_H_INCLUDED_ (c-exp ""))
(%defconstant csym::_@NETINET6_IN6_H_ (c-exp ""))
(%defconstant csym::__@KAME__ (c-exp ""))
(%defconstant csym::__@KAME_VERSION "20010528/FreeBSD")
(%defconstant csym::@IPV6PORT_RESERVED 1024)
(%defconstant csym::@IPV6PORT_ANONMIN 49152)
(%defconstant csym::@IPV6PORT_ANONMAX 65535)
(%defconstant csym::@IPV6PORT_RESERVEDMIN 600)
(%defconstant csym::@IPV6PORT_RESERVEDMAX (- csym::@IPV6PORT_RESERVED 1))
(%defconstant csym::s6_addr (fref csym::__u6_addr csym::__u6_addr8))
(%defconstant csym::@INET6_ADDRSTRLEN 46)
(%defconstant csym::@SIN6_LEN (c-exp ""))
(%defconstant csym::@IN6ADDR_ANY_INIT
 (c-exp
  "{{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}}"))
(%defconstant csym::@IN6ADDR_LOOPBACK_INIT
 (c-exp
  "{{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}"))
(%defconstant csym::@IN6ADDR_NODELOCAL_ALLNODES_INIT
 (c-exp
  "{{{ 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}"))
(%defconstant csym::@IN6ADDR_INTFACELOCAL_ALLNODES_INIT
 (c-exp
  "{{{ 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}"))
(%defconstant csym::@IN6ADDR_LINKLOCAL_ALLNODES_INIT
 (c-exp
  "{{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}}"))
(%defconstant csym::@IN6ADDR_LINKLOCAL_ALLROUTERS_INIT
 (c-exp
  "{{{ 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }}}"))
(%defmacro csym::@IN6_ARE_ADDR_EQUAL (csym::_arg0 csym::_arg1)
 (common-lisp:list '==
                   (common-lisp:list 'csym::memcmp
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::s6_addr)
                                                                         '0))
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg1)
                                                                          'csym::s6_addr)
                                                                         '0))
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::in6_addr)))
                   '0))
(%defmacro csym::@IN6_IS_ADDR_UNSPECIFIED (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '0)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '4)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '8)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '12)))))
                                     '0)))
(%defmacro csym::@IN6_IS_ADDR_LOOPBACK (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '0)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '4)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '8)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '12)))))
                                     (common-lisp:list 'csym::ntohl '1))))
(%defmacro csym::@IN6_IS_ADDR_V4COMPAT (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '0)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '4)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '8)))))
                                     '0)
                   (common-lisp:list '!=
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '12)))))
                                     '0)
                   (common-lisp:list '!=
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '12)))))
                                     (common-lisp:list 'csym::ntohl '1))))
(%defmacro csym::@IN6_IS_ADDR_V4MAPPED (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '0)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '4)))))
                                     '0)
                   (common-lisp:list '==
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          (common-lisp:list
                                                                           'const
                                                                           'csym::u_int32_t))
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'const
                                                                            'void))
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           (common-lisp:list
                                                                            'aref
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              csym::_arg0)
                                                                             'csym::s6_addr)
                                                                            '8)))))
                                     (common-lisp:list 'csym::ntohl '65535))))
(%defconstant csym::__@IPV6_ADDR_SCOPE_NODELOCAL 1)
(%defconstant csym::__@IPV6_ADDR_SCOPE_INTFACELOCAL 1)
(%defconstant csym::__@IPV6_ADDR_SCOPE_LINKLOCAL 2)
(%defconstant csym::__@IPV6_ADDR_SCOPE_SITELOCAL 5)
(%defconstant csym::__@IPV6_ADDR_SCOPE_ORGLOCAL 8)
(%defconstant csym::__@IPV6_ADDR_SCOPE_GLOBAL 14)
(%defmacro csym::@IN6_IS_ADDR_LINKLOCAL (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          csym::_arg0)
                                                                         'csym::s6_addr)
                                                       '0)
                                     '254)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::s6_addr)
                                                                         '1)
                                                       '192)
                                     '128)))
(%defmacro csym::@IN6_IS_ADDR_SITELOCAL (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          csym::_arg0)
                                                                         'csym::s6_addr)
                                                       '0)
                                     '254)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::s6_addr)
                                                                         '1)
                                                       '192)
                                     '192)))
(%defmacro csym::@IN6_IS_ADDR_MULTICAST (csym::_arg0)
 (common-lisp:list '==
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg0)
                                                       'csym::s6_addr)
                                     '0)
                   '255))
(%defmacro csym::__@IPV6_ADDR_MC_SCOPE (csym::_arg0)
 (common-lisp:list 'bit-and
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg0)
                                                       'csym::s6_addr)
                                     '1)
                   '15))
(%defmacro csym::@IN6_IS_ADDR_MC_NODELOCAL (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list 'csym::@IN6_IS_ADDR_MULTICAST csym::_arg0)
                   (common-lisp:list '==
                                     (common-lisp:list
                                      'csym::__@IPV6_ADDR_MC_SCOPE
                                      csym::_arg0)
                                     'csym::__@IPV6_ADDR_SCOPE_NODELOCAL)))
(%defmacro csym::@IN6_IS_ADDR_MC_LINKLOCAL (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list 'csym::@IN6_IS_ADDR_MULTICAST csym::_arg0)
                   (common-lisp:list '==
                                     (common-lisp:list
                                      'csym::__@IPV6_ADDR_MC_SCOPE
                                      csym::_arg0)
                                     'csym::__@IPV6_ADDR_SCOPE_LINKLOCAL)))
(%defmacro csym::@IN6_IS_ADDR_MC_SITELOCAL (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list 'csym::@IN6_IS_ADDR_MULTICAST csym::_arg0)
                   (common-lisp:list '==
                                     (common-lisp:list
                                      'csym::__@IPV6_ADDR_MC_SCOPE
                                      csym::_arg0)
                                     'csym::__@IPV6_ADDR_SCOPE_SITELOCAL)))
(%defmacro csym::@IN6_IS_ADDR_MC_ORGLOCAL (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list 'csym::@IN6_IS_ADDR_MULTICAST csym::_arg0)
                   (common-lisp:list '==
                                     (common-lisp:list
                                      'csym::__@IPV6_ADDR_MC_SCOPE
                                      csym::_arg0)
                                     'csym::__@IPV6_ADDR_SCOPE_ORGLOCAL)))
(%defmacro csym::@IN6_IS_ADDR_MC_GLOBAL (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list 'csym::@IN6_IS_ADDR_MULTICAST csym::_arg0)
                   (common-lisp:list '==
                                     (common-lisp:list
                                      'csym::__@IPV6_ADDR_MC_SCOPE
                                      csym::_arg0)
                                     'csym::__@IPV6_ADDR_SCOPE_GLOBAL)))
(%defconstant csym::@IPV6_SOCKOPT_RESERVED1 3)
(%defconstant csym::@IPV6_UNICAST_HOPS 4)
(%defconstant csym::@IPV6_MULTICAST_IF 9)
(%defconstant csym::@IPV6_MULTICAST_HOPS 10)
(%defconstant csym::@IPV6_MULTICAST_LOOP 11)
(%defconstant csym::@IPV6_JOIN_GROUP 12)
(%defconstant csym::@IPV6_LEAVE_GROUP 13)
(%defconstant csym::@IPV6_PORTRANGE 14)
(%defconstant csym::@ICMP6_FILTER 18)
(%defconstant csym::@IPV6_CHECKSUM 26)
(%defconstant csym::@IPV6_V6ONLY 27)
(%defconstant csym::@IPV6_BINDV6ONLY csym::@IPV6_V6ONLY)
(%defconstant csym::@IPV6_IPSEC_POLICY 28)
(%defconstant csym::@IPV6_FAITH 29)
(%defconstant csym::@IPV6_FW_ADD 30)
(%defconstant csym::@IPV6_FW_DEL 31)
(%defconstant csym::@IPV6_FW_FLUSH 32)
(%defconstant csym::@IPV6_FW_ZERO 33)
(%defconstant csym::@IPV6_FW_GET 34)
(%defconstant csym::@IPV6_RTHDRDSTOPTS 35)
(%defconstant csym::@IPV6_RECVPKTINFO 36)
(%defconstant csym::@IPV6_RECVHOPLIMIT 37)
(%defconstant csym::@IPV6_RECVRTHDR 38)
(%defconstant csym::@IPV6_RECVHOPOPTS 39)
(%defconstant csym::@IPV6_RECVDSTOPTS 40)
(%defconstant csym::@IPV6_USE_MIN_MTU 42)
(%defconstant csym::@IPV6_RECVPATHMTU 43)
(%defconstant csym::@IPV6_PATHMTU 44)
(%defconstant csym::@IPV6_PKTINFO 46)
(%defconstant csym::@IPV6_HOPLIMIT 47)
(%defconstant csym::@IPV6_NEXTHOP 48)
(%defconstant csym::@IPV6_HOPOPTS 49)
(%defconstant csym::@IPV6_DSTOPTS 50)
(%defconstant csym::@IPV6_RTHDR 51)
(%defconstant csym::@IPV6_RECVTCLASS 57)
(%defconstant csym::@IPV6_AUTOFLOWLABEL 59)
(%defconstant csym::@IPV6_TCLASS 61)
(%defconstant csym::@IPV6_DONTFRAG 62)
(%defconstant csym::@IPV6_PREFER_TEMPADDR 63)
(%defconstant csym::@IPV6_RTHDR_LOOSE 0)
(%defconstant csym::@IPV6_RTHDR_STRICT 1)
(%defconstant csym::@IPV6_RTHDR_TYPE_0 0)
(%defconstant csym::@IPV6_DEFAULT_MULTICAST_HOPS 1)
(%defconstant csym::@IPV6_DEFAULT_MULTICAST_LOOP 1)
(%defconstant csym::@IPV6_PORTRANGE_DEFAULT 0)
(%defconstant csym::@IPV6_PORTRANGE_HIGH 1)
(%defconstant csym::@IPV6_PORTRANGE_LOW 2)
(%defconstant csym::@IPV6PROTO_MAXID (+ csym::@IPPROTO_PIM 1))
(%defconstant csym::@IPV6CTL_FORWARDING 1)
(%defconstant csym::@IPV6CTL_SENDREDIRECTS 2)
(%defconstant csym::@IPV6CTL_DEFHLIM 3)
(%defconstant csym::@IPV6CTL_FORWSRCRT 5)
(%defconstant csym::@IPV6CTL_STATS 6)
(%defconstant csym::@IPV6CTL_MRTSTATS 7)
(%defconstant csym::@IPV6CTL_MRTPROTO 8)
(%defconstant csym::@IPV6CTL_MAXFRAGPACKETS 9)
(%defconstant csym::@IPV6CTL_SOURCECHECK 10)
(%defconstant csym::@IPV6CTL_SOURCECHECK_LOGINT 11)
(%defconstant csym::@IPV6CTL_ACCEPT_RTADV 12)
(%defconstant csym::@IPV6CTL_KEEPFAITH 13)
(%defconstant csym::@IPV6CTL_LOG_INTERVAL 14)
(%defconstant csym::@IPV6CTL_HDRNESTLIMIT 15)
(%defconstant csym::@IPV6CTL_DAD_COUNT 16)
(%defconstant csym::@IPV6CTL_AUTO_FLOWLABEL 17)
(%defconstant csym::@IPV6CTL_DEFMCASTHLIM 18)
(%defconstant csym::@IPV6CTL_GIF_HLIM 19)
(%defconstant csym::@IPV6CTL_KAME_VERSION 20)
(%defconstant csym::@IPV6CTL_USE_DEPRECATED 21)
(%defconstant csym::@IPV6CTL_RR_PRUNE 22)
(%defconstant csym::@IPV6CTL_V6ONLY 24)
(%defconstant csym::@IPV6CTL_RTEXPIRE 25)
(%defconstant csym::@IPV6CTL_RTMINEXPIRE 26)
(%defconstant csym::@IPV6CTL_RTMAXCACHE 27)
(%defconstant csym::@IPV6CTL_USETEMPADDR 32)
(%defconstant csym::@IPV6CTL_TEMPPLTIME 33)
(%defconstant csym::@IPV6CTL_TEMPVLTIME 34)
(%defconstant csym::@IPV6CTL_AUTO_LINKLOCAL 35)
(%defconstant csym::@IPV6CTL_RIP6STATS 36)
(%defconstant csym::@IPV6CTL_PREFER_TEMPADDR 37)
(%defconstant csym::@IPV6CTL_ADDRCTLPOLICY 38)
(%defconstant csym::@IPV6CTL_MAXFRAGS 41)
(%defconstant csym::@IPV6CTL_MAXID 42)
(%defconstant csym::@M_AUTHIPHDR csym::@M_PROTO2)
(%defconstant csym::@M_DECRYPTED csym::@M_PROTO3)
(%defconstant csym::@M_LOOP csym::@M_PROTO4)
(%defconstant csym::@M_AUTHIPDGM csym::@M_PROTO5)
(%defconstant csym::_@SIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@SOCKLEN_T_DECLARED (c-exp ""))
(%undef csym::__@KAME_NETINET_IN_H_INCLUDED_)
