(%line 1 "c-header/math.h")
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
(%line 1 "/usr/include/machine/_limits.h")
(%line 27 "c-header/math.h")
(extern-decl csym::__infinity
 (const
  (union csym::__infinity_un (decl csym::__uc (array unsigned-char 8))
   (decl csym::__ud double))))
(extern-decl csym::__nan
 (const
  (union csym::__nan_un (decl csym::__uc (array unsigned-char (sizeof float)))
   (decl csym::__uf float))))
(%line 121 "c-header/math.h")
(deftype csym::double_t csym::__double_t)
(deftype csym::float_t csym::__float_t)
(%line 144 "c-header/math.h")
(extern-decl csym::signgam int)
(def (enum csym::fdversion)
 ((csym::fdlibm_ieee (- 1)) csym::fdlibm_svid csym::fdlibm_xopen
  csym::fdlibm_posix))
(%line 160 "c-header/math.h")
(def (enum csym::fdversion) common-lisp:nil)
(extern-decl csym::_fdlib_version (enum csym::fdversion))
(%line 170 "c-header/math.h")
(def (struct csym::exception) (decl csym::type int)
 (decl csym::name (ptr char)) (decl csym::arg1 double) (decl csym::arg2 double)
 (decl csym::retval double))
(%line 204 "c-header/math.h")
(%line 208 "c-header/math.h")
(decl (csym::__fpclassifyd) (fn int double))
(decl (csym::__fpclassifyf) (fn int float))
(decl (csym::__fpclassifyl) (fn int long-double))
(decl (csym::__isfinitef) (fn int float))
(decl (csym::__isfinite) (fn int double))
(decl (csym::__isfinitel) (fn int long-double))
(decl (csym::__isinff) (fn int float))
(decl (csym::__isinfl) (fn int long-double))
(decl (csym::__isnanl) (fn int long-double))
(decl (csym::__isnormalf) (fn int float))
(decl (csym::__isnormal) (fn int double))
(decl (csym::__isnormall) (fn int long-double))
(decl (csym::__signbit) (fn int double))
(decl (csym::__signbitf) (fn int float))
(decl (csym::__signbitl) (fn int long-double))
(decl (csym::acos) (fn double double))
(decl (csym::asin) (fn double double))
(decl (csym::atan) (fn double double))
(decl (csym::atan2) (fn double double double))
(decl (csym::cos) (fn double double))
(decl (csym::sin) (fn double double))
(decl (csym::tan) (fn double double))
(decl (csym::cosh) (fn double double))
(decl (csym::sinh) (fn double double))
(decl (csym::tanh) (fn double double))
(decl (csym::exp) (fn double double))
(decl (csym::frexp) (fn double double (ptr int)))
(decl (csym::ldexp) (fn double double int))
(decl (csym::log) (fn double double))
(decl (csym::log10) (fn double double))
(decl (csym::modf) (fn double double (ptr double)))
(decl (csym::pow) (fn double double double))
(decl (csym::sqrt) (fn double double))
(decl (csym::ceil) (fn double double))
(decl (csym::fabs) (fn double double))
(decl (csym::floor) (fn double double))
(decl (csym::fmod) (fn double double double))
(%line 256 "c-header/math.h")
(decl (csym::acosh) (fn double double))
(decl (csym::asinh) (fn double double))
(decl (csym::atanh) (fn double double))
(decl (csym::cbrt) (fn double double))
(decl (csym::erf) (fn double double))
(decl (csym::erfc) (fn double double))
(decl (csym::expm1) (fn double double))
(decl (csym::hypot) (fn double double double))
(decl (csym::ilogb) (fn int double))
(decl (csym::isinf) (fn int double))
(decl (csym::isnan) (fn int double))
(decl (csym::lgamma) (fn double double))
(decl (csym::log1p) (fn double double))
(decl (csym::logb) (fn double double))
(decl (csym::nextafter) (fn double double double))
(decl (csym::remainder) (fn double double double))
(decl (csym::rint) (fn double double))
(decl (csym::j0) (fn double double))
(decl (csym::j1) (fn double double))
(decl (csym::jn) (fn double int double))
(decl (csym::scalb) (fn double double double))
(decl (csym::y0) (fn double double))
(decl (csym::y1) (fn double double))
(decl (csym::yn) (fn double int double))
(decl (csym::gamma) (fn double double))
(decl (csym::copysign) (fn double double double))
(decl (csym::fdim) (fn double double double))
(decl (csym::fmax) (fn double double double))
(decl (csym::fmin) (fn double double double))
(decl (csym::nearbyint) (fn double double))
(decl (csym::round) (fn double double))
(decl (csym::scalbln) (fn double double long))
(decl (csym::scalbn) (fn double double int))
(decl (csym::tgamma) (fn double double))
(decl (csym::trunc) (fn double double))
(%line 306 "c-header/math.h")
(decl (csym::drem) (fn double double double))
(decl (csym::finite) (fn int double))
(decl (csym::isnanf) (fn int float))
(%line 314 "c-header/math.h")
(decl (csym::gamma_r) (fn double double (ptr int)))
(decl (csym::lgamma_r) (fn double double (ptr int)))
(%line 320 "c-header/math.h")
(decl (csym::significand) (fn double double))
(decl (csym::matherr) (fn int (ptr (struct csym::exception))))
(decl (csym::acosf) (fn float float))
(decl (csym::asinf) (fn float float))
(decl (csym::atanf) (fn float float))
(decl (csym::atan2f) (fn float float float))
(decl (csym::cosf) (fn float float))
(decl (csym::sinf) (fn float float))
(decl (csym::tanf) (fn float float))
(decl (csym::coshf) (fn float float))
(decl (csym::sinhf) (fn float float))
(decl (csym::tanhf) (fn float float))
(decl (csym::expf) (fn float float))
(decl (csym::expm1f) (fn float float))
(decl (csym::frexpf) (fn float float (ptr int)))
(decl (csym::ilogbf) (fn int float))
(decl (csym::ldexpf) (fn float float int))
(decl (csym::log10f) (fn float float))
(decl (csym::log1pf) (fn float float))
(decl (csym::logf) (fn float float))
(decl (csym::modff) (fn float float (ptr float)))
(decl (csym::powf) (fn float float float))
(decl (csym::sqrtf) (fn float float))
(decl (csym::ceilf) (fn float float))
(decl (csym::fabsf) (fn float float))
(decl (csym::floorf) (fn float float))
(decl (csym::fmodf) (fn float float float))
(decl (csym::roundf) (fn float float))
(decl (csym::erff) (fn float float))
(decl (csym::erfcf) (fn float float))
(decl (csym::hypotf) (fn float float float))
(decl (csym::lgammaf) (fn float float))
(decl (csym::acoshf) (fn float float))
(decl (csym::asinhf) (fn float float))
(decl (csym::atanhf) (fn float float))
(decl (csym::cbrtf) (fn float float))
(decl (csym::logbf) (fn float float))
(decl (csym::copysignf) (fn float float float))
(decl (csym::nearbyintf) (fn float float))
(decl (csym::nextafterf) (fn float float float))
(decl (csym::remainderf) (fn float float float))
(decl (csym::rintf) (fn float float))
(decl (csym::scalblnf) (fn float float long))
(decl (csym::scalbnf) (fn float float int))
(decl (csym::truncf) (fn float float))
(decl (csym::fdimf) (fn float float float))
(decl (csym::fmaxf) (fn float float float))
(decl (csym::fminf) (fn float float float))
(%line 388 "c-header/math.h")
(decl (csym::dremf) (fn float float float))
(decl (csym::finitef) (fn int float))
(decl (csym::gammaf) (fn float float))
(decl (csym::j0f) (fn float float))
(decl (csym::j1f) (fn float float))
(decl (csym::jnf) (fn float int float))
(decl (csym::scalbf) (fn float float float))
(decl (csym::y0f) (fn float float))
(decl (csym::y1f) (fn float float))
(decl (csym::ynf) (fn float int float))
(%line 404 "c-header/math.h")
(decl (csym::gammaf_r) (fn float float (ptr int)))
(decl (csym::lgammaf_r) (fn float float (ptr int)))
(%line 410 "c-header/math.h")
(decl (csym::significandf) (fn float float))
(%line 428 "c-header/math.h")
(decl (csym::copysignl) (fn long-double long-double long-double))
(%line 438 "c-header/math.h")
(decl (csym::fabsl) (fn long-double long-double))
(decl (csym::fdiml) (fn long-double long-double long-double))
(%line 444 "c-header/math.h")
(decl (csym::fmaxl) (fn long-double long-double long-double))
(decl (csym::fminl) (fn long-double long-double long-double))
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
(%defconstant csym::__@DATE__ "Dec 29 2004")
(%defconstant csym::__@TIME__ "20:43:29")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::__extension (c-exp ""))
(%defmacro csym::__attribute__ (csym::arg0) (common-lisp:list 'c-exp ""))
(%defconstant csym::__inline (c-exp ""))
(%undef csym::i386)
(%undef csym::unix)
(%undef csym::__@GNUG__)
(%defconstant csym::_@MATH_H_ (c-exp ""))
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
(%defconstant csym::_@MACHINE__LIMITS_H_ (c-exp ""))
(%defconstant csym::__@CHAR_BIT 8)
(%defconstant csym::__@SCHAR_MAX 127)
(%defconstant csym::__@SCHAR_MIN (- (- 127) 1))
(%defconstant csym::__@UCHAR_MAX 255)
(%defconstant csym::__@USHRT_MAX 65535)
(%defconstant csym::__@SHRT_MAX 32767)
(%defconstant csym::__@SHRT_MIN (- (- 32767) 1))
(%defconstant csym::__@UINT_MAX (cast unsigned-int 4294967295))
(%defconstant csym::__@INT_MAX 2147483647)
(%defconstant csym::__@INT_MIN (- (- 2147483647) 1))
(%defconstant csym::__@ULONG_MAX (cast unsigned-long 4294967295))
(%defconstant csym::__@LONG_MAX (cast long 2147483647))
(%defconstant csym::__@LONG_MIN (- (- (cast long 2147483647)) 1))
(%defconstant csym::__@ULLONG_MAX
 (cast unsigned-long-long 18446744073709551615))
(%defconstant csym::__@LLONG_MAX (cast long-long 9223372036854775807))
(%defconstant csym::__@LLONG_MIN (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::__@SSIZE_MAX csym::__@INT_MAX)
(%defconstant csym::__@SIZE_T_MAX csym::__@UINT_MAX)
(%defconstant csym::__@OFF_MAX csym::__@LLONG_MAX)
(%defconstant csym::__@OFF_MIN csym::__@LLONG_MIN)
(%defconstant csym::__@UQUAD_MAX csym::__@ULLONG_MAX)
(%defconstant csym::__@QUAD_MAX csym::__@LLONG_MAX)
(%defconstant csym::__@QUAD_MIN csym::__@LLONG_MIN)
(%defconstant csym::__@LONG_BIT 32)
(%defconstant csym::__@WORD_BIT 32)
(%defconstant csym::__@MATH_BUILTIN_CONSTANTS (c-exp ""))
(%defconstant csym::__@MATH_BUILTIN_RELOPS (c-exp ""))
(%defconstant csym::@HUGE_VAL (csym::__builtin_huge_val))
(%defconstant csym::@FP_ILOGB0 (- csym::__@INT_MAX))
(%defconstant csym::@FP_ILOGBNAN csym::__@INT_MAX)
(%defconstant csym::@HUGE_VALF (csym::__builtin_huge_valf))
(%defconstant csym::@HUGE_VALL (csym::__builtin_huge_vall))
(%defconstant csym::@INFINITY (csym::__builtin_inf))
(%defconstant csym::@NAN (csym::__builtin_nan ""))
(%defconstant csym::@MATH_ERRNO 1)
(%defconstant csym::@MATH_ERREXCEPT 2)
(%defconstant csym::math_errhandling 0)
(%defconstant csym::@FP_INFINITE 1)
(%defconstant csym::@FP_NAN 2)
(%defconstant csym::@FP_NORMAL 4)
(%defconstant csym::@FP_SUBNORMAL 8)
(%defconstant csym::@FP_ZERO 16)
(%defmacro csym::fpclassify (csym::arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof csym::arg0)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__fpclassifyf csym::arg0)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        csym::arg0)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::__fpclassifyd
                                                       csym::arg0)
                                     (common-lisp:list 'csym::__fpclassifyl
                                                       csym::arg0))))
