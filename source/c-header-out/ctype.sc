(%line 454 "c-header/stdio.h")
(%line 1 "c-header/ctype.h")
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
(%line 1 "/usr/include/_ctype.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 74 "/usr/include/_ctype.h")
(decl (csym::___runetype) (fn unsigned-long csym::__ct_rune_t))
(decl (csym::___tolower) (fn csym::__ct_rune_t csym::__ct_rune_t))
(decl (csym::___toupper) (fn csym::__ct_rune_t csym::__ct_rune_t))
(%line 1 "/usr/include/runetype.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 52 "/usr/include/runetype.h")
(def (struct csym::unnamed1) (decl csym::__min csym::__rune_t)
 (decl csym::__max csym::__rune_t) (decl csym::__map csym::__rune_t)
 (decl csym::__types (ptr unsigned-long)))
(deftype csym::_@r@UNE@e@NTRY (struct csym::unnamed1))
(def (struct csym::unnamed2) (decl csym::__nranges int)
 (decl csym::__ranges (ptr csym::_@r@UNE@e@NTRY)))
(deftype csym::_@r@UNE@r@ANGE (struct csym::unnamed2))
(def (struct csym::unnamed3) (decl csym::__magic (array char 8))
 (decl csym::__encoding (array char 32))
 (decl csym::__sgetrune
  (ptr
   (fn csym::__rune_t (ptr (const char)) csym::__size_t
    (ptr (ptr (const char))))))
 (decl csym::__sputrune
  (ptr (fn int csym::__rune_t (ptr char) csym::__size_t (ptr (ptr char)))))
 (decl csym::__invalid_rune csym::__rune_t)
 (decl csym::__runetype (array unsigned-long (<< 1 8)))
 (decl csym::__maplower (array csym::__rune_t (<< 1 8)))
 (decl csym::__mapupper (array csym::__rune_t (<< 1 8)))
 (decl csym::__runetype_ext csym::_@r@UNE@r@ANGE)
 (decl csym::__maplower_ext csym::_@r@UNE@r@ANGE)
 (decl csym::__mapupper_ext csym::_@r@UNE@r@ANGE)
 (decl csym::__variable (ptr void)) (decl csym::__variable_len int))
(deftype csym::_@r@UNE@l@OCALE (struct csym::unnamed3))
(%line 81 "/usr/include/runetype.h")
(extern-decl csym::_@d@EFAULT@r@UNE@l@OCALE csym::_@r@UNE@l@OCALE)
(extern-decl csym::_@c@URRENT@r@UNE@l@OCALE (ptr csym::_@r@UNE@l@OCALE))
(%line 98 "/usr/include/_ctype.h")
(static-def (csym::__maskrune csym::_c csym::_f)
 (fn int csym::__ct_rune_t unsigned-long)
 (return
  (bit-and
   (if-exp (or (< csym::_c 0) (>= csym::_c (<< 1 8)))
    (csym::___runetype csym::_c)
    (aref (fref (mref csym::_@c@URRENT@r@UNE@l@OCALE) csym::__runetype)
     csym::_c))
   csym::_f)))
(static-def (csym::__istype csym::_c csym::_f)
 (fn int csym::__ct_rune_t unsigned-long)
 (return (not (not (csym::__maskrune csym::_c csym::_f)))))
(static-def (csym::__isctype csym::_c csym::_f)
 (fn int csym::__ct_rune_t unsigned-long)
 (return
  (if-exp (or (< csym::_c 0) (>= csym::_c (<< 1 8))) 0
   (not
    (not
     (bit-and
      (aref (fref csym::_@d@EFAULT@r@UNE@l@OCALE csym::__runetype) csym::_c)
      csym::_f))))))
(static-def (csym::__toupper csym::_c) (fn csym::__ct_rune_t csym::__ct_rune_t)
 (return
  (if-exp (or (< csym::_c 0) (>= csym::_c (<< 1 8)))
   (csym::___toupper csym::_c)
   (aref (fref (mref csym::_@c@URRENT@r@UNE@l@OCALE) csym::__mapupper)
    csym::_c))))
(static-def (csym::__tolower csym::_c) (fn csym::__ct_rune_t csym::__ct_rune_t)
 (return
  (if-exp (or (< csym::_c 0) (>= csym::_c (<< 1 8)))
   (csym::___tolower csym::_c)
   (aref (fref (mref csym::_@c@URRENT@r@UNE@l@OCALE) csym::__maplower)
    csym::_c))))
(static-def (csym::__wcwidth csym::_c) (fn int csym::__ct_rune_t)
 (decl csym::_x unsigned-int) (if (== csym::_c 0) (return 0))
 (= csym::_x
  (cast unsigned-int
   (csym::__maskrune csym::_c
    (bit-or (c2sc::cast c2sc::long 3758096384)
     (c2sc::cast c2sc::long 262144)))))
 (if (!= (bit-and csym::_x (c2sc::cast c2sc::long 3758096384)) 0)
  (return (>> (bit-and csym::_x (c2sc::cast c2sc::long 3758096384)) 30)))
 (return
  (if-exp (!= (bit-and csym::_x (c2sc::cast c2sc::long 262144)) 0) 1 (- 1))))
(%line 52 "c-header/ctype.h")
(decl (csym::isalnum) (fn int int))
(decl (csym::isalpha) (fn int int))
(decl (csym::iscntrl) (fn int int))
(decl (csym::isdigit) (fn int int))
(decl (csym::isgraph) (fn int int))
(decl (csym::islower) (fn int int))
(decl (csym::isprint) (fn int int))
(decl (csym::ispunct) (fn int int))
(decl (csym::isspace) (fn int int))
(decl (csym::isupper) (fn int int))
(decl (csym::isxdigit) (fn int int))
(decl (csym::tolower) (fn int int))
(decl (csym::toupper) (fn int int))
(decl (csym::_tolower) (fn int int))
(decl (csym::_toupper) (fn int int))
(decl (csym::isascii) (fn int int))
(decl (csym::toascii) (fn int int))
(decl (csym::isblank) (fn int int))
(decl (csym::digittoint) (fn int int))
(decl (csym::ishexnumber) (fn int int))
(decl (csym::isideogram) (fn int int))
(decl (csym::isnumber) (fn int int))
(decl (csym::isphonogram) (fn int int))
(decl (csym::isrune) (fn int int))
(decl (csym::isspecial) (fn int int))
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
(%defconstant csym::__@DATE__ "Dec 28 2004")
(%defconstant csym::__@TIME__ "16:35:52")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::__extension__ (c-exp ""))
(%defmacro csym::__attribute__ (csym::arg0) (common-lisp:list 'c-exp ""))
(%defconstant csym::__inline (c-exp ""))
(%undef csym::i386)
(%undef csym::unix)
(%undef csym::__@GNUG__)
(%defconstant csym::_@CTYPE_H_ (c-exp ""))
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
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
(%defconstant csym::__@CTYPE_H_ (c-exp ""))
(%defconstant csym::_@CTYPE_A (c2sc::cast c2sc::long 256))
(%defconstant csym::_@CTYPE_C (c2sc::cast c2sc::long 512))
(%defconstant csym::_@CTYPE_D (c2sc::cast c2sc::long 1024))
(%defconstant csym::_@CTYPE_G (c2sc::cast c2sc::long 2048))
(%defconstant csym::_@CTYPE_L (c2sc::cast c2sc::long 4096))
(%defconstant csym::_@CTYPE_P (c2sc::cast c2sc::long 8192))
(%defconstant csym::_@CTYPE_S (c2sc::cast c2sc::long 16384))
(%defconstant csym::_@CTYPE_U (c2sc::cast c2sc::long 32768))
(%defconstant csym::_@CTYPE_X (c2sc::cast c2sc::long 65536))
(%defconstant csym::_@CTYPE_B (c2sc::cast c2sc::long 131072))
(%defconstant csym::_@CTYPE_R (c2sc::cast c2sc::long 262144))
(%defconstant csym::_@CTYPE_I (c2sc::cast c2sc::long 524288))
(%defconstant csym::_@CTYPE_T (c2sc::cast c2sc::long 1048576))
(%defconstant csym::_@CTYPE_Q (c2sc::cast c2sc::long 2097152))
(%defconstant csym::_@CTYPE_SW0 (c2sc::cast c2sc::long 536870912))
(%defconstant csym::_@CTYPE_SW1 (c2sc::cast c2sc::long 1073741824))
(%defconstant csym::_@CTYPE_SW2 (c2sc::cast c2sc::long 2147483648))
(%defconstant csym::_@CTYPE_SW3 (c2sc::cast c2sc::long 3221225472))
(%defconstant csym::_@CTYPE_SWM (c2sc::cast c2sc::long 3758096384))
(%defconstant csym::_@CTYPE_SWS 30)
(%defconstant csym::_@RUNETYPE_H_ (c-exp ""))
(%defconstant csym::_@CACHED_RUNES (<< 1 8))
(%defconstant csym::_@CRMASK (bit-not (- csym::_@CACHED_RUNES 1)))
(%defconstant csym::_@RUNE_MAGIC_1 "RuneMagi")
(%defmacro csym::isalnum (csym::arg0)
 (common-lisp:list 'csym::__istype
                   csym::arg0
                   (common-lisp:list 'bit-or
                                     'csym::_@CTYPE_A
                                     'csym::_@CTYPE_D)))
(%defmacro csym::isalpha (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_A))
(%defmacro csym::iscntrl (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_C))
(%defmacro csym::isdigit (csym::arg0)
 (common-lisp:list 'csym::__isctype csym::arg0 'csym::_@CTYPE_D))
(%defmacro csym::isgraph (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_G))
(%defmacro csym::islower (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_L))
(%defmacro csym::isprint (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_R))
(%defmacro csym::ispunct (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_P))
(%defmacro csym::isspace (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_S))
(%defmacro csym::isupper (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_U))
(%defmacro csym::isxdigit (csym::arg0)
 (common-lisp:list 'csym::__isctype csym::arg0 'csym::_@CTYPE_X))
(%defmacro csym::tolower (csym::arg0)
 (common-lisp:list 'csym::__tolower csym::arg0))
(%defmacro csym::toupper (csym::arg0)
 (common-lisp:list 'csym::__toupper csym::arg0))
(%defmacro csym::_tolower (csym::arg0)
 (common-lisp:list 'csym::__tolower csym::arg0))
(%defmacro csym::_toupper (csym::arg0)
 (common-lisp:list 'csym::__toupper csym::arg0))
(%defmacro csym::isascii (csym::arg0)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     csym::arg0
                                     (common-lisp:list 'bit-not '127))
                   '0))
(%defmacro csym::toascii (csym::arg0)
 (common-lisp:list 'bit-and csym::arg0 '127))
(%defmacro csym::isblank (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_B))
(%defmacro csym::digittoint (csym::arg0)
 (common-lisp:list 'csym::__maskrune csym::arg0 '255))
(%defmacro csym::ishexnumber (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_X))
(%defmacro csym::isideogram (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_I))
(%defmacro csym::isnumber (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_D))
(%defmacro csym::isphonogram (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_Q))
(%defmacro csym::isrune (csym::arg0)
 (common-lisp:list 'csym::__istype
                   csym::arg0
                   (common-lisp:list 'c2sc::cast 'c2sc::long '4294967040)))
(%defmacro csym::isspecial (csym::arg0)
 (common-lisp:list 'csym::__istype csym::arg0 'csym::_@CTYPE_T))
