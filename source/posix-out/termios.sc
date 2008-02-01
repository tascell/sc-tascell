(%line 1 "csample/posix-header/termios.h")
(%line 178 "csample/posix-header/termios.h")
(deftype csym::tcflag_t unsigned-int)
(deftype csym::cc_t unsigned-char)
(deftype csym::speed_t unsigned-int)
(def (struct csym::termios) (decl csym::c_iflag csym::tcflag_t)
 (decl csym::c_oflag csym::tcflag_t) (decl csym::c_cflag csym::tcflag_t)
 (decl csym::c_lflag csym::tcflag_t) (decl csym::c_cc (array csym::cc_t 20))
 (decl csym::c_ispeed csym::speed_t) (decl csym::c_ospeed csym::speed_t))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 247 "csample/posix-header/termios.h")
(decl (csym::cfgetispeed)
 (fn csym::speed_t (ptr (const (struct csym::termios)))))
(decl (csym::cfgetospeed)
 (fn csym::speed_t (ptr (const (struct csym::termios)))))
(decl (csym::cfsetispeed) (fn int (ptr (struct csym::termios)) csym::speed_t))
(decl (csym::cfsetospeed) (fn int (ptr (struct csym::termios)) csym::speed_t))
(decl (csym::tcgetattr) (fn int int (ptr (struct csym::termios))))
(decl (csym::tcsetattr) (fn int int int (ptr (const (struct csym::termios)))))
(decl (csym::tcdrain) (fn int int))
(decl (csym::tcflow) (fn int int int))
(decl (csym::tcflush) (fn int int int))
(decl (csym::tcsendbreak) (fn int int int))
(decl (csym::cfmakeraw) (fn void (ptr (struct csym::termios))))
(decl (csym::cfsetspeed) (fn int (ptr (struct csym::termios)) csym::speed_t))
(%line 1 "/usr/include/sys/ttycom.h")
(%line 1 "/usr/include/sys/ioccom.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 65 "/usr/include/sys/ioccom.h")
(decl (csym::ioctl) (fn int int unsigned-long va-arg))
(%line 52 "/usr/include/sys/ttycom.h")
(def (struct csym::winsize) (decl csym::ws_row unsigned-short)
 (decl csym::ws_col unsigned-short) (decl csym::ws_xpixel unsigned-short)
 (decl csym::ws_ypixel unsigned-short))
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
(%defconstant csym::__@TIME__ "15:20:23")
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
(%defconstant csym::_@SYS_TERMIOS_H_ (c-exp ""))
(%defconstant csym::@VEOF 0)
(%defconstant csym::@VEOL 1)
(%defconstant csym::@VEOL2 2)
(%defconstant csym::@VERASE 3)
(%defconstant csym::@VWERASE 4)
(%defconstant csym::@VKILL 5)
(%defconstant csym::@VREPRINT 6)
(%defconstant csym::@VERASE2 7)
(%defconstant csym::@VINTR 8)
(%defconstant csym::@VQUIT 9)
(%defconstant csym::@VSUSP 10)
(%defconstant csym::@VDSUSP 11)
(%defconstant csym::@VSTART 12)
(%defconstant csym::@VSTOP 13)
(%defconstant csym::@VLNEXT 14)
(%defconstant csym::@VDISCARD 15)
(%defconstant csym::@VMIN 16)
(%defconstant csym::@VTIME 17)
(%defconstant csym::@VSTATUS 18)
(%defconstant csym::@NCCS 20)
(%defconstant csym::_@POSIX_VDISABLE 255)
(%defmacro csym::@CCEQ (csym::_arg0 csym::_arg1)
 (common-lisp:list 'if-exp
                   (common-lisp:list '== csym::_arg1 csym::_arg0)
                   (common-lisp:list '!= csym::_arg0 'csym::_@POSIX_VDISABLE)
                   '0))
(%defconstant csym::@IGNBRK 1)
(%defconstant csym::@BRKINT 2)
(%defconstant csym::@IGNPAR 4)
(%defconstant csym::@PARMRK 8)
(%defconstant csym::@INPCK 16)
(%defconstant csym::@ISTRIP 32)
(%defconstant csym::@INLCR 64)
(%defconstant csym::@IGNCR 128)
(%defconstant csym::@ICRNL 256)
(%defconstant csym::@IXON 512)
(%defconstant csym::@IXOFF 1024)
(%defconstant csym::@IXANY 2048)
(%defconstant csym::@IMAXBEL 8192)
(%defconstant csym::@OPOST 1)
(%defconstant csym::@ONLCR 2)
(%defconstant csym::@OXTABS 4)
(%defconstant csym::@ONOEOT 8)
(%defconstant csym::@OCRNL 16)
(%defconstant csym::@ONOCR 32)
(%defconstant csym::@ONLRET 64)
(%defconstant csym::@CIGNORE 1)
(%defconstant csym::@CSIZE 768)
(%defconstant csym::@CS5 0)
(%defconstant csym::@CS6 256)
(%defconstant csym::@CS7 512)
(%defconstant csym::@CS8 768)
(%defconstant csym::@CSTOPB 1024)
(%defconstant csym::@CREAD 2048)
(%defconstant csym::@PARENB 4096)
(%defconstant csym::@PARODD 8192)
(%defconstant csym::@HUPCL 16384)
(%defconstant csym::@CLOCAL 32768)
(%defconstant csym::@CCTS_OFLOW 65536)
(%defconstant csym::@CRTSCTS (bit-or csym::@CCTS_OFLOW csym::@CRTS_IFLOW))
(%defconstant csym::@CRTS_IFLOW 131072)
(%defconstant csym::@CDTR_IFLOW 262144)
(%defconstant csym::@CDSR_OFLOW 524288)
(%defconstant csym::@CCAR_OFLOW 1048576)
(%defconstant csym::@MDMBUF 1048576)
(%defconstant csym::@ECHOKE 1)
(%defconstant csym::@ECHOE 2)
(%defconstant csym::@ECHOK 4)
(%defconstant csym::@ECHO 8)
(%defconstant csym::@ECHONL 16)
(%defconstant csym::@ECHOPRT 32)
(%defconstant csym::@ECHOCTL 64)
(%defconstant csym::@ISIG 128)
(%defconstant csym::@ICANON 256)
(%defconstant csym::@ALTWERASE 512)
(%defconstant csym::@IEXTEN 1024)
(%defconstant csym::@EXTPROC 2048)
(%defconstant csym::@TOSTOP 4194304)
(%defconstant csym::@FLUSHO 8388608)
(%defconstant csym::@NOKERNINFO 33554432)
(%defconstant csym::@PENDIN 536870912)
(%defconstant csym::@NOFLSH 2147483648)
(%defconstant csym::@TCSANOW 0)
(%defconstant csym::@TCSADRAIN 1)
(%defconstant csym::@TCSAFLUSH 2)
(%defconstant csym::@TCSASOFT 16)
(%defconstant csym::@B0 0)
(%defconstant csym::@B50 50)
(%defconstant csym::@B75 75)
(%defconstant csym::@B110 110)
(%defconstant csym::@B134 134)
(%defconstant csym::@B150 150)
(%defconstant csym::@B200 200)
(%defconstant csym::@B300 300)
(%defconstant csym::@B600 600)
(%defconstant csym::@B1200 1200)
(%defconstant csym::@B1800 1800)
(%defconstant csym::@B2400 2400)
(%defconstant csym::@B4800 4800)
(%defconstant csym::@B9600 9600)
(%defconstant csym::@B19200 19200)
(%defconstant csym::@B38400 38400)
(%defconstant csym::@B7200 7200)
(%defconstant csym::@B14400 14400)
(%defconstant csym::@B28800 28800)
(%defconstant csym::@B57600 57600)
(%defconstant csym::@B76800 76800)
(%defconstant csym::@B115200 115200)
(%defconstant csym::@B230400 230400)
(%defconstant csym::@B460800 460800)
(%defconstant csym::@B921600 921600)
(%defconstant csym::@EXTA 19200)
(%defconstant csym::@EXTB 38400)
(%defconstant csym::@TCIFLUSH 1)
(%defconstant csym::@TCOFLUSH 2)
(%defconstant csym::@TCIOFLUSH 3)
(%defconstant csym::@TCOOFF 1)
(%defconstant csym::@TCOON 2)
(%defconstant csym::@TCIOFF 3)
(%defconstant csym::@TCION 4)
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
(%defconstant csym::_@SYS_TTYCOM_H_ (c-exp ""))
(%defconstant csym::_@SYS_IOCCOM_H_ (c-exp ""))
(%defconstant csym::@IOCPARM_MASK 8191)
(%defmacro csym::@IOCPARM_LEN (csym::_arg0)
 (common-lisp:list 'bit-and
                   (common-lisp:list '>> csym::_arg0 '16)
                   'csym::@IOCPARM_MASK))
(%defmacro csym::@IOCBASECMD (csym::_arg0)
 (common-lisp:list 'bit-and
                   csym::_arg0
                   (common-lisp:list 'bit-not
                                     (common-lisp:list '<<
                                                       'csym::@IOCPARM_MASK
                                                       '16))))
(%defmacro csym::@IOCGROUP (csym::_arg0)
 (common-lisp:list 'bit-and (common-lisp:list '>> csym::_arg0 '8) '255))
(%defconstant csym::@IOCPARM_MAX csym::@PAGE_SIZE)
(%defconstant csym::@IOC_VOID 536870912)
(%defconstant csym::@IOC_OUT 1073741824)
(%defconstant csym::@IOC_IN 2147483648)
(%defconstant csym::@IOC_INOUT (bit-or csym::@IOC_IN csym::@IOC_OUT))
(%defconstant csym::@IOC_DIRMASK 3758096384)
(%defmacro csym::_@IOC (csym::_arg0 csym::_arg1 csym::_arg2 csym::_arg3)
 (common-lisp:list 'cast
                   'unsigned-long
                   (common-lisp:list 'bit-or
                                     csym::_arg0
                                     (common-lisp:list '<<
                                                       (common-lisp:list
                                                        'bit-and
                                                        csym::_arg3
                                                        'csym::@IOCPARM_MASK)
                                                       '16)
                                     (common-lisp:list '<< csym::_arg1 '8)
                                     csym::_arg2)))
(%defmacro csym::_@IO (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::_@IOC 'csym::@IOC_VOID csym::_arg0 csym::_arg1 '0))
(%defmacro csym::_@IOR (csym::_arg0 csym::_arg1 csym::_arg2)
 (common-lisp:list 'csym::_@IOC
                   'csym::@IOC_OUT
                   csym::_arg0
                   csym::_arg1
                   (common-lisp:list 'sizeof csym::_arg2)))
(%defmacro csym::_@IOW (csym::_arg0 csym::_arg1 csym::_arg2)
 (common-lisp:list 'csym::_@IOC
                   'csym::@IOC_IN
                   csym::_arg0
                   csym::_arg1
                   (common-lisp:list 'sizeof csym::_arg2)))
(%defmacro csym::_@IOWR (csym::_arg0 csym::_arg1 csym::_arg2)
 (common-lisp:list 'csym::_@IOC
                   'csym::@IOC_INOUT
                   csym::_arg0
                   csym::_arg1
                   (common-lisp:list 'sizeof csym::_arg2)))
(%defconstant csym::@TIOCEXCL (csym::_@IO #\t 13))
(%defconstant csym::@TIOCNXCL (csym::_@IO #\t 14))
(%defconstant csym::@TIOCFLUSH (c-exp "_IOW('t', 16, int)"))
(%defconstant csym::@TIOCGETA (c-exp "_IOR('t', 19, struct termios)"))
(%defconstant csym::@TIOCSETA (c-exp "_IOW('t', 20, struct termios)"))
(%defconstant csym::@TIOCSETAW (c-exp "_IOW('t', 21, struct termios)"))
(%defconstant csym::@TIOCSETAF (c-exp "_IOW('t', 22, struct termios)"))
(%defconstant csym::@TIOCGETD (c-exp "_IOR('t', 26, int)"))
(%defconstant csym::@TIOCSETD (c-exp "_IOW('t', 27, int)"))
(%defconstant csym::@TIOCSBRK (csym::_@IO #\t 123))
(%defconstant csym::@TIOCCBRK (csym::_@IO #\t 122))
(%defconstant csym::@TIOCSDTR (csym::_@IO #\t 121))
(%defconstant csym::@TIOCCDTR (csym::_@IO #\t 120))
(%defconstant csym::@TIOCGPGRP (c-exp "_IOR('t', 119, int)"))
(%defconstant csym::@TIOCSPGRP (c-exp "_IOW('t', 118, int)"))
(%defconstant csym::@TIOCOUTQ (c-exp "_IOR('t', 115, int)"))
(%defconstant csym::@TIOCSTI (c-exp "_IOW('t', 114, char)"))
(%defconstant csym::@TIOCNOTTY (csym::_@IO #\t 113))
(%defconstant csym::@TIOCPKT (c-exp "_IOW('t', 112, int)"))
(%defconstant csym::@TIOCPKT_DATA 0)
(%defconstant csym::@TIOCPKT_FLUSHREAD 1)
(%defconstant csym::@TIOCPKT_FLUSHWRITE 2)
(%defconstant csym::@TIOCPKT_STOP 4)
(%defconstant csym::@TIOCPKT_START 8)
(%defconstant csym::@TIOCPKT_NOSTOP 16)
(%defconstant csym::@TIOCPKT_DOSTOP 32)
(%defconstant csym::@TIOCPKT_IOCTL 64)
(%defconstant csym::@TIOCSTOP (csym::_@IO #\t 111))
(%defconstant csym::@TIOCSTART (csym::_@IO #\t 110))
(%defconstant csym::@TIOCMSET (c-exp "_IOW('t', 109, int)"))
(%defconstant csym::@TIOCMBIS (c-exp "_IOW('t', 108, int)"))
(%defconstant csym::@TIOCMBIC (c-exp "_IOW('t', 107, int)"))
(%defconstant csym::@TIOCMGET (c-exp "_IOR('t', 106, int)"))
(%defconstant csym::@TIOCM_LE 1)
(%defconstant csym::@TIOCM_DTR 2)
(%defconstant csym::@TIOCM_RTS 4)
(%defconstant csym::@TIOCM_ST 8)
(%defconstant csym::@TIOCM_SR 16)
(%defconstant csym::@TIOCM_CTS 32)
(%defconstant csym::@TIOCM_DCD 64)
(%defconstant csym::@TIOCM_RI 128)
(%defconstant csym::@TIOCM_DSR 256)
(%defconstant csym::@TIOCM_CD csym::@TIOCM_DCD)
(%defconstant csym::@TIOCM_CAR csym::@TIOCM_DCD)
(%defconstant csym::@TIOCM_RNG csym::@TIOCM_RI)
(%defconstant csym::@TIOCGWINSZ (c-exp "_IOR('t', 104, struct winsize)"))
(%defconstant csym::@TIOCSWINSZ (c-exp "_IOW('t', 103, struct winsize)"))
(%defconstant csym::@TIOCUCNTL (c-exp "_IOW('t', 102, int)"))
(%defconstant csym::@TIOCSTAT (csym::_@IO #\t 101))
(%defmacro csym::@UIOCCMD (csym::_arg0)
 (common-lisp:list 'csym::_@IO '#\u csym::_arg0))
(%defconstant csym::@TIOCCONS (c-exp "_IOW('t', 98, int)"))
(%defconstant csym::@TIOCSCTTY (csym::_@IO #\t 97))
(%defconstant csym::@TIOCEXT (c-exp "_IOW('t', 96, int)"))
(%defconstant csym::@TIOCSIG (csym::_@IO #\t 95))
(%defconstant csym::@TIOCDRAIN (csym::_@IO #\t 94))
(%defconstant csym::@TIOCMSDTRWAIT (c-exp "_IOW('t', 91, int)"))
(%defconstant csym::@TIOCMGDTRWAIT (c-exp "_IOR('t', 90, int)"))
(%defconstant csym::@TIOCTIMESTAMP (c-exp "_IOR('t', 89, struct timeval)"))
(%defconstant csym::@TIOCSDRAINWAIT (c-exp "_IOW('t', 87, int)"))
(%defconstant csym::@TIOCGDRAINWAIT (c-exp "_IOR('t', 86, int)"))
(%defconstant csym::@TTYDISC 0)
(%defconstant csym::@SLIPDISC 4)
(%defconstant csym::@PPPDISC 5)
(%defconstant csym::@NETGRAPHDISC 6)
(%defconstant csym::@H4DISC 7)
(%defconstant csym::_@SYS_TTYDEFAULTS_H_ (c-exp ""))
(%defconstant csym::@TTYDEF_IFLAG
 (bit-or csym::@BRKINT csym::@ICRNL csym::@IMAXBEL csym::@IXON csym::@IXANY))
(%defconstant csym::@TTYDEF_OFLAG (bit-or csym::@OPOST csym::@ONLCR))
(%defconstant csym::@TTYDEF_LFLAG
 (bit-or csym::@ECHO csym::@ICANON csym::@ISIG csym::@IEXTEN csym::@ECHOE
  csym::@ECHOKE csym::@ECHOCTL))
(%defconstant csym::@TTYDEF_CFLAG (bit-or csym::@CREAD csym::@CS8 csym::@HUPCL))
(%defconstant csym::@TTYDEF_SPEED csym::@B9600)
(%defmacro csym::@CTRL (csym::_arg0)
 (common-lisp:list 'bit-and csym::_arg0 '31))
(%defconstant csym::@CEOF (csym::@CTRL #\d))
(%defconstant csym::@CEOL 255)
(%defconstant csym::@CERASE 127)
(%defconstant csym::@CERASE2 (csym::@CTRL #\h))
(%defconstant csym::@CINTR (csym::@CTRL #\c))
(%defconstant csym::@CSTATUS (csym::@CTRL #\t))
(%defconstant csym::@CKILL (csym::@CTRL #\u))
(%defconstant csym::@CMIN 1)
(%defconstant csym::@CQUIT 28)
(%defconstant csym::@CSUSP (csym::@CTRL #\z))
(%defconstant csym::@CTIME 0)
(%defconstant csym::@CDSUSP (csym::@CTRL #\y))
(%defconstant csym::@CSTART (csym::@CTRL #\q))
(%defconstant csym::@CSTOP (csym::@CTRL #\s))
(%defconstant csym::@CLNEXT (csym::@CTRL #\v))
(%defconstant csym::@CDISCARD (csym::@CTRL #\o))
(%defconstant csym::@CWERASE (csym::@CTRL #\w))
(%defconstant csym::@CREPRINT (csym::@CTRL #\r))
(%defconstant csym::@CEOT csym::@CEOF)
(%defconstant csym::@CBRK csym::@CEOL)
(%defconstant csym::@CRPRNT csym::@CREPRINT)
(%defconstant csym::@CFLUSH csym::@CDISCARD)
