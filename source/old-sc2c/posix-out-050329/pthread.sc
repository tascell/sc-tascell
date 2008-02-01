(%line 1 "csample/posix-header/pthread.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/machine/endian.h")
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
(deftype csym::pid_t csym::__pid_t)
(deftype csym::register_t csym::__register_t)
(deftype csym::rlim_t csym::__rlim_t)
(deftype csym::segsz_t csym::__segsz_t)
(deftype csym::size_t csym::__size_t)
(deftype csym::ssize_t csym::__ssize_t)
(deftype csym::suseconds_t csym::__suseconds_t)
(deftype csym::time_t csym::__time_t)
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
(%line 1 "/usr/include/sys/_types.h")
(%line 47 "/usr/include/sys/_timeval.h")
(def (struct csym::timeval) (decl csym::tv_sec long)
 (decl csym::tv_usec csym::suseconds_t))
(%line 1 "/usr/include/sys/timespec.h")
(%line 46 "/usr/include/sys/timespec.h")
(def (struct csym::timespec) (decl csym::tv_sec csym::time_t)
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
(%line 1 "/usr/include/sys/time.h")
(%line 1 "/usr/include/sys/_timeval.h")
(%line 1 "/usr/include/sys/types.h")
(%line 1 "/usr/include/sys/timespec.h")
(%line 40 "/usr/include/sys/time.h")
(def (struct csym::timezone) (decl csym::tz_minuteswest int)
 (decl csym::tz_dsttime int))
(def (struct csym::bintime) (decl csym::sec csym::time_t)
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
(decl (csym::ctime) (fn (ptr char) (ptr (const csym::time_t))))
(decl (csym::difftime) (fn double csym::time_t csym::time_t))
(decl (csym::gmtime) (fn (ptr (struct csym::tm)) (ptr (const csym::time_t))))
(decl (csym::localtime) (fn (ptr (struct csym::tm)) (ptr (const csym::time_t))))
(decl (csym::mktime) (fn csym::time_t (ptr (struct csym::tm))))
(decl (csym::strftime)
 (fn csym::size_t (ptr char) csym::size_t (ptr (const char))
  (ptr (const (struct csym::tm)))))
(decl (csym::time) (fn csym::time_t (ptr csym::time_t)))
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
(decl (csym::ctime_r) (fn (ptr char) (ptr (const csym::time_t)) (ptr char)))
(decl (csym::gmtime_r)
 (fn (ptr (struct csym::tm)) (ptr (const csym::time_t))
  (ptr (struct csym::tm))))
(decl (csym::localtime_r)
 (fn (ptr (struct csym::tm)) (ptr (const csym::time_t))
  (ptr (struct csym::tm))))
(decl (csym::strptime)
 (fn (ptr char) (ptr (const char)) (ptr (const char)) (ptr (struct csym::tm))))
(decl (csym::timezone) (fn (ptr char) int int))
(decl (csym::tzsetwall) (fn void void))
(decl (csym::timelocal) (fn csym::time_t (const (ptr (struct csym::tm)))))
(decl (csym::timegm) (fn csym::time_t (const (ptr (struct csym::tm)))))
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
(%line 1 "/usr/include/limits.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/limits.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/machine/_limits.h")
(%line 1 "/usr/include/sys/syslimits.h")
(%line 1 "/usr/include/sched.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 55 "/usr/include/sched.h")
(def (struct csym::sched_param) (decl csym::sched_priority int))
(%line 1 "/usr/include/sys/cdefs.h")
(%line 62 "/usr/include/sched.h")
(decl (csym::sched_get_priority_max) (fn int int))
(decl (csym::sched_get_priority_min) (fn int int))
(decl (csym::sched_getparam)
 (fn int csym::pid_t (ptr (struct csym::sched_param))))
(decl (csym::sched_getscheduler) (fn int csym::pid_t))
(decl (csym::sched_rr_get_interval)
 (fn int csym::pid_t (ptr (struct csym::timespec))))
(decl (csym::sched_setparam)
 (fn int csym::pid_t (ptr (const (struct csym::sched_param)))))
(decl (csym::sched_setscheduler)
 (fn int csym::pid_t int (ptr (const (struct csym::sched_param)))))
(decl (csym::sched_yield) (fn int void))
(%line 90 "csample/posix-header/pthread.h")
(%line 110 "csample/posix-header/pthread.h")
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
(%line 130 "csample/posix-header/pthread.h")
(deftype csym::pthread_addr_t (ptr void))
(deftype csym::pthread_startroutine_t (ptr (fn (ptr void) (ptr void))))
(%line 136 "csample/posix-header/pthread.h")
(def (struct csym::pthread_once) (decl csym::state int)
 (decl csym::mutex csym::pthread_mutex_t))
(%line 184 "csample/posix-header/pthread.h")
(def (enum csym::pthread_mutextype)
 ((csym::@PTHREAD_MUTEX_ERRORCHECK 1) (csym::@PTHREAD_MUTEX_RECURSIVE 2)
  (csym::@PTHREAD_MUTEX_NORMAL 3) csym::@MUTEX_TYPE_MAX))
(%line 198 "csample/posix-header/pthread.h")
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
(%defconstant csym::__@TIME__ "22:37:08")
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
(%defconstant csym::_@PTHREAD_H_ (c-exp ""))
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
(%defconstant csym::_@SYS_TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE_ENDIAN_H_ (c-exp ""))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
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
(%defconstant csym::__@TIME__ "22:37:08")
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
(%defconstant csym::_@PTHREAD_H_ (c-exp ""))
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
(%defconstant csym::_@SYS_TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE_ENDIAN_H_ (c-exp ""))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
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
(%defconstant csym::_@SUSECONDS_T_DECLARED (c-exp ""))
(%defconstant csym::_@TIME_T_DECLARED (c-exp ""))
(%defconstant csym::_@TIMER_T_DECLARED (c-exp ""))
(%defconstant csym::_@UID_T_DECLARED (c-exp ""))
(%defconstant csym::_@USECONDS_T_DECLARED (c-exp ""))
(%defconstant csym::_@SYS_SELECT_H_ (c-exp ""))
(%defconstant csym::_@SYS__SIGSET_H_ (c-exp ""))
(%defconstant csym::_@SIG_WORDS 4)
(%defconstant csym::_@SIG_MAXSIG 128)
(%defmacro csym::_@SIG_IDX (csym::_arg0) (common-lisp:list '- csym::_arg0 '1))
(%defmacro csym::_@SIG_WORD (csym::_arg0)
 (common-lisp:list '>> (common-lisp:list 'csym::_@SIG_IDX csym::_arg0) '5))
(%defmacro csym::_@SIG_BIT (csym::_arg0)
 (common-lisp:list '<<
                   '1
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'csym::_@SIG_IDX
                                                       csym::_arg0)
                                     '31)))
(%defmacro csym::_@SIG_VALID (csym::_arg0)
 (common-lisp:list 'and
                   (common-lisp:list '<= csym::_arg0 'csym::_@SIG_MAXSIG)
                   (common-lisp:list '> csym::_arg0 '0)))
(%defconstant csym::_@SYS__TIMEVAL_H_ (c-exp ""))
(%defconstant csym::_@SYS_TIMESPEC_H_ (c-exp ""))
(%defmacro csym::@TIMEVAL_TO_TIMESPEC (csym::_arg0 csym::_arg1)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg1)
                                                        'csym::tv_sec)
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg0)
                                                        'csym::tv_sec))
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg1)
                                                        'csym::tv_nsec)
                                      (common-lisp:list '*
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::tv_usec)
                                                        '1000)))))
(%defmacro csym::@TIMESPEC_TO_TIMEVAL (csym::_arg0 csym::_arg1)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg0)
                                                        'csym::tv_sec)
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg1)
                                                        'csym::tv_sec))
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg0)
                                                        'csym::tv_usec)
                                      (common-lisp:list '/
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg1)
                                                                          'csym::tv_nsec)
                                                        '1000)))))
(%defconstant csym::_@SIGSET_T_DECLARED (c-exp ""))
(%defconstant csym::@FD_SETSIZE (cast unsigned-int 1024))
(%defconstant csym::_@NFDBITS (* (sizeof csym::__fd_mask) 8))
(%defconstant csym::@NFDBITS csym::_@NFDBITS)
(%defmacro csym::_howmany (csym::_arg0 csym::_arg1)
 (common-lisp:list '/
                   (common-lisp:list '+
                                     csym::_arg0
                                     (common-lisp:list '- csym::_arg1 '1))
                   csym::_arg1))
(%defconstant csym::fds_bits csym::__fds_bits)
(%defmacro csym::__fdset_mask (csym::_arg0)
 (common-lisp:list '<<
                   (common-lisp:list 'cast 'csym::__fd_mask '1)
                   (common-lisp:list '% csym::_arg0 'csym::_@NFDBITS)))
(%defmacro csym::@FD_CLR (csym::_arg0 csym::_arg1)
 (common-lisp:list 'bit-and=
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg1)
                                                       'csym::__fds_bits)
                                     (common-lisp:list '/
                                                       csym::_arg0
                                                       'csym::_@NFDBITS))
                   (common-lisp:list 'bit-not
                                     (common-lisp:list 'csym::__fdset_mask
                                                       csym::_arg0))))
(%defmacro csym::@FD_COPY (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   'void
                   (common-lisp:list '=
                                     (common-lisp:list 'mref csym::_arg1)
                                     (common-lisp:list 'mref csym::_arg0))))
(%defmacro csym::@FD_ISSET (csym::_arg0 csym::_arg1)
 (common-lisp:list 'bit-and
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg1)
                                                       'csym::__fds_bits)
                                     (common-lisp:list '/
                                                       csym::_arg0
                                                       'csym::_@NFDBITS))
                   (common-lisp:list 'csym::__fdset_mask csym::_arg0)))
(%defmacro csym::@FD_SET (csym::_arg0 csym::_arg1)
 (common-lisp:list 'bit-or=
                   (common-lisp:list 'aref
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg1)
                                                       'csym::__fds_bits)
                                     (common-lisp:list '/
                                                       csym::_arg0
                                                       'csym::_@NFDBITS))
                   (common-lisp:list 'csym::__fdset_mask csym::_arg0)))
(%defmacro csym::@FD_ZERO (csym::_arg0)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list 'decl
                                      'csym::_p
                                      (common-lisp:list 'ptr 'csym::fd_set))
                    (common-lisp:list 'decl 'csym::_n 'csym::__size_t)
                    (common-lisp:list '= 'csym::_p csym::_arg0)
                    (common-lisp:list '=
                                      'csym::_n
                                      (common-lisp:list 'csym::_howmany
                                                        'csym::@FD_SETSIZE
                                                        'csym::_@NFDBITS))
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
(%defmacro csym::major (csym::_arg0)
 (common-lisp:list 'cast
                   'int
                   (common-lisp:list 'bit-and
                                     (common-lisp:list '>>
                                                       (common-lisp:list 'cast
                                                                         'csym::u_int
                                                                         csym::_arg0)
                                                       '8)
                                     '255)))
(%defmacro csym::minor (csym::_arg0)
 (common-lisp:list 'cast
                   'int
                   (common-lisp:list 'cast
                                     csym::_arg0
                                     (common-lisp:list 'ptr '4294902015))))
(%defmacro csym::makedev (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   'csym::dev_t
                   (common-lisp:list 'bit-or
                                     (common-lisp:list '<< csym::_arg0 '8)
                                     csym::_arg1)))
(%defconstant csym::_@FTRUNCATE_DECLARED (c-exp ""))
(%defconstant csym::_@LSEEK_DECLARED (c-exp ""))
(%defconstant csym::_@MMAP_DECLARED (c-exp ""))
(%defconstant csym::_@TRUNCATE_DECLARED (c-exp ""))
(%defconstant csym::_@SYS_TIME_H_ (c-exp ""))
(%defconstant csym::@DST_NONE 0)
(%defconstant csym::@DST_USA 1)
(%defconstant csym::@DST_AUST 2)
(%defconstant csym::@DST_WET 3)
(%defconstant csym::@DST_MET 4)
(%defconstant csym::@DST_EET 5)
(%defconstant csym::@DST_CAN 6)
(%defmacro csym::timerclear (csym::_arg0)
 (common-lisp:list '=
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref csym::_arg0)
                                     'csym::tv_sec)
                   (common-lisp:list '=
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg0)
                                                       'csym::tv_usec)
                                     '0)))
(%defmacro csym::timerisset (csym::_arg0)
 (common-lisp:list 'or
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref csym::_arg0)
                                     'csym::tv_sec)
                   (common-lisp:list 'fref
                                     (common-lisp:list 'mref csym::_arg0)
                                     'csym::tv_usec)))
(%defmacro csym::timercmp (csym::_arg0 csym::_arg1 csym::_arg2)
 (common-lisp:list 'c-exp
                   "( ( ( ~0@*~a ) -> tv_sec == ( ~1@*~a ) -> tv_sec ) ? ( ( ~0@*~a ) -> tv_usec ~2@*~a ( ~1@*~a ) -> tv_usec ) : ( ( ~0@*~a ) -> tv_sec ~2@*~a ( ~1@*~a ) -> tv_sec ) )"
                   csym::_arg0
                   csym::_arg1
                   csym::_arg2))
(%defmacro csym::timeradd (csym::_arg0 csym::_arg1 csym::_arg2)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg2)
                                                        'csym::tv_sec)
                                      (common-lisp:list '+
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::tv_sec)
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg1)
                                                                          'csym::tv_sec)))
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg2)
                                                        'csym::tv_usec)
                                      (common-lisp:list '+
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::tv_usec)
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg1)
                                                                          'csym::tv_usec)))
                    (common-lisp:list 'if
                                      (common-lisp:list '>=
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg2)
                                                                          'csym::tv_usec)
                                                        '1000000)
                                      (common-lisp:list 'begin
                                                        (common-lisp:list 'inc
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            csym::_arg2)
                                                                           'csym::tv_sec))
                                                        (common-lisp:list '-=
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            csym::_arg2)
                                                                           'csym::tv_usec)
                                                                          '1000000))))))
(%defmacro csym::timersub (csym::_arg0 csym::_arg1 csym::_arg2)
 (common-lisp:list
  (common-lisp:list 'do-while
                    '0
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg2)
                                                        'csym::tv_sec)
                                      (common-lisp:list '-
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::tv_sec)
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg1)
                                                                          'csym::tv_sec)))
                    (common-lisp:list '=
                                      (common-lisp:list 'fref
                                                        (common-lisp:list 'mref
                                                                          csym::_arg2)
                                                        'csym::tv_usec)
                                      (common-lisp:list '-
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::tv_usec)
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg1)
                                                                          'csym::tv_usec)))
                    (common-lisp:list 'if
                                      (common-lisp:list '<
                                                        (common-lisp:list 'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg2)
                                                                          'csym::tv_usec)
                                                        '0)
                                      (common-lisp:list 'begin
                                                        (common-lisp:list 'dec
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            csym::_arg2)
                                                                           'csym::tv_sec))
                                                        (common-lisp:list '+=
                                                                          (common-lisp:list
                                                                           'fref
                                                                           (common-lisp:list
                                                                            'mref
                                                                            csym::_arg2)
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
(%defconstant csym::@ILL_PRIVIN_FAULT csym::@T_PRIVINFLT)
(%defconstant csym::@ILL_RESOP_FAULT csym::@T_RESOPFLT)
(%defconstant csym::@ILL_ALIGN_FAULT csym::@T_ALIGNFLT)
(%defconstant csym::@ILL_FPOP_FAULT csym::@T_FPOPFLT)
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
(%defconstant csym::@BUS_PAGE_FAULT csym::@T_PAGEFLT)
(%defconstant csym::@BUS_SEGNP_FAULT csym::@T_SEGNPFLT)
(%defconstant csym::@BUS_STK_FAULT csym::@T_STKFLT)
(%defconstant csym::@BUS_SEGM_FAULT csym::@T_RESERVED)
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
(%defconstant csym::@SIGIOT csym::@SIGABRT)
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
(%defconstant csym::@SIGSTKSZ (+ csym::@MINSIGSTKSZ 32768))
(%defconstant csym::@SV_ONSTACK csym::@SA_ONSTACK)
(%defconstant csym::@SV_INTERRUPT csym::@SA_RESTART)
(%defconstant csym::@SV_RESETHAND csym::@SA_RESETHAND)
(%defconstant csym::@SV_NODEFER csym::@SA_NODEFER)
(%defconstant csym::@SV_NOCLDSTOP csym::@SA_NOCLDSTOP)
(%defconstant csym::@SV_SIGINFO csym::@SA_SIGINFO)
(%defconstant csym::sv_onstack csym::sv_flags)
(%defmacro csym::sigmask (csym::_arg0)
 (common-lisp:list '<< '1 (common-lisp:list '- csym::_arg0 '1)))
(%defconstant csym::@BADSIG csym::@SIG_ERR)
(%defconstant csym::@SIG_BLOCK 1)
(%defconstant csym::@SIG_UNBLOCK 2)
(%defconstant csym::@SIG_SETMASK 3)
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
(%defconstant csym::_@SCHED_H_ (c-exp ""))
(%defconstant csym::@SCHED_FIFO 1)
(%defconstant csym::@SCHED_OTHER 2)
(%defconstant csym::@SCHED_RR 3)
(%defconstant csym::@PTHREAD_DESTRUCTOR_ITERATIONS 4)
(%defconstant csym::@PTHREAD_KEYS_MAX 256)
(%defconstant csym::@PTHREAD_STACK_MIN 1024)
(%defconstant csym::@PTHREAD_THREADS_MAX csym::@ULONG_MAX)
(%defconstant csym::@PTHREAD_BARRIER_SERIAL_THREAD (- 1))
(%defconstant csym::@PTHREAD_DETACHED 1)
(%defconstant csym::@PTHREAD_SCOPE_SYSTEM 2)
(%defconstant csym::@PTHREAD_INHERIT_SCHED 4)
(%defconstant csym::@PTHREAD_NOFLOAT 8)
(%defconstant csym::@PTHREAD_CREATE_DETACHED csym::@PTHREAD_DETACHED)
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
(%defconstant csym::@PTHREAD_ONCE_INIT (c-exp "{ PTHREAD_NEEDS_INIT, NULL }"))
(%defconstant csym::@PTHREAD_MUTEX_INITIALIZER csym::@NULL)
(%defconstant csym::@PTHREAD_COND_INITIALIZER csym::@NULL)
(%defconstant csym::@PTHREAD_RWLOCK_INITIALIZER csym::@NULL)
(%defconstant csym::pthread_condattr_default csym::@NULL)
(%defconstant csym::pthread_mutexattr_default csym::@NULL)
(%defconstant csym::pthread_attr_default csym::@NULL)
(%defconstant csym::@PTHREAD_PRIO_NONE 0)
(%defconstant csym::@PTHREAD_PRIO_INHERIT 1)
(%defconstant csym::@PTHREAD_PRIO_PROTECT 2)
(%defconstant csym::@PTHREAD_MUTEX_DEFAULT csym::@PTHREAD_MUTEX_ERRORCHECK)
(%defconstant csym::@MUTEX_TYPE_FAST csym::@PTHREAD_MUTEX_NORMAL)
(%defconstant csym::@MUTEX_TYPE_COUNTING_FAST csym::@PTHREAD_MUTEX_RECURSIVE)
