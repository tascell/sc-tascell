(%line 1 "posix-out/ALL.h")
(%line 1 "posix-out/aio.h")
(%line 1 "/usr/include/sys/time.h")
(%line 1 "/usr/include/sys/_timeval.h")
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
(%line 35 "/usr/include/sys/_timeval.h")
(deftype csym::suseconds_t csym::__suseconds_t)
(deftype c2sc::$abstract csym::__time_t)
(%line 47 "/usr/include/sys/_timeval.h")
(def (struct csym::timeval) (decl csym::tv_sec long)
 (decl csym::tv_usec csym::suseconds_t))
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/cdefs.h")
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
(static-def (csym::__bswap32 csym::_x) (fn csym::__uint32_t csym::__uint32_t)
 (register-def csym::__@X csym::__uint32_t csym::_x) (return csym::__@X))
(static-def (csym::__bswap16 csym::_x) (fn csym::__uint16_t csym::__uint16_t)
 (register-def csym::__@X csym::__uint16_t csym::_x) (return csym::__@X))
(%line 1 "/usr/include/sys/_types.h")
(%line 48 "/usr/include/sys/types.h")
(deftype csym::u_char unsigned-char)
(deftype csym::u_short unsigned-short)
(deftype csym::u_int unsigned-int)
(deftype csym::u_long unsigned-long)
(deftype csym::ushort unsigned-short)
(deftype csym::uint unsigned-int)
(%line 62 "/usr/include/sys/types.h")
(deftype csym::int8_t csym::__int8_t)
(deftype csym::int16_t csym::__int16_t)
(deftype csym::int32_t csym::__int32_t)
(deftype csym::int64_t csym::__int64_t)
(deftype csym::uint8_t csym::__uint8_t)
(deftype csym::uint16_t csym::__uint16_t)
(deftype csym::uint32_t csym::__uint32_t)
(deftype csym::uint64_t csym::__uint64_t)
(deftype csym::intptr_t csym::__intptr_t)
(deftype csym::uintptr_t csym::__uintptr_t)
(deftype csym::u_int8_t csym::__uint8_t)
(deftype csym::u_int16_t csym::__uint16_t)
(deftype csym::u_int32_t csym::__uint32_t)
(deftype csym::u_int64_t csym::__uint64_t)
(deftype csym::u_quad_t csym::__uint64_t)
(deftype csym::quad_t csym::__int64_t)
(deftype csym::qaddr_t (ptr csym::quad_t))
(deftype csym::caddr_t (ptr char))
(deftype csym::c_caddr_t (ptr (const char)))
(deftype csym::v_caddr_t (ptr (volatile char)))
(deftype csym::clock_t csym::__clock_t)
(deftype csym::clockid_t csym::__clockid_t)
(deftype csym::critical_t csym::__critical_t)
(deftype csym::daddr_t csym::__int64_t)
(deftype csym::dev_t csym::__dev_t)
(deftype csym::fflags_t csym::__fflags_t)
(deftype csym::fixpt_t csym::__fixpt_t)
(deftype csym::fsblkcnt_t csym::__fsblkcnt_t)
(deftype csym::fsfilcnt_t csym::__fsfilcnt_t)
(deftype csym::gid_t csym::__gid_t)
(deftype csym::in_addr_t csym::__uint32_t)
(deftype csym::in_port_t csym::__uint16_t)
(deftype csym::id_t csym::__id_t)
(deftype csym::ino_t csym::__ino_t)
(deftype csym::key_t csym::__key_t)
(deftype csym::lwpid_t csym::__lwpid_t)
(deftype csym::mode_t csym::__mode_t)
(deftype csym::nlink_t csym::__nlink_t)
(deftype csym::off_t csym::__off_t)
(deftype c2sc::$abstract csym::__pid_t)
(deftype csym::register_t csym::__register_t)
(deftype csym::rlim_t csym::__rlim_t)
(deftype csym::segsz_t csym::__segsz_t)
(deftype csym::size_t csym::__size_t)
(deftype csym::ssize_t csym::__ssize_t)
(%line 236 "/usr/include/sys/types.h")
(deftype csym::timer_t csym::__timer_t)
(deftype csym::u_register_t csym::__u_register_t)
(deftype csym::uid_t csym::__uid_t)
(deftype csym::useconds_t csym::__useconds_t)
(deftype csym::vm_offset_t csym::__vm_offset_t)
(deftype csym::vm_ooffset_t csym::__vm_ooffset_t)
(deftype csym::vm_paddr_t csym::__vm_paddr_t)
(deftype csym::vm_pindex_t csym::__vm_pindex_t)
(deftype csym::vm_size_t csym::__vm_size_t)
(%line 1 "/usr/include/sys/select.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/_sigset.h")
(%line 51 "/usr/include/sys/_sigset.h")
(def (struct csym::__sigset) (decl csym::__bits (array csym::__uint32_t 4)))
(deftype csym::__sigset_t (struct csym::__sigset))
(%line 1 "/usr/include/sys/_timeval.h")
(%line 1 "/usr/include/sys/timespec.h")
(%line 46 "/usr/include/sys/timespec.h")
(def (struct csym::timespec) (decl csym::tv_sec double)
 (decl csym::tv_nsec long))
(%line 42 "/usr/include/sys/select.h")
(deftype csym::__fd_mask unsigned-long)
(deftype csym::fd_mask csym::__fd_mask)
(deftype csym::sigset_t csym::__sigset_t)
(%line 71 "/usr/include/sys/select.h")
(def (struct csym::fd_set)
 (decl csym::__fds_bits
  (array csym::__fd_mask
   (/ (+ (cast unsigned-int 1024) (- (* (sizeof csym::__fd_mask) 8) 1))
    (* (sizeof csym::__fd_mask) 8)))))
(deftype csym::fd_set (struct csym::fd_set))
(%line 97 "/usr/include/sys/select.h")
(decl (csym::pselect)
 (fn int int (ptr csym::fd_set) (ptr csym::fd_set) (ptr csym::fd_set)
  (ptr (const (struct csym::timespec))) (ptr (const csym::sigset_t))))
(decl (csym::select)
 (fn int int (ptr csym::fd_set) (ptr csym::fd_set) (ptr csym::fd_set)
  (ptr (struct csym::timeval))))
(%line 307 "/usr/include/sys/types.h")
(decl (csym::ftruncate) (fn int int csym::off_t))
(decl (csym::lseek) (fn csym::off_t int csym::off_t int))
(decl (csym::mmap)
 (fn (ptr void) (ptr void) csym::size_t int int int csym::off_t))
(decl (csym::truncate) (fn int (ptr (const char)) csym::off_t))
(%line 1 "/usr/include/sys/timespec.h")
(%line 40 "/usr/include/sys/time.h")
(def (struct csym::timezone) (decl csym::tz_minuteswest int)
 (decl csym::tz_dsttime int))
(def (struct csym::bintime) (decl csym::sec double)
 (decl csym::frac csym::uint64_t))
(static-def (csym::bintime_addx csym::bt csym::x)
 (fn void (ptr (struct csym::bintime)) csym::uint64_t)
 (decl csym::u csym::uint64_t) (= csym::u (fref (mref csym::bt) csym::frac))
 (+= (fref (mref csym::bt) csym::frac) csym::x)
 (if (> csym::u (fref (mref csym::bt) csym::frac))
  (inc (fref (mref csym::bt) csym::sec))))
(static-def (csym::bintime_add csym::bt csym::bt2)
 (fn void (ptr (struct csym::bintime)) (ptr (const (struct csym::bintime))))
 (decl csym::u csym::uint64_t) (= csym::u (fref (mref csym::bt) csym::frac))
 (+= (fref (mref csym::bt) csym::frac) (fref (mref csym::bt2) csym::frac))
 (if (> csym::u (fref (mref csym::bt) csym::frac))
  (inc (fref (mref csym::bt) csym::sec)))
 (+= (fref (mref csym::bt) csym::sec) (fref (mref csym::bt2) csym::sec)))
(static-def (csym::bintime_sub csym::bt csym::bt2)
 (fn void (ptr (struct csym::bintime)) (ptr (const (struct csym::bintime))))
 (decl csym::u csym::uint64_t) (= csym::u (fref (mref csym::bt) csym::frac))
 (-= (fref (mref csym::bt) csym::frac) (fref (mref csym::bt2) csym::frac))
 (if (< csym::u (fref (mref csym::bt) csym::frac))
  (dec (fref (mref csym::bt) csym::sec)))
 (-= (fref (mref csym::bt) csym::sec) (fref (mref csym::bt2) csym::sec)))
(%line 107 "/usr/include/sys/time.h")
(static-def (csym::bintime2timespec csym::bt csym::ts)
 (fn void (ptr (const (struct csym::bintime))) (ptr (struct csym::timespec)))
 (= (fref (mref csym::ts) csym::tv_sec) (fref (mref csym::bt) csym::sec))
 (= (fref (mref csym::ts) csym::tv_nsec)
  (>>
   (* (cast csym::uint64_t 1000000000)
    (cast csym::uint32_t (>> (fref (mref csym::bt) csym::frac) 32)))
   32)))
(static-def (csym::timespec2bintime csym::ts csym::bt)
 (fn void (ptr (const (struct csym::timespec))) (ptr (struct csym::bintime)))
 (= (fref (mref csym::bt) csym::sec) (fref (mref csym::ts) csym::tv_sec))
 (= (fref (mref csym::bt) csym::frac)
  (* (fref (mref csym::ts) csym::tv_nsec)
   (cast csym::uint64_t (cast long-long 18446744073)))))
(static-def (csym::bintime2timeval csym::bt csym::tv)
 (fn void (ptr (const (struct csym::bintime))) (ptr (struct csym::timeval)))
 (= (fref (mref csym::tv) csym::tv_sec) (fref (mref csym::bt) csym::sec))
 (= (fref (mref csym::tv) csym::tv_usec)
  (>>
   (* (cast csym::uint64_t 1000000)
    (cast csym::uint32_t (>> (fref (mref csym::bt) csym::frac) 32)))
   32)))
(static-def (csym::timeval2bintime csym::tv csym::bt)
 (fn void (ptr (const (struct csym::timeval))) (ptr (struct csym::bintime)))
 (= (fref (mref csym::bt) csym::sec) (fref (mref csym::tv) csym::tv_sec))
 (= (fref (mref csym::bt) csym::frac)
  (* (fref (mref csym::tv) csym::tv_usec)
   (cast csym::uint64_t (cast long-long 18446744073709)))))
(%line 219 "/usr/include/sys/time.h")
(def (struct csym::itimerval) (decl csym::it_interval (struct csym::timeval))
 (decl csym::it_value (struct csym::timeval)))
(%line 227 "/usr/include/sys/time.h")
(def (struct csym::clockinfo) (decl csym::hz int) (decl csym::tick int)
 (decl csym::spare int) (decl csym::stathz int) (decl csym::profhz int))
(%line 1 "/usr/include/time.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/timespec.h")
(%line 95 "/usr/include/time.h")
(def (struct csym::tm) (decl csym::tm_sec int) (decl csym::tm_min int)
 (decl csym::tm_hour int) (decl csym::tm_mday int) (decl csym::tm_mon int)
 (decl csym::tm_year int) (decl csym::tm_wday int) (decl csym::tm_yday int)
 (decl csym::tm_isdst int) (decl csym::tm_gmtoff long)
 (decl csym::tm_zone (ptr char)))
(extern-decl csym::tzname (array (ptr char)))
(decl (csym::asctime) (fn (ptr char) (ptr (const (struct csym::tm)))))
(decl (csym::clock) (fn csym::clock_t void))
(decl (csym::ctime) (fn (ptr char) (ptr (const double))))
(decl (csym::difftime) (fn double double double))
(decl (csym::gmtime) (fn (ptr (struct csym::tm)) (ptr (const double))))
(decl (csym::localtime) (fn (ptr (struct csym::tm)) (ptr (const double))))
(decl (csym::mktime) (fn double (ptr (struct csym::tm))))
(decl (csym::strftime)
 (fn csym::size_t (ptr char) csym::size_t (ptr (const char))
  (ptr (const (struct csym::tm)))))
(decl (csym::time) (fn double (ptr double)))
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
(decl (csym::ctime_r) (fn (ptr char) (ptr (const double)) (ptr char)))
(decl (csym::gmtime_r)
 (fn (ptr (struct csym::tm)) (ptr (const double)) (ptr (struct csym::tm))))
(decl (csym::localtime_r)
 (fn (ptr (struct csym::tm)) (ptr (const double)) (ptr (struct csym::tm))))
(decl (csym::strptime)
 (fn (ptr char) (ptr (const char)) (ptr (const char)) (ptr (struct csym::tm))))
(decl (csym::timezone) (fn (ptr char) int int))
(decl (csym::tzsetwall) (fn void void))
(decl (csym::timelocal) (fn double (const (ptr (struct csym::tm)))))
(decl (csym::timegm) (fn double (const (ptr (struct csym::tm)))))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 305 "/usr/include/sys/time.h")
(decl (csym::adjtime)
 (fn int (ptr (const (struct csym::timeval))) (ptr (struct csym::timeval))))
(decl (csym::futimes) (fn int int (ptr (const (struct csym::timeval)))))
(decl (csym::getitimer) (fn int int (ptr (struct csym::itimerval))))
(decl (csym::gettimeofday)
 (fn int (ptr (struct csym::timeval)) (ptr (struct csym::timezone))))
(decl (csym::lutimes)
 (fn int (ptr (const char)) (ptr (const (struct csym::timeval)))))
(decl (csym::setitimer)
 (fn int int (ptr (const (struct csym::itimerval)))
  (ptr (struct csym::itimerval))))
(decl (csym::settimeofday)
 (fn int (ptr (const (struct csym::timeval)))
  (ptr (const (struct csym::timezone)))))
(decl (csym::utimes)
 (fn int (ptr (const char)) (ptr (const (struct csym::timeval)))))
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/signal.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/_sigset.h")
(%line 1 "/usr/include/machine/signal.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_sigset.h")
(%line 43 "/usr/include/machine/signal.h")
(deftype csym::sig_atomic_t int)
(%line 1 "/usr/include/machine/trap.h")
(%line 96 "/usr/include/machine/signal.h")
(def (struct csym::sigcontext) (decl csym::sc_mask (struct csym::__sigset))
 (decl csym::sc_onstack int) (decl csym::sc_gs int) (decl csym::sc_fs int)
 (decl csym::sc_es int) (decl csym::sc_ds int) (decl csym::sc_edi int)
 (decl csym::sc_esi int) (decl csym::sc_ebp int) (decl csym::sc_isp int)
 (decl csym::sc_ebx int) (decl csym::sc_edx int) (decl csym::sc_ecx int)
 (decl csym::sc_eax int) (decl csym::sc_trapno int) (decl csym::sc_err int)
 (decl csym::sc_eip int) (decl csym::sc_cs int) (decl csym::sc_efl int)
 (decl csym::sc_esp int) (decl csym::sc_ss int) (decl csym::sc_len int)
 (decl csym::sc_fpformat int) (decl csym::sc_ownedfp int)
 (decl csym::sc_spare1 (array int 1)) (decl csym::sc_fpstate (array int 128))
 (decl csym::sc_spare2 (array int 8)))
(%line 123 "/usr/include/machine/signal.h")
(%line 140 "/usr/include/sys/signal.h")
(deftype (csym::__sighandler_t) (fn void int))
(%line 150 "/usr/include/sys/signal.h")
(def (union csym::sigval) (decl csym::sigval_int int)
 (decl csym::sigval_ptr (ptr void)))
(def (struct csym::sigevent) (decl csym::sigev_notify int)
 (def (union csym::unnamed1) (decl csym::__sigev_signo int)
  (decl csym::__sigev_notify_kqueue int))
 (decl csym::__sigev_u (union csym::unnamed1))
 (decl csym::sigev_value (union csym::sigval)))
(%line 168 "/usr/include/sys/signal.h")
(%line 185 "/usr/include/sys/signal.h")
(def (struct csym::__siginfo) (decl csym::si_signo int)
 (decl csym::si_errno int) (decl csym::si_code int)
 (decl csym::si_pid csym::__pid_t) (decl csym::si_uid csym::__uid_t)
 (decl csym::si_status int) (decl csym::si_addr (ptr void))
 (decl csym::si_value (union csym::sigval)) (decl csym::si_band long)
 (decl csym::__spare__ (array int 7)))
(deftype csym::siginfo_t (struct csym::__siginfo))
(%line 194 "/usr/include/sys/signal.h")
(%line 211 "/usr/include/sys/signal.h")
(def (struct csym::sigaction)
 (def (union csym::unnamed2) (decl csym::__sa_handler (ptr (fn void int)))
  (decl csym::__sa_sigaction
   (ptr (fn void int (ptr (struct csym::__siginfo)) (ptr void)))))
 (decl csym::__sigaction_u (union csym::unnamed2)) (decl csym::sa_flags int)
 (decl csym::sa_mask csym::sigset_t))
(%line 257 "/usr/include/sys/signal.h")
(deftype csym::sig_t (ptr csym::__sighandler_t))
(deftype (csym::__siginfohandler_t)
 (fn void int (ptr (struct csym::__siginfo)) (ptr void)))
(%line 266 "/usr/include/sys/signal.h")
(def (struct csym::sigaltstack) (decl csym::ss_sp (ptr char))
 (decl csym::ss_size csym::__size_t) (decl csym::ss_flags int))
(deftype csym::stack_t (struct csym::sigaltstack))
(%line 270 "/usr/include/sys/signal.h")
(%line 285 "/usr/include/sys/signal.h")
(def (struct csym::sigvec) (decl csym::sv_handler (ptr csym::__sighandler_t))
 (decl csym::sv_mask int) (decl csym::sv_flags int))
(%line 312 "/usr/include/sys/signal.h")
(def (struct csym::sigstack) (decl csym::ss_sp (ptr char))
 (decl csym::ss_onstack int))
(%line 344 "/usr/include/sys/signal.h")
(decl (csym::signal)
 (fn (ptr csym::__sighandler_t) int (ptr csym::__sighandler_t)))
(%line 55 "posix-out/aio.h")
(def (struct csym::__aiocb_private) (decl csym::status long)
 (decl csym::error long) (decl csym::kernelinfo (ptr void)))
(%line 64 "posix-out/aio.h")
(def (struct csym::aiocb) (decl csym::aio_fildes int)
 (decl csym::aio_offset csym::off_t) (decl csym::aio_buf (ptr (volatile void)))
 (decl csym::aio_nbytes csym::size_t)
 (decl csym::aio_sigevent (struct csym::sigevent))
 (decl csym::aio_lio_opcode int) (decl csym::aio_reqprio int)
 (decl csym::_aiocb_private (struct csym::__aiocb_private)))
(deftype csym::aiocb_t (struct csym::aiocb))
(%line 81 "posix-out/aio.h")
(decl (csym::aio_read) (fn int (ptr (struct csym::aiocb))))
(%line 86 "posix-out/aio.h")
(decl (csym::aio_write) (fn int (ptr (struct csym::aiocb))))
(%line 94 "posix-out/aio.h")
(decl (csym::lio_listio)
 (fn int int (array (const (ptr (struct csym::aiocb)))) int
  (ptr (struct csym::sigevent))))
(%line 101 "posix-out/aio.h")
(decl (csym::aio_error) (fn int (ptr (const (struct csym::aiocb)))))
(%line 109 "posix-out/aio.h")
(decl (csym::aio_return) (fn csym::ssize_t (ptr (struct csym::aiocb))))
(%line 114 "posix-out/aio.h")
(decl (csym::aio_cancel) (fn int int (ptr (struct csym::aiocb))))
(%line 119 "posix-out/aio.h")
(decl (csym::aio_suspend)
 (fn int (array (const (ptr (const (struct csym::aiocb))))) int
  (ptr (const (struct csym::timespec)))))
(decl (csym::aio_waitcomplete)
 (fn int (ptr (ptr (struct csym::aiocb))) (ptr (struct csym::timespec))))
(%line 1 "posix-out/assert.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 62 "posix-out/assert.h")
(decl (csym::__assert)
 (fn void (ptr (const char)) (ptr (const char)) int (ptr (const char))))
(%line 1 "posix-out/complex.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 44 "posix-out/complex.h")
(decl (csym::cabs) (fn double double))
(decl (csym::cabsf) (fn float float))
(decl (csym::cimag) (fn double double))
(decl (csym::cimagf) (fn float float))
(decl (csym::cimagl) (fn long-double long-double))
(decl (csym::conj) (fn double double))
(decl (csym::conjf) (fn float float))
(decl (csym::conjl) (fn long-double long-double))
(decl (csym::creal) (fn double double))
(decl (csym::crealf) (fn float float))
(decl (csym::creall) (fn long-double long-double))
(%line 1 "posix-out/cpio.h")
(%line 1 "posix-out/ctype.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
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
(def (struct csym::unnamed3) (decl csym::__min csym::__rune_t)
 (decl csym::__max csym::__rune_t) (decl csym::__map csym::__rune_t)
 (decl csym::__types (ptr unsigned-long)))
(deftype csym::_@r@UNE@e@NTRY (struct csym::unnamed3))
(def (struct csym::unnamed4) (decl csym::__nranges int)
 (decl csym::__ranges (ptr csym::_@r@UNE@e@NTRY)))
(deftype csym::_@r@UNE@r@ANGE (struct csym::unnamed4))
(def (struct csym::unnamed5) (decl csym::__magic (array char 8))
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
(deftype csym::_@r@UNE@l@OCALE (struct csym::unnamed5))
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
    (bit-or (cast long 3758096384) (cast long 262144)))))
 (if (!= (bit-and csym::_x (cast long 3758096384)) 0)
  (return (>> (bit-and csym::_x (cast long 3758096384)) 30)))
 (return (if-exp (!= (bit-and csym::_x (cast long 262144)) 0) 1 (- 1))))
(%line 52 "posix-out/ctype.h")
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
(%line 1 "posix-out/dirent.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/dirent.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 50 "/usr/include/sys/dirent.h")
(def (struct csym::dirent) (decl csym::d_fileno csym::__uint32_t)
 (decl csym::d_reclen csym::__uint16_t) (decl csym::d_type csym::__uint8_t)
 (decl csym::d_namlen csym::__uint8_t)
 (decl csym::d_name (array char (+ 255 1))))
(%line 61 "/usr/include/sys/dirent.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 62 "posix-out/dirent.h")
(def (struct csym::_dirdesc) (decl csym::dd_fd int) (decl csym::dd_loc long)
 (decl csym::dd_size long) (decl csym::dd_buf (ptr char))
 (decl csym::dd_len int) (decl csym::dd_seek long) (decl csym::dd_rewind long)
 (decl csym::dd_flags int) (decl csym::dd_lock (ptr void))
 (decl csym::dd_td (ptr (struct csym::_telldir))))
(deftype csym::@DIR (struct csym::_dirdesc))
(%line 94 "posix-out/dirent.h")
(decl (csym::__opendir2) (fn (ptr csym::@DIR) (ptr (const char)) int))
(decl (csym::alphasort) (fn int (ptr (const void)) (ptr (const void))))
(decl (csym::getdents) (fn int int (ptr char) int))
(decl (csym::getdirentries) (fn int int (ptr char) int (ptr long)))
(decl (csym::opendir) (fn (ptr csym::@DIR) (ptr (const char))))
(decl (csym::readdir) (fn (ptr (struct csym::dirent)) (ptr csym::@DIR)))
(decl (csym::readdir_r)
 (fn int (ptr csym::@DIR) (ptr (struct csym::dirent))
  (ptr (ptr (struct csym::dirent)))))
(decl (csym::rewinddir) (fn void (ptr csym::@DIR)))
(decl (csym::scandir)
 (fn int (ptr (const char)) (ptr (ptr (ptr (struct csym::dirent))))
  (ptr (fn int (ptr (struct csym::dirent))))
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::seekdir) (fn void (ptr csym::@DIR) long))
(decl (csym::telldir) (fn long (ptr csym::@DIR)))
(decl (csym::closedir) (fn int (ptr csym::@DIR)))
(%line 1 "posix-out/dlfcn.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 77 "posix-out/dlfcn.h")
(def (struct csym::dl_info) (decl csym::dli_fname (ptr (const char)))
 (decl csym::dli_fbase (ptr void)) (decl csym::dli_sname (ptr (const char)))
 (decl csym::dli_saddr (ptr void)))
(deftype csym::@d@L_INFO (struct csym::dl_info))
(%line 93 "posix-out/dlfcn.h")
(def (struct csym::__dlfunc_arg) (decl csym::__dlfunc_dummy int))
(deftype csym::dlfunc_t (ptr (fn void (struct csym::__dlfunc_arg))))
(%line 102 "posix-out/dlfcn.h")
(def (struct csym::dl_serpath) (decl csym::dls_name (ptr char))
 (decl csym::dls_flags unsigned-int))
(deftype csym::@d@L_SERPATH (struct csym::dl_serpath))
(def (struct csym::dl_serinfo) (decl csym::dls_size csym::size_t)
 (decl csym::dls_cnt unsigned-int)
 (decl csym::dls_serpath (array csym::@d@L_SERPATH 1)))
(deftype csym::@d@L_SERINFO (struct csym::dl_serinfo))
(decl (csym::dlclose) (fn int (ptr void)))
(decl (csym::dlerror) (fn (ptr (const char)) void))
(decl (csym::dlopen) (fn (ptr void) (ptr (const char)) int))
(decl (csym::dlsym) (fn (ptr void) (ptr void) (ptr (const char))))
(decl (csym::dladdr) (fn int (ptr (const void)) (ptr csym::@d@L_INFO)))
(decl (csym::dlfunc) (fn csym::dlfunc_t (ptr void) (ptr (const char))))
(decl (csym::dlinfo) (fn int (ptr void) int (ptr void)))
(decl
 (csym::dllockinit csym::_context csym::_lock_create csym::_rlock_acquire
  csym::_wlock_acquire csym::_lock_release csym::_lock_destroy
  csym::_context_destroy)
 (fn void (ptr void) (ptr (fn (ptr void) (ptr void)))
  (ptr (fn void (ptr void))) (ptr (fn void (ptr void)))
  (ptr (fn void (ptr void))) (ptr (fn void (ptr void)))
  (ptr (fn void (ptr void)))))
(%line 1 "posix-out/errno.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 43 "posix-out/errno.h")
(decl (csym::__error) (fn (ptr int) void))
(%line 1 "posix-out/fcntl.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 197 "posix-out/fcntl.h")
(def (struct csym::flock) (decl csym::l_start csym::off_t)
 (decl csym::l_len csym::off_t) (decl csym::l_pid long)
 (decl csym::l_type short) (decl csym::l_whence short))
(%line 219 "posix-out/fcntl.h")
(decl (csym::open) (fn int (ptr (const char)) int va-arg))
(decl (csym::creat) (fn int (ptr (const char)) csym::mode_t))
(decl (csym::fcntl) (fn int int int va-arg))
(decl (csym::flock) (fn int int int))
(%line 1 "posix-out/fenv.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 35 "posix-out/fenv.h")
(def (struct csym::unnamed6) (decl csym::__control csym::__uint32_t)
 (decl csym::__status csym::__uint32_t) (decl csym::__tag csym::__uint32_t)
 (decl csym::__other (array char 16)))
(deftype csym::fenv_t (struct csym::unnamed6))
(deftype csym::fexcept_t csym::__uint16_t)
(%line 62 "posix-out/fenv.h")
(extern-decl csym::__fe_dfl_env (const csym::fenv_t))
(static-def (csym::feclearexcept csym::__excepts) (fn int int)
 (decl csym::__env csym::fenv_t)
 (if (== csym::__excepts (bit-or 4 2 32 1 8 16)) (begin)
  (begin common-lisp:list common-lisp:nil
   (bit-and= (fref csym::__env csym::__status) (bit-not csym::__excepts))))
 (return 0))
(static-def (csym::fegetexceptflag csym::__flagp csym::__excepts)
 (fn int (ptr csym::fexcept_t) int) (decl csym::__status int) common-lisp:list
 common-lisp:nil
 (= (mref csym::__flagp) (bit-and csym::__status csym::__excepts)) (return 0))
(static-def (csym::fesetexceptflag csym::__flagp csym::__excepts)
 (fn int (ptr (const csym::fexcept_t)) int) (decl csym::__env csym::fenv_t)
 common-lisp:list common-lisp:nil
 (bit-and= (fref csym::__env csym::__status) (bit-not csym::__excepts))
 (bit-or= (fref csym::__env csym::__status)
  (bit-and (mref csym::__flagp) csym::__excepts))
 (return 0))
(static-def (csym::feraiseexcept csym::__excepts) (fn int int)
 (def csym::__ex csym::fexcept_t csym::__excepts)
 (csym::fesetexceptflag (ptr csym::__ex) csym::__excepts) (return 0))
(static-def (csym::fetestexcept csym::__excepts) (fn int int)
 (decl csym::__status int) common-lisp:list common-lisp:nil
 (return (bit-and csym::__status csym::__excepts)))
(static-def (csym::fegetround) (fn int void) (decl csym::__control int)
 common-lisp:list common-lisp:nil
 (return (bit-and csym::__control (bit-or 0 1024 2048 3072))))
(static-def (csym::fesetround csym::__round) (fn int int)
 (decl csym::__control int)
 (if (bit-and csym::__round (bit-not (bit-or 0 1024 2048 3072)))
  (return (- 1)))
 common-lisp:list common-lisp:nil
 (bit-and= csym::__control (bit-not (bit-or 0 1024 2048 3072)))
 (bit-or= csym::__control csym::__round) (return 0))
(static-def (csym::fegetenv csym::__envp) (fn int (ptr csym::fenv_t))
 (decl csym::__control int) (%line 164 "posix-out/fenv.h") common-lisp:list
 common-lisp:nil common-lisp:list common-lisp:nil (return 0))
(static-def (csym::feholdexcept csym::__envp) (fn int (ptr csym::fenv_t))
 common-lisp:list common-lisp:nil (return 0))
(static-def (csym::fesetenv csym::__envp) (fn int (ptr (const csym::fenv_t)))
 (return 0))
(static-def (csym::feupdateenv csym::__envp)
 (fn int (ptr (const csym::fenv_t))) (decl csym::__status int) common-lisp:list
 common-lisp:nil
 (csym::feraiseexcept (bit-and csym::__status (bit-or 4 2 32 1 8 16)))
 (return 0))
(static-def (csym::fesetmask csym::__mask) (fn int int)
 (decl csym::__control int) common-lisp:list common-lisp:nil
 (= csym::__mask
  (bit-and (bit-or csym::__control (bit-or 4 2 32 1 8 16))
   (bit-not csym::__mask)))
 (return (bit-and (bit-not csym::__control) (bit-or 4 2 32 1 8 16))))
(static-def (csym::fegetmask) (fn int void) (decl csym::__control int)
 common-lisp:list common-lisp:nil
 (return (bit-and (bit-not csym::__control) (bit-or 4 2 32 1 8 16))))
(%line 1 "posix-out/float.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 38 "posix-out/float.h")
(extern-decl (csym::__flt_rounds) (fn int void))
(%line 1 "posix-out/fmtmsg.h")
(%line 70 "posix-out/fmtmsg.h")
(decl (csym::fmtmsg)
 (fn int long (ptr (const char)) int (ptr (const char)) (ptr (const char))
  (ptr (const char))))
(%line 1 "posix-out/fnmatch.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 59 "posix-out/fnmatch.h")
(decl (csym::fnmatch) (fn int (ptr (const char)) (ptr (const char)) int))
(%line 1 "posix-out/ftw.h")
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/stat.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/time.h")
(%line 104 "/usr/include/sys/stat.h")
(def (struct csym::ostat) (decl csym::st_dev csym::__uint16_t)
 (decl csym::st_ino csym::ino_t) (decl csym::st_mode csym::mode_t)
 (decl csym::st_nlink csym::nlink_t) (decl csym::st_uid csym::__uint16_t)
 (decl csym::st_gid csym::__uint16_t) (decl csym::st_rdev csym::__uint16_t)
 (decl csym::st_size csym::__int32_t)
 (decl csym::st_atimespec (struct csym::timespec))
 (decl csym::st_mtimespec (struct csym::timespec))
 (decl csym::st_ctimespec (struct csym::timespec))
 (decl csym::st_blksize csym::__int32_t) (decl csym::st_blocks csym::__int32_t)
 (decl csym::st_flags csym::fflags_t) (decl csym::st_gen csym::__uint32_t))
(def (struct csym::stat) (decl csym::st_dev csym::__dev_t)
 (decl csym::st_ino csym::ino_t) (decl csym::st_mode csym::mode_t)
 (decl csym::st_nlink csym::nlink_t) (decl csym::st_uid csym::uid_t)
 (decl csym::st_gid csym::gid_t) (decl csym::st_rdev csym::__dev_t)
 (decl csym::st_atimespec (struct csym::timespec))
 (decl csym::st_mtimespec (struct csym::timespec))
 (decl csym::st_ctimespec (struct csym::timespec))
 (decl csym::st_size csym::off_t) (decl csym::st_blocks csym::__int64_t)
 (decl csym::st_blksize csym::__uint32_t) (decl csym::st_flags csym::fflags_t)
 (decl csym::st_gen csym::__uint32_t) (decl csym::st_lspare csym::__int32_t)
 (decl csym::st_birthtimespec (struct csym::timespec))
 (decl csym::xxx unsigned-int) :bit
 (* (/ 8 2) (- 16 (cast int (sizeof (struct csym::timespec)))))
 (decl csym::yyy unsigned-int) :bit
 (* (/ 8 2) (- 16 (cast int (sizeof (struct csym::timespec))))))
(%line 143 "/usr/include/sys/stat.h")
(%line 159 "/usr/include/sys/stat.h")
(%line 167 "/usr/include/sys/stat.h")
(def (struct csym::nstat) (decl csym::st_dev csym::__dev_t)
 (decl csym::st_ino csym::ino_t) (decl csym::st_mode csym::__uint32_t)
 (decl csym::st_nlink csym::__uint32_t) (decl csym::st_uid csym::uid_t)
 (decl csym::st_gid csym::gid_t) (decl csym::st_rdev csym::__dev_t)
 (decl csym::st_atimespec (struct csym::timespec))
 (decl csym::st_mtimespec (struct csym::timespec))
 (decl csym::st_ctimespec (struct csym::timespec))
 (decl csym::st_size csym::off_t) (decl csym::st_blocks csym::__int64_t)
 (decl csym::st_blksize csym::__uint32_t) (decl csym::st_flags csym::fflags_t)
 (decl csym::st_gen csym::__uint32_t)
 (decl csym::st_birthtimespec (struct csym::timespec))
 (decl csym::xxx unsigned-int) :bit
 (* (/ 8 2) (- 16 (cast int (sizeof (struct csym::timespec)))))
 (decl csym::yyy unsigned-int) :bit
 (* (/ 8 2) (- 16 (cast int (sizeof (struct csym::timespec))))))
(%line 190 "/usr/include/sys/stat.h")
(%line 300 "/usr/include/sys/stat.h")
(decl (csym::chflags) (fn int (ptr (const char)) unsigned-long))
(decl (csym::chmod) (fn int (ptr (const char)) csym::mode_t))
(decl (csym::fchflags) (fn int int unsigned-long))
(decl (csym::fchmod) (fn int int csym::mode_t))
(decl (csym::fstat) (fn int int (ptr (struct csym::stat))))
(decl (csym::lchflags) (fn int (ptr (const char)) int))
(decl (csym::lchmod) (fn int (ptr (const char)) csym::mode_t))
(decl (csym::lstat) (fn int (ptr (const char)) (ptr (struct csym::stat))))
(decl (csym::mkdir) (fn int (ptr (const char)) csym::mode_t))
(decl (csym::mkfifo) (fn int (ptr (const char)) csym::mode_t))
(decl (csym::stat) (fn int (ptr (const char)) (ptr (struct csym::stat))))
(decl (csym::umask) (fn csym::mode_t csym::mode_t))
(%line 51 "posix-out/ftw.h")
(def (struct csym::@FTW) (decl csym::base int) (decl csym::level int))
(decl (csym::ftw)
 (fn int (ptr (const char))
  (ptr (fn int (ptr (const char)) (ptr (const (struct csym::stat))) int)) int))
(decl (csym::nftw)
 (fn int (ptr (const char))
  (ptr
   (fn int (ptr (const char)) (ptr (const (struct csym::stat))) int
    (ptr (struct csym::@FTW))))
  int int))
(%line 1 "posix-out/glob.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 45 "posix-out/glob.h")
(def (struct csym::unnamed7) (decl csym::gl_pathc int)
 (decl csym::gl_matchc int) (decl csym::gl_offs int) (decl csym::gl_flags int)
 (decl csym::gl_pathv (ptr (ptr char)))
 (decl csym::gl_errfunc (ptr (fn int (ptr (const char)) int)))
 (decl csym::gl_closedir (ptr (fn void (ptr void))))
 (decl csym::gl_readdir (ptr (fn (ptr (struct csym::dirent)) (ptr void))))
 (decl csym::gl_opendir (ptr (fn (ptr void) (ptr (const char)))))
 (decl csym::gl_lstat
  (ptr (fn int (ptr (const char)) (ptr (struct csym::stat)))))
 (decl csym::gl_stat
  (ptr (fn int (ptr (const char)) (ptr (struct csym::stat))))))
(deftype csym::glob_t (struct csym::unnamed7))
(%line 60 "posix-out/glob.h")
(%line 98 "posix-out/glob.h")
(decl (csym::glob)
 (fn int (ptr (const char)) int (ptr (fn int (ptr (const char)) int))
  (ptr csym::glob_t)))
(decl (csym::globfree) (fn void (ptr csym::glob_t)))
(%line 1 "posix-out/grp.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 60 "posix-out/grp.h")
(def (struct csym::group) (decl csym::gr_name (ptr char))
 (decl csym::gr_passwd (ptr char)) (decl csym::gr_gid csym::gid_t)
 (decl csym::gr_mem (ptr (ptr char))))
(decl (csym::endgrent) (fn void void))
(decl (csym::getgrent) (fn (ptr (struct csym::group)) void))
(decl (csym::getgrgid) (fn (ptr (struct csym::group)) csym::gid_t))
(decl (csym::getgrnam) (fn (ptr (struct csym::group)) (ptr (const char))))
(decl (csym::group_from_gid) (fn (ptr (const char)) csym::gid_t int))
(decl (csym::setgrent) (fn int void))
(decl (csym::getgrgid_r)
 (fn int csym::gid_t (ptr (struct csym::group)) (ptr char) csym::size_t
  (ptr (ptr (struct csym::group)))))
(decl (csym::getgrnam_r)
 (fn int (ptr (const char)) (ptr (struct csym::group)) (ptr char) csym::size_t
  (ptr (ptr (struct csym::group)))))
(decl (csym::getgrent_r)
 (fn int (ptr (struct csym::group)) (ptr char) csym::size_t
  (ptr (ptr (struct csym::group)))))
(decl (csym::setgroupent) (fn int int))
(%line 1 "posix-out/if.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/time.h")
(%line 51 "posix-out/if.h")
(%line 69 "posix-out/if.h")
(def (struct csym::if_clonereq) (decl csym::ifcr_total int)
 (decl csym::ifcr_count int) (decl csym::ifcr_buffer (ptr char)))
(%line 79 "posix-out/if.h")
(def (struct csym::if_data) (decl csym::ifi_type csym::u_char)
 (decl csym::ifi_physical csym::u_char) (decl csym::ifi_addrlen csym::u_char)
 (decl csym::ifi_hdrlen csym::u_char) (decl csym::ifi_link_state csym::u_char)
 (decl csym::ifi_recvquota csym::u_char)
 (decl csym::ifi_xmitquota csym::u_char) (decl csym::ifi_datalen csym::u_char)
 (decl csym::ifi_mtu csym::u_long) (decl csym::ifi_metric csym::u_long)
 (decl csym::ifi_baudrate csym::u_long) (decl csym::ifi_ipackets csym::u_long)
 (decl csym::ifi_ierrors csym::u_long) (decl csym::ifi_opackets csym::u_long)
 (decl csym::ifi_oerrors csym::u_long) (decl csym::ifi_collisions csym::u_long)
 (decl csym::ifi_ibytes csym::u_long) (decl csym::ifi_obytes csym::u_long)
 (decl csym::ifi_imcasts csym::u_long) (decl csym::ifi_omcasts csym::u_long)
 (decl csym::ifi_iqdrops csym::u_long) (decl csym::ifi_noproto csym::u_long)
 (decl csym::ifi_hwassist csym::u_long) (decl csym::ifi_epoch double)
 (decl csym::ifi_lastchange (struct csym::timeval)))
(%line 109 "posix-out/if.h")
(%line 174 "posix-out/if.h")
(def (struct csym::if_msghdr) (decl csym::ifm_msglen csym::u_short)
 (decl csym::ifm_version csym::u_char) (decl csym::ifm_type csym::u_char)
 (decl csym::ifm_addrs int) (decl csym::ifm_flags int)
 (decl csym::ifm_index csym::u_short)
 (decl csym::ifm_data (struct csym::if_data)))
(%line 188 "posix-out/if.h")
(def (struct csym::ifa_msghdr) (decl csym::ifam_msglen csym::u_short)
 (decl csym::ifam_version csym::u_char) (decl csym::ifam_type csym::u_char)
 (decl csym::ifam_addrs int) (decl csym::ifam_flags int)
 (decl csym::ifam_index csym::u_short) (decl csym::ifam_metric int))
(%line 202 "posix-out/if.h")
(def (struct csym::ifma_msghdr) (decl csym::ifmam_msglen csym::u_short)
 (decl csym::ifmam_version csym::u_char) (decl csym::ifmam_type csym::u_char)
 (decl csym::ifmam_addrs int) (decl csym::ifmam_flags int)
 (decl csym::ifmam_index csym::u_short))
(%line 214 "posix-out/if.h")
(def (struct csym::if_announcemsghdr) (decl csym::ifan_msglen csym::u_short)
 (decl csym::ifan_version csym::u_char) (decl csym::ifan_type csym::u_char)
 (decl csym::ifan_index csym::u_short) (decl csym::ifan_name (array char 16))
 (decl csym::ifan_what csym::u_short))
(%line 232 "posix-out/if.h")
(def (struct csym::ifreq) (decl csym::ifr_name (array char 16))
 (def (union csym::unnamed8) (decl csym::ifru_addr (struct csym::sockaddr))
  (decl csym::ifru_dstaddr (struct csym::sockaddr))
  (decl csym::ifru_broadaddr (struct csym::sockaddr))
  (decl csym::ifru_flags (array short 2)) (decl csym::ifru_index short)
  (decl csym::ifru_metric int) (decl csym::ifru_mtu int)
  (decl csym::ifru_phys int) (decl csym::ifru_media int)
  (decl csym::ifru_data csym::caddr_t) (decl csym::ifru_cap (array int 2)))
 (decl csym::ifr_ifru (union csym::unnamed8)))
(%line 260 "posix-out/if.h")
(%line 267 "posix-out/if.h")
(def (struct csym::ifaliasreq) (decl csym::ifra_name (array char 16))
 (decl csym::ifra_addr (struct csym::sockaddr))
 (decl csym::ifra_broadaddr (struct csym::sockaddr))
 (decl csym::ifra_mask (struct csym::sockaddr)))
(def (struct csym::ifmediareq) (decl csym::ifm_name (array char 16))
 (decl csym::ifm_current int) (decl csym::ifm_mask int)
 (decl csym::ifm_status int) (decl csym::ifm_active int)
 (decl csym::ifm_count int) (decl csym::ifm_ulist (ptr int)))
(%line 292 "posix-out/if.h")
(def (struct csym::ifstat) (decl csym::ifs_name (array char 16))
 (decl csym::ascii (array char (+ 800 1))))
(%line 303 "posix-out/if.h")
(def (struct csym::ifconf) (decl csym::ifc_len int)
 (def (union csym::unnamed9) (decl csym::ifcu_buf csym::caddr_t)
  (decl csym::ifcu_req (ptr (struct csym::ifreq))))
 (decl csym::ifc_ifcu (union csym::unnamed9)))
(%line 317 "posix-out/if.h")
(def (struct csym::if_laddrreq) (decl csym::iflr_name (array char 16))
 (decl csym::flags csym::u_int) (decl csym::prefixlen csym::u_int)
 (decl csym::addr (struct csym::sockaddr_storage))
 (decl csym::dstaddr (struct csym::sockaddr_storage)))
(%line 336 "posix-out/if.h")
(def (struct csym::if_nameindex) (decl csym::if_index unsigned-int)
 (decl csym::if_name (ptr char)))
(decl (csym::if_freenameindex) (fn void (ptr (struct csym::if_nameindex))))
(decl (csym::if_indextoname) (fn (ptr char) unsigned-int (ptr char)))
(decl (csym::if_nameindex) (fn (ptr (struct csym::if_nameindex)) void))
(decl (csym::if_nametoindex) (fn unsigned-int (ptr (const char))))
(%line 1 "posix-out/in.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/machine/endian.h")
(%line 75 "posix-out/in.h")
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
(%line 421 "posix-out/in.h")
(def (struct csym::ip_mreq) (decl csym::imr_multiaddr (struct csym::in_addr))
 (decl csym::imr_interface (struct csym::in_addr)))
(%line 1 "/usr/include/netinet6/in6.h")
(%line 117 "/usr/include/netinet6/in6.h")
(def (struct csym::in6_addr)
 (def (union csym::unnamed10) (decl csym::__u6_addr8 (array csym::uint8_t 16))
  (decl csym::__u6_addr16 (array csym::uint16_t 8))
  (decl csym::__u6_addr32 (array csym::uint32_t 4)))
 (decl csym::__u6_addr (union csym::unnamed10)))
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
(%line 641 "/usr/include/netinet6/in6.h")
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
(%line 1 "posix-out/inet.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/machine/endian.h")
(%line 131 "posix-out/inet.h")
(%line 140 "posix-out/inet.h")
(decl (csym::__inet_addr) (fn csym::in_addr_t (ptr (const char))))
(decl (csym::__inet_ntoa) (fn (ptr char) (struct csym::in_addr)))
(decl (csym::__inet_ntop)
 (fn (ptr (const char)) int (ptr (const void)) (ptr char) csym::socklen_t))
(decl (csym::__inet_pton) (fn int int (ptr (const char)) (ptr void)))
(decl (csym::ascii2addr) (fn int int (ptr (const char)) (ptr void)))
(decl (csym::addr2ascii) (fn (ptr char) int (ptr (const void)) int (ptr char)))
(decl (csym::__inet_aton)
 (fn int (ptr (const char)) (ptr (struct csym::in_addr))))
(decl (csym::__inet_lnaof) (fn csym::in_addr_t (struct csym::in_addr)))
(decl (csym::__inet_makeaddr)
 (fn (struct csym::in_addr) csym::in_addr_t csym::in_addr_t))
(decl (csym::__inet_neta)
 (fn (ptr char) csym::in_addr_t (ptr char) csym::size_t))
(decl (csym::__inet_netof) (fn csym::in_addr_t (struct csym::in_addr)))
(decl (csym::__inet_network) (fn csym::in_addr_t (ptr (const char))))
(decl (csym::__inet_net_ntop)
 (fn (ptr char) int (ptr (const void)) int (ptr char) csym::size_t))
(decl (csym::__inet_net_pton)
 (fn int int (ptr (const char)) (ptr void) csym::size_t))
(decl (csym::__inet_nsap_addr)
 (fn unsigned-int (ptr (const char)) (ptr unsigned-char) int))
(decl (csym::__inet_nsap_ntoa)
 (fn (ptr char) int (ptr (const unsigned-char)) (ptr char)))
(%line 1 "posix-out/inttypes.h")
(%line 1 "/usr/include/machine/_inttypes.h")
(%line 1 "/usr/include/sys/stdint.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/machine/_stdint.h")
(%line 77 "/usr/include/sys/stdint.h")
(deftype csym::int_least8_t csym::__int_least8_t)
(deftype csym::int_least16_t csym::__int_least16_t)
(deftype csym::int_least32_t csym::__int_least32_t)
(deftype csym::int_least64_t csym::__int_least64_t)
(deftype csym::uint_least8_t csym::__uint_least8_t)
(deftype csym::uint_least16_t csym::__uint_least16_t)
(deftype csym::uint_least32_t csym::__uint_least32_t)
(deftype csym::uint_least64_t csym::__uint_least64_t)
(deftype csym::int_fast8_t csym::__int_fast8_t)
(deftype csym::int_fast16_t csym::__int_fast16_t)
(deftype csym::int_fast32_t csym::__int_fast32_t)
(deftype csym::int_fast64_t csym::__int_fast64_t)
(deftype csym::uint_fast8_t csym::__uint_fast8_t)
(deftype csym::uint_fast16_t csym::__uint_fast16_t)
(deftype csym::uint_fast32_t csym::__uint_fast32_t)
(deftype csym::uint_fast64_t csym::__uint_fast64_t)
(deftype csym::intmax_t csym::__intmax_t)
(deftype csym::uintmax_t csym::__uintmax_t)
(%line 35 "posix-out/inttypes.h")
(def (struct csym::unnamed11) (decl csym::quot csym::intmax_t)
 (decl csym::rem csym::intmax_t))
(deftype csym::imaxdiv_t (struct csym::unnamed11))
(decl (csym::imaxabs) (fn csym::intmax_t csym::intmax_t))
(decl (csym::imaxdiv) (fn csym::imaxdiv_t csym::intmax_t csym::intmax_t))
(decl (csym::strtoimax)
 (fn csym::intmax_t (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::strtoumax)
 (fn csym::uintmax_t (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::wcstoimax)
 (fn csym::intmax_t (ptr (const csym::__wchar_t)) (ptr (ptr csym::__wchar_t))
  int))
(decl (csym::wcstoumax)
 (fn csym::uintmax_t (ptr (const csym::__wchar_t)) (ptr (ptr csym::__wchar_t))
  int))
(%line 1 "posix-out/ipc.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 75 "posix-out/ipc.h")
(def (struct csym::ipc_perm) (decl csym::cuid unsigned-short)
 (decl csym::cgid unsigned-short) (decl csym::uid unsigned-short)
 (decl csym::gid unsigned-short) (decl csym::mode unsigned-short)
 (decl csym::seq unsigned-short) (decl csym::key csym::key_t))
(%line 122 "posix-out/ipc.h")
(decl (csym::ftok) (fn csym::key_t (ptr (const char)) int))
(%line 1 "posix-out/iso646.h")
(%line 1 "posix-out/langinfo.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 36 "posix-out/langinfo.h")
(deftype csym::nl_item csym::__nl_item)
(%line 117 "posix-out/langinfo.h")
(decl (csym::nl_langinfo) (fn (ptr char) csym::nl_item))
(%line 1 "posix-out/libgen.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 36 "posix-out/libgen.h")
(decl (csym::basename) (fn (ptr char) (ptr (const char))))
(decl (csym::dirname) (fn (ptr char) (ptr (const char))))
(%line 47 "posix-out/libgen.h")
(%line 1 "posix-out/limits.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/limits.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/machine/_limits.h")
(%line 1 "/usr/include/sys/syslimits.h")
(%line 1 "posix-out/locale.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 42 "posix-out/locale.h")
(def (struct csym::lconv) (decl csym::decimal_point (ptr char))
 (decl csym::thousands_sep (ptr char)) (decl csym::grouping (ptr char))
 (decl csym::int_curr_symbol (ptr char))
 (decl csym::currency_symbol (ptr char))
 (decl csym::mon_decimal_point (ptr char))
 (decl csym::mon_thousands_sep (ptr char)) (decl csym::mon_grouping (ptr char))
 (decl csym::positive_sign (ptr char)) (decl csym::negative_sign (ptr char))
 (decl csym::int_frac_digits char) (decl csym::frac_digits char)
 (decl csym::p_cs_precedes char) (decl csym::p_sep_by_space char)
 (decl csym::n_cs_precedes char) (decl csym::n_sep_by_space char)
 (decl csym::p_sign_posn char) (decl csym::n_sign_posn char)
 (decl csym::int_p_cs_precedes char) (decl csym::int_n_cs_precedes char)
 (decl csym::int_p_sep_by_space char) (decl csym::int_n_sep_by_space char)
 (decl csym::int_p_sign_posn char) (decl csym::int_n_sign_posn char))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 81 "posix-out/locale.h")
(decl (csym::localeconv) (fn (ptr (struct csym::lconv)) void))
(decl (csym::setlocale) (fn (ptr char) int (ptr (const char))))
(%line 1 "posix-out/math.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/machine/_limits.h")
(%line 27 "posix-out/math.h")
(extern-decl csym::__infinity
 (const
  (union csym::__infinity_un (decl csym::__uc (array unsigned-char 8))
   (decl csym::__ud double))))
(extern-decl csym::__nan
 (const
  (union csym::__nan_un (decl csym::__uc (array unsigned-char (sizeof float)))
   (decl csym::__uf float))))
(%line 121 "posix-out/math.h")
(deftype csym::double_t csym::__double_t)
(deftype csym::float_t csym::__float_t)
(%line 144 "posix-out/math.h")
(extern-decl csym::signgam int)
(def (enum csym::fdversion)
 ((csym::fdlibm_ieee (- 1)) csym::fdlibm_svid csym::fdlibm_xopen
  csym::fdlibm_posix))
(%line 160 "posix-out/math.h")
(def (enum csym::fdversion) common-lisp:nil)
(extern-decl csym::_fdlib_version (enum csym::fdversion))
(%line 170 "posix-out/math.h")
(def (struct csym::exception) (decl csym::type int)
 (decl csym::name (ptr char)) (decl csym::arg1 double) (decl csym::arg2 double)
 (decl csym::retval double))
(%line 204 "posix-out/math.h")
(%line 208 "posix-out/math.h")
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
(%line 256 "posix-out/math.h")
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
(%line 306 "posix-out/math.h")
(decl (csym::drem) (fn double double double))
(decl (csym::finite) (fn int double))
(decl (csym::isnanf) (fn int float))
(%line 314 "posix-out/math.h")
(decl (csym::gamma_r) (fn double double (ptr int)))
(decl (csym::lgamma_r) (fn double double (ptr int)))
(%line 320 "posix-out/math.h")
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
(%line 388 "posix-out/math.h")
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
(%line 404 "posix-out/math.h")
(decl (csym::gammaf_r) (fn float float (ptr int)))
(decl (csym::lgammaf_r) (fn float float (ptr int)))
(%line 410 "posix-out/math.h")
(decl (csym::significandf) (fn float float))
(%line 428 "posix-out/math.h")
(decl (csym::copysignl) (fn long-double long-double long-double))
(%line 438 "posix-out/math.h")
(decl (csym::fabsl) (fn long-double long-double))
(decl (csym::fdiml) (fn long-double long-double long-double))
(%line 444 "posix-out/math.h")
(decl (csym::fmaxl) (fn long-double long-double long-double))
(decl (csym::fminl) (fn long-double long-double long-double))
(%line 486 "posix-out/math.h")
(%line 1 "posix-out/mman.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 164 "posix-out/mman.h")
(%line 170 "posix-out/mman.h")
(decl (csym::madvise) (fn int (ptr void) csym::size_t int))
(decl (csym::mincore) (fn int (ptr (const void)) csym::size_t (ptr char)))
(decl (csym::minherit) (fn int (ptr void) csym::size_t int))
(decl (csym::mlock) (fn int (ptr (const void)) csym::size_t))
(%line 179 "posix-out/mman.h")
(decl (csym::mprotect) (fn int (ptr (const void)) csym::size_t int))
(decl (csym::msync) (fn int (ptr void) csym::size_t int))
(decl (csym::munlock) (fn int (ptr (const void)) csym::size_t))
(decl (csym::munmap) (fn int (ptr void) csym::size_t))
(decl (csym::posix_madvise) (fn int (ptr void) csym::size_t int))
(decl (csym::mlockall) (fn int int))
(decl (csym::munlockall) (fn int void))
(decl (csym::shm_open) (fn int (ptr (const char)) int csym::mode_t))
(decl (csym::shm_unlink) (fn int (ptr (const char))))
(%line 1 "posix-out/monetary.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 45 "posix-out/monetary.h")
(decl (csym::strfmon)
 (fn csym::ssize_t (ptr char) csym::size_t (ptr (const char)) va-arg))
(%line 1 "posix-out/msg.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/ipc.h")
(%line 37 "posix-out/msg.h")
(deftype csym::msglen_t unsigned-long)
(deftype csym::msgqnum_t unsigned-long)
(%line 66 "posix-out/msg.h")
(def (struct csym::msqid_ds) (decl csym::msg_perm (struct csym::ipc_perm))
 (decl csym::msg_first (ptr (struct csym::msg)))
 (decl csym::msg_last (ptr (struct csym::msg)))
 (decl csym::msg_cbytes csym::msglen_t) (decl csym::msg_qnum csym::msgqnum_t)
 (decl csym::msg_qbytes csym::msglen_t) (decl csym::msg_lspid long)
 (decl csym::msg_lrpid long) (decl csym::msg_stime double)
 (decl csym::msg_pad1 long) (decl csym::msg_rtime double)
 (decl csym::msg_pad2 long) (decl csym::msg_ctime double)
 (decl csym::msg_pad3 long) (decl csym::msg_pad4 (array long 4)))
(%line 95 "posix-out/msg.h")
(def (struct csym::mymsg) (decl csym::mtype long)
 (decl csym::mtext (array char 1)))
(%line 125 "posix-out/msg.h")
(decl (csym::msgctl) (fn int int int (ptr (struct csym::msqid_ds))))
(decl (csym::msgget) (fn int csym::key_t int))
(decl (csym::msgrcv) (fn int int (ptr void) csym::size_t long int))
(decl (csym::msgsnd) (fn int int (ptr (const void)) csym::size_t int))
(decl (csym::msgsys) (fn int int va-arg))
(%line 1 "posix-out/ndbm.h")
(%line 1 "/usr/include/db.h")
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/limits.h")
(%line 50 "/usr/include/db.h")
(deftype csym::pgno_t csym::u_int32_t)
(deftype csym::indx_t csym::u_int16_t)
(deftype csym::recno_t csym::u_int32_t)
(def (struct csym::unnamed12) (decl csym::data (ptr void))
 (decl csym::size csym::size_t))
(deftype csym::@DBT (struct csym::unnamed12))
(%line 75 "/usr/include/db.h")
(def (enum csym::unnamed13) (csym::@DB_BTREE csym::@DB_HASH csym::@DB_RECNO))
(deftype csym::@DBTYPE (enum csym::unnamed13))
(%line 101 "/usr/include/db.h")
(def (struct csym::__db) (decl csym::type csym::@DBTYPE)
 (decl csym::close (ptr (fn int (ptr (struct csym::__db)))))
 (decl csym::del
  (ptr
   (fn int (ptr (const (struct csym::__db))) (ptr (const csym::@DBT))
    csym::u_int)))
 (decl csym::get
  (ptr
   (fn int (ptr (const (struct csym::__db))) (ptr (const csym::@DBT))
    (ptr csym::@DBT) csym::u_int)))
 (decl csym::put
  (ptr
   (fn int (ptr (const (struct csym::__db))) (ptr csym::@DBT)
    (ptr (const csym::@DBT)) csym::u_int)))
 (decl csym::seq
  (ptr
   (fn int (ptr (const (struct csym::__db))) (ptr csym::@DBT) (ptr csym::@DBT)
    csym::u_int)))
 (decl csym::sync (ptr (fn int (ptr (const (struct csym::__db))) csym::u_int)))
 (decl csym::internal (ptr void))
 (decl csym::fd (ptr (fn int (ptr (const (struct csym::__db)))))))
(deftype csym::@DB (struct csym::__db))
(def (struct csym::unnamed14) (decl csym::flags csym::u_long)
 (decl csym::cachesize csym::u_int) (decl csym::maxkeypage int)
 (decl csym::minkeypage int) (decl csym::psize csym::u_int)
 (decl csym::compare
  (ptr (fn int (ptr (const csym::@DBT)) (ptr (const csym::@DBT)))))
 (decl csym::prefix
  (ptr (fn csym::size_t (ptr (const csym::@DBT)) (ptr (const csym::@DBT)))))
 (decl csym::lorder int))
(deftype csym::@BTREEINFO (struct csym::unnamed14))
(def (struct csym::unnamed15) (decl csym::bsize csym::u_int)
 (decl csym::ffactor csym::u_int) (decl csym::nelem csym::u_int)
 (decl csym::cachesize csym::u_int)
 (decl csym::hash (ptr (fn csym::u_int32_t (ptr (const void)) csym::size_t)))
 (decl csym::lorder int))
(deftype csym::@HASHINFO (struct csym::unnamed15))
(def (struct csym::unnamed16) (decl csym::flags csym::u_long)
 (decl csym::cachesize csym::u_int) (decl csym::psize csym::u_int)
 (decl csym::lorder int) (decl csym::reclen csym::size_t)
 (decl csym::bval csym::u_char) (decl csym::bfname (ptr char)))
(deftype csym::@RECNOINFO (struct csym::unnamed16))
(%line 209 "/usr/include/db.h")
(decl (csym::dbopen)
 (fn (ptr csym::@DB) (ptr (const char)) int int csym::@DBTYPE
  (ptr (const void))))
(%line 218 "/usr/include/db.h")
(%line 58 "posix-out/ndbm.h")
(def (struct csym::unnamed17) (decl csym::dptr (ptr char))
 (decl csym::dsize int))
(deftype csym::datum (struct csym::unnamed17))
(deftype csym::@DBM csym::@DB)
(decl (csym::dbm_clearerr) (fn int (ptr csym::@DBM)))
(decl (csym::dbm_close) (fn void (ptr csym::@DBM)))
(decl (csym::dbm_delete) (fn int (ptr csym::@DBM) csym::datum))
(decl (csym::dbm_error) (fn int (ptr csym::@DBM)))
(decl (csym::dbm_fetch) (fn csym::datum (ptr csym::@DBM) csym::datum))
(decl (csym::dbm_firstkey) (fn csym::datum (ptr csym::@DBM)))
(decl (csym::dbm_forder) (fn long (ptr csym::@DBM) csym::datum))
(decl (csym::dbm_nextkey) (fn csym::datum (ptr csym::@DBM)))
(decl (csym::dbm_open) (fn (ptr csym::@DBM) (ptr (const char)) int int))
(decl (csym::dbm_store) (fn int (ptr csym::@DBM) csym::datum csym::datum int))
(decl (csym::dbm_dirfno) (fn int (ptr csym::@DBM)))
(%line 1 "posix-out/netdb.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 92 "posix-out/netdb.h")
(def (struct csym::hostent) (decl csym::h_name (ptr char))
 (decl csym::h_aliases (ptr (ptr char))) (decl csym::h_addrtype int)
 (decl csym::h_length int) (decl csym::h_addr_list (ptr (ptr char))))
(%line 105 "posix-out/netdb.h")
(def (struct csym::netent) (decl csym::n_name (ptr char))
 (decl csym::n_aliases (ptr (ptr char))) (decl csym::n_addrtype int)
 (decl csym::n_net unsigned-long))
(def (struct csym::servent) (decl csym::s_name (ptr char))
 (decl csym::s_aliases (ptr (ptr char))) (decl csym::s_port int)
 (decl csym::s_proto (ptr char)))
(def (struct csym::protoent) (decl csym::p_name (ptr char))
 (decl csym::p_aliases (ptr (ptr char))) (decl csym::p_proto int))
(def (struct csym::addrinfo) (decl csym::ai_flags int)
 (decl csym::ai_family int) (decl csym::ai_socktype int)
 (decl csym::ai_protocol int) (decl csym::ai_addrlen csym::size_t)
 (decl csym::ai_canonname (ptr char))
 (decl csym::ai_addr (ptr (struct csym::sockaddr)))
 (decl csym::ai_next (ptr (struct csym::addrinfo))))
(%line 213 "posix-out/netdb.h")
(decl (csym::endhostent) (fn void void))
(decl (csym::endnetent) (fn void void))
(decl (csym::endnetgrent) (fn void void))
(decl (csym::endprotoent) (fn void void))
(decl (csym::endservent) (fn void void))
(decl (csym::freehostent) (fn void (ptr (struct csym::hostent))))
(decl (csym::gethostbyaddr)
 (fn (ptr (struct csym::hostent)) (ptr (const char)) int int))
(decl (csym::gethostbyname)
 (fn (ptr (struct csym::hostent)) (ptr (const char))))
(decl (csym::gethostbyname2)
 (fn (ptr (struct csym::hostent)) (ptr (const char)) int))
(decl (csym::gethostent) (fn (ptr (struct csym::hostent)) void))
(decl (csym::getipnodebyaddr)
 (fn (ptr (struct csym::hostent)) (ptr (const void)) csym::size_t int
  (ptr int)))
(decl (csym::getipnodebyname)
 (fn (ptr (struct csym::hostent)) (ptr (const char)) int int (ptr int)))
(decl (csym::getnetbyaddr) (fn (ptr (struct csym::netent)) unsigned-long int))
(decl (csym::getnetbyname) (fn (ptr (struct csym::netent)) (ptr (const char))))
(decl (csym::getnetent) (fn (ptr (struct csym::netent)) void))
(decl (csym::getnetgrent)
 (fn int (ptr (ptr char)) (ptr (ptr char)) (ptr (ptr char))))
(decl (csym::getprotobyname)
 (fn (ptr (struct csym::protoent)) (ptr (const char))))
(decl (csym::getprotobynumber) (fn (ptr (struct csym::protoent)) int))
(decl (csym::getprotoent) (fn (ptr (struct csym::protoent)) void))
(decl (csym::getservbyname)
 (fn (ptr (struct csym::servent)) (ptr (const char)) (ptr (const char))))
(decl (csym::getservbyport)
 (fn (ptr (struct csym::servent)) int (ptr (const char))))
(decl (csym::getservent) (fn (ptr (struct csym::servent)) void))
(decl (csym::herror) (fn void (ptr (const char))))
(decl (csym::hstrerror) (fn (ptr (const char)) int))
(decl (csym::innetgr)
 (fn int (ptr (const char)) (ptr (const char)) (ptr (const char))
  (ptr (const char))))
(decl (csym::sethostent) (fn void int))
(decl (csym::setnetent) (fn void int))
(decl (csym::setprotoent) (fn void int))
(decl (csym::getaddrinfo)
 (fn int (ptr (const char)) (ptr (const char))
  (ptr (const (struct csym::addrinfo))) (ptr (ptr (struct csym::addrinfo)))))
(decl (csym::getnameinfo)
 (fn int (ptr (const (struct csym::sockaddr))) csym::socklen_t (ptr char)
  csym::size_t (ptr char) csym::size_t int))
(decl (csym::freeaddrinfo) (fn void (ptr (struct csym::addrinfo))))
(decl (csym::gai_strerror) (fn (ptr char) int))
(decl (csym::setnetgrent) (fn void (ptr (const char))))
(decl (csym::setservent) (fn void int))
(%line 257 "posix-out/netdb.h")
(decl (csym::__h_error) (fn (ptr int) void))
(decl (csym::_sethosthtent) (fn void int))
(decl (csym::_endhosthtent) (fn void void))
(decl (csym::_sethostdnsent) (fn void int))
(decl (csym::_endhostdnsent) (fn void void))
(decl (csym::_setnethtent) (fn void int))
(decl (csym::_endnethtent) (fn void void))
(decl (csym::_setnetdnsent) (fn void int))
(decl (csym::_endnetdnsent) (fn void void))
(decl (csym::_gethostbynisname)
 (fn (ptr (struct csym::hostent)) (ptr (const char)) int))
(decl (csym::_gethostbynisaddr)
 (fn (ptr (struct csym::hostent)) (ptr (const char)) int int))
(decl (csym::_map_v4v6_address) (fn void (ptr (const char)) (ptr char)))
(decl (csym::_map_v4v6_hostent)
 (fn void (ptr (struct csym::hostent)) (ptr (ptr char)) (ptr (ptr char))))
(%line 1 "posix-out/nl_types.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 43 "posix-out/nl_types.h")
(deftype csym::nl_catd (ptr void))
(%line 50 "posix-out/nl_types.h")
(decl (csym::catopen) (fn csym::nl_catd (ptr (const char)) int))
(decl (csym::catgets) (fn (ptr char) csym::nl_catd int int (ptr (const char))))
(decl (csym::catclose) (fn int csym::nl_catd))
(%line 1 "posix-out/poll.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 40 "posix-out/poll.h")
(deftype csym::nfds_t unsigned-int)
(%line 45 "posix-out/poll.h")
(def (struct csym::pollfd) (decl csym::fd int) (decl csym::events short)
 (decl csym::revents short))
(%line 98 "posix-out/poll.h")
(decl (csym::poll csym::_pfd csym::_nfds csym::_timeout)
 (fn int (array (struct csym::pollfd)) csym::nfds_t int))
(%line 1 "posix-out/pthread.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/time.h")
(%line 1 "/usr/include/sys/signal.h")
(%line 1 "/usr/include/limits.h")
(%line 1 "/usr/include/sched.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 55 "/usr/include/sched.h")
(def (struct csym::sched_param) (decl csym::sched_priority int))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 62 "/usr/include/sched.h")
(decl (csym::sched_get_priority_max) (fn int int))
(decl (csym::sched_get_priority_min) (fn int int))
(decl (csym::sched_getparam) (fn int long (ptr (struct csym::sched_param))))
(decl (csym::sched_getscheduler) (fn int long))
(decl (csym::sched_rr_get_interval) (fn int long (ptr (struct csym::timespec))))
(decl (csym::sched_setparam)
 (fn int long (ptr (const (struct csym::sched_param)))))
(decl (csym::sched_setscheduler)
 (fn int long int (ptr (const (struct csym::sched_param)))))
(decl (csym::sched_yield) (fn int void))
(%line 90 "posix-out/pthread.h")
(%line 110 "posix-out/pthread.h")
(deftype csym::pthread_t (ptr (struct csym::pthread)))
(deftype csym::pthread_attr_t (ptr (struct csym::pthread_attr)))
(deftype csym::pthread_mutex_t (ptr (struct csym::pthread_mutex)))
(deftype csym::pthread_mutexattr_t (ptr (struct csym::pthread_mutex_attr)))
(deftype csym::pthread_cond_t (ptr (struct csym::pthread_cond)))
(deftype csym::pthread_condattr_t (ptr (struct csym::pthread_cond_attr)))
(deftype csym::pthread_key_t int)
(deftype csym::pthread_once_t (struct csym::pthread_once))
(deftype csym::pthread_rwlock_t (ptr (struct csym::pthread_rwlock)))
(deftype csym::pthread_rwlockattr_t (ptr (struct csym::pthread_rwlockattr)))
(deftype csym::pthread_barrier_t (ptr (struct csym::pthread_barrier)))
(deftype csym::pthread_barrierattr_t (ptr (struct csym::pthread_barrierattr)))
(deftype csym::pthread_spinlock_t (ptr (struct csym::pthread_spinlock)))
(%line 130 "posix-out/pthread.h")
(deftype csym::pthread_addr_t (ptr void))
(deftype csym::pthread_startroutine_t (ptr (fn (ptr void) (ptr void))))
(%line 136 "posix-out/pthread.h")
(def (struct csym::pthread_once) (decl csym::state int)
 (decl csym::mutex csym::pthread_mutex_t))
(%line 184 "posix-out/pthread.h")
(def (enum csym::pthread_mutextype)
 ((csym::@PTHREAD_MUTEX_ERRORCHECK 1) (csym::@PTHREAD_MUTEX_RECURSIVE 2)
  (csym::@PTHREAD_MUTEX_NORMAL 3) csym::@MUTEX_TYPE_MAX))
(%line 198 "posix-out/pthread.h")
(decl (csym::pthread_atfork csym::prepare csym::parent csym::child)
 (fn int (ptr (fn void void)) (ptr (fn void void)) (ptr (fn void void))))
(decl (csym::pthread_attr_destroy) (fn int (ptr csym::pthread_attr_t)))
(decl (csym::pthread_attr_getstack)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr (ptr void))
  (ptr csym::size_t)))
(decl (csym::pthread_attr_getstacksize)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr csym::size_t)))
(decl (csym::pthread_attr_getguardsize)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr csym::size_t)))
(decl (csym::pthread_attr_getstackaddr)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr (ptr void))))
(decl (csym::pthread_attr_getdetachstate)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr int)))
(decl (csym::pthread_attr_init) (fn int (ptr csym::pthread_attr_t)))
(decl (csym::pthread_attr_setstacksize)
 (fn int (ptr csym::pthread_attr_t) csym::size_t))
(decl (csym::pthread_attr_setguardsize)
 (fn int (ptr csym::pthread_attr_t) csym::size_t))
(decl (csym::pthread_attr_setstack)
 (fn int (ptr csym::pthread_attr_t) (ptr void) csym::size_t))
(decl (csym::pthread_attr_setstackaddr)
 (fn int (ptr csym::pthread_attr_t) (ptr void)))
(decl (csym::pthread_attr_setdetachstate)
 (fn int (ptr csym::pthread_attr_t) int))
(decl (csym::pthread_barrier_destroy) (fn int (ptr csym::pthread_barrier_t)))
(decl (csym::pthread_barrier_init)
 (fn int (ptr csym::pthread_barrier_t)
  (ptr (const csym::pthread_barrierattr_t)) unsigned-int))
(decl (csym::pthread_barrier_wait) (fn int (ptr csym::pthread_barrier_t)))
(decl (csym::pthread_barrierattr_destroy)
 (fn int (ptr csym::pthread_barrierattr_t)))
(decl (csym::pthread_barrierattr_getpshared)
 (fn int (ptr (const csym::pthread_barrierattr_t)) (ptr int)))
(decl (csym::pthread_barrierattr_init)
 (fn int (ptr csym::pthread_barrierattr_t)))
(decl (csym::pthread_barrierattr_setpshared)
 (fn int (ptr csym::pthread_barrierattr_t) int))
(decl (csym::pthread_cleanup_pop) (fn void int))
(decl (csym::pthread_cleanup_push)
 (fn void (ptr (fn void (ptr void))) (ptr void)))
(decl (csym::pthread_condattr_destroy) (fn int (ptr csym::pthread_condattr_t)))
(decl (csym::pthread_condattr_init) (fn int (ptr csym::pthread_condattr_t)))
(decl (csym::pthread_cond_broadcast) (fn int (ptr csym::pthread_cond_t)))
(decl (csym::pthread_cond_destroy) (fn int (ptr csym::pthread_cond_t)))
(decl (csym::pthread_cond_init)
 (fn int (ptr csym::pthread_cond_t) (ptr (const csym::pthread_condattr_t))))
(decl (csym::pthread_cond_signal) (fn int (ptr csym::pthread_cond_t)))
(decl (csym::pthread_cond_timedwait)
 (fn int (ptr csym::pthread_cond_t) (ptr csym::pthread_mutex_t)
  (ptr (const (struct csym::timespec)))))
(decl (csym::pthread_cond_wait)
 (fn int (ptr csym::pthread_cond_t) (ptr csym::pthread_mutex_t)))
(decl (csym::pthread_create)
 (fn int (ptr csym::pthread_t) (ptr (const csym::pthread_attr_t))
  (ptr (fn (ptr void) (ptr void))) (ptr void)))
(decl (csym::pthread_detach) (fn int csym::pthread_t))
(decl (csym::pthread_equal) (fn int csym::pthread_t csym::pthread_t))
(decl (csym::pthread_exit) (fn void (ptr void)))
(decl (csym::pthread_getspecific) (fn (ptr void) csym::pthread_key_t))
(decl (csym::pthread_join) (fn int csym::pthread_t (ptr (ptr void))))
(decl (csym::pthread_key_create)
 (fn int (ptr csym::pthread_key_t) (ptr (fn void (ptr void)))))
(decl (csym::pthread_key_delete) (fn int csym::pthread_key_t))
(decl (csym::pthread_kill) (fn int csym::pthread_t int))
(decl (csym::pthread_mutexattr_init) (fn int (ptr csym::pthread_mutexattr_t)))
(decl (csym::pthread_mutexattr_destroy)
 (fn int (ptr csym::pthread_mutexattr_t)))
(decl (csym::pthread_mutexattr_gettype)
 (fn int (ptr csym::pthread_mutexattr_t) (ptr int)))
(decl (csym::pthread_mutexattr_settype)
 (fn int (ptr csym::pthread_mutexattr_t) int))
(decl (csym::pthread_mutex_destroy) (fn int (ptr csym::pthread_mutex_t)))
(decl (csym::pthread_mutex_init)
 (fn int (ptr csym::pthread_mutex_t) (ptr (const csym::pthread_mutexattr_t))))
(decl (csym::pthread_mutex_lock) (fn int (ptr csym::pthread_mutex_t)))
(decl (csym::pthread_mutex_trylock) (fn int (ptr csym::pthread_mutex_t)))
(decl (csym::pthread_mutex_timedlock)
 (fn int (ptr csym::pthread_mutex_t) (ptr (const (struct csym::timespec)))))
(decl (csym::pthread_mutex_unlock) (fn int (ptr csym::pthread_mutex_t)))
(decl (csym::pthread_once)
 (fn int (ptr csym::pthread_once_t) (ptr (fn void void))))
(decl (csym::pthread_rwlock_destroy) (fn int (ptr csym::pthread_rwlock_t)))
(decl (csym::pthread_rwlock_init)
 (fn int (ptr csym::pthread_rwlock_t) (ptr (const csym::pthread_rwlockattr_t))))
(decl (csym::pthread_rwlock_rdlock) (fn int (ptr csym::pthread_rwlock_t)))
(decl (csym::pthread_rwlock_timedrdlock)
 (fn int (ptr csym::pthread_rwlock_t) (ptr (const (struct csym::timespec)))))
(decl (csym::pthread_rwlock_timedrwlock)
 (fn int (ptr csym::pthread_rwlock_t) (ptr (const (struct csym::timespec)))))
(decl (csym::pthread_rwlock_tryrdlock) (fn int (ptr csym::pthread_rwlock_t)))
(decl (csym::pthread_rwlock_trywrlock) (fn int (ptr csym::pthread_rwlock_t)))
(decl (csym::pthread_rwlock_unlock) (fn int (ptr csym::pthread_rwlock_t)))
(decl (csym::pthread_rwlock_wrlock) (fn int (ptr csym::pthread_rwlock_t)))
(decl (csym::pthread_rwlockattr_init) (fn int (ptr csym::pthread_rwlockattr_t)))
(decl (csym::pthread_rwlockattr_getpshared)
 (fn int (ptr (const csym::pthread_rwlockattr_t)) (ptr int)))
(decl (csym::pthread_rwlockattr_setpshared)
 (fn int (ptr csym::pthread_rwlockattr_t) int))
(decl (csym::pthread_rwlockattr_destroy)
 (fn int (ptr csym::pthread_rwlockattr_t)))
(decl (csym::pthread_self) (fn csym::pthread_t void))
(decl (csym::pthread_setspecific)
 (fn int csym::pthread_key_t (ptr (const void))))
(decl (csym::pthread_sigmask)
 (fn int int (ptr (const csym::sigset_t)) (ptr csym::sigset_t)))
(decl (csym::pthread_spin_init) (fn int (ptr csym::pthread_spinlock_t) int))
(decl (csym::pthread_spin_destroy) (fn int (ptr csym::pthread_spinlock_t)))
(decl (csym::pthread_spin_lock) (fn int (ptr csym::pthread_spinlock_t)))
(decl (csym::pthread_spin_trylock) (fn int (ptr csym::pthread_spinlock_t)))
(decl (csym::pthread_spin_unlock) (fn int (ptr csym::pthread_spinlock_t)))
(decl (csym::pthread_cancel) (fn int csym::pthread_t))
(decl (csym::pthread_setcancelstate) (fn int int (ptr int)))
(decl (csym::pthread_setcanceltype) (fn int int (ptr int)))
(decl (csym::pthread_testcancel) (fn void void))
(decl (csym::pthread_getprio) (fn int csym::pthread_t))
(decl (csym::pthread_setprio) (fn int csym::pthread_t int))
(decl (csym::pthread_yield) (fn void void))
(decl (csym::pthread_mutexattr_getprioceiling)
 (fn int (ptr csym::pthread_mutexattr_t) (ptr int)))
(decl (csym::pthread_mutexattr_setprioceiling)
 (fn int (ptr csym::pthread_mutexattr_t) int))
(decl (csym::pthread_mutex_getprioceiling)
 (fn int (ptr csym::pthread_mutex_t) (ptr int)))
(decl (csym::pthread_mutex_setprioceiling)
 (fn int (ptr csym::pthread_mutex_t) int (ptr int)))
(decl (csym::pthread_mutexattr_getprotocol)
 (fn int (ptr csym::pthread_mutexattr_t) (ptr int)))
(decl (csym::pthread_mutexattr_setprotocol)
 (fn int (ptr csym::pthread_mutexattr_t) int))
(decl (csym::pthread_attr_getinheritsched)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr int)))
(decl (csym::pthread_attr_getschedparam)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr (struct csym::sched_param))))
(decl (csym::pthread_attr_getschedpolicy)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr int)))
(decl (csym::pthread_attr_getscope)
 (fn int (ptr (const csym::pthread_attr_t)) (ptr int)))
(decl (csym::pthread_attr_setinheritsched)
 (fn int (ptr csym::pthread_attr_t) int))
(decl (csym::pthread_attr_setschedparam)
 (fn int (ptr csym::pthread_attr_t) (ptr (const (struct csym::sched_param)))))
(decl (csym::pthread_attr_setschedpolicy)
 (fn int (ptr csym::pthread_attr_t) int))
(decl (csym::pthread_attr_setscope) (fn int (ptr csym::pthread_attr_t) int))
(decl (csym::pthread_getschedparam csym::pthread)
 (fn int csym::pthread_t (ptr int) (ptr (struct csym::sched_param))))
(decl (csym::pthread_setschedparam)
 (fn int csym::pthread_t int (ptr (const (struct csym::sched_param)))))
(decl (csym::pthread_getconcurrency) (fn int void))
(decl (csym::pthread_setconcurrency) (fn int int))
(%line 1 "posix-out/pwd.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 116 "posix-out/pwd.h")
(def (struct csym::passwd) (decl csym::pw_name (ptr char))
 (decl csym::pw_passwd (ptr char)) (decl csym::pw_uid csym::uid_t)
 (decl csym::pw_gid csym::gid_t) (decl csym::pw_change double)
 (decl csym::pw_class (ptr char)) (decl csym::pw_gecos (ptr char))
 (decl csym::pw_dir (ptr char)) (decl csym::pw_shell (ptr char))
 (decl csym::pw_expire double) (decl csym::pw_fields int))
(%line 151 "posix-out/pwd.h")
(decl (csym::getpwnam) (fn (ptr (struct csym::passwd)) (ptr (const char))))
(decl (csym::getpwuid) (fn (ptr (struct csym::passwd)) csym::uid_t))
(decl (csym::endpwent) (fn void void))
(decl (csym::getpwent) (fn (ptr (struct csym::passwd)) void))
(decl (csym::setpwent) (fn void void))
(decl (csym::getpwnam_r)
 (fn int (ptr (const char)) (ptr (struct csym::passwd)) (ptr char) csym::size_t
  (ptr (ptr (struct csym::passwd)))))
(decl (csym::getpwuid_r)
 (fn int csym::uid_t (ptr (struct csym::passwd)) (ptr char) csym::size_t
  (ptr (ptr (struct csym::passwd)))))
(decl (csym::getpwent_r)
 (fn int (ptr (struct csym::passwd)) (ptr char) csym::size_t
  (ptr (ptr (struct csym::passwd)))))
(decl (csym::setpassent) (fn int int))
(decl (csym::user_from_uid) (fn (ptr (const char)) csym::uid_t int))
(%line 1 "posix-out/regex.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 48 "posix-out/regex.h")
(deftype csym::regoff_t csym::__off_t)
(%line 55 "posix-out/regex.h")
(def (struct csym::unnamed18) (decl csym::re_magic int)
 (decl csym::re_nsub csym::size_t) (decl csym::re_endp (ptr (const char)))
 (decl csym::re_g (ptr (struct csym::re_guts))))
(deftype csym::regex_t (struct csym::unnamed18))
(def (struct csym::unnamed19) (decl csym::rm_so csym::regoff_t)
 (decl csym::rm_eo csym::regoff_t))
(deftype csym::regmatch_t (struct csym::unnamed19))
(%line 107 "posix-out/regex.h")
(decl (csym::regcomp) (fn int (ptr csym::regex_t) (ptr (const char)) int))
(decl (csym::regerror)
 (fn csym::size_t int (ptr (const csym::regex_t)) (ptr char) csym::size_t))
(%line 115 "posix-out/regex.h")
(decl (csym::regexec)
 (fn int (ptr (const csym::regex_t)) (ptr (const char)) csym::size_t
  (ptr csym::regmatch_t) int))
(decl (csym::regfree) (fn void (ptr csym::regex_t)))
(%line 1 "posix-out/resource.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_timeval.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 57 "posix-out/resource.h")
(def (struct csym::rusage) (decl csym::ru_utime (struct csym::timeval))
 (decl csym::ru_stime (struct csym::timeval)) (decl csym::ru_maxrss long)
 (decl csym::ru_ixrss long) (decl csym::ru_idrss long)
 (decl csym::ru_isrss long) (decl csym::ru_minflt long)
 (decl csym::ru_majflt long) (decl csym::ru_nswap long)
 (decl csym::ru_inblock long) (decl csym::ru_oublock long)
 (decl csym::ru_msgsnd long) (decl csym::ru_msgrcv long)
 (decl csym::ru_nsignals long) (decl csym::ru_nvcsw long)
 (decl csym::ru_nivcsw long))
(%line 125 "posix-out/resource.h")
(def (struct csym::rlimit) (decl csym::rlim_cur csym::rlim_t)
 (decl csym::rlim_max csym::rlim_t))
(def (struct csym::orlimit) (decl csym::rlim_cur csym::__int32_t)
 (decl csym::rlim_max csym::__int32_t))
(def (struct csym::loadavg) (decl csym::ldavg (array csym::__fixpt_t 3))
 (decl csym::fscale long))
(%line 159 "posix-out/resource.h")
(decl (csym::getpriority) (fn int int int))
(decl (csym::getrlimit) (fn int int (ptr (struct csym::rlimit))))
(decl (csym::getrusage) (fn int int (ptr (struct csym::rusage))))
(decl (csym::setpriority) (fn int int int int))
(decl (csym::setrlimit) (fn int int (ptr (const (struct csym::rlimit)))))
(%line 1 "posix-out/sched.h")
(%line 1 "posix-out/search.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 20 "posix-out/search.h")
(def (struct csym::entry) (decl csym::key (ptr char))
 (decl csym::data (ptr void)))
(deftype csym::@ENTRY (struct csym::entry))
(def (enum csym::unnamed20) (csym::@FIND csym::@ENTER))
(deftype csym::@ACTION (enum csym::unnamed20))
(def (enum csym::unnamed21)
 (csym::preorder csym::postorder csym::endorder csym::leaf))
(deftype csym::@VISIT (enum csym::unnamed21))
(%line 48 "posix-out/search.h")
(decl (csym::hcreate) (fn int csym::size_t))
(decl (csym::hdestroy) (fn void void))
(decl (csym::hsearch) (fn (ptr csym::@ENTRY) csym::@ENTRY csym::@ACTION))
(decl (csym::insque) (fn void (ptr void) (ptr void)))
(decl (csym::lfind)
 (fn (ptr void) (ptr (const void)) (ptr (const void)) (ptr csym::size_t)
  csym::size_t (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::lsearch)
 (fn (ptr void) (ptr (const void)) (ptr void) (ptr csym::size_t) csym::size_t
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::remque) (fn void (ptr void)))
(decl (csym::tdelete)
 (fn (ptr void) (ptr (const void)) (ptr (ptr void))
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::tfind)
 (fn (ptr void) (ptr (const void)) (ptr (const (ptr void)))
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::tsearch)
 (fn (ptr void) (ptr (const void)) (ptr (ptr void))
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::twalk)
 (fn void (ptr (const void))
  (ptr (fn void (ptr (const void)) csym::@VISIT int))))
(%line 1 "posix-out/select.h")
(%line 1 "posix-out/sem.h")
(%line 1 "/usr/include/sys/ipc.h")
(%line 15 "posix-out/sem.h")
(def (struct csym::semid_ds) (decl csym::sem_perm (struct csym::ipc_perm))
 (decl csym::sem_base (ptr (struct csym::sem)))
 (decl csym::sem_nsems unsigned-short) (decl csym::sem_otime double)
 (decl csym::sem_pad1 long) (decl csym::sem_ctime double)
 (decl csym::sem_pad2 long) (decl csym::sem_pad3 (array long 4)))
(%line 33 "posix-out/sem.h")
(def (struct csym::sembuf) (decl csym::sem_num unsigned-short)
 (decl csym::sem_op short) (decl csym::sem_flg short))
(%line 43 "posix-out/sem.h")
(def (union csym::semun) (decl csym::val int)
 (decl csym::buf (ptr (struct csym::semid_ds)))
 (decl csym::array (ptr unsigned-short)))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 111 "posix-out/sem.h")
(decl (csym::semsys) (fn int int va-arg))
(decl (csym::semctl) (fn int int int int va-arg))
(decl (csym::semget) (fn int csym::key_t int int))
(decl (csym::semop) (fn int int (ptr (struct csym::sembuf)) csym::size_t))
(%line 1 "posix-out/semaphore.h")
(%line 41 "posix-out/semaphore.h")
(deftype csym::sem_t (ptr (struct csym::sem)))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 50 "posix-out/semaphore.h")
(decl (csym::sem_close) (fn int (ptr csym::sem_t)))
(decl (csym::sem_destroy) (fn int (ptr csym::sem_t)))
(decl (csym::sem_getvalue) (fn int (ptr csym::sem_t) (ptr int)))
(decl (csym::sem_init) (fn int (ptr csym::sem_t) int unsigned-int))
(decl (csym::sem_open) (fn (ptr csym::sem_t) (ptr (const char)) int va-arg))
(decl (csym::sem_post) (fn int (ptr csym::sem_t)))
(decl (csym::sem_timedwait)
 (fn int (ptr csym::sem_t) (ptr (const (struct csym::timespec)))))
(decl (csym::sem_trywait) (fn int (ptr csym::sem_t)))
(decl (csym::sem_unlink) (fn int (ptr (const char))))
(decl (csym::sem_wait) (fn int (ptr csym::sem_t)))
(%line 1 "posix-out/setjmp.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/machine/setjmp.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 48 "/usr/include/machine/setjmp.h")
(def (struct csym::_sigjmp_buf) (decl csym::_sjb (array int (+ 11 1))))
(deftype csym::sigjmp_buf (array (struct csym::_sigjmp_buf) 1))
(def (struct csym::_jmp_buf) (decl csym::_jb (array int (+ 11 1))))
(deftype csym::jmp_buf (array (struct csym::_jmp_buf) 1))
(%line 50 "posix-out/setjmp.h")
(decl (csym::_longjmp) (fn void csym::jmp_buf int))
(decl (csym::_setjmp) (fn int csym::jmp_buf))
(decl (csym::longjmp) (fn void csym::jmp_buf int))
(decl (csym::longjmperror) (fn void void))
(decl (csym::setjmp) (fn int csym::jmp_buf))
(decl (csym::siglongjmp) (fn void csym::sigjmp_buf int))
(decl (csym::sigsetjmp) (fn int csym::sigjmp_buf int))
(%line 1 "posix-out/shm.h")
(%line 1 "/usr/include/sys/ipc.h")
(%line 61 "posix-out/shm.h")
(def (struct csym::shmid_ds) (decl csym::shm_perm (struct csym::ipc_perm))
 (decl csym::shm_segsz int) (decl csym::shm_lpid long)
 (decl csym::shm_cpid long) (decl csym::shm_nattch short)
 (decl csym::shm_atime double) (decl csym::shm_dtime double)
 (decl csym::shm_ctime double) (decl csym::shm_internal (ptr void)))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 113 "posix-out/shm.h")
(decl (csym::shmsys) (fn int int va-arg))
(decl (csym::shmat) (fn (ptr void) int (ptr (const void)) int))
(decl (csym::shmget) (fn int csym::key_t csym::size_t int))
(decl (csym::shmctl) (fn int int int (ptr (struct csym::shmid_ds))))
(decl (csym::shmdt) (fn int (ptr (const void))))
(%line 1 "posix-out/signal.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/signal.h")
(%line 49 "posix-out/signal.h")
(extern-decl csym::sys_signame (array (const (ptr (const char))) 32))
(extern-decl csym::sys_siglist (array (const (ptr (const char))) 32))
(extern-decl csym::sys_nsig (const int))
(%line 61 "posix-out/signal.h")
(decl (csym::raise) (fn int int))
(decl (csym::kill) (fn int csym::__pid_t int))
(decl (csym::sigaction)
 (fn int int (ptr (const (struct csym::sigaction)))
  (ptr (struct csym::sigaction))))
(decl (csym::sigaddset) (fn int (ptr csym::sigset_t) int))
(decl (csym::sigdelset) (fn int (ptr csym::sigset_t) int))
(decl (csym::sigemptyset) (fn int (ptr csym::sigset_t)))
(decl (csym::sigfillset) (fn int (ptr csym::sigset_t)))
(decl (csym::sigismember) (fn int (ptr (const csym::sigset_t)) int))
(decl (csym::sigpending) (fn int (ptr csym::sigset_t)))
(decl (csym::sigprocmask)
 (fn int int (ptr (const csym::sigset_t)) (ptr csym::sigset_t)))
(decl (csym::sigsuspend) (fn int (ptr (const csym::sigset_t))))
(decl (csym::sigwait) (fn int (ptr (const csym::sigset_t)) (ptr int)))
(%line 89 "posix-out/signal.h")
(decl (csym::sigtimedwait)
 (fn int (ptr (const csym::sigset_t)) (ptr csym::siginfo_t)
  (ptr (const (struct csym::timespec)))))
(decl (csym::sigwaitinfo)
 (fn int (ptr (const csym::sigset_t)) (ptr csym::siginfo_t)))
(decl (csym::killpg) (fn int csym::__pid_t int))
(decl (csym::sigaltstack)
 (fn int (ptr (const csym::stack_t)) (ptr csym::stack_t)))
(decl (csym::sigpause) (fn int int))
(decl (csym::siginterrupt) (fn int int int))
(decl (csym::sigblock) (fn int int))
(decl (csym::sigreturn) (fn int (ptr (const (struct csym::__ucontext)))))
(decl (csym::sigsetmask) (fn int int))
(decl (csym::sigstack)
 (fn int (ptr (const (struct csym::sigstack))) (ptr (struct csym::sigstack))))
(decl (csym::sigvec)
 (fn int int (ptr (struct csym::sigvec)) (ptr (struct csym::sigvec))))
(decl (csym::psignal) (fn void unsigned-int (ptr (const char))))
(%line 1 "posix-out/socket.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/_iovec.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 43 "/usr/include/sys/_iovec.h")
(def (struct csym::iovec) (decl csym::iov_base (ptr void))
 (decl csym::iov_len csym::size_t))
(%line 1 "/usr/include/machine/param.h")
(%line 141 "posix-out/socket.h")
(def (struct csym::linger) (decl csym::l_onoff int) (decl csym::l_linger int))
(def (struct csym::accept_filter_arg) (decl csym::af_name (array char 16))
 (decl csym::af_arg (array char (- 256 16))))
(%line 215 "posix-out/socket.h")
(def (struct csym::sockaddr) (decl csym::sa_len unsigned-char)
 (decl csym::sa_family csym::sa_family_t) (decl csym::sa_data (array char 14)))
(%line 227 "posix-out/socket.h")
(def (struct csym::sockproto) (decl csym::sp_family unsigned-short)
 (decl csym::sp_protocol unsigned-short))
(%line 243 "posix-out/socket.h")
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
(%line 376 "posix-out/socket.h")
(def (struct csym::msghdr) (decl csym::msg_name (ptr void))
 (decl csym::msg_namelen csym::socklen_t)
 (decl csym::msg_iov (ptr (struct csym::iovec))) (decl csym::msg_iovlen int)
 (decl csym::msg_control (ptr void))
 (decl csym::msg_controllen csym::socklen_t) (decl csym::msg_flags int))
(%line 406 "posix-out/socket.h")
(def (struct csym::cmsghdr) (decl csym::cmsg_len csym::socklen_t)
 (decl csym::cmsg_level int) (decl csym::cmsg_type int))
(%line 428 "posix-out/socket.h")
(def (struct csym::cmsgcred) (decl csym::cmcred_pid long)
 (decl csym::cmcred_uid csym::uid_t) (decl csym::cmcred_euid csym::uid_t)
 (decl csym::cmcred_gid csym::gid_t) (decl csym::cmcred_ngroups short)
 (decl csym::cmcred_groups (array csym::gid_t 16)))
(%line 481 "posix-out/socket.h")
(def (struct csym::osockaddr) (decl csym::sa_family unsigned-short)
 (decl csym::sa_data (array char 14)))
(%line 489 "posix-out/socket.h")
(def (struct csym::omsghdr) (decl csym::msg_name (ptr char))
 (decl csym::msg_namelen int) (decl csym::msg_iov (ptr (struct csym::iovec)))
 (decl csym::msg_iovlen int) (decl csym::msg_accrights (ptr char))
 (decl csym::msg_accrightslen int))
(%line 510 "posix-out/socket.h")
(def (struct csym::sf_hdtr) (decl csym::headers (ptr (struct csym::iovec)))
 (decl csym::hdr_cnt int) (decl csym::trailers (ptr (struct csym::iovec)))
 (decl csym::trl_cnt int))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 527 "posix-out/socket.h")
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
(%line 1 "posix-out/stat.h")
(%line 1 "posix-out/statvfs.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 59 "posix-out/statvfs.h")
(def (struct csym::statvfs) (decl csym::f_bavail csym::fsblkcnt_t)
 (decl csym::f_bfree csym::fsblkcnt_t) (decl csym::f_blocks csym::fsblkcnt_t)
 (decl csym::f_favail csym::fsfilcnt_t) (decl csym::f_ffree csym::fsfilcnt_t)
 (decl csym::f_files csym::fsfilcnt_t) (decl csym::f_bsize unsigned-long)
 (decl csym::f_flag unsigned-long) (decl csym::f_frsize unsigned-long)
 (decl csym::f_fsid unsigned-long) (decl csym::f_namemax unsigned-long))
(decl (csym::fstatvfs) (fn int int (ptr (struct csym::statvfs))))
(decl (csym::statvfs) (fn int (ptr (const char)) (ptr (struct csym::statvfs))))
(%line 1 "posix-out/stdarg.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 46 "posix-out/stdarg.h")
(deftype csym::va_list csym::__va_list)
(%line 1 "posix-out/stdbool.h")
(%line 1 "posix-out/stddef.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 45 "posix-out/stddef.h")
(deftype csym::ptrdiff_t csym::__ptrdiff_t)
(deftype csym::rune_t csym::__rune_t)
(%line 61 "posix-out/stddef.h")
(deftype csym::wchar_t csym::__wchar_t)
(%line 1 "posix-out/stdint.h")
(%line 1 "posix-out/stdio.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 47 "posix-out/stdio.h")
(deftype csym::fpos_t csym::__off_t)
(%line 70 "posix-out/stdio.h")
(def (struct csym::__sbuf) (decl csym::_base (ptr unsigned-char))
 (decl csym::_size int))
(%line 102 "posix-out/stdio.h")
(def (struct csym::__S@file) (decl csym::_p (ptr unsigned-char))
 (decl csym::_r int) (decl csym::_w int) (decl csym::_flags short)
 (decl csym::_file short) (decl csym::_bf (struct csym::__sbuf))
 (decl csym::_lbfsize int) (decl csym::_cookie (ptr void))
 (decl csym::_close (ptr (fn int (ptr void))))
 (decl csym::_read (ptr (fn int (ptr void) (ptr char) int)))
 (decl csym::_seek (ptr (fn csym::fpos_t (ptr void) csym::fpos_t int)))
 (decl csym::_write (ptr (fn int (ptr void) (ptr (const char)) int)))
 (decl csym::_ub (struct csym::__sbuf))
 (decl csym::_extra (ptr (struct csym::__S@filex))) (decl csym::_ur int)
 (decl csym::_ubuf (array unsigned-char 3))
 (decl csym::_nbuf (array unsigned-char 1))
 (decl csym::_lb (struct csym::__sbuf)) (decl csym::_blksize int)
 (decl csym::_offset csym::fpos_t))
(deftype csym::@FILE (struct csym::__S@file))
(extern-decl csym::__stdinp (ptr csym::@FILE))
(extern-decl csym::__stdoutp (ptr csym::@FILE))
(extern-decl csym::__stderrp (ptr csym::@FILE))
(%line 208 "posix-out/stdio.h")
(%line 212 "posix-out/stdio.h")
(decl (csym::clearerr) (fn void (ptr csym::@FILE)))
(decl (csym::fclose) (fn int (ptr csym::@FILE)))
(decl (csym::feof) (fn int (ptr csym::@FILE)))
(decl (csym::ferror) (fn int (ptr csym::@FILE)))
(decl (csym::fflush) (fn int (ptr csym::@FILE)))
(decl (csym::fgetc) (fn int (ptr csym::@FILE)))
(decl (csym::fgetpos) (fn int (ptr csym::@FILE) (ptr csym::fpos_t)))
(decl (csym::fgets) (fn (ptr char) (ptr char) int (ptr csym::@FILE)))
(decl (csym::fopen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char))))
(decl (csym::fprintf) (fn int (ptr csym::@FILE) (ptr (const char)) va-arg))
(decl (csym::fputc) (fn int int (ptr csym::@FILE)))
(decl (csym::fputs) (fn int (ptr (const char)) (ptr csym::@FILE)))
(decl (csym::fread)
 (fn csym::size_t (ptr void) csym::size_t csym::size_t (ptr csym::@FILE)))
(decl (csym::freopen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char)) (ptr csym::@FILE)))
(decl (csym::fscanf) (fn int (ptr csym::@FILE) (ptr (const char)) va-arg))
(decl (csym::fseek) (fn int (ptr csym::@FILE) long int))
(decl (csym::fsetpos) (fn int (ptr csym::@FILE) (ptr (const csym::fpos_t))))
(decl (csym::ftell) (fn long (ptr csym::@FILE)))
(decl (csym::fwrite)
 (fn csym::size_t (ptr (const void)) csym::size_t csym::size_t
  (ptr csym::@FILE)))
(decl (csym::getc) (fn int (ptr csym::@FILE)))
(decl (csym::getchar) (fn int void))
(decl (csym::gets) (fn (ptr char) (ptr char)))
(decl (csym::perror) (fn void (ptr (const char))))
(decl (csym::printf) (fn int (ptr (const char)) va-arg))
(decl (csym::putc) (fn int int (ptr csym::@FILE)))
(decl (csym::putchar) (fn int int))
(decl (csym::puts) (fn int (ptr (const char))))
(decl (csym::remove) (fn int (ptr (const char))))
(decl (csym::rename) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::rewind) (fn void (ptr csym::@FILE)))
(decl (csym::scanf) (fn int (ptr (const char)) va-arg))
(decl (csym::setbuf) (fn void (ptr csym::@FILE) (ptr char)))
(decl (csym::setvbuf) (fn int (ptr csym::@FILE) (ptr char) int csym::size_t))
(decl (csym::sprintf) (fn int (ptr char) (ptr (const char)) va-arg))
(decl (csym::sscanf) (fn int (ptr (const char)) (ptr (const char)) va-arg))
(decl (csym::tmpfile) (fn (ptr csym::@FILE) void))
(decl (csym::tmpnam) (fn (ptr char) (ptr char)))
(decl (csym::ungetc) (fn int int (ptr csym::@FILE)))
(decl (csym::vfprintf)
 (fn int (ptr csym::@FILE) (ptr (const char)) csym::__va_list))
(decl (csym::vprintf) (fn int (ptr (const char)) csym::__va_list))
(decl (csym::vsprintf) (fn int (ptr char) (ptr (const char)) csym::__va_list))
(decl (csym::snprintf)
 (fn int (ptr char) csym::size_t (ptr (const char)) va-arg))
(decl (csym::vfscanf)
 (fn int (ptr csym::@FILE) (ptr (const char)) csym::__va_list))
(decl (csym::vscanf) (fn int (ptr (const char)) csym::__va_list))
(decl (csym::vsnprintf)
 (fn int (ptr char) csym::size_t (ptr (const char)) csym::__va_list))
(decl (csym::vsscanf)
 (fn int (ptr (const char)) (ptr (const char)) csym::__va_list))
(%line 279 "posix-out/stdio.h")
(decl (csym::ctermid) (fn (ptr char) (ptr char)))
(decl (csym::fdopen) (fn (ptr csym::@FILE) int (ptr (const char))))
(decl (csym::fileno) (fn int (ptr csym::@FILE)))
(decl (csym::pclose) (fn int (ptr csym::@FILE)))
(decl (csym::popen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char))))
(decl (csym::ftrylockfile) (fn int (ptr csym::@FILE)))
(decl (csym::flockfile) (fn void (ptr csym::@FILE)))
(decl (csym::funlockfile) (fn void (ptr csym::@FILE)))
(%line 298 "posix-out/stdio.h")
(decl (csym::getc_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::getchar_unlocked) (fn int void))
(decl (csym::putc_unlocked) (fn int int (ptr csym::@FILE)))
(decl (csym::putchar_unlocked) (fn int int))
(decl (csym::clearerr_unlocked) (fn void (ptr csym::@FILE)))
(decl (csym::feof_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::ferror_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::fileno_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::fseeko) (fn int (ptr csym::@FILE) csym::__off_t int))
(decl (csym::ftello) (fn csym::__off_t (ptr csym::@FILE)))
(decl (csym::getw) (fn int (ptr csym::@FILE)))
(decl (csym::putw) (fn int int (ptr csym::@FILE)))
(decl (csym::tempnam) (fn (ptr char) (ptr (const char)) (ptr (const char))))
(%line 328 "posix-out/stdio.h")
(decl (csym::asprintf) (fn int (ptr (ptr char)) (ptr (const char)) va-arg))
(decl (csym::ctermid_r) (fn (ptr char) (ptr char)))
(decl (csym::fgetln) (fn (ptr char) (ptr csym::@FILE) (ptr csym::size_t)))
(%line 336 "posix-out/stdio.h")
(decl (csym::fmtcheck)
 (fn (ptr (const char)) (ptr (const char)) (ptr (const char))))
(decl (csym::fpurge) (fn int (ptr csym::@FILE)))
(decl (csym::setbuffer) (fn void (ptr csym::@FILE) (ptr char) int))
(decl (csym::setlinebuf) (fn int (ptr csym::@FILE)))
(decl (csym::vasprintf)
 (fn int (ptr (ptr char)) (ptr (const char)) csym::__va_list))
(%line 348 "posix-out/stdio.h")
(extern-decl csym::sys_nerr (const int))
(extern-decl csym::sys_errlist (array (const (ptr (const char)))))
(%line 354 "posix-out/stdio.h")
(decl (csym::funopen)
 (fn (ptr csym::@FILE) (ptr (const void))
  (ptr (fn int (ptr void) (ptr char) int))
  (ptr (fn int (ptr void) (ptr (const char)) int))
  (ptr (fn csym::fpos_t (ptr void) csym::fpos_t int))
  (ptr (fn int (ptr void)))))
(%line 386 "posix-out/stdio.h")
(decl (csym::__srget) (fn int (ptr csym::@FILE)))
(decl (csym::__swbuf) (fn int int (ptr csym::@FILE)))
(%line 395 "posix-out/stdio.h")
(static-def (csym::__sputc csym::_c csym::_p) (fn int int (ptr csym::@FILE))
 (if
  (or (>= (-- (fref (mref csym::_p) csym::_w)) 0)
   (and
    (>= (fref (mref csym::_p) csym::_w) (fref (mref csym::_p) csym::_lbfsize))
    (!= (cast char csym::_c) #\Newline)))
  (return (= (mref (inc (fref (mref csym::_p) csym::_p))) csym::_c))
  (return (csym::__swbuf csym::_c csym::_p))))
(%line 420 "posix-out/stdio.h")
(extern-decl csym::__isthreaded int)
(%line 454 "posix-out/stdio.h")
(%line 1 "posix-out/stdlib.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 63 "posix-out/stdlib.h")
(def (struct csym::unnamed22) (decl csym::quot int) (decl csym::rem int))
(deftype csym::div_t (struct csym::unnamed22))
(def (struct csym::unnamed23) (decl csym::quot long) (decl csym::rem long))
(deftype csym::ldiv_t (struct csym::unnamed23))
(extern-decl csym::__mb_cur_max int)
(decl (csym::abort) (fn void void))
(decl (csym::abs) (fn int int))
(decl (csym::atexit) (fn int (ptr (fn void void))))
(decl (csym::atof) (fn double (ptr (const char))))
(decl (csym::atoi) (fn int (ptr (const char))))
(decl (csym::atol) (fn long (ptr (const char))))
(decl (csym::bsearch)
 (fn (ptr void) (ptr (const void)) (ptr (const void)) csym::size_t csym::size_t
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::calloc) (fn (ptr void) csym::size_t csym::size_t))
(decl (csym::div) (fn csym::div_t int int))
(decl (csym::exit) (fn void int))
(decl (csym::free) (fn void (ptr void)))
(decl (csym::getenv) (fn (ptr char) (ptr (const char))))
(decl (csym::labs) (fn long long))
(decl (csym::ldiv) (fn csym::ldiv_t long long))
(decl (csym::malloc) (fn (ptr void) csym::size_t))
(decl (csym::mblen) (fn int (ptr (const char)) csym::size_t))
(decl (csym::mbstowcs)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const char)) csym::size_t))
(decl (csym::mbtowc)
 (fn int (ptr csym::wchar_t) (ptr (const char)) csym::size_t))
(decl (csym::qsort)
 (fn void (ptr void) csym::size_t csym::size_t
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::rand) (fn int void))
(decl (csym::realloc) (fn (ptr void) (ptr void) csym::size_t))
(decl (csym::srand) (fn void unsigned-int))
(decl (csym::strtod) (fn double (ptr (const char)) (ptr (ptr char))))
(decl (csym::strtof) (fn float (ptr (const char)) (ptr (ptr char))))
(decl (csym::strtol) (fn long (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::strtold) (fn long-double (ptr (const char)) (ptr (ptr char))))
(decl (csym::strtoul)
 (fn unsigned-long (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::system) (fn int (ptr (const char))))
(decl (csym::wctomb) (fn int (ptr char) csym::wchar_t))
(decl (csym::wcstombs)
 (fn csym::size_t (ptr char) (ptr (const csym::wchar_t)) csym::size_t))
(%line 130 "posix-out/stdlib.h")
(def (struct csym::unnamed24) (decl csym::quot long-long)
 (decl csym::rem long-long))
(deftype csym::lldiv_t (struct csym::unnamed24))
(decl (csym::atoll) (fn long-long (ptr (const char))))
(decl (csym::llabs) (fn long-long long-long))
(decl (csym::lldiv) (fn csym::lldiv_t long-long long-long))
(decl (csym::strtoll) (fn long-long (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::strtoull)
 (fn unsigned-long-long (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::_@e@XIT) (fn void int))
(%line 161 "posix-out/stdlib.h")
(decl (csym::rand_r) (fn int (ptr unsigned-int)))
(decl (csym::setenv) (fn int (ptr (const char)) (ptr (const char)) int))
(decl (csym::unsetenv) (fn void (ptr (const char))))
(%line 175 "posix-out/stdlib.h")
(decl (csym::drand48) (fn double void))
(decl (csym::erand48) (fn double (array unsigned-short 3)))
(decl (csym::getsubopt)
 (fn int (ptr (ptr char)) (ptr (const (ptr char))) (ptr (ptr char))))
(decl (csym::grantpt) (fn int int))
(decl (csym::initstate) (fn (ptr char) unsigned-long (ptr char) long))
(decl (csym::jrand48) (fn long (array unsigned-short 3)))
(decl (csym::lcong48) (fn void (array unsigned-short 7)))
(decl (csym::lrand48) (fn long void))
(decl (csym::mkstemp) (fn int (ptr char)))
(decl (csym::mktemp) (fn (ptr char) (ptr char)))
(decl (csym::mrand48) (fn long void))
(decl (csym::nrand48) (fn long (array unsigned-short 3)))
(decl (csym::posix_openpt) (fn int int))
(decl (csym::ptsname) (fn (ptr char) int))
(decl (csym::putenv) (fn int (ptr (const char))))
(decl (csym::random) (fn long void))
(decl (csym::realpath) (fn (ptr char) (ptr (const char)) (array char)))
(decl (csym::seed48) (fn (ptr unsigned-short) (array unsigned-short 3)))
(decl (csym::setkey) (fn int (ptr (const char))))
(decl (csym::setstate) (fn (ptr char) (ptr char)))
(decl (csym::srand48) (fn void long))
(decl (csym::srandom) (fn void unsigned-long))
(decl (csym::unlockpt) (fn int int))
(extern-decl csym::_malloc_options (ptr (const char)))
(extern-decl csym::_malloc_message
 (ptr
  (fn void (ptr (const char)) (ptr (const char)) (ptr (const char))
   (ptr (const char)))))
(%line 235 "posix-out/stdlib.h")
(decl (csym::arc4random) (fn csym::__uint32_t void))
(decl (csym::arc4random_addrandom csym::dat csym::datlen)
 (fn void (ptr unsigned-char) int))
(decl (csym::arc4random_stir) (fn void void))
(decl (csym::getbsize) (fn (ptr char) (ptr int) (ptr long)))
(decl (csym::cgetcap) (fn (ptr char) (ptr char) (ptr (const char)) int))
(decl (csym::cgetclose) (fn int void))
(decl (csym::cgetent)
 (fn int (ptr (ptr char)) (ptr (ptr char)) (ptr (const char))))
(decl (csym::cgetfirst) (fn int (ptr (ptr char)) (ptr (ptr char))))
(decl (csym::cgetmatch) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::cgetnext) (fn int (ptr (ptr char)) (ptr (ptr char))))
(decl (csym::cgetnum) (fn int (ptr char) (ptr (const char)) (ptr long)))
(decl (csym::cgetset) (fn int (ptr (const char))))
(decl (csym::cgetstr) (fn int (ptr char) (ptr (const char)) (ptr (ptr char))))
(decl (csym::cgetustr) (fn int (ptr char) (ptr (const char)) (ptr (ptr char))))
(decl (csym::daemon) (fn int int int))
(decl (csym::devname) (fn (ptr char) int int))
(decl (csym::devname_r) (fn (ptr char) int int (ptr char) int))
(decl (csym::getloadavg) (fn int (array double) int))
(decl (csym::getprogname) (fn (ptr (const char)) void))
(decl (csym::heapsort)
 (fn int (ptr void) csym::size_t csym::size_t
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::mergesort)
 (fn int (ptr void) csym::size_t csym::size_t
  (ptr (fn int (ptr (const void)) (ptr (const void))))))
(decl (csym::qsort_r)
 (fn void (ptr void) csym::size_t csym::size_t (ptr void)
  (ptr (fn int (ptr void) (ptr (const void)) (ptr (const void))))))
(decl (csym::radixsort)
 (fn int (ptr (ptr (const unsigned-char))) int (ptr (const unsigned-char))
  unsigned-int))
(decl (csym::reallocf) (fn (ptr void) (ptr void) csym::size_t))
(decl (csym::setprogname) (fn void (ptr (const char))))
(decl (csym::sradixsort)
 (fn int (ptr (ptr (const unsigned-char))) int (ptr (const unsigned-char))
  unsigned-int))
(decl (csym::sranddev) (fn void void))
(decl (csym::srandomdev) (fn void void))
(decl (csym::strtoq)
 (fn csym::__int64_t (ptr (const char)) (ptr (ptr char)) int))
(decl (csym::strtouq)
 (fn csym::__uint64_t (ptr (const char)) (ptr (ptr char)) int))
(extern-decl csym::suboptarg (ptr char))
(%line 1 "posix-out/string.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/strings.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 40 "/usr/include/strings.h")
(decl (csym::bcmp) (fn int (ptr (const void)) (ptr (const void)) csym::size_t))
(decl (csym::bcopy) (fn void (ptr (const void)) (ptr void) csym::size_t))
(decl (csym::bzero) (fn void (ptr void) csym::size_t))
(decl (csym::ffs) (fn int int))
(decl (csym::ffsl) (fn int long))
(decl (csym::fls) (fn int int))
(decl (csym::flsl) (fn int long))
(decl (csym::index) (fn (ptr char) (ptr (const char)) int))
(decl (csym::rindex) (fn (ptr char) (ptr (const char)) int))
(decl (csym::strcasecmp) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::strncasecmp)
 (fn int (ptr (const char)) (ptr (const char)) csym::size_t))
(%line 57 "posix-out/string.h")
(decl (csym::memccpy)
 (fn (ptr void) (ptr void) (ptr (const void)) int csym::size_t))
(decl (csym::memchr) (fn (ptr void) (ptr (const void)) int csym::size_t))
(decl (csym::memcmp)
 (fn int (ptr (const void)) (ptr (const void)) csym::size_t))
(decl (csym::memcpy) (fn (ptr void) (ptr void) (ptr (const void)) csym::size_t))
(decl (csym::memmove)
 (fn (ptr void) (ptr void) (ptr (const void)) csym::size_t))
(decl (csym::memset) (fn (ptr void) (ptr void) int csym::size_t))
(decl (csym::stpcpy) (fn (ptr char) (ptr char) (ptr (const char))))
(decl (csym::strcasestr) (fn (ptr char) (ptr (const char)) (ptr (const char))))
(decl (csym::strcat) (fn (ptr char) (ptr char) (ptr (const char))))
(decl (csym::strchr) (fn (ptr char) (ptr (const char)) int))
(decl (csym::strcmp) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::strcoll) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::strcpy) (fn (ptr char) (ptr char) (ptr (const char))))
(decl (csym::strcspn) (fn csym::size_t (ptr (const char)) (ptr (const char))))
(decl (csym::strdup) (fn (ptr char) (ptr (const char))))
(decl (csym::strerror) (fn (ptr char) int))
(decl (csym::strerror_r) (fn int int (ptr char) csym::size_t))
(decl (csym::strlcat)
 (fn csym::size_t (ptr char) (ptr (const char)) csym::size_t))
(decl (csym::strlcpy)
 (fn csym::size_t (ptr char) (ptr (const char)) csym::size_t))
(decl (csym::strlen) (fn csym::size_t (ptr (const char))))
(decl (csym::strmode) (fn void int (ptr char)))
(decl (csym::strncat)
 (fn (ptr char) (ptr char) (ptr (const char)) csym::size_t))
(decl (csym::strncmp)
 (fn int (ptr (const char)) (ptr (const char)) csym::size_t))
(decl (csym::strncpy)
 (fn (ptr char) (ptr char) (ptr (const char)) csym::size_t))
(decl (csym::strnstr)
 (fn (ptr char) (ptr (const char)) (ptr (const char)) csym::size_t))
(decl (csym::strpbrk) (fn (ptr char) (ptr (const char)) (ptr (const char))))
(decl (csym::strrchr) (fn (ptr char) (ptr (const char)) int))
(decl (csym::strsep) (fn (ptr char) (ptr (ptr char)) (ptr (const char))))
(decl (csym::strsignal) (fn (ptr char) int))
(decl (csym::strspn) (fn csym::size_t (ptr (const char)) (ptr (const char))))
(decl (csym::strstr) (fn (ptr char) (ptr (const char)) (ptr (const char))))
(decl (csym::strtok) (fn (ptr char) (ptr char) (ptr (const char))))
(decl (csym::strtok_r)
 (fn (ptr char) (ptr char) (ptr (const char)) (ptr (ptr char))))
(decl (csym::strxfrm)
 (fn csym::size_t (ptr char) (ptr (const char)) csym::size_t))
(decl (csym::swab) (fn void (ptr (const void)) (ptr void) csym::size_t))
(%line 1 "posix-out/strings.h")
(%line 1 "posix-out/syslog.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 191 "posix-out/syslog.h")
(decl (csym::closelog) (fn void void))
(decl (csym::openlog) (fn void (ptr (const char)) int int))
(decl (csym::setlogmask) (fn int int))
(decl (csym::syslog) (fn void int (ptr (const char)) va-arg))
(decl (csym::vsyslog) (fn void int (ptr (const char)) csym::__va_list))
(%line 1 "posix-out/tar.h")
(%line 1 "posix-out/tcp.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 40 "posix-out/tcp.h")
(deftype csym::tcp_seq csym::u_int32_t)
(deftype csym::tcp_cc csym::u_int32_t)
(%line 50 "posix-out/tcp.h")
(def (struct csym::tcphdr) (decl csym::th_sport csym::u_short)
 (decl csym::th_dport csym::u_short) (decl csym::th_seq csym::tcp_seq)
 (decl csym::th_ack csym::tcp_seq) (decl csym::th_x2 csym::u_int) :bit 4
 (decl csym::th_off csym::u_int) :bit 4 (decl csym::th_flags csym::u_char)
 (decl csym::th_win csym::u_short) (decl csym::th_sum csym::u_short)
 (decl csym::th_urp csym::u_short))
(%line 63 "posix-out/tcp.h")
(%line 1 "posix-out/termios.h")
(%line 178 "posix-out/termios.h")
(deftype csym::tcflag_t unsigned-int)
(deftype csym::cc_t unsigned-char)
(deftype csym::speed_t unsigned-int)
(def (struct csym::termios) (decl csym::c_iflag csym::tcflag_t)
 (decl csym::c_oflag csym::tcflag_t) (decl csym::c_cflag csym::tcflag_t)
 (decl csym::c_lflag csym::tcflag_t) (decl csym::c_cc (array csym::cc_t 20))
 (decl csym::c_ispeed csym::speed_t) (decl csym::c_ospeed csym::speed_t))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 247 "posix-out/termios.h")
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
(%line 1 "/usr/include/sys/ttydefaults.h")
(%line 1 "posix-out/tgmath.h")
(%line 1 "/usr/include/complex.h")
(%line 1 "/usr/include/math.h")
(%line 1 "posix-out/time.h")
(%line 1 "posix-out/timeb.h")
(%line 42 "posix-out/timeb.h")
(def (struct csym::timeb) (decl csym::time double)
 (decl csym::millitm unsigned-short) (decl csym::timezone short)
 (decl csym::dstflag short))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 52 "posix-out/timeb.h")
(decl (csym::ftime) (fn int (ptr (struct csym::timeb))))
(%line 1 "posix-out/times.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 48 "posix-out/times.h")
(def (struct csym::tms) (decl csym::tms_utime csym::clock_t)
 (decl csym::tms_stime csym::clock_t) (decl csym::tms_cutime csym::clock_t)
 (decl csym::tms_cstime csym::clock_t))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 58 "posix-out/times.h")
(decl (csym::times) (fn csym::clock_t (ptr (struct csym::tms))))
(%line 1 "posix-out/types.h")
(%line 1 "posix-out/ucontext.h")
(%line 1 "/usr/include/sys/signal.h")
(%line 1 "/usr/include/machine/ucontext.h")
(%line 34 "/usr/include/machine/ucontext.h")
(def (struct csym::__mcontext) (decl csym::mc_onstack int)
 (decl csym::mc_gs int) (decl csym::mc_fs int) (decl csym::mc_es int)
 (decl csym::mc_ds int) (decl csym::mc_edi int) (decl csym::mc_esi int)
 (decl csym::mc_ebp int) (decl csym::mc_isp int) (decl csym::mc_ebx int)
 (decl csym::mc_edx int) (decl csym::mc_ecx int) (decl csym::mc_eax int)
 (decl csym::mc_trapno int) (decl csym::mc_err int) (decl csym::mc_eip int)
 (decl csym::mc_cs int) (decl csym::mc_eflags int) (decl csym::mc_esp int)
 (decl csym::mc_ss int) (decl csym::mc_len int) (decl csym::mc_fpformat int)
 (decl csym::mc_ownedfp int) (decl csym::mc_spare1 (array int 1))
 (decl csym::mc_fpstate (array int 128)) (decl csym::mc_spare2 (array int 8)))
(deftype csym::mcontext_t (struct csym::__mcontext))
(%line 40 "/usr/include/machine/ucontext.h")
(%line 74 "/usr/include/machine/ucontext.h")
(%line 37 "posix-out/ucontext.h")
(def (struct csym::__ucontext) (decl csym::uc_sigmask csym::sigset_t)
 (decl csym::uc_mcontext csym::mcontext_t)
 (decl csym::uc_link (ptr (struct csym::__ucontext)))
 (decl csym::uc_stack csym::stack_t) (decl csym::uc_flags int)
 (decl csym::__spare__ (array int 4)))
(deftype csym::ucontext_t (struct csym::__ucontext))
(%line 46 "posix-out/ucontext.h")
(%line 72 "posix-out/ucontext.h")
(decl (csym::getcontext) (fn int (ptr csym::ucontext_t)))
(decl (csym::setcontext) (fn int (ptr (const csym::ucontext_t))))
(decl (csym::makecontext)
 (fn void (ptr csym::ucontext_t) (ptr (fn void void)) int va-arg))
(decl (csym::signalcontext)
 (fn int (ptr csym::ucontext_t) int (ptr csym::__sighandler_t)))
(decl (csym::swapcontext)
 (fn int (ptr csym::ucontext_t) (ptr (const csym::ucontext_t))))
(%line 1 "posix-out/uio.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/_iovec.h")
(%line 46 "posix-out/uio.h")
(def (enum csym::uio_rw) (csym::@UIO_READ csym::@UIO_WRITE))
(def (enum csym::uio_seg)
 (csym::@UIO_USERSPACE csym::@UIO_SYSSPACE csym::@UIO_NOCOPY))
(%line 102 "posix-out/uio.h")
(decl (csym::readv)
 (fn csym::ssize_t int (ptr (const (struct csym::iovec))) int))
(decl (csym::writev)
 (fn csym::ssize_t int (ptr (const (struct csym::iovec))) int))
(%line 1 "posix-out/ulimit.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 37 "posix-out/ulimit.h")
(decl (csym::ulimit) (fn long int va-arg))
(%line 1 "posix-out/un.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 47 "posix-out/un.h")
(def (struct csym::sockaddr_un) (decl csym::sun_len unsigned-char)
 (decl csym::sun_family csym::sa_family_t)
 (decl csym::sun_path (array char 104)))
(%line 1 "posix-out/unistd.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/unistd.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 318 "posix-out/unistd.h")
(decl (csym::_exit) (fn void int))
(decl (csym::access) (fn int (ptr (const char)) int))
(decl (csym::alarm) (fn unsigned-int unsigned-int))
(decl (csym::chdir) (fn int (ptr (const char))))
(decl (csym::chown) (fn int (ptr (const char)) csym::uid_t csym::gid_t))
(decl (csym::close) (fn int int))
(decl (csym::dup) (fn int int))
(decl (csym::dup2) (fn int int int))
(decl (csym::eaccess) (fn int (ptr (const char)) int))
(decl (csym::execl) (fn int (ptr (const char)) (ptr (const char)) va-arg))
(decl (csym::execle) (fn int (ptr (const char)) (ptr (const char)) va-arg))
(decl (csym::execlp) (fn int (ptr (const char)) (ptr (const char)) va-arg))
(decl (csym::execv) (fn int (ptr (const char)) (ptr (const (ptr char)))))
(decl (csym::execve)
 (fn int (ptr (const char)) (ptr (const (ptr char))) (ptr (const (ptr char)))))
(decl (csym::execvp) (fn int (ptr (const char)) (ptr (const (ptr char)))))
(decl (csym::fork) (fn long void))
(decl (csym::fpathconf) (fn long int int))
(decl (csym::getcwd) (fn (ptr char) (ptr char) csym::size_t))
(decl (csym::getegid) (fn csym::gid_t void))
(decl (csym::geteuid) (fn csym::uid_t void))
(decl (csym::getgid) (fn csym::gid_t void))
(decl (csym::getgroups) (fn int int (array csym::gid_t)))
(decl (csym::getlogin) (fn (ptr char) void))
(decl (csym::getpgrp) (fn long void))
(decl (csym::getpid) (fn long void))
(decl (csym::getppid) (fn long void))
(decl (csym::getuid) (fn csym::uid_t void))
(decl (csym::isatty) (fn int int))
(decl (csym::link) (fn int (ptr (const char)) (ptr (const char))))
(%line 353 "posix-out/unistd.h")
(decl (csym::pathconf) (fn long (ptr (const char)) int))
(decl (csym::pause) (fn int void))
(decl (csym::pipe) (fn int (ptr int)))
(decl (csym::read) (fn csym::ssize_t int (ptr void) csym::size_t))
(decl (csym::rmdir) (fn int (ptr (const char))))
(decl (csym::setgid) (fn int csym::gid_t))
(decl (csym::setpgid) (fn int long long))
(decl (csym::setproctitle csym::_fmt) (fn void (ptr (const char)) va-arg))
(decl (csym::setsid) (fn long void))
(decl (csym::setuid) (fn int csym::uid_t))
(decl (csym::sleep) (fn unsigned-int unsigned-int))
(decl (csym::sysconf) (fn long int))
(decl (csym::tcgetpgrp) (fn long int))
(decl (csym::tcsetpgrp) (fn int int long))
(decl (csym::ttyname) (fn (ptr char) int))
(decl (csym::unlink) (fn int (ptr (const char))))
(decl (csym::write) (fn csym::ssize_t int (ptr (const void)) csym::size_t))
(decl (csym::confstr) (fn csym::size_t int (ptr char) csym::size_t))
(decl (csym::getopt) (fn int int (array (const (ptr char))) (ptr (const char))))
(extern-decl csym::optarg (ptr char))
(extern-decl csym::optind int)
(extern-decl csym::opterr int)
(extern-decl csym::optopt int)
(decl (csym::fsync) (fn int int))
(%line 398 "posix-out/unistd.h")
(decl (csym::getlogin_r) (fn int (ptr char) int))
(decl (csym::fchown) (fn int int csym::uid_t csym::gid_t))
(decl (csym::readlink) (fn int (ptr (const char)) (ptr char) int))
(decl (csym::gethostname) (fn int (ptr char) csym::size_t))
(decl (csym::setegid) (fn int csym::gid_t))
(decl (csym::seteuid) (fn int csym::uid_t))
(%line 418 "posix-out/unistd.h")
(decl (csym::symlink) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::crypt) (fn (ptr char) (ptr (const char)) (ptr (const char))))
(decl (csym::encrypt) (fn int (ptr char) int))
(decl (csym::fchdir) (fn int int))
(decl (csym::gethostid) (fn long void))
(decl (csym::getpgid csym::_pid) (fn int long))
(decl (csym::getsid csym::_pid) (fn int long))
(decl (csym::getwd) (fn (ptr char) (ptr char)))
(decl (csym::lchown) (fn int (ptr (const char)) csym::uid_t csym::gid_t))
(decl (csym::lockf) (fn int int int csym::off_t))
(decl (csym::nice) (fn int int))
(decl (csym::pread) (fn csym::ssize_t int (ptr void) csym::size_t csym::off_t))
(decl (csym::pwrite)
 (fn csym::ssize_t int (ptr (const void)) csym::size_t csym::off_t))
(decl (csym::setpgrp csym::_pid csym::_pgrp) (fn int long long))
(decl (csym::setregid) (fn int csym::gid_t csym::gid_t))
(decl (csym::setreuid) (fn int csym::uid_t csym::uid_t))
(decl (csym::sync) (fn void void))
(decl (csym::ualarm) (fn csym::useconds_t csym::useconds_t csym::useconds_t))
(decl (csym::usleep) (fn int csym::useconds_t))
(decl (csym::vfork) (fn long void))
(%line 453 "posix-out/unistd.h")
(decl (csym::brk) (fn int (ptr (const void))))
(decl (csym::chroot) (fn int (ptr (const char))))
(decl (csym::getdtablesize) (fn int void))
(decl (csym::getpagesize) (fn int void))
(decl (csym::getpass) (fn (ptr char) (ptr (const char))))
(decl (csym::sbrk) (fn (ptr void) csym::intptr_t))
(decl (csym::acct) (fn int (ptr (const char))))
(decl (csym::async_daemon) (fn int void))
(decl (csym::check_utility_compat) (fn int (ptr (const char))))
(decl (csym::crypt_get_format) (fn (ptr (const char)) void))
(decl (csym::crypt_set_format) (fn int (ptr (const char))))
(decl (csym::des_cipher) (fn int (ptr (const char)) (ptr char) long int))
(decl (csym::des_setkey csym::key) (fn int (ptr (const char))))
(decl (csym::endusershell) (fn void void))
(decl (csym::exect)
 (fn int (ptr (const char)) (ptr (const (ptr char))) (ptr (const (ptr char)))))
(decl (csym::EXECV@p)
 (fn int (ptr (const char)) (ptr (const char)) (ptr (const (ptr char)))))
(decl (csym::fflagstostr) (fn (ptr char) csym::u_long))
(decl (csym::getdomainname) (fn int (ptr char) int))
(decl (csym::getgrouplist)
 (fn int (ptr (const char)) csym::gid_t (ptr csym::gid_t) (ptr int)))
(decl (csym::getmode) (fn csym::mode_t (ptr (const void)) csym::mode_t))
(decl (csym::getpeereid) (fn int int (ptr csym::uid_t) (ptr csym::gid_t)))
(decl (csym::getresgid)
 (fn int (ptr csym::gid_t) (ptr csym::gid_t) (ptr csym::gid_t)))
(decl (csym::getresuid)
 (fn int (ptr csym::uid_t) (ptr csym::uid_t) (ptr csym::uid_t)))
(decl (csym::getusershell) (fn (ptr char) void))
(decl (csym::initgroups) (fn int (ptr (const char)) csym::gid_t))
(decl (csym::iruserok)
 (fn int unsigned-long int (ptr (const char)) (ptr (const char))))
(decl (csym::iruserok_sa)
 (fn int (ptr (const void)) int int (ptr (const char)) (ptr (const char))))
(decl (csym::issetugid) (fn int void))
(decl (csym::mkdtemp) (fn (ptr char) (ptr char)))
(decl (csym::mknod) (fn int (ptr (const char)) csym::mode_t csym::dev_t))
(%line 492 "posix-out/unistd.h")
(decl (csym::mkstemps) (fn int (ptr char) int))
(%line 497 "posix-out/unistd.h")
(decl (csym::nfsclnt) (fn int int (ptr void)))
(decl (csym::nfssvc) (fn int int (ptr void)))
(decl (csym::profil) (fn int (ptr char) csym::size_t csym::vm_offset_t int))
(decl (csym::rcmd)
 (fn int (ptr (ptr char)) int (ptr (const char)) (ptr (const char))
  (ptr (const char)) (ptr int)))
(decl (csym::rcmd_af)
 (fn int (ptr (ptr char)) int (ptr (const char)) (ptr (const char))
  (ptr (const char)) (ptr int) int))
(decl (csym::rcmdsh)
 (fn int (ptr (ptr char)) int (ptr (const char)) (ptr (const char))
  (ptr (const char)) (ptr (const char))))
(decl (csym::re_comp) (fn (ptr char) (ptr (const char))))
(decl (csym::re_exec) (fn int (ptr (const char))))
(decl (csym::reboot) (fn int int))
(decl (csym::revoke) (fn int (ptr (const char))))
(decl (csym::rfork) (fn long int))
(decl (csym::rfork_thread)
 (fn long int (ptr void) (ptr (fn int (ptr void))) (ptr void)))
(decl (csym::rresvport) (fn int (ptr int)))
(decl (csym::rresvport_af) (fn int (ptr int) int))
(decl (csym::ruserok)
 (fn int (ptr (const char)) int (ptr (const char)) (ptr (const char))))
(%line 520 "posix-out/unistd.h")
(decl (csym::setdomainname) (fn int (ptr (const char)) int))
(decl (csym::setgroups) (fn int int (ptr (const csym::gid_t))))
(decl (csym::sethostid) (fn void long))
(decl (csym::sethostname) (fn int (ptr (const char)) int))
(%line 528 "posix-out/unistd.h")
(decl (csym::setlogin) (fn int (ptr (const char))))
(decl (csym::setmode) (fn (ptr void) (ptr (const char))))
(decl (csym::setresgid) (fn int csym::gid_t csym::gid_t csym::gid_t))
(decl (csym::setresuid) (fn int csym::uid_t csym::uid_t csym::uid_t))
(decl (csym::setrgid) (fn int csym::gid_t))
(decl (csym::setruid) (fn int csym::uid_t))
(decl (csym::setusershell) (fn void void))
(decl (csym::strtofflags)
 (fn int (ptr (ptr char)) (ptr csym::u_long) (ptr csym::u_long)))
(decl (csym::swapon) (fn int (ptr (const char))))
(decl (csym::swapoff) (fn int (ptr (const char))))
(decl (csym::syscall) (fn int int va-arg))
(decl (csym::__syscall) (fn csym::off_t csym::quad_t va-arg))
(decl (csym::ttyslot) (fn int void))
(decl (csym::undelete) (fn int (ptr (const char))))
(decl (csym::unwhiteout) (fn int (ptr (const char))))
(decl (csym::valloc) (fn (ptr void) csym::size_t))
(extern-decl csym::optreset int)
(%line 1 "posix-out/utime.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 48 "posix-out/utime.h")
(def (struct csym::utimbuf) (decl csym::actime double)
 (decl csym::modtime double))
(decl (csym::utime)
 (fn int (ptr (const char)) (ptr (const (struct csym::utimbuf)))))
(%line 1 "posix-out/utsname.h")
(%line 47 "posix-out/utsname.h")
(def (struct csym::utsname) (decl csym::sysname (array char 256))
 (decl csym::nodename (array char 256)) (decl csym::release (array char 256))
 (decl csym::version (array char 256)) (decl csym::machine (array char 256)))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 58 "posix-out/utsname.h")
(decl (csym::__xuname) (fn int int (ptr void)))
(static-def (csym::uname csym::name) (fn int (ptr (struct csym::utsname)))
 (return (csym::__xuname 256 (cast (ptr void) csym::name))))
(%line 1 "posix-out/wait.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/types.h")
(%line 102 "posix-out/wait.h")
(decl (csym::wait) (fn long (ptr int)))
(decl (csym::waitpid) (fn long long (ptr int) int))
(decl (csym::wait3) (fn long (ptr int) int (ptr (struct csym::rusage))))
(decl (csym::wait4) (fn long long (ptr int) int (ptr (struct csym::rusage))))
(%line 1 "posix-out/wchar.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/machine/_limits.h")
(%line 1 "/usr/include/_ctype.h")
(%line 77 "posix-out/wchar.h")
(deftype csym::mbstate_t csym::__mbstate_t)
(%line 94 "posix-out/wchar.h")
(deftype csym::wint_t csym::__wint_t)
(%line 107 "posix-out/wchar.h")
(decl (csym::btowc) (fn csym::wint_t int))
(decl (csym::fgetwc) (fn csym::wint_t (ptr (struct csym::__S@file))))
(decl (csym::fgetws)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) int (ptr (struct csym::__S@file))))
(decl (csym::fputwc)
 (fn csym::wint_t csym::wchar_t (ptr (struct csym::__S@file))))
(decl (csym::fputws)
 (fn int (ptr (const csym::wchar_t)) (ptr (struct csym::__S@file))))
(decl (csym::fwide) (fn int (ptr (struct csym::__S@file)) int))
(decl (csym::fwprintf)
 (fn int (ptr (struct csym::__S@file)) (ptr (const csym::wchar_t)) va-arg))
(decl (csym::fwscanf)
 (fn int (ptr (struct csym::__S@file)) (ptr (const csym::wchar_t)) va-arg))
(decl (csym::getwc) (fn csym::wint_t (ptr (struct csym::__S@file))))
(decl (csym::getwchar) (fn csym::wint_t void))
(decl (csym::mbrlen)
 (fn csym::size_t (ptr (const char)) csym::size_t (ptr csym::mbstate_t)))
(decl (csym::mbrtowc)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const char)) csym::size_t
  (ptr csym::mbstate_t)))
(decl (csym::mbsinit) (fn int (ptr (const csym::mbstate_t))))
(decl (csym::mbsrtowcs)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (ptr (const char))) csym::size_t
  (ptr csym::mbstate_t)))
(decl (csym::putwc)
 (fn csym::wint_t csym::wchar_t (ptr (struct csym::__S@file))))
(decl (csym::putwchar) (fn csym::wint_t csym::wchar_t))
(decl (csym::swprintf)
 (fn int (ptr csym::wchar_t) csym::size_t (ptr (const csym::wchar_t)) va-arg))
(decl (csym::swscanf)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t)) va-arg))
(decl (csym::ungetwc)
 (fn csym::wint_t csym::wint_t (ptr (struct csym::__S@file))))
(decl (csym::vfwprintf)
 (fn int (ptr (struct csym::__S@file)) (ptr (const csym::wchar_t))
  csym::__va_list))
(decl (csym::vswprintf)
 (fn int (ptr csym::wchar_t) csym::size_t (ptr (const csym::wchar_t))
  csym::__va_list))
(decl (csym::vwprintf) (fn int (ptr (const csym::wchar_t)) csym::__va_list))
(decl (csym::wcrtomb)
 (fn csym::size_t (ptr char) csym::wchar_t (ptr csym::mbstate_t)))
(decl (csym::wcscat)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))))
(decl (csym::wcschr)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::wchar_t))
(decl (csym::wcscmp)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))))
(decl (csym::wcscoll)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))))
(decl (csym::wcscpy)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))))
(decl (csym::wcscspn)
 (fn csym::size_t (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))))
(decl (csym::wcsftime)
 (fn csym::size_t (ptr csym::wchar_t) csym::size_t (ptr (const csym::wchar_t))
  (ptr (const (struct csym::tm)))))
(decl (csym::wcslen) (fn csym::size_t (ptr (const csym::wchar_t))))
(decl (csym::wcsncat)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  csym::size_t))
(decl (csym::wcsncmp)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wcsncpy)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  csym::size_t))
(decl (csym::wcspbrk)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  (ptr (const csym::wchar_t))))
(decl (csym::wcsrchr)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::wchar_t))
(decl (csym::wcsrtombs)
 (fn csym::size_t (ptr char) (ptr (ptr (const csym::wchar_t))) csym::size_t
  (ptr csym::mbstate_t)))
(decl (csym::wcsspn)
 (fn csym::size_t (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))))
(decl (csym::wcsstr)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  (ptr (const csym::wchar_t))))
(decl (csym::wcsxfrm)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wctob) (fn int csym::wint_t))
(decl (csym::wcstod)
 (fn double (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t))))
(decl (csym::wcstok)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  (ptr (ptr csym::wchar_t))))
(decl (csym::wcstol)
 (fn long (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t)) int))
(decl (csym::wcstoul)
 (fn unsigned-long (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t)) int))
(decl (csym::wmemchr)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::wchar_t
  csym::size_t))
(decl (csym::wmemcmp)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wmemcpy)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  csym::size_t))
(decl (csym::wmemmove)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  csym::size_t))
(decl (csym::wmemset)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) csym::wchar_t csym::size_t))
(decl (csym::wprintf) (fn int (ptr (const csym::wchar_t)) va-arg))
(decl (csym::wscanf) (fn int (ptr (const csym::wchar_t)) va-arg))
(%line 189 "posix-out/wchar.h")
(decl (csym::vfwscanf)
 (fn int (ptr (struct csym::__S@file)) (ptr (const csym::wchar_t))
  csym::__va_list))
(decl (csym::vswscanf)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))
  csym::__va_list))
(decl (csym::vwscanf) (fn int (ptr (const csym::wchar_t)) csym::__va_list))
(decl (csym::wcstof)
 (fn float (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t))))
(decl (csym::wcstold)
 (fn long-double (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t))))
(decl (csym::wcstoll)
 (fn long-long (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t)) int))
(decl (csym::wcstoull)
 (fn unsigned-long-long (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t))
  int))
(decl (csym::wcswidth) (fn int (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wcwidth) (fn int csym::wchar_t))
(decl (csym::fgetwln)
 (fn (ptr csym::wchar_t) (ptr (struct csym::__S@file)) (ptr csym::size_t)))
(decl (csym::mbsnrtowcs)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (ptr (const char))) csym::size_t
  csym::size_t (ptr csym::mbstate_t)))
(decl (csym::wcsnrtombs)
 (fn csym::size_t (ptr char) (ptr (ptr (const csym::wchar_t))) csym::size_t
  csym::size_t (ptr csym::mbstate_t)))
(decl (csym::wcslcat)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wcslcpy)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::size_t))
(%line 1 "posix-out/wctype.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/_ctype.h")
(%line 40 "posix-out/wctype.h")
(deftype csym::wctrans_t int)
(deftype csym::wctype_t unsigned-long)
(%line 58 "posix-out/wctype.h")
(decl (csym::iswalnum) (fn int csym::wint_t))
(decl (csym::iswalpha) (fn int csym::wint_t))
(decl (csym::iswblank) (fn int csym::wint_t))
(decl (csym::iswcntrl) (fn int csym::wint_t))
(decl (csym::iswctype) (fn int csym::wint_t csym::wctype_t))
(decl (csym::iswdigit) (fn int csym::wint_t))
(decl (csym::iswgraph) (fn int csym::wint_t))
(decl (csym::iswlower) (fn int csym::wint_t))
(decl (csym::iswprint) (fn int csym::wint_t))
(decl (csym::iswpunct) (fn int csym::wint_t))
(decl (csym::iswspace) (fn int csym::wint_t))
(decl (csym::iswupper) (fn int csym::wint_t))
(decl (csym::iswxdigit) (fn int csym::wint_t))
(decl (csym::towctrans) (fn csym::wint_t csym::wint_t csym::wctrans_t))
(decl (csym::towlower) (fn csym::wint_t csym::wint_t))
(decl (csym::towupper) (fn csym::wint_t csym::wint_t))
(decl (csym::wctrans) (fn csym::wctrans_t (ptr (const char))))
(decl (csym::wctype) (fn csym::wctype_t (ptr (const char))))
(decl (csym::iswascii) (fn csym::wint_t csym::wint_t))
(decl (csym::iswhexnumber) (fn csym::wint_t csym::wint_t))
(decl (csym::iswideogram) (fn csym::wint_t csym::wint_t))
(decl (csym::iswnumber) (fn csym::wint_t csym::wint_t))
(decl (csym::iswphonogram) (fn csym::wint_t csym::wint_t))
(decl (csym::iswrune) (fn csym::wint_t csym::wint_t))
(decl (csym::iswspecial) (fn csym::wint_t csym::wint_t))
(decl (csym::nextwctype) (fn csym::wint_t csym::wint_t csym::wctype_t))
(%line 1 "posix-out/wordexp.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 40 "posix-out/wordexp.h")
(def (struct csym::unnamed25) (decl csym::we_wordc csym::__size_t)
 (decl csym::we_wordv (ptr (ptr char))) (decl csym::we_offs csym::__size_t)
 (decl csym::we_strings (ptr char)) (decl csym::we_nbytes csym::__size_t))
(deftype csym::wordexp_t (struct csym::unnamed25))
(%line 70 "posix-out/wordexp.h")
(decl (csym::wordexp) (fn int (ptr (const char)) (ptr csym::wordexp_t) int))
(decl (csym::wordfree) (fn void (ptr csym::wordexp_t)))
(%undef csym::__@KAME_NETINET_IN_H_INCLUDED_)
(%undef csym::_@NO_NAMESPACE_POLLUTION)
(%defconstant csym::time_t double)
(%defconstant csym::pid_t long)
(%defconstant csym::__complex__ (c-exp ""))
(%defmacro csym::__extension__ (common-lisp-user::x) 'csym::x)
(%defmacro csym::__attribute__ (common-lisp-user::x)
 (common-lisp:list 'c-exp "" common-lisp-user::x))
(%defconstant csym::__inline (c-exp ""))
(%defmacro csym::__asm (common-lisp-user::x)
 (common-lisp:list 'c-exp "" common-lisp-user::x))
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
(%defconstant csym::__@DATE__ "Apr  4 2005")
(%defconstant csym::__@TIME__ "17:10:49")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::_@SYS_AIO_H_ (c-exp ""))
(%defconstant csym::_@SYS_TIME_H_ (c-exp ""))
(%defconstant csym::_@SYS__TIMEVAL_H_ (c-exp ""))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@SYS_CDEFS_H_ (c-exp ""))
(%defconstant csym::__@BEGIN_DECLS (c-exp ""))
(%defconstant csym::__@END_DECLS (c-exp ""))
(%defmacro csym::__@GNUC_PREREQ__ (common-lisp-user::ma common-lisp-user::mi)
 (common-lisp:list 'or
                   (common-lisp:list '> '3 'csym::ma)
                   (common-lisp:list 'and
                                     (common-lisp:list '== '3 'csym::ma)
                                     (common-lisp:list '>= '4 'csym::mi))))
(%defmacro csym::__@P (common-lisp-user::protos) 'csym::protos)
(%defmacro csym::__@CONCAT1 (common-lisp-user::x common-lisp-user::y)
 '"<macro with ## operator>")
(%defmacro csym::__@CONCAT (common-lisp-user::x common-lisp-user::y) 'csym::xy)
(%defmacro csym::__@STRING (common-lisp-user::x)
 (common-lisp:list 'csym::_@C2SC_STRINGIFY 'csym::x))
(%defmacro csym::__@XSTRING (common-lisp-user::x) '"x")
(%defconstant csym::__const (const int))
(%defconstant csym::__signed signed-int)
(%defconstant csym::__volatile (volatile int))
(%defconstant csym::__dead2 (c-exp ""))
(%defconstant csym::__pure2 (c-exp ""))
(%defconstant csym::__unused (c-exp ""))
(%defconstant csym::__used (c-exp ""))
(%defconstant csym::__packed (c-exp ""))
(%defmacro csym::__aligned (common-lisp-user::x)
 (common-lisp:list 'c-exp "" common-lisp-user::x))
(%defmacro csym::__section (common-lisp-user::x)
 (common-lisp:list 'c-exp "" common-lisp-user::x))
(%defconstant csym::__pure (c-exp ""))
(%defconstant csym::__always_inline (c-exp ""))
(%defmacro csym::__nonnull (common-lisp-user::x)
 (common-lisp:list 'c-exp "" common-lisp-user::x))
(%defconstant csym::__@LONG_LONG_SUPPORTED (c-exp ""))
(%defconstant csym::__restrict (c-exp ""))
(%defmacro csym::__predict_true (common-lisp:exp)
 (common-lisp:list 'csym::__builtin_expect 'csym::exp '1))
(%defmacro csym::__predict_false (common-lisp:exp)
 (common-lisp:list 'csym::__builtin_expect 'csym::exp '0))
(%defmacro csym::__offsetof (common-lisp:type common-lisp-user::field)
 (common-lisp:list 'cast
                   'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           'csym::type)
                                                                          '0))
                                                       'csym::field))))
(%defmacro csym::__rangeof
 (common-lisp:type common-lisp-user::start common-lisp-user::end)
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
                                                                            'csym::type)
                                                                           '0))
                                                                         'csym::end)))
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
                                                                            'csym::type)
                                                                           '0))
                                                                         'csym::start)))))
(%defmacro csym::__printflike
 (common-lisp-user::fmtarg common-lisp-user::firstvararg)
 (common-lisp:list 'c-exp
                   ""
                   common-lisp-user::fmtarg
                   common-lisp-user::firstvararg))
(%defmacro csym::__scanflike
 (common-lisp-user::fmtarg common-lisp-user::firstvararg)
 (common-lisp:list 'c-exp
                   ""
                   common-lisp-user::fmtarg
                   common-lisp-user::firstvararg))
(%defmacro csym::__printf0like
 (common-lisp-user::fmtarg common-lisp-user::firstvararg)
 (common-lisp:list 'c-exp
                   ""
                   common-lisp-user::fmtarg
                   common-lisp-user::firstvararg))
(%defmacro csym::__strong_reference
 (common-lisp-user::sym common-lisp-user::aliassym)
 (common-lisp:list 'c-exp
                   "extern __typeof ( ~0@*~a ) ~1@*~a ;"
                   common-lisp-user::sym
                   common-lisp-user::aliassym))
(%defmacro csym::__weak_reference
 (common-lisp-user::sym common-lisp-user::alias)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".weak \" _C2SC_STRINGIFY ( ~1@*~a ) ) ; __asm__ ( \".equ \" _C2SC_STRINGIFY ( ~1@*~a ) \", \" _C2SC_STRINGIFY ( ~0@*~a ) )"
                   common-lisp-user::sym
                   common-lisp-user::alias))
(%defmacro csym::__warn_references
 (common-lisp-user::sym common-lisp-user::msg)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".section .gnu.warning.\" _C2SC_STRINGIFY ( ~0@*~a ) ) ; __asm__ ( \".asciz \\\"\" ~1@*~a \"\\\"\" ) ; __asm__ ( \".previous\" )"
                   common-lisp-user::sym
                   common-lisp-user::msg))
(%defmacro csym::__@IDSTRING (common-lisp-user::name common-lisp:string)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~1@*~a \"\\\"\" )"
                   common-lisp-user::name
                   common-lisp:string))
(%defmacro csym::__@FBSDID (common-lisp-user::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   common-lisp-user::s))
(%defmacro csym::__@RCSID (common-lisp-user::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   common-lisp-user::s))
(%defmacro csym::__@RCSID_SOURCE (common-lisp-user::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   common-lisp-user::s))
(%defmacro csym::__@SCCSID (common-lisp-user::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   common-lisp-user::s))
(%defmacro csym::__@COPYRIGHT (common-lisp-user::s)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~0@*~a \"\\\"\" )"
                   common-lisp-user::s))
(%defmacro csym::__@DECONST (common-lisp:type common-lisp-user::var)
 (common-lisp:list 'cast
                   'csym::type
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'void))
                                                       'csym::var))))
(%defmacro csym::__@DEVOLATILE (common-lisp:type common-lisp-user::var)
 (common-lisp:list 'cast
                   'csym::type
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'volatile
                                                                          'void))
                                                       'csym::var))))
(%defmacro csym::__@DEQUALIFY (common-lisp:type common-lisp-user::var)
 (common-lisp:list 'cast
                   'csym::type
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'volatile
                                                                          'void))
                                                       'csym::var))))
(%defconstant csym::__@POSIX_VISIBLE 200112)
(%defconstant csym::__@XSI_VISIBLE 600)
(%defconstant csym::__@BSD_VISIBLE 1)
(%defconstant csym::__@ISO_C_VISIBLE 1999)
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
(%defconstant csym::_@SUSECONDS_T_DECLARED (c-exp ""))
(%defconstant csym::_@TIME_T_DECLARED (c-exp ""))
(%defconstant csym::_@SYS_TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE_ENDIAN_H_ (c-exp ""))
(%defconstant csym::_@QUAD_HIGHWORD 1)
(%defconstant csym::_@QUAD_LOWWORD 0)
(%defconstant csym::_@LITTLE_ENDIAN 1234)
(%defconstant csym::_@BIG_ENDIAN 4321)
(%defconstant csym::_@PDP_ENDIAN 3412)
(%defconstant csym::_@BYTE_ORDER 1234)
(%defconstant csym::@LITTLE_ENDIAN 1234)
(%defconstant csym::@BIG_ENDIAN 4321)
(%defconstant csym::@PDP_ENDIAN 3412)
(%defconstant csym::@BYTE_ORDER 1234)
(%defmacro csym::__word_swap_int_var (common-lisp-user::x)
 (common-lisp:list
  (common-lisp:list 'begin
                    (common-lisp:list 'register-def
                                      'csym::__@X
                                      'csym::__uint32_t
                                      'csym::x)
                    'common-lisp:list
                    'common-lisp:nil
                    'csym::__@X)))
(%defmacro csym::__word_swap_int (common-lisp-user::x)
 (common-lisp:list
  (common-lisp:list 'begin
                    (common-lisp:list 'register-def
                                      'csym::__@X
                                      'csym::__uint32_t
                                      'csym::x)
                    'common-lisp:list
                    'common-lisp:nil
                    'csym::__@X)))
(%defmacro csym::__byte_swap_int_var (common-lisp-user::x)
 (common-lisp:list
  (common-lisp:list 'begin
                    (common-lisp:list 'register-def
                                      'csym::__@X
                                      'csym::__uint32_t
                                      'csym::x)
                    'common-lisp:list
                    'common-lisp:nil
                    'csym::__@X)))
(%defmacro csym::__byte_swap_int (common-lisp-user::x)
 (common-lisp:list
  (common-lisp:list 'begin
                    (common-lisp:list 'register-def
                                      'csym::__@X
                                      'csym::__uint32_t
                                      'csym::x)
                    'common-lisp:list
                    'common-lisp:nil
                    'csym::__@X)))
(%defmacro csym::__byte_swap_word_var (common-lisp-user::x)
 (common-lisp:list
  (common-lisp:list 'begin
                    (common-lisp:list 'register-def
                                      'csym::__@X
                                      'csym::__uint16_t
                                      'csym::x)
                    'common-lisp:list
                    'common-lisp:nil
                    'csym::__@X)))
(%defmacro csym::__byte_swap_word (common-lisp-user::x)
 (common-lisp:list
  (common-lisp:list 'begin
                    (common-lisp:list 'register-def
                                      'csym::__@X
                                      'csym::__uint16_t
                                      'csym::x)
                    'common-lisp:list
                    'common-lisp:nil
                    'csym::__@X)))
(%defmacro csym::__htonl (common-lisp-user::x)
 (common-lisp:list 'csym::__bswap32 'csym::x))
(%defmacro csym::__htons (common-lisp-user::x)
 (common-lisp:list 'csym::__bswap16 'csym::x))
(%defmacro csym::__ntohl (common-lisp-user::x)
 (common-lisp:list 'csym::__bswap32 'csym::x))
(%defmacro csym::__ntohs (common-lisp-user::x)
 (common-lisp:list 'csym::__bswap16 'csym::x))
(%defconstant csym::_@INT8_T_DECLARED (c-exp ""))
(%defconstant csym::_@INT16_T_DECLARED (c-exp ""))
(%defconstant csym::_@INT32_T_DECLARED (c-exp ""))
(%defconstant csym::_@INT64_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT8_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT16_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT32_T_DECLARED (c-exp ""))
(%defconstant csym::_@UINT64_T_DECLARED (c-exp ""))
(%defconstant csym::_@INTPTR_T_DECLARED (c-exp ""))
(%defconstant csym::_@CLOCK_T_DECLARED (c-exp ""))
(%defconstant csym::_@CLOCKID_T_DECLARED (c-exp ""))
(%defconstant csym::_@DEV_T_DECLARED (c-exp ""))
(%defconstant csym::_@FFLAGS_T_DECLARED (c-exp ""))
(%defconstant csym::_@FSBLKCNT_T_DECLARED (c-exp ""))
(%defconstant csym::_@GID_T_DECLARED (c-exp ""))
(%defconstant csym::_@IN_ADDR_T_DECLARED (c-exp ""))
(%defconstant csym::_@IN_PORT_T_DECLARED (c-exp ""))
(%defconstant csym::_@ID_T_DECLARED (c-exp ""))
(%defconstant csym::_@INO_T_DECLARED (c-exp ""))
(%defconstant csym::_@KEY_T_DECLARED (c-exp ""))
(%defconstant csym::_@LWPID_T_DECLARED (c-exp ""))
(%defconstant csym::_@MODE_T_DECLARED (c-exp ""))
(%defconstant csym::_@NLINK_T_DECLARED (c-exp ""))
(%defconstant csym::_@OFF_T_DECLARED (c-exp ""))
(%defconstant csym::_@PID_T_DECLARED (c-exp ""))
(%defconstant csym::_@RLIM_T_DECLARED (c-exp ""))
(%defconstant csym::_@SIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@SSIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@TIMER_T_DECLARED (c-exp ""))
(%defconstant csym::_@UID_T_DECLARED (c-exp ""))
(%defconstant csym::_@USECONDS_T_DECLARED (c-exp ""))
(%defconstant csym::_@SYS_SELECT_H_ (c-exp ""))
(%defconstant csym::_@SYS__SIGSET_H_ (c-exp ""))
(%defconstant csym::_@SIG_WORDS 4)
(%defconstant csym::_@SIG_MAXSIG 128)
(%defmacro csym::_@SIG_IDX (common-lisp-user::sig)
 (common-lisp:list '- 'csym::sig '1))
(%defmacro csym::_@SIG_WORD (common-lisp-user::sig)
 (common-lisp:list '>> (common-lisp:list '- 'csym::sig '1) '5))
(%defmacro csym::_@SIG_BIT (common-lisp-user::sig)
 (common-lisp:list '<<
                   '1
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '- 'csym::sig '1)
                                     '31)))
(%defmacro csym::_@SIG_VALID (common-lisp-user::sig)
 (common-lisp:list 'and
                   (common-lisp:list '<= 'csym::sig '128)
                   (common-lisp:list '> 'csym::sig '0)))
(%defconstant csym::_@SYS_TIMESPEC_H_ (c-exp ""))
(%defmacro csym::@TIMEVAL_TO_TIMESPEC
 (common-lisp-user::tv common-lisp-user::ts)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::ts)
                                                        'csym::tv_sec)
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::tv)
                                                        'csym::tv_sec))
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::ts)
                                                        'csym::tv_nsec)
                                      (common-lisp:list '*
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::tv)
                                                                          'csym::tv_usec)
                                                        '1000)))))
(%defmacro csym::@TIMESPEC_TO_TIMEVAL
 (common-lisp-user::tv common-lisp-user::ts)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::tv)
                                                        'csym::tv_sec)
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::ts)
                                                        'csym::tv_sec))
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::tv)
                                                        'csym::tv_usec)
                                      (common-lisp:list '/
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::ts)
                                                                          'csym::tv_nsec)
                                                        '1000)))))
(%defconstant csym::_@SIGSET_T_DECLARED (c-exp ""))
(%defconstant csym::@FD_SETSIZE (cast unsigned-int 1024))
(%defconstant csym::_@NFDBITS (* (sizeof csym::__fd_mask) 8))
(%defconstant csym::@NFDBITS (* (sizeof csym::__fd_mask) 8))
(%defmacro csym::_howmany (common-lisp-user::x common-lisp-user::y)
 (common-lisp:list '/
                   (common-lisp:list '+
                                     'csym::x
                                     (common-lisp:list '- 'csym::y '1))
                   'csym::y))
(%defconstant csym::fds_bits csym::__fds_bits)
(%defmacro csym::__fdset_mask (common-lisp-user::n)
 (common-lisp:list '<<
                   (common-lisp:list 'cast 'csym::__fd_mask '1)
                   (common-lisp:list '%
                                     'csym::n
                                     (common-lisp:list '*
                                                       (common-lisp:list
                                                        'sizeof
                                                        'csym::__fd_mask)
                                                       '8))))
(%defmacro csym::@FD_CLR (common-lisp-user::n common-lisp-user::p)
 (common-lisp:list 'bit-and=
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::__fds_bits)
                                     (common-lisp:list '/
                                                       'csym::n
                                                       (common-lisp:list '*
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'csym::__fd_mask)
                                                                         '8)))
                   (common-lisp:list 'bit-not
                                     (common-lisp:list '<<
                                                       (common-lisp:list 'cast
                                                                         'csym::__fd_mask
                                                                         '1)
                                                       (common-lisp:list '%
                                                                         'csym::n
                                                                         (common-lisp:list
                                                                          '*
                                                                          (common-lisp:list
                                                                           'sizeof
                                                                           'csym::__fd_mask)
                                                                          '8))))))
(%defmacro csym::@FD_COPY (common-lisp-user::f common-lisp:t)
 (common-lisp:list 'cast
                   'void
                   (common-lisp:list '=
                                     (common-lisp:list 'mref 'csym::t)
                                     (common-lisp:list 'mref 'csym::f))))
(%defmacro csym::@FD_ISSET (common-lisp-user::n common-lisp-user::p)
 (common-lisp:list 'bit-and
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::__fds_bits)
                                     (common-lisp:list '/
                                                       'csym::n
                                                       (common-lisp:list '*
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'csym::__fd_mask)
                                                                         '8)))
                   (common-lisp:list '<<
                                     (common-lisp:list 'cast
                                                       'csym::__fd_mask
                                                       '1)
                                     (common-lisp:list '%
                                                       'csym::n
                                                       (common-lisp:list '*
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'csym::__fd_mask)
                                                                         '8)))))
(%defmacro csym::@FD_SET (common-lisp-user::n common-lisp-user::p)
 (common-lisp:list 'bit-or=
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::__fds_bits)
                                     (common-lisp:list '/
                                                       'csym::n
                                                       (common-lisp:list '*
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'csym::__fd_mask)
                                                                         '8)))
                   (common-lisp:list '<<
                                     (common-lisp:list 'cast
                                                       'csym::__fd_mask
                                                       '1)
                                     (common-lisp:list '%
                                                       'csym::n
                                                       (common-lisp:list '*
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'csym::__fd_mask)
                                                                         '8)))))
(%defmacro csym::@FD_ZERO (common-lisp-user::p)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list 'decl
                                      'csym::_p
                                      (common-lisp:list 'ptr 'csym::fd_set))
                    (common-lisp:list 'decl 'csym::_n 'csym::__size_t)
                    (common-lisp:list '= 'csym::_p 'csym::p)
                    (common-lisp:list '=
                                      'csym::_n
                                      (common-lisp:list '/
                                                        (common-lisp:list '+
                                                                          (common-lisp:list
                                                                           'cast
                                                                           'unsigned-int
                                                                           '1024)
                                                                          (common-lisp:list
                                                                           '-
                                                                           (common-lisp:list
                                                                            '*
                                                                            (common-lisp:list
                                                                             'sizeof
                                                                             'csym::__fd_mask)
                                                                            '8)
                                                                           '1))
                                                        (common-lisp:list '*
                                                                          (common-lisp:list
                                                                           'sizeof
                                                                           'csym::__fd_mask)
                                                                          '8)))
                    (common-lisp:list 'while
                                      (common-lisp:list '> 'csym::_n '0)
                                      (common-lisp:list '=
                                                        (common-lisp:list 'aref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::_p)
                                                                           'csym::__fds_bits)
                                                                          (common-lisp:list
                                                                           '--
                                                                           'csym::_n))
                                                        '0)))))
(%defconstant csym::_@SELECT_DECLARED (c-exp ""))
(%defmacro csym::major (common-lisp-user::x)
 (common-lisp:list 'cast
                   'int
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '>>
                                                       (common-lisp:list 'cast
                                                                         'csym::u_int
                                                                         'csym::x)
                                                       '8)
                                     '255)))
(%defmacro csym::minor (common-lisp-user::x)
 (common-lisp:list 'cast 'int (common-lisp:list 'bit-and 'csym::x '4294902015)))
(%defmacro csym::makedev (common-lisp-user::x common-lisp-user::y)
 (common-lisp:list 'cast
                   'csym::dev_t
                   (common-lisp:list 'bit-or
                                     (common-lisp:list '<< 'csym::x '8)
                                     'csym::y)))
(%defconstant csym::_@FTRUNCATE_DECLARED (c-exp ""))
(%defconstant csym::_@LSEEK_DECLARED (c-exp ""))
(%defconstant csym::_@MMAP_DECLARED (c-exp ""))
(%defconstant csym::_@TRUNCATE_DECLARED (c-exp ""))
(%defconstant csym::@DST_NONE 0)
(%defconstant csym::@DST_USA 1)
(%defconstant csym::@DST_AUST 2)
(%defconstant csym::@DST_WET 3)
(%defconstant csym::@DST_MET 4)
(%defconstant csym::@DST_EET 5)
(%defconstant csym::@DST_CAN 6)
(%defmacro csym::timerclear (common-lisp-user::tvp)
 (common-lisp:list '=
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref 'csym::tvp)
                                     'csym::tv_sec)
                   (common-lisp:list '=
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::tvp)
                                                       'csym::tv_usec)
                                     '0)))
(%defmacro csym::timerisset (common-lisp-user::tvp)
 (common-lisp:list 'or
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref 'csym::tvp)
                                     'csym::tv_sec)
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref 'csym::tvp)
                                     'csym::tv_usec)))
(%defmacro csym::timercmp
 (common-lisp-user::tvp common-lisp-user::uvp common-lisp-user::cmp)
 (common-lisp:list 'c-exp
                   "( ( ( ~0@*~a ) -> tv_sec == ( ~1@*~a ) -> tv_sec ) ? ( ( ~0@*~a ) -> tv_usec ~2@*~a ( ~1@*~a ) -> tv_usec ) : ( ( ~0@*~a ) -> tv_sec ~2@*~a ( ~1@*~a ) -> tv_sec ) )"
                   common-lisp-user::tvp
                   common-lisp-user::uvp
                   common-lisp-user::cmp))
(%defmacro csym::timeradd
 (common-lisp-user::tvp common-lisp-user::uvp common-lisp-user::vvp)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::vvp)
                                                        'csym::tv_sec)
                                      (common-lisp:list '+
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::tvp)
                                                                          'csym::tv_sec)
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::uvp)
                                                                          'csym::tv_sec)))
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::vvp)
                                                        'csym::tv_usec)
                                      (common-lisp:list '+
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::tvp)
                                                                          'csym::tv_usec)
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::uvp)
                                                                          'csym::tv_usec)))
                    (common-lisp:list 'if
                                      (common-lisp:list '>=
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::vvp)
                                                                          'csym::tv_usec)
                                                        '1000000)
                                      (common-lisp:list 'begin
                                                        (common-lisp:list 'inc
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::vvp)
                                                                           'csym::tv_sec))
                                                        (common-lisp:list '-=
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::vvp)
                                                                           'csym::tv_usec)
                                                                          '1000000))))))
(%defmacro csym::timersub
 (common-lisp-user::tvp common-lisp-user::uvp common-lisp-user::vvp)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::vvp)
                                                        'csym::tv_sec)
                                      (common-lisp:list '-
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::tvp)
                                                                          'csym::tv_sec)
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::uvp)
                                                                          'csym::tv_sec)))
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          'csym::vvp)
                                                        'csym::tv_usec)
                                      (common-lisp:list '-
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::tvp)
                                                                          'csym::tv_usec)
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::uvp)
                                                                          'csym::tv_usec)))
                    (common-lisp:list 'if
                                      (common-lisp:list '<
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::vvp)
                                                                          'csym::tv_usec)
                                                        '0)
                                      (common-lisp:list 'begin
                                                        (common-lisp:list 'dec
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::vvp)
                                                                           'csym::tv_sec))
                                                        (common-lisp:list '+=
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::vvp)
                                                                           'csym::tv_usec)
                                                                          '1000000))))))
(%defconstant csym::@ITIMER_REAL 0)
(%defconstant csym::@ITIMER_VIRTUAL 1)
(%defconstant csym::@ITIMER_PROF 2)
(%defconstant csym::@CLOCK_REALTIME 0)
(%defconstant csym::@CLOCK_VIRTUAL 1)
(%defconstant csym::@CLOCK_PROF 2)
(%defconstant csym::@CLOCK_MONOTONIC 4)
(%defconstant csym::@TIMER_RELTIME 0)
(%defconstant csym::@TIMER_ABSTIME 1)
(%defconstant csym::_@TIME_H_ (c-exp ""))
(%defconstant csym::@NULL (cast (ptr void) 0))
(%defconstant csym::@CLK_TCK 128)
(%defconstant csym::@CLOCKS_PER_SEC 128)
(%defconstant csym::_@SYS_SIGNAL_H_ (c-exp ""))
(%defconstant csym::_@MACHINE_SIGNAL_H_ (c-exp ""))
(%defconstant csym::@MINSIGSTKSZ (* 512 4))
(%defconstant csym::_@MACHINE_TRAP_H_ (c-exp ""))
(%defconstant csym::@T_PRIVINFLT 1)
(%defconstant csym::@T_BPTFLT 3)
(%defconstant csym::@T_ARITHTRAP 6)
(%defconstant csym::@T_PROTFLT 9)
(%defconstant csym::@T_TRCTRAP 10)
(%defconstant csym::@T_PAGEFLT 12)
(%defconstant csym::@T_ALIGNFLT 14)
(%defconstant csym::@T_DIVIDE 18)
(%defconstant csym::@T_NMI 19)
(%defconstant csym::@T_OFLOW 20)
(%defconstant csym::@T_BOUND 21)
(%defconstant csym::@T_DNA 22)
(%defconstant csym::@T_DOUBLEFLT 23)
(%defconstant csym::@T_FPOPFLT 24)
(%defconstant csym::@T_TSSFLT 25)
(%defconstant csym::@T_SEGNPFLT 26)
(%defconstant csym::@T_STKFLT 27)
(%defconstant csym::@T_MCHK 28)
(%defconstant csym::@T_XMMFLT 29)
(%defconstant csym::@T_RESERVED 30)
(%defconstant csym::@ILL_RESAD_FAULT csym::@T_RESADFLT)
(%defconstant csym::@ILL_PRIVIN_FAULT 1)
(%defconstant csym::@ILL_RESOP_FAULT csym::@T_RESOPFLT)
(%defconstant csym::@ILL_ALIGN_FAULT 14)
(%defconstant csym::@ILL_FPOP_FAULT 24)
(%defconstant csym::@FPE_INTOVF 1)
(%defconstant csym::@FPE_INTDIV 2)
(%defconstant csym::@FPE_FLTDIV 3)
(%defconstant csym::@FPE_FLTOVF 4)
(%defconstant csym::@FPE_FLTUND 5)
(%defconstant csym::@FPE_FLTRES 6)
(%defconstant csym::@FPE_FLTINV 7)
(%defconstant csym::@FPE_FLTSUB 8)
(%defconstant csym::@FPE_INTOVF_TRAP 1)
(%defconstant csym::@FPE_INTDIV_TRAP 2)
(%defconstant csym::@FPE_FLTDIV_TRAP 3)
(%defconstant csym::@FPE_FLTOVF_TRAP 4)
(%defconstant csym::@FPE_FLTUND_TRAP 5)
(%defconstant csym::@FPE_FPU_NP_TRAP 6)
(%defconstant csym::@FPE_SUBRNG_TRAP 7)
(%defconstant csym::@BUS_PAGE_FAULT 12)
(%defconstant csym::@BUS_SEGNP_FAULT 26)
(%defconstant csym::@BUS_STK_FAULT 27)
(%defconstant csym::@BUS_SEGM_FAULT 30)
(%defconstant csym::@T_USER 256)
(%defconstant csym::sc_sp csym::sc_esp)
(%defconstant csym::sc_fp csym::sc_ebp)
(%defconstant csym::sc_pc csym::sc_eip)
(%defconstant csym::sc_ps csym::sc_efl)
(%defconstant csym::sc_eflags csym::sc_efl)
(%defconstant csym::@SIGHUP 1)
(%defconstant csym::@SIGINT 2)
(%defconstant csym::@SIGQUIT 3)
(%defconstant csym::@SIGILL 4)
(%defconstant csym::@SIGTRAP 5)
(%defconstant csym::@SIGABRT 6)
(%defconstant csym::@SIGIOT 6)
(%defconstant csym::@SIGEMT 7)
(%defconstant csym::@SIGFPE 8)
(%defconstant csym::@SIGKILL 9)
(%defconstant csym::@SIGBUS 10)
(%defconstant csym::@SIGSEGV 11)
(%defconstant csym::@SIGSYS 12)
(%defconstant csym::@SIGPIPE 13)
(%defconstant csym::@SIGALRM 14)
(%defconstant csym::@SIGTERM 15)
(%defconstant csym::@SIGURG 16)
(%defconstant csym::@SIGSTOP 17)
(%defconstant csym::@SIGTSTP 18)
(%defconstant csym::@SIGCONT 19)
(%defconstant csym::@SIGCHLD 20)
(%defconstant csym::@SIGTTIN 21)
(%defconstant csym::@SIGTTOU 22)
(%defconstant csym::@SIGIO 23)
(%defconstant csym::@SIGXCPU 24)
(%defconstant csym::@SIGXFSZ 25)
(%defconstant csym::@SIGVTALRM 26)
(%defconstant csym::@SIGPROF 27)
(%defconstant csym::@SIGWINCH 28)
(%defconstant csym::@SIGINFO 29)
(%defconstant csym::@SIGUSR1 30)
(%defconstant csym::@SIGUSR2 31)
(%defconstant csym::@SIGTHR 32)
(%defconstant csym::@SIG_DFL (cast (ptr csym::__sighandler_t) 0))
(%defconstant csym::@SIG_IGN (cast (ptr csym::__sighandler_t) 1))
(%defconstant csym::@SIG_ERR (cast (ptr csym::__sighandler_t) (- 1)))
(%defconstant csym::sigev_signo (fref csym::__sigev_u csym::__sigev_signo))
(%defconstant csym::sigev_notify_kqueue
 (fref csym::__sigev_u csym::__sigev_notify_kqueue))
(%defconstant csym::@SIGEV_NONE 0)
(%defconstant csym::@SIGEV_SIGNAL 1)
(%defconstant csym::@SIGEV_KEVENT 3)
(%defconstant csym::sa_handler (fref csym::__sigaction_u csym::__sa_handler))
(%defconstant csym::sa_sigaction
 (fref csym::__sigaction_u csym::__sa_sigaction))
(%defconstant csym::@SA_NOCLDSTOP 8)
(%defconstant csym::@SA_ONSTACK 1)
(%defconstant csym::@SA_RESTART 2)
(%defconstant csym::@SA_RESETHAND 4)
(%defconstant csym::@SA_NODEFER 16)
(%defconstant csym::@SA_NOCLDWAIT 32)
(%defconstant csym::@SA_SIGINFO 64)
(%defconstant csym::@NSIG 32)
(%defconstant csym::@SI_USER 65537)
(%defconstant csym::@SI_QUEUE 65538)
(%defconstant csym::@SI_TIMER 65539)
(%defconstant csym::@SI_ASYNCIO 65540)
(%defconstant csym::@SI_MESGQ 65541)
(%defconstant csym::@SI_UNDEFINED 0)
(%defconstant csym::@SS_ONSTACK 1)
(%defconstant csym::@SS_DISABLE 4)
(%defconstant csym::@SIGSTKSZ (+ (* 512 4) 32768))
(%defconstant csym::@SV_ONSTACK 1)
(%defconstant csym::@SV_INTERRUPT 2)
(%defconstant csym::@SV_RESETHAND 4)
(%defconstant csym::@SV_NODEFER 16)
(%defconstant csym::@SV_NOCLDSTOP 8)
(%defconstant csym::@SV_SIGINFO 64)
(%defconstant csym::sv_onstack csym::sv_flags)
(%defmacro csym::sigmask (common-lisp-user::m)
 (common-lisp:list '<< '1 (common-lisp:list '- 'csym::m '1)))
(%defconstant csym::@BADSIG (cast (ptr csym::__sighandler_t) (- 1)))
(%defconstant csym::@SIG_BLOCK 1)
(%defconstant csym::@SIG_UNBLOCK 2)
(%defconstant csym::@SIG_SETMASK 3)
(%defconstant csym::@AIO_CANCELED 1)
(%defconstant csym::@AIO_NOTCANCELED 2)
(%defconstant csym::@AIO_ALLDONE 3)
(%defconstant csym::@LIO_NOP 0)
(%defconstant csym::@LIO_WRITE 1)
(%defconstant csym::@LIO_READ 2)
(%defconstant csym::@LIO_NOWAIT 0)
(%defconstant csym::@LIO_WAIT 1)
(%defconstant csym::@AIO_LISTIO_MAX 16)
(%defmacro csym::_assert (common-lisp-user::e)
 (common-lisp:list 'c-exp
                   "( ( ~0@*~a ) ? ( void ) 0 : __assert ( __func__ , , - 1234567890 , \"e\" ) )"
                   common-lisp-user::e))
(%defmacro csym::assert (common-lisp-user::e)
 (common-lisp:list 'c-exp
                   "( ( ~0@*~a ) ? ( void ) 0 : __assert ( __func__ , , - 1234567890 , _C2SC_STRINGIFY ( ~0@*~a ) ) )"
                   common-lisp-user::e))
(%defconstant csym::_@COMPLEX_H (c-exp ""))
(%defconstant csym::_@C@omplex (c-exp ""))
(%defconstant csym::_@C@omplex_@I (c-exp "1.0fi"))
(%defconstant csym::complex (c-exp ""))
(%defconstant csym::@I (c-exp "1.0fi"))
(%defconstant csym::_@CPIO_H_ (c-exp ""))
(%defconstant csym::@C_ISSOCK 49152)
(%defconstant csym::@C_ISLNK 40960)
(%defconstant csym::@C_ISCTG 36864)
(%defconstant csym::@C_ISREG 32768)
(%defconstant csym::@C_ISBLK 24576)
(%defconstant csym::@C_ISDIR 16384)
(%defconstant csym::@C_ISCHR 8192)
(%defconstant csym::@C_ISFIFO 4096)
(%defconstant csym::@C_ISUID 2048)
(%defconstant csym::@C_ISGID 1024)
(%defconstant csym::@C_ISVTX 512)
(%defconstant csym::@C_IRUSR 256)
(%defconstant csym::@C_IWUSR 128)
(%defconstant csym::@C_IXUSR 64)
(%defconstant csym::@C_IRGRP 32)
(%defconstant csym::@C_IWGRP 16)
(%defconstant csym::@C_IXGRP 8)
(%defconstant csym::@C_IROTH 4)
(%defconstant csym::@C_IWOTH 2)
(%defconstant csym::@C_IXOTH 1)
(%defconstant csym::@MAGIC "070707")
(%defconstant csym::_@CTYPE_H_ (c-exp ""))
(%defconstant csym::__@CTYPE_H_ (c-exp ""))
(%defconstant csym::_@CTYPE_A (cast long 256))
(%defconstant csym::_@CTYPE_C (cast long 512))
(%defconstant csym::_@CTYPE_D (cast long 1024))
(%defconstant csym::_@CTYPE_G (cast long 2048))
(%defconstant csym::_@CTYPE_L (cast long 4096))
(%defconstant csym::_@CTYPE_P (cast long 8192))
(%defconstant csym::_@CTYPE_S (cast long 16384))
(%defconstant csym::_@CTYPE_U (cast long 32768))
(%defconstant csym::_@CTYPE_X (cast long 65536))
(%defconstant csym::_@CTYPE_B (cast long 131072))
(%defconstant csym::_@CTYPE_R (cast long 262144))
(%defconstant csym::_@CTYPE_I (cast long 524288))
(%defconstant csym::_@CTYPE_T (cast long 1048576))
(%defconstant csym::_@CTYPE_Q (cast long 2097152))
(%defconstant csym::_@CTYPE_SW0 (cast long 536870912))
(%defconstant csym::_@CTYPE_SW1 (cast long 1073741824))
(%defconstant csym::_@CTYPE_SW2 (cast long 2147483648))
(%defconstant csym::_@CTYPE_SW3 (cast long 3221225472))
(%defconstant csym::_@CTYPE_SWM (cast long 3758096384))
(%defconstant csym::_@CTYPE_SWS 30)
(%defconstant csym::_@RUNETYPE_H_ (c-exp ""))
(%defconstant csym::_@CACHED_RUNES (<< 1 8))
(%defconstant csym::_@CRMASK (bit-not (- (<< 1 8) 1)))
(%defconstant csym::_@RUNE_MAGIC_1 "RuneMagi")
(%defmacro csym::isalnum (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'bit-or
                                     (common-lisp:list 'cast 'long '256)
                                     (common-lisp:list 'cast 'long '1024))))
(%defmacro csym::isalpha (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '256)))
(%defmacro csym::iscntrl (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '512)))
(%defmacro csym::isdigit (common-lisp-user::c)
 (common-lisp:list 'csym::__isctype
                   'csym::c
                   (common-lisp:list 'cast 'long '1024)))
(%defmacro csym::isgraph (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '2048)))
(%defmacro csym::islower (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '4096)))
(%defmacro csym::isprint (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '262144)))
(%defmacro csym::ispunct (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '8192)))
(%defmacro csym::isspace (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '16384)))
(%defmacro csym::isupper (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '32768)))
(%defmacro csym::isxdigit (common-lisp-user::c)
 (common-lisp:list 'csym::__isctype
                   'csym::c
                   (common-lisp:list 'cast 'long '65536)))
(%defmacro csym::tolower (common-lisp-user::c)
 (common-lisp:list 'csym::__tolower 'csym::c))
(%defmacro csym::toupper (common-lisp-user::c)
 (common-lisp:list 'csym::__toupper 'csym::c))
(%defmacro csym::_tolower (common-lisp-user::c)
 (common-lisp:list 'csym::__tolower 'csym::c))
(%defmacro csym::_toupper (common-lisp-user::c)
 (common-lisp:list 'csym::__toupper 'csym::c))
(%defmacro csym::isascii (common-lisp-user::c)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     'csym::c
                                     (common-lisp:list 'bit-not '127))
                   '0))
(%defmacro csym::toascii (common-lisp-user::c)
 (common-lisp:list 'bit-and 'csym::c '127))
(%defmacro csym::isblank (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '131072)))
(%defmacro csym::digittoint (common-lisp-user::c)
 (common-lisp:list 'csym::__maskrune 'csym::c '255))
(%defmacro csym::ishexnumber (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '65536)))
(%defmacro csym::isideogram (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '524288)))
(%defmacro csym::isnumber (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '1024)))
(%defmacro csym::isphonogram (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '2097152)))
(%defmacro csym::isrune (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '4294967040)))
(%defmacro csym::isspecial (common-lisp-user::c)
 (common-lisp:list 'csym::__istype
                   'csym::c
                   (common-lisp:list 'cast 'long '1048576)))
(%defconstant csym::_@DIRENT_H_ (c-exp ""))
(%defconstant csym::_@SYS_DIRENT_H_ (c-exp ""))
(%defconstant csym::@MAXNAMLEN 255)
(%defconstant csym::@DT_UNKNOWN 0)
(%defconstant csym::@DT_FIFO 1)
(%defconstant csym::@DT_CHR 2)
(%defconstant csym::@DT_DIR 4)
(%defconstant csym::@DT_BLK 6)
(%defconstant csym::@DT_REG 8)
(%defconstant csym::@DT_LNK 10)
(%defconstant csym::@DT_SOCK 12)
(%defconstant csym::@DT_WHT 14)
(%defmacro csym::@IFTODT (common-lisp-user::mode)
 (common-lisp:list '>> (common-lisp:list 'bit-and 'csym::mode '61440) '12))
(%defmacro csym::@DTTOIF (common-lisp-user::dirtype)
 (common-lisp:list '<< 'csym::dirtype '12))
(%defmacro csym::_@GENERIC_DIRSIZ (common-lisp-user::dp)
 (common-lisp:list '+
                   (common-lisp:list '-
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::dirent))
                                     (common-lisp:list '+ '255 '1))
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '+
                                                       (common-lisp:list '+
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::dp)
                                                                          'csym::d_namlen)
                                                                         '1)
                                                       '3)
                                     (common-lisp:list 'bit-not '3))))
(%defconstant csym::d_ino csym::d_fileno)
(%defconstant csym::@DIRBLKSIZ 1024)
(%defmacro csym::dirfd (common-lisp-user::dirp)
 (common-lisp:list 'fref (common-lisp:list 'mref 'csym::dirp) 'csym::dd_fd))
(%defconstant csym::@DTF_HIDEW 1)
(%defconstant csym::@DTF_NODUP 2)
(%defconstant csym::@DTF_REWIND 4)
(%defconstant csym::__@DTF_READALL 8)
(%defconstant csym::_@DLFCN_H_ (c-exp ""))
(%defconstant csym::@RTLD_LAZY 1)
(%defconstant csym::@RTLD_NOW 2)
(%defconstant csym::@RTLD_MODEMASK 3)
(%defconstant csym::@RTLD_GLOBAL 256)
(%defconstant csym::@RTLD_LOCAL 0)
(%defconstant csym::@RTLD_TRACE 512)
(%defconstant csym::@RTLD_DI_LINKMAP 2)
(%defconstant csym::@RTLD_DI_SERINFO 4)
(%defconstant csym::@RTLD_DI_SERINFOSIZE 5)
(%defconstant csym::@RTLD_DI_ORIGIN 6)
(%defconstant csym::@RTLD_DI_MAX 6)
(%defconstant csym::@RTLD_NEXT (cast (ptr void) (- 1)))
(%defconstant csym::@RTLD_DEFAULT (cast (ptr void) (- 2)))
(%defconstant csym::@RTLD_SELF (cast (ptr void) (- 3)))
(%defconstant csym::_@SYS_ERRNO_H_ (c-exp ""))
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
(%defconstant csym::@EWOULDBLOCK 35)
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
(%defconstant csym::@ENOTSUP 45)
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
(%defconstant csym::_@SYS_FCNTL_H_ (c-exp ""))
(%defconstant csym::@O_RDONLY 0)
(%defconstant csym::@O_WRONLY 1)
(%defconstant csym::@O_RDWR 2)
(%defconstant csym::@O_ACCMODE 3)
(%defconstant csym::@FREAD 1)
(%defconstant csym::@FWRITE 2)
(%defconstant csym::@O_NONBLOCK 4)
(%defconstant csym::@O_APPEND 8)
(%defconstant csym::@O_SHLOCK 16)
(%defconstant csym::@O_EXLOCK 32)
(%defconstant csym::@O_ASYNC 64)
(%defconstant csym::@O_FSYNC 128)
(%defconstant csym::@O_SYNC 128)
(%defconstant csym::@O_NOFOLLOW 256)
(%defconstant csym::@O_CREAT 512)
(%defconstant csym::@O_TRUNC 1024)
(%defconstant csym::@O_EXCL 2048)
(%defconstant csym::@O_NOCTTY 32768)
(%defconstant csym::@O_DIRECT 65536)
(%defconstant csym::@FAPPEND 8)
(%defconstant csym::@FASYNC 64)
(%defconstant csym::@FFSYNC 128)
(%defconstant csym::@FNONBLOCK 4)
(%defconstant csym::@FNDELAY 4)
(%defconstant csym::@O_NDELAY 4)
(%defconstant csym::@FPOSIXSHM 256)
(%defconstant csym::@F_DUPFD 0)
(%defconstant csym::@F_GETFD 1)
(%defconstant csym::@F_SETFD 2)
(%defconstant csym::@F_GETFL 3)
(%defconstant csym::@F_SETFL 4)
(%defconstant csym::@F_GETOWN 5)
(%defconstant csym::@F_SETOWN 6)
(%defconstant csym::@F_GETLK 7)
(%defconstant csym::@F_SETLK 8)
(%defconstant csym::@F_SETLKW 9)
(%defconstant csym::@FD_CLOEXEC 1)
(%defconstant csym::@F_RDLCK 1)
(%defconstant csym::@F_UNLCK 2)
(%defconstant csym::@F_WRLCK 3)
(%defconstant csym::@LOCK_SH 1)
(%defconstant csym::@LOCK_EX 2)
(%defconstant csym::@LOCK_NB 4)
(%defconstant csym::@LOCK_UN 8)
(%defconstant csym::_@FENV_H_ (c-exp ""))
(%defconstant csym::@FE_INVALID 1)
(%defconstant csym::@FE_DENORMAL 2)
(%defconstant csym::@FE_DIVBYZERO 4)
(%defconstant csym::@FE_OVERFLOW 8)
(%defconstant csym::@FE_UNDERFLOW 16)
(%defconstant csym::@FE_INEXACT 32)
(%defconstant csym::@FE_ALL_EXCEPT (bit-or 4 2 32 1 8 16))
(%defconstant csym::@FE_TONEAREST 0)
(%defconstant csym::@FE_DOWNWARD 1024)
(%defconstant csym::@FE_UPWARD 2048)
(%defconstant csym::@FE_TOWARDZERO 3072)
(%defconstant csym::_@ROUND_MASK (bit-or 0 1024 2048 3072))
(%defconstant csym::@FE_DFL_ENV (ptr csym::__fe_dfl_env))
(%defmacro csym::__fldcw (common-lisp-user::__cw)
 (common-lisp:list 'c-exp
                   "__asm volatile ( \"fldcw %0\" : : \"m\" ( ~0@*~a ) )"
                   common-lisp-user::__cw))
(%defmacro csym::__fldenv (common-lisp-user::__env)
 (common-lisp:list 'c-exp
                   "__asm volatile ( \"fldenv %0\" : : \"m\" ( ~0@*~a ) )"
                   common-lisp-user::__env))
(%defmacro csym::__fnclex common-lisp:nil
 (common-lisp:list 'c-exp "__asm volatile ( \"fnclex\" )"))
(%defmacro csym::__fnstenv (common-lisp-user::__env)
 (common-lisp:list 'c-exp "" common-lisp-user::__env))
(%defmacro csym::__fnstcw (common-lisp-user::__cw)
 (common-lisp:list 'c-exp "" common-lisp-user::__cw))
(%defmacro csym::__fnstsw (common-lisp-user::__sw)
 (common-lisp:list 'c-exp "" common-lisp-user::__sw))
(%defmacro csym::__fwait common-lisp:nil
 (common-lisp:list 'c-exp "__asm volatile ( \"fwait\" )"))
(%defconstant csym::_@MACHINE_FLOAT_H_ 1)
(%defconstant csym::@FLT_RADIX 2)
(%defconstant csym::@FLT_ROUNDS (csym::__flt_rounds))
(%defconstant csym::@FLT_EVAL_METHOD (- 1))
(%defconstant csym::@DECIMAL_DIG 21)
(%defconstant csym::@FLT_MANT_DIG 24)
(%defconstant csym::@FLT_EPSILON (cast float 1.1920929d-7))
(%defconstant csym::@FLT_DIG 6)
(%defconstant csym::@FLT_MIN_EXP (- 125))
(%defconstant csym::@FLT_MIN (cast float 1.17549435d-38))
(%defconstant csym::@FLT_MIN_10_EXP (- 37))
(%defconstant csym::@FLT_MAX_EXP 128)
(%defconstant csym::@FLT_MAX (cast float 3.40282347d+38))
(%defconstant csym::@FLT_MAX_10_EXP 38)
(%defconstant csym::@DBL_MANT_DIG 53)
(%defconstant csym::@DBL_EPSILON 2.220446049250313d-16)
(%defconstant csym::@DBL_DIG 15)
(%defconstant csym::@DBL_MIN_EXP (- 1021))
(%defconstant csym::@DBL_MIN 2.2250738585072014d-308)
(%defconstant csym::@DBL_MIN_10_EXP (- 307))
(%defconstant csym::@DBL_MAX_EXP 1024)
(%defconstant csym::@DBL_MAX 1.7976931348623157d+308)
(%defconstant csym::@DBL_MAX_10_EXP 308)
(%defconstant csym::@LDBL_MANT_DIG 64)
(%defconstant csym::@LDBL_EPSILON (cast long-double 1.8420217248550443d-19))
(%defconstant csym::@LDBL_DIG 18)
(%defconstant csym::@LDBL_MIN_EXP (- 16381))
(%defconstant csym::@LDBL_MIN
 (cast long-double
  (/ 2.9643938750474793d-323
   10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))
(%defconstant csym::@LDBL_MIN_10_EXP (- 4931))
(%defconstant csym::@LDBL_MAX_EXP 16384)
(%defconstant csym::@LDBL_MAX
 (cast long-double
  (* 1.1897314953572318d+308
   10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))
(%defconstant csym::@LDBL_MAX_10_EXP 4932)
(%defconstant csym::_@FMTMSG_H_ (c-exp ""))
(%defconstant csym::@MM_HARD 1)
(%defconstant csym::@MM_SOFT 2)
(%defconstant csym::@MM_FIRM 4)
(%defconstant csym::@MM_APPL 16)
(%defconstant csym::@MM_UTIL 32)
(%defconstant csym::@MM_OPSYS 64)
(%defconstant csym::@MM_PRINT 256)
(%defconstant csym::@MM_CONSOLE 512)
(%defconstant csym::@MM_RECOVER 4096)
(%defconstant csym::@MM_NRECOV 8192)
(%defconstant csym::@MM_NOSEV 0)
(%defconstant csym::@MM_HALT 1)
(%defconstant csym::@MM_ERROR 2)
(%defconstant csym::@MM_WARNING 3)
(%defconstant csym::@MM_INFO 4)
(%defconstant csym::@MM_NULLLBL (cast (ptr char) 0))
(%defconstant csym::@MM_NULLSEV 0)
(%defconstant csym::@MM_NULLMC (cast long 0))
(%defconstant csym::@MM_NULLTXT (cast (ptr char) 0))
(%defconstant csym::@MM_NULLACT (cast (ptr char) 0))
(%defconstant csym::@MM_NULLTAG (cast (ptr char) 0))
(%defconstant csym::@MM_OK 0)
(%defconstant csym::@MM_NOMSG 1)
(%defconstant csym::@MM_NOCON 2)
(%defconstant csym::@MM_NOTOK 3)
(%defconstant csym::_@FNMATCH_H_ (c-exp ""))
(%defconstant csym::@FNM_NOMATCH 1)
(%defconstant csym::@FNM_NOESCAPE 1)
(%defconstant csym::@FNM_PATHNAME 2)
(%defconstant csym::@FNM_PERIOD 4)
(%defconstant csym::@FNM_NOSYS (- 1))
(%defconstant csym::@FNM_LEADING_DIR 8)
(%defconstant csym::@FNM_CASEFOLD 16)
(%defconstant csym::@FNM_IGNORECASE 16)
(%defconstant csym::@FNM_FILE_NAME 2)
(%defconstant csym::_@FTW_H (c-exp ""))
(%defconstant csym::_@SYS_STAT_H_ (c-exp ""))
(%defconstant csym::st_atime (fref csym::st_atimespec csym::tv_sec))
(%defconstant csym::st_mtime (fref csym::st_mtimespec csym::tv_sec))
(%defconstant csym::st_ctime (fref csym::st_ctimespec csym::tv_sec))
(%defconstant csym::st_birthtime (fref csym::st_birthtimespec csym::tv_sec))
(%defconstant csym::@S_ISUID 2048)
(%defconstant csym::@S_ISGID 1024)
(%defconstant csym::@S_ISTXT 512)
(%defconstant csym::@S_IRWXU 448)
(%defconstant csym::@S_IRUSR 256)
(%defconstant csym::@S_IWUSR 128)
(%defconstant csym::@S_IXUSR 64)
(%defconstant csym::@S_IREAD 256)
(%defconstant csym::@S_IWRITE 128)
(%defconstant csym::@S_IEXEC 64)
(%defconstant csym::@S_IRWXG 56)
(%defconstant csym::@S_IRGRP 32)
(%defconstant csym::@S_IWGRP 16)
(%defconstant csym::@S_IXGRP 8)
(%defconstant csym::@S_IRWXO 7)
(%defconstant csym::@S_IROTH 4)
(%defconstant csym::@S_IWOTH 2)
(%defconstant csym::@S_IXOTH 1)
(%defconstant csym::@S_IFMT 61440)
(%defconstant csym::@S_IFIFO 4096)
(%defconstant csym::@S_IFCHR 8192)
(%defconstant csym::@S_IFDIR 16384)
(%defconstant csym::@S_IFBLK 24576)
(%defconstant csym::@S_IFREG 32768)
(%defconstant csym::@S_IFLNK 40960)
(%defconstant csym::@S_IFSOCK 49152)
(%defconstant csym::@S_ISVTX 512)
(%defconstant csym::@S_IFWHT 57344)
(%defmacro csym::@S_ISDIR (common-lisp-user::m)
 (common-lisp:list '== (common-lisp:list 'bit-and 'csym::m '61440) '16384))
(%defmacro csym::@S_ISCHR (common-lisp-user::m)
 (common-lisp:list '== (common-lisp:list 'bit-and 'csym::m '61440) '8192))
(%defmacro csym::@S_ISBLK (common-lisp-user::m)
 (common-lisp:list '== (common-lisp:list 'bit-and 'csym::m '61440) '24576))
(%defmacro csym::@S_ISREG (common-lisp-user::m)
 (common-lisp:list '== (common-lisp:list 'bit-and 'csym::m '61440) '32768))
(%defmacro csym::@S_ISFIFO (common-lisp-user::m)
 (common-lisp:list '== (common-lisp:list 'bit-and 'csym::m '61440) '4096))
(%defmacro csym::@S_ISLNK (common-lisp-user::m)
 (common-lisp:list '== (common-lisp:list 'bit-and 'csym::m '61440) '40960))
(%defmacro csym::@S_ISSOCK (common-lisp-user::m)
 (common-lisp:list '== (common-lisp:list 'bit-and 'csym::m '61440) '49152))
(%defmacro csym::@S_ISWHT (common-lisp-user::m)
 (common-lisp:list '== (common-lisp:list 'bit-and 'csym::m '61440) '57344))
(%defconstant csym::@ACCESSPERMS (bit-or 448 56 7))
(%defconstant csym::@ALLPERMS (bit-or 2048 1024 512 448 56 7))
(%defconstant csym::@DEFFILEMODE (bit-or 256 128 32 16 4 2))
(%defconstant csym::@S_BLKSIZE 512)
(%defconstant csym::@UF_SETTABLE 65535)
(%defconstant csym::@UF_NODUMP 1)
(%defconstant csym::@UF_IMMUTABLE 2)
(%defconstant csym::@UF_APPEND 4)
(%defconstant csym::@UF_OPAQUE 8)
(%defconstant csym::@UF_NOUNLINK 16)
(%defconstant csym::@SF_SETTABLE 4294901760)
(%defconstant csym::@SF_ARCHIVED 65536)
(%defconstant csym::@SF_IMMUTABLE 131072)
(%defconstant csym::@SF_APPEND 262144)
(%defconstant csym::@SF_NOUNLINK 1048576)
(%defconstant csym::@SF_SNAPSHOT 2097152)
(%defconstant csym::@FTW_F 0)
(%defconstant csym::@FTW_D 1)
(%defconstant csym::@FTW_DNR 2)
(%defconstant csym::@FTW_DP 3)
(%defconstant csym::@FTW_NS 4)
(%defconstant csym::@FTW_SL 5)
(%defconstant csym::@FTW_SLN 6)
(%defconstant csym::@FTW_PHYS 1)
(%defconstant csym::@FTW_MOUNT 2)
(%defconstant csym::@FTW_DEPTH 4)
(%defconstant csym::@FTW_CHDIR 8)
(%defconstant csym::_@GLOB_H_ (c-exp ""))
(%defconstant csym::@GLOB_APPEND 1)
(%defconstant csym::@GLOB_DOOFFS 2)
(%defconstant csym::@GLOB_ERR 4)
(%defconstant csym::@GLOB_MARK 8)
(%defconstant csym::@GLOB_NOCHECK 16)
(%defconstant csym::@GLOB_NOSORT 32)
(%defconstant csym::@GLOB_NOESCAPE 8192)
(%defconstant csym::@GLOB_NOSPACE (- 1))
(%defconstant csym::@GLOB_ABORTED (- 2))
(%defconstant csym::@GLOB_NOMATCH (- 3))
(%defconstant csym::@GLOB_NOSYS (- 4))
(%defconstant csym::@GLOB_ALTDIRFUNC 64)
(%defconstant csym::@GLOB_BRACE 128)
(%defconstant csym::@GLOB_MAGCHAR 256)
(%defconstant csym::@GLOB_NOMAGIC 512)
(%defconstant csym::@GLOB_QUOTE 1024)
(%defconstant csym::@GLOB_TILDE 2048)
(%defconstant csym::@GLOB_LIMIT 4096)
(%defconstant csym::@GLOB_MAXPATH 4096)
(%defconstant csym::@GLOB_ABEND (- 2))
(%defconstant csym::_@GRP_H_ (c-exp ""))
(%defconstant csym::_@PATH_GROUP "/etc/group")
(%defconstant csym::_@NET_IF_H_ (c-exp ""))
(%defconstant csym::@IF_NAMESIZE 16)
(%defconstant csym::@IFNAMSIZ 16)
(%defconstant csym::@IF_MAXUNIT 32767)
(%defconstant csym::@IFF_UP 1)
(%defconstant csym::@IFF_BROADCAST 2)
(%defconstant csym::@IFF_DEBUG 4)
(%defconstant csym::@IFF_LOOPBACK 8)
(%defconstant csym::@IFF_POINTOPOINT 16)
(%defconstant csym::@IFF_SMART 32)
(%defconstant csym::@IFF_RUNNING 64)
(%defconstant csym::@IFF_NOARP 128)
(%defconstant csym::@IFF_PROMISC 256)
(%defconstant csym::@IFF_ALLMULTI 512)
(%defconstant csym::@IFF_OACTIVE 1024)
(%defconstant csym::@IFF_SIMPLEX 2048)
(%defconstant csym::@IFF_LINK0 4096)
(%defconstant csym::@IFF_LINK1 8192)
(%defconstant csym::@IFF_LINK2 16384)
(%defconstant csym::@IFF_ALTPHYS 16384)
(%defconstant csym::@IFF_MULTICAST 32768)
(%defconstant csym::@IFF_POLLING 65536)
(%defconstant csym::@IFF_PPROMISC 131072)
(%defconstant csym::@IFF_MONITOR 262144)
(%defconstant csym::@IFF_STATICARP 524288)
(%defconstant csym::@IFF_NEEDSGIANT 1048576)
(%defconstant csym::@IFF_CANTCHANGE
 (bit-or 2 16 64 1024 2048 32768 512 32 256 65536))
(%defconstant csym::@LINK_STATE_UNKNOWN 0)
(%defconstant csym::@LINK_STATE_DOWN 1)
(%defconstant csym::@LINK_STATE_UP 2)
(%defmacro csym::@IF_K@bps (common-lisp-user::x)
 (common-lisp:list '* 'csym::x '1000))
(%defmacro csym::@IF_M@bps (common-lisp-user::x)
 (common-lisp:list '* (common-lisp:list '* 'csym::x '1000) '1000))
(%defmacro csym::@IF_G@bps (common-lisp-user::x)
 (common-lisp:list '*
                   (common-lisp:list '*
                                     (common-lisp:list '* 'csym::x '1000)
                                     '1000)
                   '1000))
(%defconstant csym::@IFCAP_RXCSUM 1)
(%defconstant csym::@IFCAP_TXCSUM 2)
(%defconstant csym::@IFCAP_NETCONS 4)
(%defconstant csym::@IFCAP_VLAN_MTU 8)
(%defconstant csym::@IFCAP_VLAN_HWTAGGING 16)
(%defconstant csym::@IFCAP_JUMBO_MTU 32)
(%defconstant csym::@IFCAP_POLLING 64)
(%defconstant csym::@IFCAP_HWCSUM (bit-or 1 2))
(%defconstant csym::@IFQ_MAXLEN 50)
(%defconstant csym::@IFNET_SLOWHZ 1)
(%defconstant csym::@IFAN_ARRIVAL 0)
(%defconstant csym::@IFAN_DEPARTURE 1)
(%defconstant csym::ifr_addr (fref csym::ifr_ifru csym::ifru_addr))
(%defconstant csym::ifr_dstaddr (fref csym::ifr_ifru csym::ifru_dstaddr))
(%defconstant csym::ifr_broadaddr (fref csym::ifr_ifru csym::ifru_broadaddr))
(%defconstant csym::ifr_flags (aref (fref csym::ifr_ifru csym::ifru_flags) 0))
(%defconstant csym::ifr_flagshigh
 (aref (fref csym::ifr_ifru csym::ifru_flags) 1))
(%defconstant csym::ifr_metric (fref csym::ifr_ifru csym::ifru_metric))
(%defconstant csym::ifr_mtu (fref csym::ifr_ifru csym::ifru_mtu))
(%defconstant csym::ifr_phys (fref csym::ifr_ifru csym::ifru_phys))
(%defconstant csym::ifr_media (fref csym::ifr_ifru csym::ifru_media))
(%defconstant csym::ifr_data (fref csym::ifr_ifru csym::ifru_data))
(%defconstant csym::ifr_reqcap (aref (fref csym::ifr_ifru csym::ifru_cap) 0))
(%defconstant csym::ifr_curcap (aref (fref csym::ifr_ifru csym::ifru_cap) 1))
(%defconstant csym::ifr_index (fref csym::ifr_ifru csym::ifru_index))
(%defmacro csym::_@SIZEOF_ADDR_IFREQ (common-lisp-user::ifr)
 (common-lisp:list 'if-exp
                   (common-lisp:list '>
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          'csym::ifr
                                                                          'csym::ifr_ifru)
                                                                         'csym::ifru_addr)
                                                       'csym::sa_len)
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::sockaddr)))
                   (common-lisp:list '+
                                     (common-lisp:list '-
                                                       (common-lisp:list
                                                        'sizeof
                                                        (common-lisp:list
                                                         'struct
                                                         'csym::ifreq))
                                                       (common-lisp:list
                                                        'sizeof
                                                        (common-lisp:list
                                                         'struct
                                                         'csym::sockaddr)))
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          'csym::ifr
                                                                          'csym::ifr_ifru)
                                                                         'csym::ifru_addr)
                                                       'csym::sa_len))
                   (common-lisp:list 'sizeof
                                     (common-lisp:list 'struct 'csym::ifreq))))
(%defconstant csym::@IFSTATMAX 800)
(%defconstant csym::ifc_buf (fref csym::ifc_ifcu csym::ifcu_buf))
(%defconstant csym::ifc_req (fref csym::ifc_ifcu csym::ifcu_req))
(%defconstant csym::@IFLR_PREFIX 32768)
(%defconstant csym::_@NETINET_IN_H_ (c-exp ""))
(%defconstant csym::@IPPROTO_IP 0)
(%defconstant csym::@IPPROTO_ICMP 1)
(%defconstant csym::@IPPROTO_TCP 6)
(%defconstant csym::@IPPROTO_UDP 17)
(%defconstant csym::@INADDR_ANY (cast csym::u_int32_t 0))
(%defconstant csym::@INADDR_BROADCAST (cast csym::u_int32_t 4294967295))
(%defconstant csym::_@SA_FAMILY_T_DECLARED (c-exp ""))
(%defconstant csym::_@STRUCT_IN_ADDR_DECLARED (c-exp ""))
(%defconstant csym::_@BYTEORDER_PROTOTYPED (c-exp ""))
(%defconstant csym::_@BYTEORDER_FUNC_DEFINED (c-exp ""))
(%defmacro csym::htonl (common-lisp-user::x)
 (common-lisp:list 'csym::__bswap32 'csym::x))
(%defmacro csym::htons (common-lisp-user::x)
 (common-lisp:list 'csym::__bswap16 'csym::x))
(%defmacro csym::ntohl (common-lisp-user::x)
 (common-lisp:list 'csym::__bswap32 'csym::x))
(%defmacro csym::ntohs (common-lisp-user::x)
 (common-lisp:list 'csym::__bswap16 'csym::x))
(%defconstant csym::@IPPROTO_RAW 255)
(%defconstant csym::@INET_ADDRSTRLEN 16)
(%defconstant csym::@IPPROTO_HOPOPTS 0)
(%defconstant csym::@IPPROTO_IGMP 2)
(%defconstant csym::@IPPROTO_GGP 3)
(%defconstant csym::@IPPROTO_IPV4 4)
(%defconstant csym::@IPPROTO_IPIP 4)
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
(%defmacro csym::@IN_CLASSA (common-lisp-user::i)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       'csym::i)
                                     '2147483648)
                   '0))
(%defconstant csym::@IN_CLASSA_NET 4278190080)
(%defconstant csym::@IN_CLASSA_NSHIFT 24)
(%defconstant csym::@IN_CLASSA_HOST 16777215)
(%defconstant csym::@IN_CLASSA_MAX 128)
(%defmacro csym::@IN_CLASSB (common-lisp-user::i)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       'csym::i)
                                     '3221225472)
                   '2147483648))
(%defconstant csym::@IN_CLASSB_NET 4294901760)
(%defconstant csym::@IN_CLASSB_NSHIFT 16)
(%defconstant csym::@IN_CLASSB_HOST 65535)
(%defconstant csym::@IN_CLASSB_MAX 65536)
(%defmacro csym::@IN_CLASSC (common-lisp-user::i)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       'csym::i)
                                     '3758096384)
                   '3221225472))
(%defconstant csym::@IN_CLASSC_NET 4294967040)
(%defconstant csym::@IN_CLASSC_NSHIFT 8)
(%defconstant csym::@IN_CLASSC_HOST 255)
(%defmacro csym::@IN_CLASSD (common-lisp-user::i)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       'csym::i)
                                     '4026531840)
                   '3758096384))
(%defconstant csym::@IN_CLASSD_NET 4026531840)
(%defconstant csym::@IN_CLASSD_NSHIFT 28)
(%defconstant csym::@IN_CLASSD_HOST 268435455)
(%defmacro csym::@IN_MULTICAST (common-lisp-user::i)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       'csym::i)
                                     '4026531840)
                   '3758096384))
(%defmacro csym::@IN_EXPERIMENTAL (common-lisp-user::i)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       'csym::i)
                                     '4026531840)
                   '4026531840))
(%defmacro csym::@IN_BADCLASS (common-lisp-user::i)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'cast
                                                       'csym::u_int32_t
                                                       'csym::i)
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
(%defconstant csym::@IP_SENDSRCADDR 7)
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
(%defconstant csym::@IPPROTO_MAXID (+ 51 1))
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
(%defconstant csym::@IPV6PORT_RESERVEDMAX (- 1024 1))
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
(%defmacro csym::@IN6_ARE_ADDR_EQUAL (common-lisp-user::a common-lisp-user::b)
 (common-lisp:list '==
                   (common-lisp:list 'csym::memcmp
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::a)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '0))
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::b)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '0))
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::in6_addr)))
                   '0))
(%defmacro csym::@IN6_IS_ADDR_UNSPECIFIED (common-lisp-user::a)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
                                                                            '12)))))
                                     '0)))
(%defmacro csym::@IN6_IS_ADDR_LOOPBACK (common-lisp-user::a)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
                                                                            '12)))))
                                     (common-lisp:list 'csym::__bswap32 '1))))
(%defmacro csym::@IN6_IS_ADDR_V4COMPAT (common-lisp-user::a)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
                                                                            '12)))))
                                     (common-lisp:list 'csym::__bswap32 '1))))
(%defmacro csym::@IN6_IS_ADDR_V4MAPPED (common-lisp-user::a)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
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
                                                                              'fref
                                                                              (common-lisp:list
                                                                               'mref
                                                                               'csym::a)
                                                                              'csym::__u6_addr)
                                                                             'csym::__u6_addr8)
                                                                            '8)))))
                                     (common-lisp:list 'csym::__bswap32
                                                       '65535))))
(%defconstant csym::__@IPV6_ADDR_SCOPE_NODELOCAL 1)
(%defconstant csym::__@IPV6_ADDR_SCOPE_INTFACELOCAL 1)
(%defconstant csym::__@IPV6_ADDR_SCOPE_LINKLOCAL 2)
(%defconstant csym::__@IPV6_ADDR_SCOPE_SITELOCAL 5)
(%defconstant csym::__@IPV6_ADDR_SCOPE_ORGLOCAL 8)
(%defconstant csym::__@IPV6_ADDR_SCOPE_GLOBAL 14)
(%defmacro csym::@IN6_IS_ADDR_LINKLOCAL (common-lisp-user::a)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::a)
                                                                          'csym::__u6_addr)
                                                                         'csym::__u6_addr8)
                                                       '0)
                                     '254)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::a)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '1)
                                                       '192)
                                     '128)))
(%defmacro csym::@IN6_IS_ADDR_SITELOCAL (common-lisp-user::a)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::a)
                                                                          'csym::__u6_addr)
                                                                         'csym::__u6_addr8)
                                                       '0)
                                     '254)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::a)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '1)
                                                       '192)
                                     '192)))
(%defmacro csym::@IN6_IS_ADDR_MULTICAST (common-lisp-user::a)
 (common-lisp:list '==
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::a)
                                                                         'csym::__u6_addr)
                                                       'csym::__u6_addr8)
                                     '0)
                   '255))
(%defmacro csym::__@IPV6_ADDR_MC_SCOPE (common-lisp-user::a)
 (common-lisp:list 'bit-and
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::a)
                                                                         'csym::__u6_addr)
                                                       'csym::__u6_addr8)
                                     '1)
                   '15))
(%defmacro csym::@IN6_IS_ADDR_MC_NODELOCAL (common-lisp-user::a)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::a)
                                                                          'csym::__u6_addr)
                                                                         'csym::__u6_addr8)
                                                       '0)
                                     '255)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::a)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '1)
                                                       '15)
                                     '1)))
(%defmacro csym::@IN6_IS_ADDR_MC_LINKLOCAL (common-lisp-user::a)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::a)
                                                                          'csym::__u6_addr)
                                                                         'csym::__u6_addr8)
                                                       '0)
                                     '255)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::a)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '1)
                                                       '15)
                                     '2)))
(%defmacro csym::@IN6_IS_ADDR_MC_SITELOCAL (common-lisp-user::a)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::a)
                                                                          'csym::__u6_addr)
                                                                         'csym::__u6_addr8)
                                                       '0)
                                     '255)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::a)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '1)
                                                       '15)
                                     '5)))
(%defmacro csym::@IN6_IS_ADDR_MC_ORGLOCAL (common-lisp-user::a)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::a)
                                                                          'csym::__u6_addr)
                                                                         'csym::__u6_addr8)
                                                       '0)
                                     '255)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::a)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '1)
                                                       '15)
                                     '8)))
(%defmacro csym::@IN6_IS_ADDR_MC_GLOBAL (common-lisp-user::a)
 (common-lisp:list 'and
                   (common-lisp:list '==
                                     (common-lisp:list 'aref
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::a)
                                                                          'csym::__u6_addr)
                                                                         'csym::__u6_addr8)
                                                       '0)
                                     '255)
                   (common-lisp:list '==
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'aref
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::a)
                                                                           'csym::__u6_addr)
                                                                          'csym::__u6_addr8)
                                                                         '1)
                                                       '15)
                                     '14)))
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
(%defconstant csym::@IPV6_BINDV6ONLY 27)
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
(%defconstant csym::@IPV6PROTO_MAXID (+ 103 1))
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
(%defconstant csym::_@SOCKLEN_T_DECLARED (c-exp ""))
(%defconstant csym::_@ARPA_INET_H_ (c-exp ""))
(%defconstant csym::inet_addr csym::__inet_addr)
(%defconstant csym::inet_aton csym::__inet_aton)
(%defconstant csym::inet_lnaof csym::__inet_lnaof)
(%defconstant csym::inet_makeaddr csym::__inet_makeaddr)
(%defconstant csym::inet_neta csym::__inet_neta)
(%defconstant csym::inet_netof csym::__inet_netof)
(%defconstant csym::inet_network csym::__inet_network)
(%defconstant csym::inet_net_ntop csym::__inet_net_ntop)
(%defconstant csym::inet_net_pton csym::__inet_net_pton)
(%defconstant csym::inet_ntoa csym::__inet_ntoa)
(%defconstant csym::inet_pton csym::__inet_pton)
(%defconstant csym::inet_ntop csym::__inet_ntop)
(%defconstant csym::inet_nsap_addr csym::__inet_nsap_addr)
(%defconstant csym::inet_nsap_ntoa csym::__inet_nsap_ntoa)
(%defconstant csym::_@INTTYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE_INTTYPES_H_ (c-exp ""))
(%defconstant csym::@PRI@d8 "d")
(%defconstant csym::@PRI@d16 "d")
(%defconstant csym::@PRI@d32 "d")
(%defconstant csym::@PRI@d64 "lld")
(%defconstant csym::@PRI@d@LEAST8 "d")
(%defconstant csym::@PRI@d@LEAST16 "d")
(%defconstant csym::@PRI@d@LEAST32 "d")
(%defconstant csym::@PRI@d@LEAST64 "lld")
(%defconstant csym::@PRI@d@FAST8 "d")
(%defconstant csym::@PRI@d@FAST16 "d")
(%defconstant csym::@PRI@d@FAST32 "d")
(%defconstant csym::@PRI@d@FAST64 "lld")
(%defconstant csym::@PRI@d@MAX "jd")
(%defconstant csym::@PRI@d@PTR "d")
(%defconstant csym::@PRI@i8 "i")
(%defconstant csym::@PRI@i16 "i")
(%defconstant csym::@PRI@i32 "i")
(%defconstant csym::@PRI@i64 "lli")
(%defconstant csym::@PRI@i@LEAST8 "i")
(%defconstant csym::@PRI@i@LEAST16 "i")
(%defconstant csym::@PRI@i@LEAST32 "i")
(%defconstant csym::@PRI@i@LEAST64 "lli")
(%defconstant csym::@PRI@i@FAST8 "i")
(%defconstant csym::@PRI@i@FAST16 "i")
(%defconstant csym::@PRI@i@FAST32 "i")
(%defconstant csym::@PRI@i@FAST64 "lli")
(%defconstant csym::@PRI@i@MAX "ji")
(%defconstant csym::@PRI@i@PTR "i")
(%defconstant csym::@PRI@o8 "o")
(%defconstant csym::@PRI@o16 "o")
(%defconstant csym::@PRI@o32 "o")
(%defconstant csym::@PRI@o64 "llo")
(%defconstant csym::@PRI@o@LEAST8 "o")
(%defconstant csym::@PRI@o@LEAST16 "o")
(%defconstant csym::@PRI@o@LEAST32 "o")
(%defconstant csym::@PRI@o@LEAST64 "llo")
(%defconstant csym::@PRI@o@FAST8 "o")
(%defconstant csym::@PRI@o@FAST16 "o")
(%defconstant csym::@PRI@o@FAST32 "o")
(%defconstant csym::@PRI@o@FAST64 "llo")
(%defconstant csym::@PRI@o@MAX "jo")
(%defconstant csym::@PRI@o@PTR "o")
(%defconstant csym::@PRI@u8 "u")
(%defconstant csym::@PRI@u16 "u")
(%defconstant csym::@PRI@u32 "u")
(%defconstant csym::@PRI@u64 "llu")
(%defconstant csym::@PRI@u@LEAST8 "u")
(%defconstant csym::@PRI@u@LEAST16 "u")
(%defconstant csym::@PRI@u@LEAST32 "u")
(%defconstant csym::@PRI@u@LEAST64 "llu")
(%defconstant csym::@PRI@u@FAST8 "u")
(%defconstant csym::@PRI@u@FAST16 "u")
(%defconstant csym::@PRI@u@FAST32 "u")
(%defconstant csym::@PRI@u@FAST64 "llu")
(%defconstant csym::@PRI@u@MAX "ju")
(%defconstant csym::@PRI@u@PTR "u")
(%defconstant csym::@PRI@x8 "x")
(%defconstant csym::@PRI@x16 "x")
(%defconstant csym::@PRI@x32 "x")
(%defconstant csym::@PRI@x64 "llx")
(%defconstant csym::@PRI@x@LEAST8 "x")
(%defconstant csym::@PRI@x@LEAST16 "x")
(%defconstant csym::@PRI@x@LEAST32 "x")
(%defconstant csym::@PRI@x@LEAST64 "llx")
(%defconstant csym::@PRI@x@FAST8 "x")
(%defconstant csym::@PRI@x@FAST16 "x")
(%defconstant csym::@PRI@x@FAST32 "x")
(%defconstant csym::@PRI@x@FAST64 "llx")
(%defconstant csym::@PRI@x@MAX "jx")
(%defconstant csym::@PRI@x@PTR "x")
(%defconstant csym::@PRIX8 "X")
(%defconstant csym::@PRIX16 "X")
(%defconstant csym::@PRIX32 "X")
(%defconstant csym::@PRIX64 "llX")
(%defconstant csym::@PRIXLEAST8 "X")
(%defconstant csym::@PRIXLEAST16 "X")
(%defconstant csym::@PRIXLEAST32 "X")
(%defconstant csym::@PRIXLEAST64 "llX")
(%defconstant csym::@PRIXFAST8 "X")
(%defconstant csym::@PRIXFAST16 "X")
(%defconstant csym::@PRIXFAST32 "X")
(%defconstant csym::@PRIXFAST64 "llX")
(%defconstant csym::@PRIXMAX "jX")
(%defconstant csym::@PRIXPTR "X")
(%defconstant csym::@SCN@d8 "hhd")
(%defconstant csym::@SCN@d16 "hd")
(%defconstant csym::@SCN@d32 "d")
(%defconstant csym::@SCN@d64 "lld")
(%defconstant csym::@SCN@d@LEAST8 "hhd")
(%defconstant csym::@SCN@d@LEAST16 "hd")
(%defconstant csym::@SCN@d@LEAST32 "d")
(%defconstant csym::@SCN@d@LEAST64 "lld")
(%defconstant csym::@SCN@d@FAST8 "d")
(%defconstant csym::@SCN@d@FAST16 "d")
(%defconstant csym::@SCN@d@FAST32 "d")
(%defconstant csym::@SCN@d@FAST64 "lld")
(%defconstant csym::@SCN@d@MAX "jd")
(%defconstant csym::@SCN@d@PTR "d")
(%defconstant csym::@SCN@i8 "hhi")
(%defconstant csym::@SCN@i16 "hi")
(%defconstant csym::@SCN@i32 "i")
(%defconstant csym::@SCN@i64 "lli")
(%defconstant csym::@SCN@i@LEAST8 "hhi")
(%defconstant csym::@SCN@i@LEAST16 "hi")
(%defconstant csym::@SCN@i@LEAST32 "i")
(%defconstant csym::@SCN@i@LEAST64 "lli")
(%defconstant csym::@SCN@i@FAST8 "i")
(%defconstant csym::@SCN@i@FAST16 "i")
(%defconstant csym::@SCN@i@FAST32 "i")
(%defconstant csym::@SCN@i@FAST64 "lli")
(%defconstant csym::@SCN@i@MAX "ji")
(%defconstant csym::@SCN@i@PTR "i")
(%defconstant csym::@SCN@o8 "hho")
(%defconstant csym::@SCN@o16 "ho")
(%defconstant csym::@SCN@o32 "o")
(%defconstant csym::@SCN@o64 "llo")
(%defconstant csym::@SCN@o@LEAST8 "hho")
(%defconstant csym::@SCN@o@LEAST16 "ho")
(%defconstant csym::@SCN@o@LEAST32 "o")
(%defconstant csym::@SCN@o@LEAST64 "llo")
(%defconstant csym::@SCN@o@FAST8 "o")
(%defconstant csym::@SCN@o@FAST16 "o")
(%defconstant csym::@SCN@o@FAST32 "o")
(%defconstant csym::@SCN@o@FAST64 "llo")
(%defconstant csym::@SCN@o@MAX "jo")
(%defconstant csym::@SCN@o@PTR "o")
(%defconstant csym::@SCN@u8 "hhu")
(%defconstant csym::@SCN@u16 "hu")
(%defconstant csym::@SCN@u32 "u")
(%defconstant csym::@SCN@u64 "llu")
(%defconstant csym::@SCN@u@LEAST8 "hhu")
(%defconstant csym::@SCN@u@LEAST16 "hu")
(%defconstant csym::@SCN@u@LEAST32 "u")
(%defconstant csym::@SCN@u@LEAST64 "llu")
(%defconstant csym::@SCN@u@FAST8 "u")
(%defconstant csym::@SCN@u@FAST16 "u")
(%defconstant csym::@SCN@u@FAST32 "u")
(%defconstant csym::@SCN@u@FAST64 "llu")
(%defconstant csym::@SCN@u@MAX "ju")
(%defconstant csym::@SCN@u@PTR "u")
(%defconstant csym::@SCN@x8 "hhx")
(%defconstant csym::@SCN@x16 "hx")
(%defconstant csym::@SCN@x32 "x")
(%defconstant csym::@SCN@x64 "llx")
(%defconstant csym::@SCN@x@LEAST8 "hhx")
(%defconstant csym::@SCN@x@LEAST16 "hx")
(%defconstant csym::@SCN@x@LEAST32 "x")
(%defconstant csym::@SCN@x@LEAST64 "llx")
(%defconstant csym::@SCN@x@FAST8 "x")
(%defconstant csym::@SCN@x@FAST16 "x")
(%defconstant csym::@SCN@x@FAST32 "x")
(%defconstant csym::@SCN@x@FAST64 "llx")
(%defconstant csym::@SCN@x@MAX "jx")
(%defconstant csym::@SCN@x@PTR "x")
(%defconstant csym::_@SYS_STDINT_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__STDINT_H_ (c-exp ""))
(%defmacro csym::@INT8_C (common-lisp-user::c) 'csym::c)
(%defmacro csym::@INT16_C (common-lisp-user::c) 'csym::c)
(%defmacro csym::@INT32_C (common-lisp-user::c) 'csym::c)
(%defmacro csym::@INT64_C (common-lisp-user::c) '"<macro with ## operator>")
(%defmacro csym::@UINT8_C (common-lisp-user::c) 'csym::c)
(%defmacro csym::@UINT16_C (common-lisp-user::c) 'csym::c)
(%defmacro csym::@UINT32_C (common-lisp-user::c) '"<macro with ## operator>")
(%defmacro csym::@UINT64_C (common-lisp-user::c) '"<macro with ## operator>")
(%defmacro csym::@INTMAX_C (common-lisp-user::c) '"<macro with ## operator>")
(%defmacro csym::@UINTMAX_C (common-lisp-user::c) '"<macro with ## operator>")
(%defconstant csym::@INT8_MIN (- (- 127) 1))
(%defconstant csym::@INT16_MIN (- (- 32767) 1))
(%defconstant csym::@INT32_MIN (- (- 2147483647) 1))
(%defconstant csym::@INT64_MIN (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::@INT8_MAX 127)
(%defconstant csym::@INT16_MAX 32767)
(%defconstant csym::@INT32_MAX 2147483647)
(%defconstant csym::@INT64_MAX (cast long-long 9223372036854775807))
(%defconstant csym::@UINT8_MAX 255)
(%defconstant csym::@UINT16_MAX 65535)
(%defconstant csym::@UINT32_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@UINT64_MAX (cast unsigned-long-long 18446744073709551615))
(%defconstant csym::@INT_LEAST8_MIN (- (- 127) 1))
(%defconstant csym::@INT_LEAST16_MIN (- (- 32767) 1))
(%defconstant csym::@INT_LEAST32_MIN (- (- 2147483647) 1))
(%defconstant csym::@INT_LEAST64_MIN
 (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::@INT_LEAST8_MAX 127)
(%defconstant csym::@INT_LEAST16_MAX 32767)
(%defconstant csym::@INT_LEAST32_MAX 2147483647)
(%defconstant csym::@INT_LEAST64_MAX (cast long-long 9223372036854775807))
(%defconstant csym::@UINT_LEAST8_MAX 255)
(%defconstant csym::@UINT_LEAST16_MAX 65535)
(%defconstant csym::@UINT_LEAST32_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@UINT_LEAST64_MAX
 (cast unsigned-long-long 18446744073709551615))
(%defconstant csym::@INT_FAST8_MIN (- (- 2147483647) 1))
(%defconstant csym::@INT_FAST16_MIN (- (- 2147483647) 1))
(%defconstant csym::@INT_FAST32_MIN (- (- 2147483647) 1))
(%defconstant csym::@INT_FAST64_MIN
 (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::@INT_FAST8_MAX 2147483647)
(%defconstant csym::@INT_FAST16_MAX 2147483647)
(%defconstant csym::@INT_FAST32_MAX 2147483647)
(%defconstant csym::@INT_FAST64_MAX (cast long-long 9223372036854775807))
(%defconstant csym::@UINT_FAST8_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@UINT_FAST16_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@UINT_FAST32_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@UINT_FAST64_MAX
 (cast unsigned-long-long 18446744073709551615))
(%defconstant csym::@INTPTR_MIN (- (- 2147483647) 1))
(%defconstant csym::@INTPTR_MAX 2147483647)
(%defconstant csym::@UINTPTR_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@INTMAX_MIN (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::@INTMAX_MAX (cast long-long 9223372036854775807))
(%defconstant csym::@UINTMAX_MAX (cast unsigned-long-long 18446744073709551615))
(%defconstant csym::@PTRDIFF_MIN (- (- 2147483647) 1))
(%defconstant csym::@PTRDIFF_MAX 2147483647)
(%defconstant csym::@SIG_ATOMIC_MIN (- (- 2147483647) 1))
(%defconstant csym::@SIG_ATOMIC_MAX 2147483647)
(%defconstant csym::@SIZE_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@WCHAR_MIN (- (- 2147483647) 1))
(%defconstant csym::@WCHAR_MAX 2147483647)
(%defconstant csym::@WINT_MIN (- (- 2147483647) 1))
(%defconstant csym::@WINT_MAX 2147483647)
(%defconstant csym::_@SYS_IPC_H_ (c-exp ""))
(%defconstant csym::@IPC_R 256)
(%defconstant csym::@IPC_W 128)
(%defconstant csym::@IPC_M 4096)
(%defconstant csym::@IPC_CREAT 512)
(%defconstant csym::@IPC_EXCL 1024)
(%defconstant csym::@IPC_NOWAIT 2048)
(%defconstant csym::@IPC_PRIVATE (cast csym::key_t 0))
(%defconstant csym::@IPC_RMID 0)
(%defconstant csym::@IPC_SET 1)
(%defconstant csym::@IPC_STAT 2)
(%defconstant csym::@IPC_INFO 3)
(%defconstant csym::_@ISO646_H_ (c-exp ""))
(%defconstant csym::and (c-exp "&&"))
(%defconstant csym::and_eq (c-exp "&="))
(%defconstant csym::bitand (c-exp "&"))
(%defconstant csym::bitor (c-exp "|"))
(%defconstant csym::compl (c-exp "~"))
(%defconstant csym::not (c-exp "!"))
(%defconstant csym::not_eq (c-exp "!="))
(%defconstant csym::or (c-exp "||"))
(%defconstant csym::or_eq (c-exp "|="))
(%defconstant csym::xor (c-exp "^"))
(%defconstant csym::xor_eq (c-exp "^="))
(%defconstant csym::_@LANGINFO_H_ (c-exp ""))
(%defconstant csym::_@NL_ITEM_DECLARED (c-exp ""))
(%defconstant csym::@CODESET 0)
(%defconstant csym::@D_T_FMT 1)
(%defconstant csym::@D_FMT 2)
(%defconstant csym::@T_FMT 3)
(%defconstant csym::@T_FMT_AMPM 4)
(%defconstant csym::@AM_STR 5)
(%defconstant csym::@PM_STR 6)
(%defconstant csym::@DAY_1 7)
(%defconstant csym::@DAY_2 8)
(%defconstant csym::@DAY_3 9)
(%defconstant csym::@DAY_4 10)
(%defconstant csym::@DAY_5 11)
(%defconstant csym::@DAY_6 12)
(%defconstant csym::@DAY_7 13)
(%defconstant csym::@ABDAY_1 14)
(%defconstant csym::@ABDAY_2 15)
(%defconstant csym::@ABDAY_3 16)
(%defconstant csym::@ABDAY_4 17)
(%defconstant csym::@ABDAY_5 18)
(%defconstant csym::@ABDAY_6 19)
(%defconstant csym::@ABDAY_7 20)
(%defconstant csym::@MON_1 21)
(%defconstant csym::@MON_2 22)
(%defconstant csym::@MON_3 23)
(%defconstant csym::@MON_4 24)
(%defconstant csym::@MON_5 25)
(%defconstant csym::@MON_6 26)
(%defconstant csym::@MON_7 27)
(%defconstant csym::@MON_8 28)
(%defconstant csym::@MON_9 29)
(%defconstant csym::@MON_10 30)
(%defconstant csym::@MON_11 31)
(%defconstant csym::@MON_12 32)
(%defconstant csym::@ABMON_1 33)
(%defconstant csym::@ABMON_2 34)
(%defconstant csym::@ABMON_3 35)
(%defconstant csym::@ABMON_4 36)
(%defconstant csym::@ABMON_5 37)
(%defconstant csym::@ABMON_6 38)
(%defconstant csym::@ABMON_7 39)
(%defconstant csym::@ABMON_8 40)
(%defconstant csym::@ABMON_9 41)
(%defconstant csym::@ABMON_10 42)
(%defconstant csym::@ABMON_11 43)
(%defconstant csym::@ABMON_12 44)
(%defconstant csym::@ERA 45)
(%defconstant csym::@ERA_D_FMT 46)
(%defconstant csym::@ERA_D_T_FMT 47)
(%defconstant csym::@ERA_T_FMT 48)
(%defconstant csym::@ALT_DIGITS 49)
(%defconstant csym::@RADIXCHAR 50)
(%defconstant csym::@THOUSEP 51)
(%defconstant csym::@YESEXPR 52)
(%defconstant csym::@NOEXPR 53)
(%defconstant csym::@YESSTR 54)
(%defconstant csym::@NOSTR 55)
(%defconstant csym::@CRNCYSTR 56)
(%defconstant csym::@D_MD_ORDER 57)
(%defconstant csym::_@LIBGEN_H_ (c-exp ""))
(%defconstant csym::_@LIMITS_H_ (c-exp ""))
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
(%defconstant csym::_@POSIX_RE_DUP_MAX 255)
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
(%defconstant csym::__@SSIZE_MAX 2147483647)
(%defconstant csym::__@SIZE_T_MAX (cast unsigned-int 4294967295))
(%defconstant csym::__@OFF_MAX (cast long-long 9223372036854775807))
(%defconstant csym::__@OFF_MIN (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::__@UQUAD_MAX (cast unsigned-long-long 18446744073709551615))
(%defconstant csym::__@QUAD_MAX (cast long-long 9223372036854775807))
(%defconstant csym::__@QUAD_MIN (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::__@LONG_BIT 32)
(%defconstant csym::__@WORD_BIT 32)
(%defconstant csym::@CHAR_BIT 8)
(%defconstant csym::@SCHAR_MAX 127)
(%defconstant csym::@SCHAR_MIN (- (- 127) 1))
(%defconstant csym::@UCHAR_MAX 255)
(%defconstant csym::@CHAR_MAX 127)
(%defconstant csym::@CHAR_MIN (- (- 127) 1))
(%defconstant csym::@USHRT_MAX 65535)
(%defconstant csym::@SHRT_MAX 32767)
(%defconstant csym::@SHRT_MIN (- (- 32767) 1))
(%defconstant csym::@UINT_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@INT_MAX 2147483647)
(%defconstant csym::@INT_MIN (- (- 2147483647) 1))
(%defconstant csym::@ULONG_MAX (cast unsigned-long 4294967295))
(%defconstant csym::@LONG_MAX (cast long 2147483647))
(%defconstant csym::@LONG_MIN (- (- (cast long 2147483647)) 1))
(%defconstant csym::@ULLONG_MAX (cast unsigned-long-long 18446744073709551615))
(%defconstant csym::@LLONG_MAX (cast long-long 9223372036854775807))
(%defconstant csym::@LLONG_MIN (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::@SSIZE_MAX 2147483647)
(%defconstant csym::@SIZE_T_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@OFF_MAX (cast long-long 9223372036854775807))
(%defconstant csym::@OFF_MIN (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::@GID_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@UID_MAX (cast unsigned-int 4294967295))
(%defconstant csym::@UQUAD_MAX (cast unsigned-long-long 18446744073709551615))
(%defconstant csym::@QUAD_MAX (cast long-long 9223372036854775807))
(%defconstant csym::@QUAD_MIN (- (- (cast long-long 9223372036854775807)) 1))
(%defconstant csym::@LONG_BIT 32)
(%defconstant csym::@WORD_BIT 32)
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
(%defconstant csym::_@LOCALE_H_ (c-exp ""))
(%defconstant csym::@LC_ALL 0)
(%defconstant csym::@LC_COLLATE 1)
(%defconstant csym::@LC_CTYPE 2)
(%defconstant csym::@LC_MONETARY 3)
(%defconstant csym::@LC_NUMERIC 4)
(%defconstant csym::@LC_TIME 5)
(%defconstant csym::@LC_MESSAGES 6)
(%defconstant csym::_@LC_LAST 7)
(%defconstant csym::_@MATH_H_ (c-exp ""))
(%defconstant csym::__@MATH_BUILTIN_CONSTANTS (c-exp ""))
(%defconstant csym::__@MATH_BUILTIN_RELOPS (c-exp ""))
(%defconstant csym::@HUGE_VAL (csym::__builtin_huge_val))
(%defconstant csym::@FP_ILOGB0 (- 2147483647))
(%defconstant csym::@FP_ILOGBNAN 2147483647)
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
(%defmacro csym::fpclassify (common-lisp-user::x)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof 'csym::x)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__fpclassifyf 'csym::x)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        'csym::x)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::__fpclassifyd
                                                       'csym::x)
                                     (common-lisp:list 'csym::__fpclassifyl
                                                       'csym::x))))
(%defmacro csym::isfinite (common-lisp-user::x)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof 'csym::x)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__isfinitef 'csym::x)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        'csym::x)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::__isfinite
                                                       'csym::x)
                                     (common-lisp:list 'csym::__isfinitel
                                                       'csym::x))))
(%defmacro csym::isinf (common-lisp-user::x)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof 'csym::x)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__isinff 'csym::x)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        'csym::x)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'if-exp
                                                       (common-lisp:list '==
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'csym::x)
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'float))
                                                       (common-lisp:list
                                                        'csym::__isinff
                                                        'csym::x)
                                                       (common-lisp:list
                                                        'if-exp
                                                        (common-lisp:list '==
                                                                          (common-lisp:list
                                                                           'sizeof
                                                                           'csym::x)
                                                                          (common-lisp:list
                                                                           'sizeof
                                                                           'double))
                                                        (common-lisp:list
                                                         'csym::isinf
                                                         'csym::x)
                                                        (common-lisp:list
                                                         'csym::__isinfl
                                                         'csym::x)))
                                     (common-lisp:list 'csym::__isinfl
                                                       'csym::x))))
(%defmacro csym::isnan (common-lisp-user::x)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof 'csym::x)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::isnanf 'csym::x)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        'csym::x)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'if-exp
                                                       (common-lisp:list '==
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'csym::x)
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'float))
                                                       (common-lisp:list
                                                        'csym::isnanf
                                                        'csym::x)
                                                       (common-lisp:list
                                                        'if-exp
                                                        (common-lisp:list '==
                                                                          (common-lisp:list
                                                                           'sizeof
                                                                           'csym::x)
                                                                          (common-lisp:list
                                                                           'sizeof
                                                                           'double))
                                                        (common-lisp:list
                                                         'csym::isnan
                                                         'csym::x)
                                                        (common-lisp:list
                                                         'csym::__isnanl
                                                         'csym::x)))
                                     (common-lisp:list 'csym::__isnanl
                                                       'csym::x))))
(%defmacro csym::isnormal (common-lisp-user::x)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof 'csym::x)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__isnormalf 'csym::x)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        'csym::x)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::__isnormal
                                                       'csym::x)
                                     (common-lisp:list 'csym::__isnormall
                                                       'csym::x))))
(%defmacro csym::isgreater (common-lisp-user::x common-lisp-user::y)
 (common-lisp:list 'csym::__builtin_isgreater 'csym::x 'csym::y))
(%defmacro csym::isgreaterequal (common-lisp-user::x common-lisp-user::y)
 (common-lisp:list 'csym::__builtin_isgreaterequal 'csym::x 'csym::y))
(%defmacro csym::isless (common-lisp-user::x common-lisp-user::y)
 (common-lisp:list 'csym::__builtin_isless 'csym::x 'csym::y))
(%defmacro csym::islessequal (common-lisp-user::x common-lisp-user::y)
 (common-lisp:list 'csym::__builtin_islessequal 'csym::x 'csym::y))
(%defmacro csym::islessgreater (common-lisp-user::x common-lisp-user::y)
 (common-lisp:list 'csym::__builtin_islessgreater 'csym::x 'csym::y))
(%defmacro csym::isunordered (common-lisp-user::x common-lisp-user::y)
 (common-lisp:list 'csym::__builtin_isunordered 'csym::x 'csym::y))
(%defmacro csym::signbit (common-lisp-user::x)
 (common-lisp:list 'if-exp
                   (common-lisp:list '==
                                     (common-lisp:list 'sizeof 'csym::x)
                                     (common-lisp:list 'sizeof 'float))
                   (common-lisp:list 'csym::__signbitf 'csym::x)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '==
                                                       (common-lisp:list
                                                        'sizeof
                                                        'csym::x)
                                                       (common-lisp:list
                                                        'sizeof
                                                        'double))
                                     (common-lisp:list 'csym::__signbit
                                                       'csym::x)
                                     (common-lisp:list 'csym::__signbitl
                                                       'csym::x))))
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
(%defconstant csym::@HUGE (cast float 3.4028234663852886d+38))
(%defconstant csym::@X_TLOSS 1.414847550405688d+16)
(%defconstant csym::@DOMAIN 1)
(%defconstant csym::@SING 2)
(%defconstant csym::@OVERFLOW 3)
(%defconstant csym::@UNDERFLOW 4)
(%defconstant csym::@TLOSS 5)
(%defconstant csym::@PLOSS 6)
(%defconstant csym::_@SYS_MMAN_H_ (c-exp ""))
(%defconstant csym::@INHERIT_SHARE 0)
(%defconstant csym::@INHERIT_COPY 1)
(%defconstant csym::@INHERIT_NONE 2)
(%defconstant csym::@PROT_NONE 0)
(%defconstant csym::@PROT_READ 1)
(%defconstant csym::@PROT_WRITE 2)
(%defconstant csym::@PROT_EXEC 4)
(%defconstant csym::@MAP_SHARED 1)
(%defconstant csym::@MAP_PRIVATE 2)
(%defconstant csym::@MAP_COPY 2)
(%defconstant csym::@MAP_FIXED 16)
(%defconstant csym::@MAP_RENAME 32)
(%defconstant csym::@MAP_NORESERVE 64)
(%defconstant csym::@MAP_RESERVED0080 128)
(%defconstant csym::@MAP_RESERVED0100 256)
(%defconstant csym::@MAP_HASSEMAPHORE 512)
(%defconstant csym::@MAP_STACK 1024)
(%defconstant csym::@MAP_NOSYNC 2048)
(%defconstant csym::@MAP_FILE 0)
(%defconstant csym::@MAP_ANON 4096)
(%defconstant csym::@MAP_NOCORE 131072)
(%defconstant csym::@MCL_CURRENT 1)
(%defconstant csym::@MCL_FUTURE 2)
(%defconstant csym::@MAP_FAILED (cast (ptr void) (- 1)))
(%defconstant csym::@MS_SYNC 0)
(%defconstant csym::@MS_ASYNC 1)
(%defconstant csym::@MS_INVALIDATE 2)
(%defconstant csym::@MADV_NORMAL 0)
(%defconstant csym::@MADV_RANDOM 1)
(%defconstant csym::@MADV_SEQUENTIAL 2)
(%defconstant csym::@MADV_WILLNEED 3)
(%defconstant csym::@MADV_DONTNEED 4)
(%defconstant csym::@MADV_FREE 5)
(%defconstant csym::@MADV_NOSYNC 6)
(%defconstant csym::@MADV_AUTOSYNC 7)
(%defconstant csym::@MADV_NOCORE 8)
(%defconstant csym::@MADV_CORE 9)
(%defconstant csym::@MADV_PROTECT 10)
(%defconstant csym::@MINCORE_INCORE 1)
(%defconstant csym::@MINCORE_REFERENCED 2)
(%defconstant csym::@MINCORE_MODIFIED 4)
(%defconstant csym::@MINCORE_REFERENCED_OTHER 8)
(%defconstant csym::@MINCORE_MODIFIED_OTHER 16)
(%defconstant csym::@POSIX_MADV_NORMAL 0)
(%defconstant csym::@POSIX_MADV_RANDOM 1)
(%defconstant csym::@POSIX_MADV_SEQUENTIAL 2)
(%defconstant csym::@POSIX_MADV_WILLNEED 3)
(%defconstant csym::@POSIX_MADV_DONTNEED 4)
(%defconstant csym::_@MONETARY_H_ (c-exp ""))
(%defconstant csym::_@SYS_MSG_H_ (c-exp ""))
(%defconstant csym::@MSG_NOERROR 4096)
(%defconstant csym::_@NDBM_H_ (c-exp ""))
(%defconstant csym::_@DB_H_ (c-exp ""))
(%defconstant csym::@RET_ERROR (- 1))
(%defconstant csym::@RET_SUCCESS 0)
(%defconstant csym::@RET_SPECIAL 1)
(%defconstant csym::@MAX_PAGE_NUMBER 4294967295)
(%defconstant csym::@MAX_PAGE_OFFSET 65535)
(%defconstant csym::@MAX_REC_NUMBER 4294967295)
(%defconstant csym::@R_CURSOR 1)
(%defconstant csym::__@R_UNUSED 2)
(%defconstant csym::@R_FIRST 3)
(%defconstant csym::@R_IAFTER 4)
(%defconstant csym::@R_IBEFORE 5)
(%defconstant csym::@R_LAST 6)
(%defconstant csym::@R_NEXT 7)
(%defconstant csym::@R_NOOVERWRITE 8)
(%defconstant csym::@R_PREV 9)
(%defconstant csym::@R_SETCURSOR 10)
(%defconstant csym::@R_RECNOSYNC 11)
(%defconstant csym::@DB_LOCK 536870912)
(%defconstant csym::@DB_SHMEM 1073741824)
(%defconstant csym::@DB_TXN 2147483648)
(%defconstant csym::@BTREEMAGIC 340322)
(%defconstant csym::@BTREEVERSION 3)
(%defconstant csym::@R_DUP 1)
(%defconstant csym::@HASHMAGIC 398689)
(%defconstant csym::@HASHVERSION 2)
(%defconstant csym::@R_FIXEDLEN 1)
(%defconstant csym::@R_NOKEY 2)
(%defconstant csym::@R_SNAPSHOT 4)
(%defconstant csym::@DBM_RDONLY 0)
(%defconstant csym::@DBM_INSERT 0)
(%defconstant csym::@DBM_REPLACE 1)
(%defconstant csym::@DBM_SUFFIX ".db")
(%defmacro csym::dbm_pagfno (common-lisp-user::a)
 'csym::@DBM_PAGFNO_NOT_AVAILABLE)
(%defconstant csym::_@NETDB_H_ (c-exp ""))
(%defconstant csym::_@PATH_HEQUIV "/etc/hosts.equiv")
(%defconstant csym::_@PATH_HOSTS "/etc/hosts")
(%defconstant csym::_@PATH_NETWORKS "/etc/networks")
(%defconstant csym::_@PATH_PROTOCOLS "/etc/protocols")
(%defconstant csym::_@PATH_SERVICES "/etc/services")
(%defconstant csym::h_errno (mref (csym::__h_error)))
(%defconstant csym::h_addr (aref csym::h_addr_list 0))
(%defconstant csym::@NETDB_INTERNAL (- 1))
(%defconstant csym::@NETDB_SUCCESS 0)
(%defconstant csym::@HOST_NOT_FOUND 1)
(%defconstant csym::@TRY_AGAIN 2)
(%defconstant csym::@NO_RECOVERY 3)
(%defconstant csym::@NO_DATA 4)
(%defconstant csym::@NO_ADDRESS 4)
(%defconstant csym::@EAI_AGAIN 2)
(%defconstant csym::@EAI_BADFLAGS 3)
(%defconstant csym::@EAI_FAIL 4)
(%defconstant csym::@EAI_FAMILY 5)
(%defconstant csym::@EAI_MEMORY 6)
(%defconstant csym::@EAI_NONAME 8)
(%defconstant csym::@EAI_SERVICE 9)
(%defconstant csym::@EAI_SOCKTYPE 10)
(%defconstant csym::@EAI_SYSTEM 11)
(%defconstant csym::@EAI_BADHINTS 12)
(%defconstant csym::@EAI_PROTOCOL 13)
(%defconstant csym::@EAI_MAX 14)
(%defconstant csym::@AI_PASSIVE 1)
(%defconstant csym::@AI_CANONNAME 2)
(%defconstant csym::@AI_NUMERICHOST 4)
(%defconstant csym::@AI_MASK (bit-or 1 2 4 1024))
(%defconstant csym::@AI_ALL 256)
(%defconstant csym::@AI_V4MAPPED_CFG 512)
(%defconstant csym::@AI_ADDRCONFIG 1024)
(%defconstant csym::@AI_V4MAPPED 2048)
(%defconstant csym::@AI_DEFAULT (bit-or 512 1024))
(%defconstant csym::@NI_MAXHOST 1025)
(%defconstant csym::@NI_MAXSERV 32)
(%defconstant csym::@NI_NOFQDN 1)
(%defconstant csym::@NI_NUMERICHOST 2)
(%defconstant csym::@NI_NAMEREQD 4)
(%defconstant csym::@NI_NUMERICSERV 8)
(%defconstant csym::@NI_DGRAM 16)
(%defconstant csym::@NI_WITHSCOPEID 32)
(%defconstant csym::@SCOPE_DELIMITER #\%)
(%defconstant csym::_@NL_TYPES_H_ (c-exp ""))
(%defconstant csym::@NL_SETD 0)
(%defconstant csym::@NL_CAT_LOCALE 1)
(%defconstant csym::_@SYS_POLL_H_ (c-exp ""))
(%defconstant csym::@POLLIN 1)
(%defconstant csym::@POLLPRI 2)
(%defconstant csym::@POLLOUT 4)
(%defconstant csym::@POLLRDNORM 64)
(%defconstant csym::@POLLWRNORM 4)
(%defconstant csym::@POLLRDBAND 128)
(%defconstant csym::@POLLWRBAND 256)
(%defconstant csym::@POLLINIGNEOF 8192)
(%defconstant csym::@POLLERR 8)
(%defconstant csym::@POLLHUP 16)
(%defconstant csym::@POLLNVAL 32)
(%defconstant csym::@POLLSTANDARD (bit-or 1 2 4 64 128 256 8 16 32))
(%defconstant csym::@INFTIM (- 1))
(%defconstant csym::_@PTHREAD_H_ (c-exp ""))
(%defconstant csym::_@SCHED_H_ (c-exp ""))
(%defconstant csym::@SCHED_FIFO 1)
(%defconstant csym::@SCHED_OTHER 2)
(%defconstant csym::@SCHED_RR 3)
(%defconstant csym::@PTHREAD_DESTRUCTOR_ITERATIONS 4)
(%defconstant csym::@PTHREAD_KEYS_MAX 256)
(%defconstant csym::@PTHREAD_STACK_MIN 1024)
(%defconstant csym::@PTHREAD_THREADS_MAX (cast unsigned-long 4294967295))
(%defconstant csym::@PTHREAD_BARRIER_SERIAL_THREAD (- 1))
(%defconstant csym::@PTHREAD_DETACHED 1)
(%defconstant csym::@PTHREAD_SCOPE_SYSTEM 2)
(%defconstant csym::@PTHREAD_INHERIT_SCHED 4)
(%defconstant csym::@PTHREAD_NOFLOAT 8)
(%defconstant csym::@PTHREAD_CREATE_DETACHED 1)
(%defconstant csym::@PTHREAD_CREATE_JOINABLE 0)
(%defconstant csym::@PTHREAD_SCOPE_PROCESS 0)
(%defconstant csym::@PTHREAD_EXPLICIT_SCHED 0)
(%defconstant csym::@PTHREAD_PROCESS_PRIVATE 0)
(%defconstant csym::@PTHREAD_PROCESS_SHARED 1)
(%defconstant csym::@PTHREAD_CANCEL_ENABLE 0)
(%defconstant csym::@PTHREAD_CANCEL_DISABLE 1)
(%defconstant csym::@PTHREAD_CANCEL_DEFERRED 0)
(%defconstant csym::@PTHREAD_CANCEL_ASYNCHRONOUS 2)
(%defconstant csym::@PTHREAD_CANCELED (cast (ptr void) 1))
(%defconstant csym::@PTHREAD_NEEDS_INIT 0)
(%defconstant csym::@PTHREAD_DONE_INIT 1)
(%defconstant csym::@PTHREAD_ONCE_INIT (c-exp "{ 0 , ((void *)0) }"))
(%defconstant csym::@PTHREAD_MUTEX_INITIALIZER (cast (ptr void) 0))
(%defconstant csym::@PTHREAD_COND_INITIALIZER (cast (ptr void) 0))
(%defconstant csym::@PTHREAD_RWLOCK_INITIALIZER (cast (ptr void) 0))
(%defconstant csym::pthread_condattr_default (cast (ptr void) 0))
(%defconstant csym::pthread_mutexattr_default (cast (ptr void) 0))
(%defconstant csym::pthread_attr_default (cast (ptr void) 0))
(%defconstant csym::@PTHREAD_PRIO_NONE 0)
(%defconstant csym::@PTHREAD_PRIO_INHERIT 1)
(%defconstant csym::@PTHREAD_PRIO_PROTECT 2)
(%defconstant csym::@PTHREAD_MUTEX_DEFAULT csym::@PTHREAD_MUTEX_ERRORCHECK)
(%defconstant csym::@MUTEX_TYPE_FAST csym::@PTHREAD_MUTEX_NORMAL)
(%defconstant csym::@MUTEX_TYPE_COUNTING_FAST csym::@PTHREAD_MUTEX_RECURSIVE)
(%defconstant csym::_@PWD_H_ (c-exp ""))
(%defconstant csym::_@PATH_PWD "/etc")
(%defconstant csym::_@PATH_PASSWD "/etc/passwd")
(%defconstant csym::_@PASSWD "passwd")
(%defconstant csym::_@PATH_MASTERPASSWD "/etc/master.passwd")
(%defconstant csym::_@MASTERPASSWD "master.passwd")
(%defconstant csym::_@PATH_MP_DB "/etc/pwd.db")
(%defconstant csym::_@MP_DB "pwd.db")
(%defconstant csym::_@PATH_SMP_DB "/etc/spwd.db")
(%defconstant csym::_@SMP_DB "spwd.db")
(%defconstant csym::_@PATH_PWD_MKDB "/usr/sbin/pwd_mkdb")
(%defconstant csym::_@PW_VERSION_MASK #\ð)
(%defmacro csym::_@PW_VERSIONED (common-lisp-user::x common-lisp-user::v)
 (common-lisp:list 'cast
                   'unsigned-char
                   (common-lisp:list 'bit-or
                                     (common-lisp:list 'bit-and 'csym::x '207)
                                     (common-lisp:list '<< 'csym::v '4))))
(%defconstant csym::_@PW_KEYBYNAME #\1)
(%defconstant csym::_@PW_KEYBYNUM #\2)
(%defconstant csym::_@PW_KEYBYUID #\3)
(%defconstant csym::_@PW_KEYYPENABLED #\4)
(%defconstant csym::_@PW_KEYYPBYNUM #\5)
(%defconstant csym::_@PWD_VERSION_KEY "~|377|VERSION")
(%defconstant csym::_@PWD_CURRENT_VERSION #\^d)
(%defconstant csym::_@PASSWORD_EFMT1 #\_)
(%defconstant csym::_@PASSWORD_LEN 128)
(%defmacro csym::_@PWF (common-lisp-user::x) (common-lisp:list '<< '1 'csym::x))
(%defconstant csym::_@PWF_NAME (<< 1 0))
(%defconstant csym::_@PWF_PASSWD (<< 1 1))
(%defconstant csym::_@PWF_UID (<< 1 2))
(%defconstant csym::_@PWF_GID (<< 1 3))
(%defconstant csym::_@PWF_CHANGE (<< 1 4))
(%defconstant csym::_@PWF_CLASS (<< 1 5))
(%defconstant csym::_@PWF_GECOS (<< 1 6))
(%defconstant csym::_@PWF_DIR (<< 1 7))
(%defconstant csym::_@PWF_SHELL (<< 1 8))
(%defconstant csym::_@PWF_EXPIRE (<< 1 9))
(%defconstant csym::_@PWF_SOURCE 12288)
(%defconstant csym::_@PWF_FILES 4096)
(%defconstant csym::_@PWF_NIS 8192)
(%defconstant csym::_@PWF_HESIOD 12288)
(%defconstant csym::_@REGEX_H_ (c-exp ""))
(%defconstant csym::@REG_BASIC 0)
(%defconstant csym::@REG_EXTENDED 1)
(%defconstant csym::@REG_ICASE 2)
(%defconstant csym::@REG_NOSUB 4)
(%defconstant csym::@REG_NEWLINE 8)
(%defconstant csym::@REG_NOSPEC 16)
(%defconstant csym::@REG_PEND 32)
(%defconstant csym::@REG_DUMP 128)
(%defconstant csym::@REG_ENOSYS (- 1))
(%defconstant csym::@REG_NOMATCH 1)
(%defconstant csym::@REG_BADPAT 2)
(%defconstant csym::@REG_ECOLLATE 3)
(%defconstant csym::@REG_ECTYPE 4)
(%defconstant csym::@REG_EESCAPE 5)
(%defconstant csym::@REG_ESUBREG 6)
(%defconstant csym::@REG_EBRACK 7)
(%defconstant csym::@REG_EPAREN 8)
(%defconstant csym::@REG_EBRACE 9)
(%defconstant csym::@REG_BADBR 10)
(%defconstant csym::@REG_ERANGE 11)
(%defconstant csym::@REG_ESPACE 12)
(%defconstant csym::@REG_BADRPT 13)
(%defconstant csym::@REG_EMPTY 14)
(%defconstant csym::@REG_ASSERT 15)
(%defconstant csym::@REG_INVARG 16)
(%defconstant csym::@REG_ILLSEQ 17)
(%defconstant csym::@REG_ATOI 255)
(%defconstant csym::@REG_ITOA 256)
(%defconstant csym::@REG_NOTBOL 1)
(%defconstant csym::@REG_NOTEOL 2)
(%defconstant csym::@REG_STARTEND 4)
(%defconstant csym::@REG_TRACE 256)
(%defconstant csym::@REG_LARGE 512)
(%defconstant csym::@REG_BACKR 1024)
(%defconstant csym::_@SYS_RESOURCE_H_ (c-exp ""))
(%defconstant csym::@PRIO_MIN (- 20))
(%defconstant csym::@PRIO_MAX 20)
(%defconstant csym::@PRIO_PROCESS 0)
(%defconstant csym::@PRIO_PGRP 1)
(%defconstant csym::@PRIO_USER 2)
(%defconstant csym::@RUSAGE_SELF 0)
(%defconstant csym::@RUSAGE_CHILDREN (- 1))
(%defconstant csym::ru_first csym::ru_ixrss)
(%defconstant csym::ru_last csym::ru_nivcsw)
(%defconstant csym::@RLIMIT_CPU 0)
(%defconstant csym::@RLIMIT_FSIZE 1)
(%defconstant csym::@RLIMIT_DATA 2)
(%defconstant csym::@RLIMIT_STACK 3)
(%defconstant csym::@RLIMIT_CORE 4)
(%defconstant csym::@RLIMIT_RSS 5)
(%defconstant csym::@RLIMIT_MEMLOCK 6)
(%defconstant csym::@RLIMIT_NPROC 7)
(%defconstant csym::@RLIMIT_NOFILE 8)
(%defconstant csym::@RLIMIT_SBSIZE 9)
(%defconstant csym::@RLIMIT_VMEM 10)
(%defconstant csym::@RLIMIT_AS 10)
(%defconstant csym::@RLIM_NLIMITS 11)
(%defconstant csym::@RLIM_INFINITY
 (cast csym::rlim_t (- (<< (cast csym::u_quad_t 1) 63) 1)))
(%defconstant csym::@CP_USER 0)
(%defconstant csym::@CP_NICE 1)
(%defconstant csym::@CP_SYS 2)
(%defconstant csym::@CP_INTR 3)
(%defconstant csym::@CP_IDLE 4)
(%defconstant csym::@CPUSTATES 5)
(%defconstant csym::_@SEARCH_H_ (c-exp ""))
(%defconstant csym::_@SYS_SEM_H_ (c-exp ""))
(%defconstant csym::@SEM_UNDO 4096)
(%defconstant csym::@GETNCNT 3)
(%defconstant csym::@GETPID 4)
(%defconstant csym::@GETVAL 5)
(%defconstant csym::@GETALL 6)
(%defconstant csym::@GETZCNT 7)
(%defconstant csym::@SETVAL 8)
(%defconstant csym::@SETALL 9)
(%defconstant csym::@SEM_STAT 10)
(%defconstant csym::@SEM_INFO 11)
(%defconstant csym::@SEM_A 128)
(%defconstant csym::@SEM_R 256)
(%defconstant csym::_@SEMAPHORE_H_ (c-exp ""))
(%defconstant csym::@SEM_FAILED (cast (ptr csym::sem_t) 0))
(%defconstant csym::@SEM_VALUE_MAX (bit-not (cast unsigned-int 0)))
(%defconstant csym::_@SETJMP_H_ (c-exp ""))
(%defconstant csym::_@MACHINE_SETJMP_H_ (c-exp ""))
(%defconstant csym::_@JBLEN 11)
(%defconstant csym::_@SYS_SHM_H_ (c-exp ""))
(%defconstant csym::@SHM_RDONLY 4096)
(%defconstant csym::@SHM_RND 8192)
(%defconstant csym::@SHMLBA csym::@PAGE_SIZE)
(%defconstant csym::@SHM_R 256)
(%defconstant csym::@SHM_W 128)
(%defconstant csym::@SHM_LOCK 11)
(%defconstant csym::@SHM_UNLOCK 12)
(%defconstant csym::@SHM_STAT 13)
(%defconstant csym::@SHM_INFO 14)
(%defconstant csym::_@SIGNAL_H_ (c-exp ""))
(%defconstant csym::_@SYS_SOCKET_H_ (c-exp ""))
(%defconstant csym::_@SYS__IOVEC_H_ (c-exp ""))
(%defconstant csym::_@NO_NAMESPACE_POLLUTION (c-exp ""))
(%defconstant csym::_@ALIGNBYTES (- (sizeof int) 1))
(%defmacro csym::_@ALIGN (common-lisp-user::p)
 (common-lisp:list 'bit-and
                   (common-lisp:list '+
                                     (common-lisp:list 'cast
                                                       'unsigned-int
                                                       'csym::p)
                                     (common-lisp:list '-
                                                       (common-lisp:list
                                                        'sizeof
                                                        'int)
                                                       '1))
                   (common-lisp:list 'bit-not
                                     (common-lisp:list '-
                                                       (common-lisp:list
                                                        'sizeof
                                                        'int)
                                                       '1))))
(%defconstant csym::_@MACHINE 1)
(%defconstant csym::_@MACHINE_ARCH 1)
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
(%defconstant csym::@AF_LOCAL 1)
(%defconstant csym::@AF_UNIX 1)
(%defconstant csym::@AF_INET 2)
(%defconstant csym::@AF_IMPLINK 3)
(%defconstant csym::@AF_PUP 4)
(%defconstant csym::@AF_CHAOS 5)
(%defconstant csym::@AF_NETBIOS 6)
(%defconstant csym::@AF_ISO 7)
(%defconstant csym::@AF_OSI 7)
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
(%defconstant csym::@AF_E164 26)
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
 (- (- (sizeof csym::__int64_t) (sizeof unsigned-char))
  (sizeof csym::sa_family_t)))
(%defconstant csym::_@SS_PAD2SIZE
 (-
  (-
   (- (- (cast unsigned-int 128) (sizeof unsigned-char))
    (sizeof csym::sa_family_t))
   (- (- (sizeof csym::__int64_t) (sizeof unsigned-char))
    (sizeof csym::sa_family_t)))
  (sizeof csym::__int64_t)))
(%defconstant csym::@PF_UNSPEC 0)
(%defconstant csym::@PF_LOCAL 1)
(%defconstant csym::@PF_UNIX 1)
(%defconstant csym::@PF_INET 2)
(%defconstant csym::@PF_IMPLINK 3)
(%defconstant csym::@PF_PUP 4)
(%defconstant csym::@PF_CHAOS 5)
(%defconstant csym::@PF_NETBIOS 6)
(%defconstant csym::@PF_ISO 7)
(%defconstant csym::@PF_OSI 7)
(%defconstant csym::@PF_ECMA 8)
(%defconstant csym::@PF_DATAKIT 9)
(%defconstant csym::@PF_CCITT 10)
(%defconstant csym::@PF_SNA 11)
(%defconstant csym::@PF_DEC@net 12)
(%defconstant csym::@PF_DLI 13)
(%defconstant csym::@PF_LAT 14)
(%defconstant csym::@PF_HYLINK 15)
(%defconstant csym::@PF_APPLETALK 16)
(%defconstant csym::@PF_ROUTE 17)
(%defconstant csym::@PF_LINK 18)
(%defconstant csym::@PF_XTP 19)
(%defconstant csym::@PF_COIP 20)
(%defconstant csym::@PF_CNT 21)
(%defconstant csym::@PF_SIP 24)
(%defconstant csym::@PF_IPX 23)
(%defconstant csym::@PF_RTIP 22)
(%defconstant csym::@PF_PIP 25)
(%defconstant csym::@PF_ISDN 26)
(%defconstant csym::@PF_KEY 27)
(%defconstant csym::@PF_INET6 28)
(%defconstant csym::@PF_NATM 29)
(%defconstant csym::@PF_ATM 30)
(%defconstant csym::@PF_NETGRAPH 32)
(%defconstant csym::@PF_SLOW 33)
(%defconstant csym::@PF_SCLUSTER 34)
(%defconstant csym::@PF_ARP 35)
(%defconstant csym::@PF_BLUETOOTH 36)
(%defconstant csym::@PF_MAX 37)
(%defconstant csym::@NET_MAXID 37)
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
(%defmacro csym::@CMSG_DATA (common-lisp-user::cmsg)
 (common-lisp:list '+
                   (common-lisp:list 'cast
                                     (common-lisp:list 'ptr 'unsigned-char)
                                     'csym::cmsg)
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '+
                                                       (common-lisp:list 'cast
                                                                         'unsigned-int
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          (common-lisp:list
                                                                           'struct
                                                                           'csym::cmsghdr)))
                                                       (common-lisp:list '-
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'int)
                                                                         '1))
                                     (common-lisp:list 'bit-not
                                                       (common-lisp:list '-
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'int)
                                                                         '1)))))
(%defmacro csym::@CMSG_NXTHDR (common-lisp-user::mhdr common-lisp-user::cmsg)
 (common-lisp:list 'if-exp
                   (common-lisp:list '>
                                     (common-lisp:list '+
                                                       (common-lisp:list '+
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           'char)
                                                                          'csym::cmsg)
                                                                         (common-lisp:list
                                                                          'bit-and
                                                                          (common-lisp:list
                                                                           '+
                                                                           (common-lisp:list
                                                                            'cast
                                                                            'unsigned-int
                                                                            (common-lisp:list
                                                                             'fref
                                                                             (common-lisp:list
                                                                              'mref
                                                                              'csym::cmsg)
                                                                             'csym::cmsg_len))
                                                                           (common-lisp:list
                                                                            '-
                                                                            (common-lisp:list
                                                                             'sizeof
                                                                             'int)
                                                                            '1))
                                                                          (common-lisp:list
                                                                           'bit-not
                                                                           (common-lisp:list
                                                                            '-
                                                                            (common-lisp:list
                                                                             'sizeof
                                                                             'int)
                                                                            '1))))
                                                       (common-lisp:list
                                                        'bit-and
                                                        (common-lisp:list '+
                                                                          (common-lisp:list
                                                                           'cast
                                                                           'unsigned-int
                                                                           (common-lisp:list
                                                                            'sizeof
                                                                            (common-lisp:list
                                                                             'struct
                                                                             'csym::cmsghdr)))
                                                                          (common-lisp:list
                                                                           '-
                                                                           (common-lisp:list
                                                                            'sizeof
                                                                            'int)
                                                                           '1))
                                                        (common-lisp:list
                                                         'bit-not
                                                         (common-lisp:list '-
                                                                           (common-lisp:list
                                                                            'sizeof
                                                                            'int)
                                                                           '1))))
                                     (common-lisp:list '+
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          'char)
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::mhdr)
                                                                          'csym::msg_control))
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::mhdr)
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
                                                                         'csym::cmsg)
                                                       (common-lisp:list
                                                        'bit-and
                                                        (common-lisp:list '+
                                                                          (common-lisp:list
                                                                           'cast
                                                                           'unsigned-int
                                                                           (common-lisp:list
                                                                            'fref
                                                                            (common-lisp:list
                                                                             'mref
                                                                             'csym::cmsg)
                                                                            'csym::cmsg_len))
                                                                          (common-lisp:list
                                                                           '-
                                                                           (common-lisp:list
                                                                            'sizeof
                                                                            'int)
                                                                           '1))
                                                        (common-lisp:list
                                                         'bit-not
                                                         (common-lisp:list '-
                                                                           (common-lisp:list
                                                                            'sizeof
                                                                            'int)
                                                                           '1)))))))
(%defmacro csym::@CMSG_FIRSTHDR (common-lisp-user::mhdr)
 (common-lisp:list 'if-exp
                   (common-lisp:list '>=
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::mhdr)
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
                                                                         'csym::mhdr)
                                                       'csym::msg_control))
                   (common-lisp:list 'cast
                                     (common-lisp:list 'ptr
                                                       (common-lisp:list
                                                        'struct
                                                        'csym::cmsghdr))
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'void)
                                                       '0))))
(%defmacro csym::@CMSG_SPACE (common-lisp-user::l)
 (common-lisp:list '+
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '+
                                                       (common-lisp:list 'cast
                                                                         'unsigned-int
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          (common-lisp:list
                                                                           'struct
                                                                           'csym::cmsghdr)))
                                                       (common-lisp:list '-
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'int)
                                                                         '1))
                                     (common-lisp:list 'bit-not
                                                       (common-lisp:list '-
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'int)
                                                                         '1)))
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '+
                                                       (common-lisp:list 'cast
                                                                         'unsigned-int
                                                                         'csym::l)
                                                       (common-lisp:list '-
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'int)
                                                                         '1))
                                     (common-lisp:list 'bit-not
                                                       (common-lisp:list '-
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'int)
                                                                         '1)))))
(%defmacro csym::@CMSG_LEN (common-lisp-user::l)
 (common-lisp:list '+
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '+
                                                       (common-lisp:list 'cast
                                                                         'unsigned-int
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          (common-lisp:list
                                                                           'struct
                                                                           'csym::cmsghdr)))
                                                       (common-lisp:list '-
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'int)
                                                                         '1))
                                     (common-lisp:list 'bit-not
                                                       (common-lisp:list '-
                                                                         (common-lisp:list
                                                                          'sizeof
                                                                          'int)
                                                                         '1)))
                   'csym::l))
(%defconstant csym::@SCM_RIGHTS 1)
(%defconstant csym::@SCM_TIMESTAMP 2)
(%defconstant csym::@SCM_CREDS 3)
(%defconstant csym::@SCM_BINTIME 4)
(%defconstant csym::@SHUT_RD 0)
(%defconstant csym::@SHUT_WR 1)
(%defconstant csym::@SHUT_RDWR 2)
(%defconstant csym::@SF_NODISKIO 1)
(%defconstant csym::_@SYS_STATVFS_H_ (c-exp ""))
(%defconstant csym::@ST_RDONLY 1)
(%defconstant csym::@ST_NOSUID 2)
(%defconstant csym::_@MACHINE_STDARG_H_ (c-exp ""))
(%defconstant csym::_@VA_LIST_DECLARED (c-exp ""))
(%defmacro csym::va_start (common-lisp-user::ap common-lisp:last)
 (common-lisp:list 'csym::__builtin_stdarg_start 'csym::ap 'csym::last))
(%defmacro csym::va_arg (common-lisp-user::ap common-lisp:type)
 (common-lisp:list 'csym::__builtin_va_arg 'csym::ap 'csym::type))
(%defmacro csym::va_copy (common-lisp-user::dest common-lisp-user::src)
 (common-lisp:list 'csym::__builtin_va_copy 'csym::dest 'csym::src))
(%defmacro csym::va_end (common-lisp-user::ap)
 (common-lisp:list 'csym::__builtin_va_end 'csym::ap))
(%defconstant csym::_@STDBOOL_H_ (c-exp ""))
(%defconstant csym::__bool_true_false_are_defined 1)
(%defconstant csym::false 0)
(%defconstant csym::true 1)
(%defconstant csym::bool (c-exp "_Bool"))
(%defconstant csym::_@STDDEF_H_ (c-exp ""))
(%defconstant csym::_@RUNE_T_DECLARED (c-exp ""))
(%defconstant csym::_@WCHAR_T_DECLARED (c-exp ""))
(%defmacro csym::offsetof (common-lisp:type common-lisp:member)
 (common-lisp:list 'cast
                   'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           'csym::type)
                                                                          '0))
                                                       'csym::member))))
(%defconstant csym::_@STDIO_H_ (c-exp ""))
(%defconstant csym::_@FSTDIO (c-exp ""))
(%defconstant csym::_@STDSTREAM_DECLARED (c-exp ""))
(%defconstant csym::__@SLBF 1)
(%defconstant csym::__@SNBF 2)
(%defconstant csym::__@SRD 4)
(%defconstant csym::__@SWR 8)
(%defconstant csym::__@SRW 16)
(%defconstant csym::__@SEOF 32)
(%defconstant csym::__@SERR 64)
(%defconstant csym::__@SMBF 128)
(%defconstant csym::__@SAPP 256)
(%defconstant csym::__@SSTR 512)
(%defconstant csym::__@SOPT 1024)
(%defconstant csym::__@SNPT 2048)
(%defconstant csym::__@SOFF 4096)
(%defconstant csym::__@SMOD 8192)
(%defconstant csym::__@SALC 16384)
(%defconstant csym::__@SIGN 32768)
(%defconstant csym::_@IOFBF 0)
(%defconstant csym::_@IOLBF 1)
(%defconstant csym::_@IONBF 2)
(%defconstant csym::@BUFSIZ 1024)
(%defconstant csym::@EOF (- 1))
(%defconstant csym::@FOPEN_MAX 20)
(%defconstant csym::@FILENAME_MAX 1024)
(%defconstant csym::@P_@tmpdir "/var/tmp/")
(%defconstant csym::@L_@tmpnam 1024)
(%defconstant csym::@TMP_MAX 308915776)
(%defconstant csym::@SEEK_SET 0)
(%defconstant csym::@SEEK_CUR 1)
(%defconstant csym::@SEEK_END 2)
(%defconstant csym::stdin csym::__stdinp)
(%defconstant csym::stdout csym::__stdoutp)
(%defconstant csym::stderr csym::__stderrp)
(%defconstant csym::@L_@cuserid 17)
(%defconstant csym::@L_@ctermid 1024)
(%defconstant csym::__@ATTR_FORMAT_ARG (c-exp ""))
(%defmacro csym::fropen (common-lisp-user::cookie common-lisp-user::fn)
 (common-lisp:list 'c-exp
                   "funopen ( ~0@*~a , ~1@*~a , 0 , 0 , 0 )"
                   common-lisp-user::cookie
                   common-lisp-user::fn))
(%defmacro csym::fwopen (common-lisp-user::cookie common-lisp-user::fn)
 (common-lisp:list 'c-exp
                   "funopen ( ~0@*~a , 0 , ~1@*~a , 0 , 0 )"
                   common-lisp-user::cookie
                   common-lisp-user::fn))
(%defmacro csym::__sgetc (common-lisp-user::p)
 (common-lisp:list 'if-exp
                   (common-lisp:list '<
                                     (common-lisp:list '--
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::p)
                                                                         'csym::_r))
                                     '0)
                   (common-lisp:list 'csym::__srget 'csym::p)
                   (common-lisp:list 'cast
                                     'int
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'inc
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::p)
                                                                          'csym::_p))))))
(%defmacro csym::__sfeof (common-lisp-user::p)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::_flags)
                                     '32)
                   '0))
(%defmacro csym::__sferror (common-lisp-user::p)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::_flags)
                                     '64)
                   '0))
(%defmacro csym::__sclearerr (common-lisp-user::p)
 (common-lisp:list 'cast
                   'void
                   (common-lisp:list 'bit-and=
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::_flags)
                                     (common-lisp:list 'bit-not
                                                       (common-lisp:list
                                                        'bit-or
                                                        '64
                                                        '32)))))
(%defmacro csym::__sfileno (common-lisp-user::p)
 (common-lisp:list 'fref (common-lisp:list 'mref 'csym::p) 'csym::_file))
(%defmacro csym::feof (common-lisp-user::p)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list '!=
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::p)
                                                                         'csym::_flags)
                                                       '32)
                                     '0)
                   (common-lisp:list 'csym::feof 'csym::p)))
(%defmacro csym::ferror (common-lisp-user::p)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list '!=
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::p)
                                                                         'csym::_flags)
                                                       '64)
                                     '0)
                   (common-lisp:list 'csym::ferror 'csym::p)))
(%defmacro csym::clearerr (common-lisp-user::p)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'cast
                                     'void
                                     (common-lisp:list 'bit-and=
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::p)
                                                                         'csym::_flags)
                                                       (common-lisp:list
                                                        'bit-not
                                                        (common-lisp:list
                                                         'bit-or
                                                         '64
                                                         '32))))
                   (common-lisp:list 'csym::clearerr 'csym::p)))
(%defmacro csym::fileno (common-lisp-user::p)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref 'csym::p)
                                     'csym::_file)
                   (common-lisp:list 'csym::fileno 'csym::p)))
(%defmacro csym::getc (common-lisp-user::fp)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '<
                                                       (common-lisp:list '--
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::fp)
                                                                          'csym::_r))
                                                       '0)
                                     (common-lisp:list 'csym::__srget
                                                       'csym::fp)
                                     (common-lisp:list 'cast
                                                       'int
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'inc
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::fp)
                                                                           'csym::_p)))))
                   (common-lisp:list 'csym::getc 'csym::fp)))
(%defmacro csym::putc (common-lisp-user::x common-lisp-user::fp)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sputc 'csym::x 'csym::fp)
                   (common-lisp:list 'csym::putc 'csym::x 'csym::fp)))
(%defmacro csym::getchar common-lisp:nil
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'if-exp
                                     (common-lisp:list '<
                                                       (common-lisp:list '--
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::__stdinp)
                                                                          'csym::_r))
                                                       '0)
                                     (common-lisp:list 'csym::__srget
                                                       'csym::__stdinp)
                                     (common-lisp:list 'cast
                                                       'int
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'inc
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            'csym::__stdinp)
                                                                           'csym::_p)))))
                   (common-lisp:list 'csym::getc 'csym::__stdinp)))
(%defmacro csym::putchar (common-lisp-user::x)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sputc 'csym::x 'csym::__stdoutp)
                   (common-lisp:list 'csym::putc 'csym::x 'csym::__stdoutp)))
(%defmacro csym::feof_unlocked (common-lisp-user::p)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::_flags)
                                     '32)
                   '0))
(%defmacro csym::ferror_unlocked (common-lisp-user::p)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::_flags)
                                     '64)
                   '0))
(%defmacro csym::clearerr_unlocked (common-lisp-user::p)
 (common-lisp:list 'cast
                   'void
                   (common-lisp:list 'bit-and=
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::p)
                                                       'csym::_flags)
                                     (common-lisp:list 'bit-not
                                                       (common-lisp:list
                                                        'bit-or
                                                        '64
                                                        '32)))))
(%defmacro csym::fileno_unlocked (common-lisp-user::p)
 (common-lisp:list 'fref (common-lisp:list 'mref 'csym::p) 'csym::_file))
(%defmacro csym::getc_unlocked (common-lisp-user::fp)
 (common-lisp:list 'if-exp
                   (common-lisp:list '<
                                     (common-lisp:list '--
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::fp)
                                                                         'csym::_r))
                                     '0)
                   (common-lisp:list 'csym::__srget 'csym::fp)
                   (common-lisp:list 'cast
                                     'int
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'inc
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::fp)
                                                                          'csym::_p))))))
(%defmacro csym::putc_unlocked (common-lisp-user::x common-lisp-user::fp)
 (common-lisp:list 'csym::__sputc 'csym::x 'csym::fp))
(%defmacro csym::getchar_unlocked common-lisp:nil
 (common-lisp:list 'if-exp
                   (common-lisp:list '<
                                     (common-lisp:list '--
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::__stdinp)
                                                                         'csym::_r))
                                     '0)
                   (common-lisp:list 'csym::__srget 'csym::__stdinp)
                   (common-lisp:list 'cast
                                     'int
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'inc
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           'csym::__stdinp)
                                                                          'csym::_p))))))
(%defmacro csym::putchar_unlocked (common-lisp-user::x)
 (common-lisp:list 'csym::__sputc 'csym::x 'csym::__stdoutp))
(%defconstant csym::_@STDLIB_H_ (c-exp ""))
(%defconstant csym::@EXIT_FAILURE 1)
(%defconstant csym::@EXIT_SUCCESS 0)
(%defconstant csym::@RAND_MAX 2147483647)
(%defconstant csym::@MB_CUR_MAX csym::__mb_cur_max)
(%defconstant csym::_@MKSTEMP_DECLARED (c-exp ""))
(%defconstant csym::_@MKTEMP_DECLARED (c-exp ""))
(%defconstant csym::_@SETKEY_DECLARED (c-exp ""))
(%defmacro csym::alloca (common-lisp-user::sz)
 (common-lisp:list 'csym::__builtin_alloca 'csym::sz))
(%defconstant csym::_@STRING_H_ (c-exp ""))
(%defconstant csym::_@STRINGS_H_ (c-exp ""))
(%defconstant csym::_@SYS_SYSLOG_H_ (c-exp ""))
(%defconstant csym::_@PATH_LOG "/var/run/log")
(%defconstant csym::_@PATH_LOG_PRIV "/var/run/logpriv")
(%defconstant csym::_@PATH_OLDLOG "/dev/log")
(%defconstant csym::@LOG_EMERG 0)
(%defconstant csym::@LOG_ALERT 1)
(%defconstant csym::@LOG_CRIT 2)
(%defconstant csym::@LOG_ERR 3)
(%defconstant csym::@LOG_WARNING 4)
(%defconstant csym::@LOG_NOTICE 5)
(%defconstant csym::@LOG_INFO 6)
(%defconstant csym::@LOG_DEBUG 7)
(%defconstant csym::@LOG_PRIMASK 7)
(%defmacro csym::@LOG_PRI (common-lisp-user::p)
 (common-lisp:list 'bit-and 'csym::p '7))
(%defmacro csym::@LOG_MAKEPRI (common-lisp-user::fac common-lisp-user::pri)
 (common-lisp:list 'bit-or 'csym::fac 'csym::pri))
(%defconstant csym::@LOG_KERN (<< 0 3))
(%defconstant csym::@LOG_USER (<< 1 3))
(%defconstant csym::@LOG_MAIL (<< 2 3))
(%defconstant csym::@LOG_DAEMON (<< 3 3))
(%defconstant csym::@LOG_AUTH (<< 4 3))
(%defconstant csym::@LOG_SYSLOG (<< 5 3))
(%defconstant csym::@LOG_LPR (<< 6 3))
(%defconstant csym::@LOG_NEWS (<< 7 3))
(%defconstant csym::@LOG_UUCP (<< 8 3))
(%defconstant csym::@LOG_CRON (<< 9 3))
(%defconstant csym::@LOG_AUTHPRIV (<< 10 3))
(%defconstant csym::@LOG_FTP (<< 11 3))
(%defconstant csym::@LOG_NTP (<< 12 3))
(%defconstant csym::@LOG_SECURITY (<< 13 3))
(%defconstant csym::@LOG_CONSOLE (<< 14 3))
(%defconstant csym::@LOG_LOCAL0 (<< 16 3))
(%defconstant csym::@LOG_LOCAL1 (<< 17 3))
(%defconstant csym::@LOG_LOCAL2 (<< 18 3))
(%defconstant csym::@LOG_LOCAL3 (<< 19 3))
(%defconstant csym::@LOG_LOCAL4 (<< 20 3))
(%defconstant csym::@LOG_LOCAL5 (<< 21 3))
(%defconstant csym::@LOG_LOCAL6 (<< 22 3))
(%defconstant csym::@LOG_LOCAL7 (<< 23 3))
(%defconstant csym::@LOG_NFACILITIES 24)
(%defconstant csym::@LOG_FACMASK 1016)
(%defmacro csym::@LOG_FAC (common-lisp-user::p)
 (common-lisp:list '>> (common-lisp:list 'bit-and 'csym::p '1016) '3))
(%defmacro csym::@LOG_MASK (common-lisp-user::pri)
 (common-lisp:list '<< '1 'csym::pri))
(%defmacro csym::@LOG_UPTO (common-lisp-user::pri)
 (common-lisp:list '-
                   (common-lisp:list '<<
                                     '1
                                     (common-lisp:list '+ 'csym::pri '1))
                   '1))
(%defconstant csym::@LOG_PID 1)
(%defconstant csym::@LOG_CONS 2)
(%defconstant csym::@LOG_ODELAY 4)
(%defconstant csym::@LOG_NDELAY 8)
(%defconstant csym::@LOG_NOWAIT 16)
(%defconstant csym::@LOG_PERROR 32)
(%defconstant csym::_@TAR_H (c-exp ""))
(%defconstant csym::@TMAGIC "ustar")
(%defconstant csym::@TMAGLEN 6)
(%defconstant csym::@TVERSION "00")
(%defconstant csym::@TVERSLEN 2)
(%defconstant csym::@REGTYPE #\0)
(%defconstant csym::@AREGTYPE #\Null)
(%defconstant csym::@LNKTYPE #\1)
(%defconstant csym::@SYMTYPE #\2)
(%defconstant csym::@CHRTYPE #\3)
(%defconstant csym::@BLKTYPE #\4)
(%defconstant csym::@DIRTYPE #\5)
(%defconstant csym::@FIFOTYPE #\6)
(%defconstant csym::@CONTTYPE #\7)
(%defconstant csym::@TSUID 2048)
(%defconstant csym::@TSGID 1024)
(%defconstant csym::@TSVTX 512)
(%defconstant csym::@TUREAD 256)
(%defconstant csym::@TUWRITE 128)
(%defconstant csym::@TUEXEC 64)
(%defconstant csym::@TGREAD 32)
(%defconstant csym::@TGWRITE 16)
(%defconstant csym::@TGEXEC 8)
(%defconstant csym::@TOREAD 4)
(%defconstant csym::@TOWRITE 2)
(%defconstant csym::@TOEXEC 1)
(%defconstant csym::_@NETINET_TCP_H_ (c-exp ""))
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
(%defconstant csym::@TH_FLAGS (bit-or 1 2 4 16 32 64 128))
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
(%defconstant csym::@TCPOLEN_TSTAMP_APPA (+ 10 2))
(%defconstant csym::@TCPOPT_TSTAMP_HDR (bit-or (<< 1 24) (<< 1 16) (<< 8 8) 10))
(%defconstant csym::@MAX_TCPOPTLEN 40)
(%defconstant csym::@TCPOPT_CC 11)
(%defconstant csym::@TCPOPT_CCNEW 12)
(%defconstant csym::@TCPOPT_CCECHO 13)
(%defconstant csym::@TCPOLEN_CC 6)
(%defconstant csym::@TCPOLEN_CC_APPA (+ 6 2))
(%defmacro csym::@TCPOPT_CC_HDR (common-lisp-user::ccopt)
 (common-lisp:list 'bit-or
                   (common-lisp:list '<< '1 '24)
                   (common-lisp:list '<< '1 '16)
                   (common-lisp:list '<< 'csym::ccopt '8)
                   '6))
(%defconstant csym::@TCPOPT_SIGNATURE 19)
(%defconstant csym::@TCPOLEN_SIGNATURE 18)
(%defconstant csym::@TCPOPT_SACK_PERMIT_HDR
 (bit-or (<< 1 24) (<< 1 16) (<< 4 8) 2))
(%defconstant csym::@TCPOPT_SACK_HDR (bit-or (<< 1 24) (<< 1 16) (<< 5 8)))
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
(%defconstant csym::@TCP_MAXOLEN (- (<< 15 2) (sizeof (struct csym::tcphdr))))
(%defconstant csym::@TCP_NODELAY 1)
(%defconstant csym::@TCP_MAXSEG 2)
(%defconstant csym::@TCP_NOPUSH 4)
(%defconstant csym::@TCP_NOOPT 8)
(%defconstant csym::@TCP_MD5SIG 16)
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
(%defmacro csym::@CCEQ (common-lisp-user::val common-lisp-user::c)
 (common-lisp:list 'if-exp
                   (common-lisp:list '== 'csym::c 'csym::val)
                   (common-lisp:list '!= 'csym::val '255)
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
(%defconstant csym::@CRTSCTS (bit-or 65536 131072))
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
(%defconstant csym::_@SYS_TTYCOM_H_ (c-exp ""))
(%defconstant csym::_@SYS_IOCCOM_H_ (c-exp ""))
(%defconstant csym::@IOCPARM_MASK 8191)
(%defmacro csym::@IOCPARM_LEN (common-lisp-user::x)
 (common-lisp:list 'bit-and (common-lisp:list '>> 'csym::x '16) '8191))
(%defmacro csym::@IOCBASECMD (common-lisp-user::x)
 (common-lisp:list 'bit-and
                   'csym::x
                   (common-lisp:list 'bit-not
                                     (common-lisp:list '<< '8191 '16))))
(%defmacro csym::@IOCGROUP (common-lisp-user::x)
 (common-lisp:list 'bit-and (common-lisp:list '>> 'csym::x '8) '255))
(%defconstant csym::@IOCPARM_MAX csym::@PAGE_SIZE)
(%defconstant csym::@IOC_VOID 536870912)
(%defconstant csym::@IOC_OUT 1073741824)
(%defconstant csym::@IOC_IN 2147483648)
(%defconstant csym::@IOC_INOUT (bit-or 2147483648 1073741824))
(%defconstant csym::@IOC_DIRMASK 3758096384)
(%defmacro csym::_@IOC
 (common-lisp-user::inout common-lisp-user::group common-lisp-user::num
  common-lisp-user::len)
 (common-lisp:list 'cast
                   'unsigned-long
                   (common-lisp:list 'bit-or
                                     'csym::inout
                                     (common-lisp:list '<<
                                                       (common-lisp:list
                                                        'bit-and
                                                        'csym::len
                                                        '8191)
                                                       '16)
                                     (common-lisp:list '<< 'csym::group '8)
                                     'csym::num)))
(%defmacro csym::_@IO (common-lisp-user::g common-lisp-user::n)
 (common-lisp:list 'cast
                   'unsigned-long
                   (common-lisp:list 'bit-or
                                     '536870912
                                     (common-lisp:list '<<
                                                       (common-lisp:list
                                                        'bit-and
                                                        '0
                                                        '8191)
                                                       '16)
                                     (common-lisp:list '<< 'csym::g '8)
                                     'csym::n)))
(%defmacro csym::_@IOR (common-lisp-user::g common-lisp-user::n common-lisp:t)
 (common-lisp:list 'cast
                   'unsigned-long
                   (common-lisp:list 'bit-or
                                     '1073741824
                                     (common-lisp:list '<<
                                                       (common-lisp:list
                                                        'bit-and
                                                        (common-lisp:list
                                                         'sizeof
                                                         'csym::t)
                                                        '8191)
                                                       '16)
                                     (common-lisp:list '<< 'csym::g '8)
                                     'csym::n)))
(%defmacro csym::_@IOW (common-lisp-user::g common-lisp-user::n common-lisp:t)
 (common-lisp:list 'cast
                   'unsigned-long
                   (common-lisp:list 'bit-or
                                     '2147483648
                                     (common-lisp:list '<<
                                                       (common-lisp:list
                                                        'bit-and
                                                        (common-lisp:list
                                                         'sizeof
                                                         'csym::t)
                                                        '8191)
                                                       '16)
                                     (common-lisp:list '<< 'csym::g '8)
                                     'csym::n)))
(%defmacro csym::_@IOWR (common-lisp-user::g common-lisp-user::n common-lisp:t)
 (common-lisp:list 'cast
                   'unsigned-long
                   (common-lisp:list 'bit-or
                                     (common-lisp:list 'bit-or
                                                       '2147483648
                                                       '1073741824)
                                     (common-lisp:list '<<
                                                       (common-lisp:list
                                                        'bit-and
                                                        (common-lisp:list
                                                         'sizeof
                                                         'csym::t)
                                                        '8191)
                                                       '16)
                                     (common-lisp:list '<< 'csym::g '8)
                                     'csym::n)))
(%defconstant csym::@TIOCEXCL
 (cast unsigned-long (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 13)))
(%defconstant csym::@TIOCNXCL
 (cast unsigned-long (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 14)))
(%defconstant csym::@TIOCFLUSH
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 16)))
(%defconstant csym::@TIOCGETA
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof (struct csym::termios)) 8191) 16)
   (<< #\t 8) 19)))
(%defconstant csym::@TIOCSETA
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof (struct csym::termios)) 8191) 16)
   (<< #\t 8) 20)))
(%defconstant csym::@TIOCSETAW
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof (struct csym::termios)) 8191) 16)
   (<< #\t 8) 21)))
(%defconstant csym::@TIOCSETAF
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof (struct csym::termios)) 8191) 16)
   (<< #\t 8) 22)))
(%defconstant csym::@TIOCGETD
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 26)))
(%defconstant csym::@TIOCSETD
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 27)))
(%defconstant csym::@TIOCSBRK
 (cast unsigned-long
  (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 123)))
(%defconstant csym::@TIOCCBRK
 (cast unsigned-long
  (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 122)))
(%defconstant csym::@TIOCSDTR
 (cast unsigned-long
  (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 121)))
(%defconstant csym::@TIOCCDTR
 (cast unsigned-long
  (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 120)))
(%defconstant csym::@TIOCGPGRP
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 119)))
(%defconstant csym::@TIOCSPGRP
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 118)))
(%defconstant csym::@TIOCOUTQ
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 115)))
(%defconstant csym::@TIOCSTI
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof char) 8191) 16) (<< #\t 8) 114)))
(%defconstant csym::@TIOCNOTTY
 (cast unsigned-long
  (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 113)))
(%defconstant csym::@TIOCPKT
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 112)))
(%defconstant csym::@TIOCPKT_DATA 0)
(%defconstant csym::@TIOCPKT_FLUSHREAD 1)
(%defconstant csym::@TIOCPKT_FLUSHWRITE 2)
(%defconstant csym::@TIOCPKT_STOP 4)
(%defconstant csym::@TIOCPKT_START 8)
(%defconstant csym::@TIOCPKT_NOSTOP 16)
(%defconstant csym::@TIOCPKT_DOSTOP 32)
(%defconstant csym::@TIOCPKT_IOCTL 64)
(%defconstant csym::@TIOCSTOP
 (cast unsigned-long
  (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 111)))
(%defconstant csym::@TIOCSTART
 (cast unsigned-long
  (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 110)))
(%defconstant csym::@TIOCMSET
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 109)))
(%defconstant csym::@TIOCMBIS
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 108)))
(%defconstant csym::@TIOCMBIC
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 107)))
(%defconstant csym::@TIOCMGET
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 106)))
(%defconstant csym::@TIOCM_LE 1)
(%defconstant csym::@TIOCM_DTR 2)
(%defconstant csym::@TIOCM_RTS 4)
(%defconstant csym::@TIOCM_ST 8)
(%defconstant csym::@TIOCM_SR 16)
(%defconstant csym::@TIOCM_CTS 32)
(%defconstant csym::@TIOCM_DCD 64)
(%defconstant csym::@TIOCM_RI 128)
(%defconstant csym::@TIOCM_DSR 256)
(%defconstant csym::@TIOCM_CD 64)
(%defconstant csym::@TIOCM_CAR 64)
(%defconstant csym::@TIOCM_RNG 128)
(%defconstant csym::@TIOCGWINSZ
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof (struct csym::winsize)) 8191) 16)
   (<< #\t 8) 104)))
(%defconstant csym::@TIOCSWINSZ
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof (struct csym::winsize)) 8191) 16)
   (<< #\t 8) 103)))
(%defconstant csym::@TIOCUCNTL
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 102)))
(%defconstant csym::@TIOCSTAT
 (cast unsigned-long
  (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 101)))
(%defmacro csym::@UIOCCMD (common-lisp-user::n)
 (common-lisp:list 'cast
                   'unsigned-long
                   (common-lisp:list 'bit-or
                                     '536870912
                                     (common-lisp:list '<<
                                                       (common-lisp:list
                                                        'bit-and
                                                        '0
                                                        '8191)
                                                       '16)
                                     (common-lisp:list '<< '#\u '8)
                                     'csym::n)))
(%defconstant csym::@TIOCCONS
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 98)))
(%defconstant csym::@TIOCSCTTY
 (cast unsigned-long (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 97)))
(%defconstant csym::@TIOCEXT
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 96)))
(%defconstant csym::@TIOCSIG
 (cast unsigned-long (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 95)))
(%defconstant csym::@TIOCDRAIN
 (cast unsigned-long (bit-or 536870912 (<< (bit-and 0 8191) 16) (<< #\t 8) 94)))
(%defconstant csym::@TIOCMSDTRWAIT
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 91)))
(%defconstant csym::@TIOCMGDTRWAIT
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 90)))
(%defconstant csym::@TIOCTIMESTAMP
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof (struct csym::timeval)) 8191) 16)
   (<< #\t 8) 89)))
(%defconstant csym::@TIOCSDRAINWAIT
 (cast unsigned-long
  (bit-or 2147483648 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 87)))
(%defconstant csym::@TIOCGDRAINWAIT
 (cast unsigned-long
  (bit-or 1073741824 (<< (bit-and (sizeof int) 8191) 16) (<< #\t 8) 86)))
(%defconstant csym::@TTYDISC 0)
(%defconstant csym::@SLIPDISC 4)
(%defconstant csym::@PPPDISC 5)
(%defconstant csym::@NETGRAPHDISC 6)
(%defconstant csym::@H4DISC 7)
(%defconstant csym::_@SYS_TTYDEFAULTS_H_ (c-exp ""))
(%defconstant csym::@TTYDEF_IFLAG (bit-or 2 256 8192 512 2048))
(%defconstant csym::@TTYDEF_OFLAG (bit-or 1 2))
(%defconstant csym::@TTYDEF_LFLAG (bit-or 8 256 128 1024 2 1 64))
(%defconstant csym::@TTYDEF_CFLAG (bit-or 2048 768 16384))
(%defconstant csym::@TTYDEF_SPEED 9600)
(%defmacro csym::@CTRL (common-lisp-user::x)
 (common-lisp:list 'bit-and 'csym::x '31))
(%defconstant csym::@CEOF (bit-and #\d 31))
(%defconstant csym::@CEOL 255)
(%defconstant csym::@CERASE 127)
(%defconstant csym::@CERASE2 (bit-and #\h 31))
(%defconstant csym::@CINTR (bit-and #\c 31))
(%defconstant csym::@CSTATUS (bit-and #\t 31))
(%defconstant csym::@CKILL (bit-and #\u 31))
(%defconstant csym::@CMIN 1)
(%defconstant csym::@CQUIT 28)
(%defconstant csym::@CSUSP (bit-and #\z 31))
(%defconstant csym::@CTIME 0)
(%defconstant csym::@CDSUSP (bit-and #\y 31))
(%defconstant csym::@CSTART (bit-and #\q 31))
(%defconstant csym::@CSTOP (bit-and #\s 31))
(%defconstant csym::@CLNEXT (bit-and #\v 31))
(%defconstant csym::@CDISCARD (bit-and #\o 31))
(%defconstant csym::@CWERASE (bit-and #\w 31))
(%defconstant csym::@CREPRINT (bit-and #\r 31))
(%defconstant csym::@CEOT (bit-and #\d 31))
(%defconstant csym::@CBRK 255)
(%defconstant csym::@CRPRNT (bit-and #\r 31))
(%defconstant csym::@CFLUSH (bit-and #\o 31))
(%defconstant csym::_@TGMATH_H_ (c-exp ""))
(%defmacro csym::__tg_type (common-lisp-user::e common-lisp:t)
 (common-lisp:list 'csym::__builtin_types_compatible_p
                   (common-lisp:list 'csym::__typeof__ 'csym::e)
                   'csym::t))
(%defmacro csym::__tg_type3
 (common-lisp-user::e1 common-lisp-user::e2 common-lisp-user::e3 common-lisp:t)
 (common-lisp:list 'or
                   (common-lisp:list 'csym::__builtin_types_compatible_p
                                     (common-lisp:list 'csym::__typeof__
                                                       'csym::e1)
                                     'csym::t)
                   (common-lisp:list 'csym::__builtin_types_compatible_p
                                     (common-lisp:list 'csym::__typeof__
                                                       'csym::e2)
                                     'csym::t)
                   (common-lisp:list 'csym::__builtin_types_compatible_p
                                     (common-lisp:list 'csym::__typeof__
                                                       'csym::e3)
                                     'csym::t)))
(%defmacro csym::__tg_type_corr
 (common-lisp-user::e1 common-lisp-user::e2 common-lisp-user::e3 common-lisp:t)
 (common-lisp:list 'or
                   (common-lisp:list 'or
                                     (common-lisp:list
                                      'csym::__builtin_types_compatible_p
                                      (common-lisp:list 'csym::__typeof__
                                                        'csym::e1)
                                      'csym::t)
                                     (common-lisp:list
                                      'csym::__builtin_types_compatible_p
                                      (common-lisp:list 'csym::__typeof__
                                                        'csym::e2)
                                      'csym::t)
                                     (common-lisp:list
                                      'csym::__builtin_types_compatible_p
                                      (common-lisp:list 'csym::__typeof__
                                                        'csym::e3)
                                      'csym::t))
                   (common-lisp:list 'or
                                     (common-lisp:list
                                      'csym::__builtin_types_compatible_p
                                      (common-lisp:list 'csym::__typeof__
                                                        'csym::e1)
                                      'csym::t)
                                     (common-lisp:list
                                      'csym::__builtin_types_compatible_p
                                      (common-lisp:list 'csym::__typeof__
                                                        'csym::e2)
                                      'csym::t)
                                     (common-lisp:list
                                      'csym::__builtin_types_compatible_p
                                      (common-lisp:list 'csym::__typeof__
                                                        'csym::e3)
                                      'csym::t))))
(%defmacro csym::__tg_integer
 (common-lisp-user::e1 common-lisp-user::e2 common-lisp-user::e3)
 (common-lisp:list 'c-exp
                   "( ( ( __typeof__ ( ~0@*~a ) ) 1.5 == 1 ) || ( ( __typeof__ ( ~1@*~a ) ) 1.5 == 1 ) || ( ( __typeof__ ( ~2@*~a ) ) 1.5 == 1 ) )"
                   common-lisp-user::e1
                   common-lisp-user::e2
                   common-lisp-user::e3))
(%defmacro csym::__tg_impl_simple
 (common-lisp-user::x common-lisp-user::y common-lisp-user::z
  common-lisp-user::fn common-lisp-user::fnf common-lisp-user::fnl
  common-lisp-user::va_arg)
 (common-lisp:list 'c-exp
                   "__builtin_choose_expr ( ( ( __builtin_types_compatible_p ( __typeof__ ( ~0@*~a ) , long double ) || __builtin_types_compatible_p ( __typeof__ ( ~1@*~a ) , long double ) || __builtin_types_compatible_p ( __typeof__ ( ~2@*~a ) , long double ) ) || ( __builtin_types_compatible_p ( __typeof__ ( ~0@*~a ) , long double ) || __builtin_types_compatible_p ( __typeof__ ( ~1@*~a ) , long double ) || __builtin_types_compatible_p ( __typeof__ ( ~2@*~a ) , long double ) ) ) , ~5@*~a ( ~6@*~a ) , __builtin_choose_expr ( ( ( __builtin_types_compatible_p ( __typeof__ ( ~0@*~a ) , double ) || __builtin_types_compatible_p ( __typeof__ ( ~1@*~a ) , double ) || __builtin_types_compatible_p ( __typeof__ ( ~2@*~a ) , double ) ) || ( __builtin_types_compatible_p ( __typeof__ ( ~0@*~a ) , double ) || __builtin_types_compatible_p ( __typeof__ ( ~1@*~a ) , double ) || __builtin_types_compatible_p ( __typeof__ ( ~2@*~a ) , double ) ) ) || ( ( ( __typeof__ ( ~0@*~a ) ) 1.5 == 1 ) || ( ( __typeof__ ( ~1@*~a ) ) 1.5 == 1 ) || ( ( __typeof__ ( ~2@*~a ) ) 1.5 == 1 ) ) , ~3@*~a ( ~6@*~a ) , ~4@*~a ( ~6@*~a ) ) )"
                   common-lisp-user::x
                   common-lisp-user::y
                   common-lisp-user::z
                   common-lisp-user::fn
                   common-lisp-user::fnf
                   common-lisp-user::fnl
                   common-lisp-user::va_arg))
(%defmacro csym::__tg_simple (common-lisp-user::x common-lisp-user::fn)
 '"<macro with ## operator>")
(%defmacro csym::__tg_simple2
 (common-lisp-user::x common-lisp-user::y common-lisp-user::fn)
 '"<macro with ## operator>")
(%defmacro csym::__tg_simplev
 (common-lisp-user::x common-lisp-user::fn common-lisp-user::va_arg)
 '"<macro with ## operator>")
(%defmacro csym::__tg_full (common-lisp-user::x common-lisp-user::fn)
 '"<macro with ## operator>")
(%defconstant csym::_@SYS_TIMEB_H_ (c-exp ""))
(%defconstant csym::_@SYS_TIMES_H_ (c-exp ""))
(%defconstant csym::_@SYS_UCONTEXT_H_ (c-exp ""))
(%defconstant csym::_@MACHINE_UCONTEXT_H_ (c-exp ""))
(%defconstant csym::_@MC_FPFMT_NODEV 65536)
(%defconstant csym::_@MC_FPFMT_387 65537)
(%defconstant csym::_@MC_FPFMT_XMM 65538)
(%defconstant csym::_@MC_FPOWNED_NONE 131072)
(%defconstant csym::_@MC_FPOWNED_FPU 131073)
(%defconstant csym::_@MC_FPOWNED_PCB 131074)
(%defconstant csym::@UCF_SWAPPED 1)
(%defconstant csym::_@SYS_UIO_H_ (c-exp ""))
(%defconstant csym::_@ULIMIT_H_ (c-exp ""))
(%defconstant csym::@UL_GETFSIZE 1)
(%defconstant csym::@UL_SETFSIZE 2)
(%defconstant csym::_@SYS_UN_H_ (c-exp ""))
(%defconstant csym::@LOCAL_PEERCRED 1)
(%defmacro csym::@SUN_LEN (common-lisp-user::su)
 (common-lisp:list '+
                   (common-lisp:list '-
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list 'mref
                                                                         'csym::su))
                                     (common-lisp:list 'sizeof
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          'csym::su)
                                                                         'csym::sun_path)))
                   (common-lisp:list 'csym::strlen
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         'csym::su)
                                                       'csym::sun_path))))
(%defconstant csym::_@UNISTD_H_ (c-exp ""))
(%defconstant csym::_@SYS_UNISTD_H_ (c-exp ""))
(%defconstant csym::_@POSIX_ADVISORY_INFO (- 1))
(%defconstant csym::_@POSIX_ASYNCHRONOUS_IO 0)
(%defconstant csym::_@POSIX_CHOWN_RESTRICTED 1)
(%defconstant csym::_@POSIX_CLOCK_SELECTION (- 1))
(%defconstant csym::_@POSIX_CPUTIME (- 1))
(%defconstant csym::_@POSIX_FSYNC (cast long 200112))
(%defconstant csym::_@POSIX_IPV6 0)
(%defconstant csym::_@POSIX_JOB_CONTROL 1)
(%defconstant csym::_@POSIX_MAPPED_FILES (cast long 200112))
(%defconstant csym::_@POSIX_MEMLOCK (- 1))
(%defconstant csym::_@POSIX_MEMLOCK_RANGE (cast long 200112))
(%defconstant csym::_@POSIX_MEMORY_PROTECTION (cast long 200112))
(%defconstant csym::_@POSIX_MESSAGE_PASSING (- 1))
(%defconstant csym::_@POSIX_MONOTONIC_CLOCK (- 1))
(%defconstant csym::_@POSIX_NO_TRUNC 1)
(%defconstant csym::_@POSIX_PRIORITIZED_IO (- 1))
(%defconstant csym::_@POSIX_PRIORITY_SCHEDULING (cast long 200112))
(%defconstant csym::_@POSIX_RAW_SOCKETS (cast long 200112))
(%defconstant csym::_@POSIX_REALTIME_SIGNALS (- 1))
(%defconstant csym::_@POSIX_SEMAPHORES (- 1))
(%defconstant csym::_@POSIX_SHARED_MEMORY_OBJECTS (cast long 200112))
(%defconstant csym::_@POSIX_SPORADIC_SERVER (- 1))
(%defconstant csym::_@POSIX_SYNCHRONIZED_IO (- 1))
(%defconstant csym::_@POSIX_TIMEOUTS (- 1))
(%defconstant csym::_@POSIX_TYPED_MEMORY_OBJECTS (- 1))
(%defconstant csym::_@XOPEN_SHM 1)
(%defconstant csym::_@XOPEN_STREAMS (- 1))
(%defconstant csym::_@POSIX_VERSION (cast long 200112))
(%defconstant csym::@F_OK 0)
(%defconstant csym::@X_OK 1)
(%defconstant csym::@W_OK 2)
(%defconstant csym::@R_OK 4)
(%defconstant csym::@L_SET 0)
(%defconstant csym::@L_INCR 1)
(%defconstant csym::@L_XTND 2)
(%defconstant csym::_@PC_LINK_MAX 1)
(%defconstant csym::_@PC_MAX_CANON 2)
(%defconstant csym::_@PC_MAX_INPUT 3)
(%defconstant csym::_@PC_NAME_MAX 4)
(%defconstant csym::_@PC_PATH_MAX 5)
(%defconstant csym::_@PC_PIPE_BUF 6)
(%defconstant csym::_@PC_CHOWN_RESTRICTED 7)
(%defconstant csym::_@PC_NO_TRUNC 8)
(%defconstant csym::_@PC_VDISABLE 9)
(%defconstant csym::_@PC_ASYNC_IO 53)
(%defconstant csym::_@PC_PRIO_IO 54)
(%defconstant csym::_@PC_SYNC_IO 55)
(%defconstant csym::_@PC_ALLOC_SIZE_MIN 10)
(%defconstant csym::_@PC_FILESIZEBITS 12)
(%defconstant csym::_@PC_REC_INCR_XFER_SIZE 14)
(%defconstant csym::_@PC_REC_MAX_XFER_SIZE 15)
(%defconstant csym::_@PC_REC_MIN_XFER_SIZE 16)
(%defconstant csym::_@PC_REC_XFER_ALIGN 17)
(%defconstant csym::_@PC_SYMLINK_MAX 18)
(%defconstant csym::_@PC_ACL_EXTENDED 59)
(%defconstant csym::_@PC_ACL_PATH_MAX 60)
(%defconstant csym::_@PC_CAP_PRESENT 61)
(%defconstant csym::_@PC_INF_PRESENT 62)
(%defconstant csym::_@PC_MAC_PRESENT 63)
(%defconstant csym::@RFNAMEG (<< 1 0))
(%defconstant csym::@RFENVG (<< 1 1))
(%defconstant csym::@RFFDG (<< 1 2))
(%defconstant csym::@RFNOTEG (<< 1 3))
(%defconstant csym::@RFPROC (<< 1 4))
(%defconstant csym::@RFMEM (<< 1 5))
(%defconstant csym::@RFNOWAIT (<< 1 6))
(%defconstant csym::@RFCNAMEG (<< 1 10))
(%defconstant csym::@RFCENVG (<< 1 11))
(%defconstant csym::@RFCFDG (<< 1 12))
(%defconstant csym::@RFTHREAD (<< 1 13))
(%defconstant csym::@RFSIGSHARE (<< 1 14))
(%defconstant csym::@RFLINUXTHPN (<< 1 16))
(%defconstant csym::@RFSTOPPED (<< 1 17))
(%defconstant csym::@RFHIGHPID (<< 1 18))
(%defconstant csym::@RFPPWAIT (<< 1 31))
(%defconstant csym::@RFKERNELONLY (bit-or (<< 1 17) (<< 1 18) (<< 1 31)))
(%defconstant csym::@STDIN_FILENO 0)
(%defconstant csym::@STDOUT_FILENO 1)
(%defconstant csym::@STDERR_FILENO 2)
(%defconstant csym::@F_ULOCK 0)
(%defconstant csym::@F_LOCK 1)
(%defconstant csym::@F_TLOCK 2)
(%defconstant csym::@F_TEST 3)
(%defconstant csym::_@POSIX_BARRIERS (- 1))
(%defconstant csym::_@POSIX_READER_WRITER_LOCKS (cast long 200112))
(%defconstant csym::_@POSIX_REGEXP 1)
(%defconstant csym::_@POSIX_SHELL 1)
(%defconstant csym::_@POSIX_SPAWN (- 1))
(%defconstant csym::_@POSIX_SPIN_LOCKS (- 1))
(%defconstant csym::_@POSIX_THREAD_ATTR_STACKADDR (cast long 200112))
(%defconstant csym::_@POSIX_THREAD_ATTR_STACKSIZE (cast long 200112))
(%defconstant csym::_@POSIX_THREAD_CPUTIME (- 1))
(%defconstant csym::_@POSIX_THREAD_PRIO_INHERIT (cast long 200112))
(%defconstant csym::_@POSIX_THREAD_PRIO_PROTECT (cast long 200112))
(%defconstant csym::_@POSIX_THREAD_PRIORITY_SCHEDULING (cast long 200112))
(%defconstant csym::_@POSIX_THREAD_PROCESS_SHARED (- 1))
(%defconstant csym::_@POSIX_THREAD_SAFE_FUNCTIONS (- 1))
(%defconstant csym::_@POSIX_THREAD_SPORADIC_SERVER (- 1))
(%defconstant csym::_@POSIX_THREADS (cast long 200112))
(%defconstant csym::_@POSIX_TRACE (- 1))
(%defconstant csym::_@POSIX_TRACE_EVENT_FILTER (- 1))
(%defconstant csym::_@POSIX_TRACE_INHERIT (- 1))
(%defconstant csym::_@POSIX_TRACE_LOG (- 1))
(%defconstant csym::_@POSIX2_C_BIND (cast long 200112))
(%defconstant csym::_@POSIX2_C_DEV (- 1))
(%defconstant csym::_@POSIX2_CHAR_TERM 1)
(%defconstant csym::_@POSIX2_FORT_DEV (- 1))
(%defconstant csym::_@POSIX2_FORT_RUN (cast long 200112))
(%defconstant csym::_@POSIX2_LOCALEDEF (- 1))
(%defconstant csym::_@POSIX2_PBS (- 1))
(%defconstant csym::_@POSIX2_PBS_ACCOUNTING (- 1))
(%defconstant csym::_@POSIX2_PBS_CHECKPOINT (- 1))
(%defconstant csym::_@POSIX2_PBS_LOCATE (- 1))
(%defconstant csym::_@POSIX2_PBS_MESSAGE (- 1))
(%defconstant csym::_@POSIX2_PBS_TRACK (- 1))
(%defconstant csym::_@POSIX2_SW_DEV (- 1))
(%defconstant csym::_@POSIX2_UPE (cast long 200112))
(%defconstant csym::_@V6_ILP32_OFF32 (- 1))
(%defconstant csym::_@V6_ILP32_OFFBIG 0)
(%defconstant csym::_@V6_LP64_OFF64 0)
(%defconstant csym::_@V6_LPBIG_OFFBIG (- 1))
(%defconstant csym::_@XOPEN_CRYPT (- 1))
(%defconstant csym::_@XOPEN_ENH_I18N (- 1))
(%defconstant csym::_@XOPEN_LEGACY (- 1))
(%defconstant csym::_@XOPEN_REALTIME (- 1))
(%defconstant csym::_@XOPEN_REALTIME_THREADS (- 1))
(%defconstant csym::_@XOPEN_UNIX (- 1))
(%defconstant csym::_@POSIX2_VERSION (cast long 199212))
(%defconstant csym::_@SC_ARG_MAX 1)
(%defconstant csym::_@SC_CHILD_MAX 2)
(%defconstant csym::_@SC_CLK_TCK 3)
(%defconstant csym::_@SC_NGROUPS_MAX 4)
(%defconstant csym::_@SC_OPEN_MAX 5)
(%defconstant csym::_@SC_JOB_CONTROL 6)
(%defconstant csym::_@SC_SAVED_IDS 7)
(%defconstant csym::_@SC_VERSION 8)
(%defconstant csym::_@SC_BC_BASE_MAX 9)
(%defconstant csym::_@SC_BC_DIM_MAX 10)
(%defconstant csym::_@SC_BC_SCALE_MAX 11)
(%defconstant csym::_@SC_BC_STRING_MAX 12)
(%defconstant csym::_@SC_COLL_WEIGHTS_MAX 13)
(%defconstant csym::_@SC_EXPR_NEST_MAX 14)
(%defconstant csym::_@SC_LINE_MAX 15)
(%defconstant csym::_@SC_RE_DUP_MAX 16)
(%defconstant csym::_@SC_2_VERSION 17)
(%defconstant csym::_@SC_2_C_BIND 18)
(%defconstant csym::_@SC_2_C_DEV 19)
(%defconstant csym::_@SC_2_CHAR_TERM 20)
(%defconstant csym::_@SC_2_FORT_DEV 21)
(%defconstant csym::_@SC_2_FORT_RUN 22)
(%defconstant csym::_@SC_2_LOCALEDEF 23)
(%defconstant csym::_@SC_2_SW_DEV 24)
(%defconstant csym::_@SC_2_UPE 25)
(%defconstant csym::_@SC_STREAM_MAX 26)
(%defconstant csym::_@SC_TZNAME_MAX 27)
(%defconstant csym::_@SC_ASYNCHRONOUS_IO 28)
(%defconstant csym::_@SC_MAPPED_FILES 29)
(%defconstant csym::_@SC_MEMLOCK 30)
(%defconstant csym::_@SC_MEMLOCK_RANGE 31)
(%defconstant csym::_@SC_MEMORY_PROTECTION 32)
(%defconstant csym::_@SC_MESSAGE_PASSING 33)
(%defconstant csym::_@SC_PRIORITIZED_IO 34)
(%defconstant csym::_@SC_PRIORITY_SCHEDULING 35)
(%defconstant csym::_@SC_REALTIME_SIGNALS 36)
(%defconstant csym::_@SC_SEMAPHORES 37)
(%defconstant csym::_@SC_FSYNC 38)
(%defconstant csym::_@SC_SHARED_MEMORY_OBJECTS 39)
(%defconstant csym::_@SC_SYNCHRONIZED_IO 40)
(%defconstant csym::_@SC_TIMERS 41)
(%defconstant csym::_@SC_AIO_LISTIO_MAX 42)
(%defconstant csym::_@SC_AIO_MAX 43)
(%defconstant csym::_@SC_AIO_PRIO_DELTA_MAX 44)
(%defconstant csym::_@SC_DELAYTIMER_MAX 45)
(%defconstant csym::_@SC_MQ_OPEN_MAX 46)
(%defconstant csym::_@SC_PAGESIZE 47)
(%defconstant csym::_@SC_RTSIG_MAX 48)
(%defconstant csym::_@SC_SEM_NSEMS_MAX 49)
(%defconstant csym::_@SC_SEM_VALUE_MAX 50)
(%defconstant csym::_@SC_SIGQUEUE_MAX 51)
(%defconstant csym::_@SC_TIMER_MAX 52)
(%defconstant csym::_@SC_2_PBS 59)
(%defconstant csym::_@SC_2_PBS_ACCOUNTING 60)
(%defconstant csym::_@SC_2_PBS_CHECKPOINT 61)
(%defconstant csym::_@SC_2_PBS_LOCATE 62)
(%defconstant csym::_@SC_2_PBS_MESSAGE 63)
(%defconstant csym::_@SC_2_PBS_TRACK 64)
(%defconstant csym::_@SC_ADVISORY_INFO 65)
(%defconstant csym::_@SC_BARRIERS 66)
(%defconstant csym::_@SC_CLOCK_SELECTION 67)
(%defconstant csym::_@SC_CPUTIME 68)
(%defconstant csym::_@SC_FILE_LOCKING 69)
(%defconstant csym::_@SC_GETGR_R_SIZE_MAX 70)
(%defconstant csym::_@SC_GETPW_R_SIZE_MAX 71)
(%defconstant csym::_@SC_HOST_NAME_MAX 72)
(%defconstant csym::_@SC_LOGIN_NAME_MAX 73)
(%defconstant csym::_@SC_MONOTONIC_CLOCK 74)
(%defconstant csym::_@SC_MQ_PRIO_MAX 75)
(%defconstant csym::_@SC_READER_WRITER_LOCKS 76)
(%defconstant csym::_@SC_REGEXP 77)
(%defconstant csym::_@SC_SHELL 78)
(%defconstant csym::_@SC_SPAWN 79)
(%defconstant csym::_@SC_SPIN_LOCKS 80)
(%defconstant csym::_@SC_SPORADIC_SERVER 81)
(%defconstant csym::_@SC_THREAD_ATTR_STACKADDR 82)
(%defconstant csym::_@SC_THREAD_ATTR_STACKSIZE 83)
(%defconstant csym::_@SC_THREAD_CPUTIME 84)
(%defconstant csym::_@SC_THREAD_DESTRUCTOR_ITERATIONS 85)
(%defconstant csym::_@SC_THREAD_KEYS_MAX 86)
(%defconstant csym::_@SC_THREAD_PRIO_INHERIT 87)
(%defconstant csym::_@SC_THREAD_PRIO_PROTECT 88)
(%defconstant csym::_@SC_THREAD_PRIORITY_SCHEDULING 89)
(%defconstant csym::_@SC_THREAD_PROCESS_SHARED 90)
(%defconstant csym::_@SC_THREAD_SAFE_FUNCTIONS 91)
(%defconstant csym::_@SC_THREAD_SPORADIC_SERVER 92)
(%defconstant csym::_@SC_THREAD_STACK_MIN 93)
(%defconstant csym::_@SC_THREAD_THREADS_MAX 94)
(%defconstant csym::_@SC_TIMEOUTS 95)
(%defconstant csym::_@SC_THREADS 96)
(%defconstant csym::_@SC_TRACE 97)
(%defconstant csym::_@SC_TRACE_EVENT_FILTER 98)
(%defconstant csym::_@SC_TRACE_INHERIT 99)
(%defconstant csym::_@SC_TRACE_LOG 100)
(%defconstant csym::_@SC_TTY_NAME_MAX 101)
(%defconstant csym::_@SC_TYPED_MEMORY_OBJECTS 102)
(%defconstant csym::_@SC_V6_ILP32_OFF32 103)
(%defconstant csym::_@SC_V6_ILP32_OFFBIG 104)
(%defconstant csym::_@SC_V6_LP64_OFF64 105)
(%defconstant csym::_@SC_V6_LPBIG_OFFBIG 106)
(%defconstant csym::_@SC_IPV6 118)
(%defconstant csym::_@SC_RAW_SOCKETS 119)
(%defconstant csym::_@SC_SYMLOOP_MAX 120)
(%defconstant csym::_@SC_ATEXIT_MAX 107)
(%defconstant csym::_@SC_IOV_MAX 56)
(%defconstant csym::_@SC_PAGE_SIZE 47)
(%defconstant csym::_@SC_XOPEN_CRYPT 108)
(%defconstant csym::_@SC_XOPEN_ENH_I18N 109)
(%defconstant csym::_@SC_XOPEN_LEGACY 110)
(%defconstant csym::_@SC_XOPEN_REALTIME 111)
(%defconstant csym::_@SC_XOPEN_REALTIME_THREADS 112)
(%defconstant csym::_@SC_XOPEN_SHM 113)
(%defconstant csym::_@SC_XOPEN_STREAMS 114)
(%defconstant csym::_@SC_XOPEN_UNIX 115)
(%defconstant csym::_@SC_XOPEN_VERSION 116)
(%defconstant csym::_@SC_XOPEN_XCU_VERSION 117)
(%defconstant csym::_@SC_NPROCESSORS_CONF 57)
(%defconstant csym::_@SC_NPROCESSORS_ONLN 58)
(%defconstant csym::_@CS_PATH 1)
(%defconstant csym::_@CS_POSIX_V6_ILP32_OFF32_CFLAGS 2)
(%defconstant csym::_@CS_POSIX_V6_ILP32_OFF32_LDFLAGS 3)
(%defconstant csym::_@CS_POSIX_V6_ILP32_OFF32_LIBS 4)
(%defconstant csym::_@CS_POSIX_V6_ILP32_OFFBIG_CFLAGS 5)
(%defconstant csym::_@CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS 6)
(%defconstant csym::_@CS_POSIX_V6_ILP32_OFFBIG_LIBS 7)
(%defconstant csym::_@CS_POSIX_V6_LP64_OFF64_CFLAGS 8)
(%defconstant csym::_@CS_POSIX_V6_LP64_OFF64_LDFLAGS 9)
(%defconstant csym::_@CS_POSIX_V6_LP64_OFF64_LIBS 10)
(%defconstant csym::_@CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS 11)
(%defconstant csym::_@CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS 12)
(%defconstant csym::_@CS_POSIX_V6_LPBIG_OFFBIG_LIBS 13)
(%defconstant csym::_@CS_POSIX_V6_WIDTH_RESTRICTED_ENVS 14)
(%defconstant csym::_@GETOPT_DECLARED (c-exp ""))
(%defconstant csym::_@OPTRESET_DECLARED (c-exp ""))
(%defconstant csym::_@UTIME_H_ (c-exp ""))
(%defconstant csym::_@SYS_UTSNAME_H (c-exp ""))
(%defconstant csym::@SYS_NMLN 256)
(%defconstant csym::_@SYS_WAIT_H_ (c-exp ""))
(%defmacro csym::_@W_INT (common-lisp-user::w)
 (common-lisp:list 'mref
                   (common-lisp:list 'cast
                                     (common-lisp:list 'ptr 'int)
                                     (common-lisp:list 'ptr 'csym::w))))
(%defconstant csym::@WCOREFLAG 128)
(%defmacro csym::_@WSTATUS (common-lisp-user::x)
 (common-lisp:list 'bit-and
                   (common-lisp:list 'mref
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'int)
                                                       (common-lisp:list 'ptr
                                                                         'csym::x)))
                   '127))
(%defconstant csym::_@WSTOPPED 127)
(%defmacro csym::@WIFSTOPPED (common-lisp-user::x)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          'int)
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          'csym::x)))
                                     '127)
                   '127))
(%defmacro csym::@WSTOPSIG (common-lisp-user::x)
 (common-lisp:list '>>
                   (common-lisp:list 'mref
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'int)
                                                       (common-lisp:list 'ptr
                                                                         'csym::x)))
                   '8))
(%defmacro csym::@WIFSIGNALED (common-lisp-user::x)
 (common-lisp:list 'and
                   (common-lisp:list '!=
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           'int)
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           'csym::x)))
                                                       '127)
                                     '127)
                   (common-lisp:list '!=
                                     (common-lisp:list 'bit-and
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           'int)
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           'csym::x)))
                                                       '127)
                                     '0)))
(%defmacro csym::@WTERMSIG (common-lisp-user::x)
 (common-lisp:list 'bit-and
                   (common-lisp:list 'mref
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'int)
                                                       (common-lisp:list 'ptr
                                                                         'csym::x)))
                   '127))
(%defmacro csym::@WIFEXITED (common-lisp-user::x)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'cast
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          'int)
                                                                         (common-lisp:list
                                                                          'ptr
                                                                          'csym::x)))
                                     '127)
                   '0))
(%defmacro csym::@WEXITSTATUS (common-lisp-user::x)
 (common-lisp:list '>>
                   (common-lisp:list 'mref
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'int)
                                                       (common-lisp:list 'ptr
                                                                         'csym::x)))
                   '8))
(%defmacro csym::@WIFCONTINUED (common-lisp-user::x)
 (common-lisp:list '== 'csym::x '19))
(%defmacro csym::@WCOREDUMP (common-lisp-user::x)
 (common-lisp:list 'bit-and
                   (common-lisp:list 'mref
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         'int)
                                                       (common-lisp:list 'ptr
                                                                         'csym::x)))
                   '128))
(%defmacro csym::@W_EXITCODE (common-lisp-user::ret common-lisp-user::sig)
 (common-lisp:list 'bit-or (common-lisp:list '<< 'csym::ret '8) 'csym::sig))
(%defmacro csym::@W_STOPCODE (common-lisp-user::sig)
 (common-lisp:list 'bit-or (common-lisp:list '<< 'csym::sig '8) '127))
(%defconstant csym::@WNOHANG 1)
(%defconstant csym::@WUNTRACED 2)
(%defconstant csym::@WCONTINUED 4)
(%defconstant csym::@WLINUXCLONE 2147483648)
(%defconstant csym::@WAIT_ANY (- 1))
(%defconstant csym::@WAIT_MYPGRP 0)
(%defconstant csym::_@WCHAR_H_ (c-exp ""))
(%defconstant csym::_@MBSTATE_T_DECLARED (c-exp ""))
(%defconstant csym::_@WINT_T_DECLARED (c-exp ""))
(%defconstant csym::@WEOF (cast csym::wint_t (- 1)))
(%defmacro csym::getwc (common-lisp-user::fp)
 (common-lisp:list 'csym::fgetwc 'csym::fp))
(%defmacro csym::getwchar common-lisp:nil
 (common-lisp:list 'csym::fgetwc 'csym::__stdinp))
(%defmacro csym::putwc (common-lisp-user::wc common-lisp-user::fp)
 (common-lisp:list 'csym::fputwc 'csym::wc 'csym::fp))
(%defmacro csym::putwchar (common-lisp-user::wc)
 (common-lisp:list 'csym::fputwc 'csym::wc 'csym::__stdoutp))
(%defmacro csym::wcwidth (common-lisp-user::_c)
 (common-lisp:list 'csym::__wcwidth 'csym::_c))
(%defconstant csym::_@WCTYPE_H_ (c-exp ""))
(%defconstant csym::_@WCTRANS_T (c-exp ""))
(%defconstant csym::_@WCTYPE_T (c-exp ""))
(%defmacro csym::iswalnum (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'bit-or
                                     (common-lisp:list 'cast 'long '256)
                                     (common-lisp:list 'cast 'long '1024))))
(%defmacro csym::iswalpha (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '256)))
(%defmacro csym::iswblank (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '131072)))
(%defmacro csym::iswcntrl (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '512)))
(%defmacro csym::iswctype (common-lisp-user::wc common-lisp-user::charclass)
 (common-lisp:list 'csym::__istype 'csym::wc 'csym::charclass))
(%defmacro csym::iswdigit (common-lisp-user::wc)
 (common-lisp:list 'csym::__isctype
                   'csym::wc
                   (common-lisp:list 'cast 'long '1024)))
(%defmacro csym::iswgraph (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '2048)))
(%defmacro csym::iswlower (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '4096)))
(%defmacro csym::iswprint (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '262144)))
(%defmacro csym::iswpunct (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '8192)))
(%defmacro csym::iswspace (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '16384)))
(%defmacro csym::iswupper (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '32768)))
(%defmacro csym::iswxdigit (common-lisp-user::wc)
 (common-lisp:list 'csym::__isctype
                   'csym::wc
                   (common-lisp:list 'cast 'long '65536)))
(%defmacro csym::towlower (common-lisp-user::wc)
 (common-lisp:list 'csym::__tolower 'csym::wc))
(%defmacro csym::towupper (common-lisp-user::wc)
 (common-lisp:list 'csym::__toupper 'csym::wc))
(%defmacro csym::iswascii (common-lisp-user::wc)
 (common-lisp:list '==
                   (common-lisp:list 'bit-and
                                     'csym::wc
                                     (common-lisp:list 'bit-not '127))
                   '0))
(%defmacro csym::iswhexnumber (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '65536)))
(%defmacro csym::iswideogram (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '524288)))
(%defmacro csym::iswnumber (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '1024)))
(%defmacro csym::iswphonogram (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '2097152)))
(%defmacro csym::iswrune (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '4294967040)))
(%defmacro csym::iswspecial (common-lisp-user::wc)
 (common-lisp:list 'csym::__istype
                   'csym::wc
                   (common-lisp:list 'cast 'long '1048576)))
(%defconstant csym::_@WORDEXP_H_ (c-exp ""))
(%defconstant csym::@WRDE_APPEND 1)
(%defconstant csym::@WRDE_DOOFFS 2)
(%defconstant csym::@WRDE_NOCMD 4)
(%defconstant csym::@WRDE_REUSE 8)
(%defconstant csym::@WRDE_SHOWERR 16)
(%defconstant csym::@WRDE_UNDEF 32)
(%defconstant csym::@WRDE_BADCHAR 1)
(%defconstant csym::@WRDE_BADVAL 2)
(%defconstant csym::@WRDE_CMDSUB 3)
(%defconstant csym::@WRDE_NOSPACE 4)
(%defconstant csym::@WRDE_NOSYS 5)
(%defconstant csym::@WRDE_SYNTAX 6)