#| (if-exp (== (sizeof arg0) (sizeof float))
           (__fpclassifyf arg0)
           (if-exp (== (sizeof arg0) (sizeof double))
                   (__fpclassifyd arg0)
                   (__fpclassifyl arg0))) |#
(%defmacro csym::isfinite (csym::arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof csym::arg0)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__isfinitef csym::arg0)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        csym::arg0)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::__isfinite
                                                       csym::arg0)
                                     (common-lisp:list 'csym::__isfinitel
                                                       csym::arg0))))
(%defmacro csym::isinf (csym::arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof csym::arg0)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__isinff csym::arg0)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        csym::arg0)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::isinf csym::arg0)
                                     (common-lisp:list 'csym::__isinfl
                                                       csym::arg0))))
(%defmacro csym::isnan (csym::arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof csym::arg0)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::isnanf csym::arg0)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        csym::arg0)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::isnan csym::arg0)
                                     (common-lisp:list 'csym::__isnanl
                                                       csym::arg0))))
(%defmacro csym::isnormal (csym::arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof csym::arg0)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__isnormalf csym::arg0)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        csym::arg0)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::__isnormal
                                                       csym::arg0)
                                     (common-lisp:list 'csym::__isnormall
                                                       csym::arg0))))
(%defmacro csym::isgreater (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__builtin_isgreater csym::arg0 csym::arg1))
(%defmacro csym::isgreaterequal (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__builtin_isgreaterequal csym::arg0 csym::arg1))
(%defmacro csym::isless (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__builtin_isless csym::arg0 csym::arg1))
(%defmacro csym::islessequal (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__builtin_islessequal csym::arg0 csym::arg1))
(%defmacro csym::islessgreater (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__builtin_islessgreater csym::arg0 csym::arg1))
(%defmacro csym::isunordered (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__builtin_isunordered csym::arg0 csym::arg1))
(%defmacro csym::signbit (csym::arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof csym::arg0)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__signbitf csym::arg0)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        csym::arg0)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::__signbit
                                                       csym::arg0)
                                     (common-lisp:list 'csym::__signbitl
                                                       csym::arg0))))
