(%line 1 "csample/posix-header/tcp.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 40 "csample/posix-header/tcp.h")
(deftype csym::u_int32_t unsigned-long)
(deftype csym::u_short unsigned-short)
(deftype csym::u_char unsigned-char)
(deftype csym::tcp_seq csym::u_int32_t)
(deftype csym::tcp_cc csym::u_int32_t)
(%line 50 "csample/posix-header/tcp.h")
(def (struct csym::tcphdr) (decl csym::th_sport csym::u_short)
 (decl csym::th_dport csym::u_short) (decl csym::th_seq csym::tcp_seq)
 (decl csym::th_ack csym::tcp_seq) (decl csym::th_flags csym::u_char)
 (decl csym::th_win csym::u_short) (decl csym::th_sum csym::u_short)
 (decl csym::th_urp csym::u_short))
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
(%defconstant csym::__@TIME__ "14:50:57")
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
(%defconstant csym::_@NETINET_TCP_H_ (c-exp ""))
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
(%defconstant csym::tcp6_seq csym::tcp_seq)
(%defconstant csym::tcp6hdr csym::tcphdr)
(%defconstant csym::@TH_FIN 1)
(%defconstant csym::@TH_SYN 2)
(%defconstant csym::@TH_RST 4)
(%defconstant csym::@TH_PUSH 8)
(%defconstant csym::@TH_ACK 16)
(%defconstant csym::@TH_URG 32)
(%defconstant csym::@TH_ECE 64)
(%defconstant csym::@TH_CWR 128)
(%defconstant csym::@TH_FLAGS
 (bit-or csym::@TH_FIN csym::@TH_SYN csym::@TH_RST csym::@TH_ACK csym::@TH_URG
  csym::@TH_ECE csym::@TH_CWR))
(%defconstant csym::@TCPOPT_EOL 0)
(%defconstant csym::@TCPOPT_NOP 1)
(%defconstant csym::@TCPOPT_MAXSEG 2)
(%defconstant csym::@TCPOLEN_MAXSEG 4)
(%defconstant csym::@TCPOPT_WINDOW 3)
(%defconstant csym::@TCPOLEN_WINDOW 3)
(%defconstant csym::@TCPOPT_SACK_PERMITTED 4)
(%defconstant csym::@TCPOLEN_SACK_PERMITTED 2)
(%defconstant csym::@TCPOPT_SACK 5)
(%defconstant csym::@TCPOLEN_SACK 8)
(%defconstant csym::@TCPOPT_TIMESTAMP 8)
(%defconstant csym::@TCPOLEN_TIMESTAMP 10)
(%defconstant csym::@TCPOLEN_TSTAMP_APPA (+ csym::@TCPOLEN_TIMESTAMP 2))
(%defconstant csym::@TCPOPT_TSTAMP_HDR
 (bit-or (<< csym::@TCPOPT_NOP 24) (<< csym::@TCPOPT_NOP 16)
  (<< csym::@TCPOPT_TIMESTAMP 8) csym::@TCPOLEN_TIMESTAMP))
(%defconstant csym::@MAX_TCPOPTLEN 40)
(%defconstant csym::@TCPOPT_CC 11)
(%defconstant csym::@TCPOPT_CCNEW 12)
(%defconstant csym::@TCPOPT_CCECHO 13)
(%defconstant csym::@TCPOLEN_CC 6)
(%defconstant csym::@TCPOLEN_CC_APPA (+ csym::@TCPOLEN_CC 2))
(%defmacro csym::@TCPOPT_CC_HDR (csym::_arg0)
 (common-lisp:list 'bit-or
                   (common-lisp:list '<< 'csym::@TCPOPT_NOP '24)
                   (common-lisp:list '<< 'csym::@TCPOPT_NOP '16)
                   (common-lisp:list '<< csym::_arg0 '8)
                   'csym::@TCPOLEN_CC))
(%defconstant csym::@TCPOPT_SIGNATURE 19)
(%defconstant csym::@TCPOLEN_SIGNATURE 18)
(%defconstant csym::@TCPOPT_SACK_PERMIT_HDR
 (bit-or (<< csym::@TCPOPT_NOP 24) (<< csym::@TCPOPT_NOP 16)
  (<< csym::@TCPOPT_SACK_PERMITTED 8) csym::@TCPOLEN_SACK_PERMITTED))
(%defconstant csym::@TCPOPT_SACK_HDR
 (bit-or (<< csym::@TCPOPT_NOP 24) (<< csym::@TCPOPT_NOP 16)
  (<< csym::@TCPOPT_SACK 8)))
(%defconstant csym::@MAX_SACK_BLKS 6)
(%defconstant csym::@TCP_MAX_SACK 3)
(%defconstant csym::@TCP_MSS 512)
(%defconstant csym::@TCP_MINMSS 216)
(%defconstant csym::@TCP_MINMSSOVERLOAD 0)
(%defconstant csym::@TCP6_MSS 1024)
(%defconstant csym::@TCP_MAXWIN 65535)
(%defconstant csym::@TTCP_CLIENT_SND_WND 4096)
(%defconstant csym::@TCP_MAX_WINSHIFT 14)
(%defconstant csym::@TCP_MAXBURST 4)
(%defconstant csym::@TCP_MAXHLEN (<< 15 2))
(%defconstant csym::@TCP_MAXOLEN
 (- csym::@TCP_MAXHLEN (sizeof (struct csym::tcphdr))))
(%defconstant csym::@TCP_NODELAY 1)
(%defconstant csym::@TCP_MAXSEG 2)
(%defconstant csym::@TCP_NOPUSH 4)
(%defconstant csym::@TCP_NOOPT 8)
(%defconstant csym::@TCP_MD5SIG 16)