(%defconstant csym::@M_E 2.718281828459045d0)
(%defconstant csym::@M_LOG2E 1.4426950408889634d0)
(%defconstant csym::@M_LOG10E 0.4342944819032518d0)
(%defconstant csym::@M_LN2 0.6931471805599453d0)
(%defconstant csym::@M_LN10 2.302585092994046d0)
(%defconstant csym::@M_PI 3.141592653589793d0)
(%defconstant csym::@M_PI_2 1.5707963267948966d0)
(%defconstant csym::@M_PI_4 0.7853981633974483d0)
(%defconstant csym::@M_1_PI 0.3183098861837907d0)
(%defconstant csym::@M_2_PI 0.6366197723675814d0)
(%defconstant csym::@M_2_SQRTPI 1.1283791670955126d0)
(%defconstant csym::@M_SQRT2 1.4142135623730951d0)
(%defconstant csym::@M_SQRT1_2 0.7071067811865476d0)
(%defconstant csym::@MAXFLOAT (cast float 3.4028234663852886d+38))
(%defconstant csym::_@LIB_VERSION_TYPE (enum csym::fdversion))
(%defconstant csym::_@LIB_VERSION csym::_fdlib_version)
(%defconstant csym::_@IEEE_ csym::fdlibm_ieee)
(%defconstant csym::_@SVID_ csym::fdlibm_svid)
(%defconstant csym::_@XOPEN_ csym::fdlibm_xopen)
(%defconstant csym::_@POSIX_ csym::fdlibm_posix)
(%defconstant csym::@HUGE csym::@MAXFLOAT)
(%defconstant csym::@X_TLOSS 1.414847550405688d+16)
(%defconstant csym::@DOMAIN 1)
(%defconstant csym::@SING 2)
(%defconstant csym::@OVERFLOW 3)
(%defconstant csym::@UNDERFLOW 4)
(%defconstant csym::@TLOSS 5)
(%defconstant csym::@PLOSS 6)
