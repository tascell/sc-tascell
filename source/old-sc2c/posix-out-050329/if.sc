(%line 1 "csample/posix-header/if.h")
(%line 1 "/usr/include/sys/cdefs.h")
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
(deftype csym::time_t csym::__time_t)
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
(%line 51 "csample/posix-header/if.h")
(%line 69 "csample/posix-header/if.h")
(def (struct csym::if_clonereq) (decl csym::ifcr_total int)
 (decl csym::ifcr_count int) (decl csym::ifcr_buffer (ptr char)))
(%line 79 "csample/posix-header/if.h")
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
 (decl csym::ifi_hwassist csym::u_long) (decl csym::ifi_epoch csym::time_t)
 (decl csym::ifi_lastchange (struct csym::timeval)))
(%line 109 "csample/posix-header/if.h")
(%line 174 "csample/posix-header/if.h")
(def (struct csym::if_msghdr) (decl csym::ifm_msglen csym::u_short)
 (decl csym::ifm_version csym::u_char) (decl csym::ifm_type csym::u_char)
 (decl csym::ifm_addrs int) (decl csym::ifm_flags int)
 (decl csym::ifm_index csym::u_short)
 (decl csym::ifm_data (struct csym::if_data)))
(%line 188 "csample/posix-header/if.h")
(def (struct csym::ifa_msghdr) (decl csym::ifam_msglen csym::u_short)
 (decl csym::ifam_version csym::u_char) (decl csym::ifam_type csym::u_char)
 (decl csym::ifam_addrs int) (decl csym::ifam_flags int)
 (decl csym::ifam_index csym::u_short) (decl csym::ifam_metric int))
(%line 202 "csample/posix-header/if.h")
(def (struct csym::ifma_msghdr) (decl csym::ifmam_msglen csym::u_short)
 (decl csym::ifmam_version csym::u_char) (decl csym::ifmam_type csym::u_char)
 (decl csym::ifmam_addrs int) (decl csym::ifmam_flags int)
 (decl csym::ifmam_index csym::u_short))
(%line 214 "csample/posix-header/if.h")
(def (struct csym::if_announcemsghdr) (decl csym::ifan_msglen csym::u_short)
 (decl csym::ifan_version csym::u_char) (decl csym::ifan_type csym::u_char)
 (decl csym::ifan_index csym::u_short) (decl csym::ifan_name (array char 16))
 (decl csym::ifan_what csym::u_short))
(%line 232 "csample/posix-header/if.h")
(def (struct csym::ifreq) (decl csym::ifr_name (array char 16))
 (def (union csym::unnamed1) (decl csym::ifru_addr (struct csym::sockaddr))
  (decl csym::ifru_dstaddr (struct csym::sockaddr))
  (decl csym::ifru_broadaddr (struct csym::sockaddr))
  (decl csym::ifru_flags (array short 2)) (decl csym::ifru_index short)
  (decl csym::ifru_metric int) (decl csym::ifru_mtu int)
  (decl csym::ifru_phys int) (decl csym::ifru_media int)
  (decl csym::ifru_data csym::caddr_t) (decl csym::ifru_cap (array int 2)))
 (decl csym::ifr_ifru (union csym::unnamed1)))
(%line 260 "csample/posix-header/if.h")
(%line 267 "csample/posix-header/if.h")
(def (struct csym::ifaliasreq) (decl csym::ifra_name (array char 16))
 (decl csym::ifra_addr (struct csym::sockaddr))
 (decl csym::ifra_broadaddr (struct csym::sockaddr))
 (decl csym::ifra_mask (struct csym::sockaddr)))
(def (struct csym::ifmediareq) (decl csym::ifm_name (array char 16))
 (decl csym::ifm_current int) (decl csym::ifm_mask int)
 (decl csym::ifm_status int) (decl csym::ifm_active int)
 (decl csym::ifm_count int) (decl csym::ifm_ulist (ptr int)))
(%line 292 "csample/posix-header/if.h")
(def (struct csym::ifstat) (decl csym::ifs_name (array char 16))
 (decl csym::ascii (array char (+ 800 1))))
(%line 303 "csample/posix-header/if.h")
(def (struct csym::ifconf) (decl csym::ifc_len int)
 (def (union csym::unnamed2) (decl csym::ifcu_buf csym::caddr_t)
  (decl csym::ifcu_req (ptr (struct csym::ifreq))))
 (decl csym::ifc_ifcu (union csym::unnamed2)))
(%line 317 "csample/posix-header/if.h")
(def (struct csym::if_laddrreq) (decl csym::iflr_name (array char 16))
 (decl csym::flags csym::u_int) (decl csym::prefixlen csym::u_int)
 (decl csym::addr (struct csym::sockaddr_storage))
 (decl csym::dstaddr (struct csym::sockaddr_storage)))
(%line 336 "csample/posix-header/if.h")
(def (struct csym::if_nameindex) (decl csym::if_index unsigned-int)
 (decl csym::if_name (ptr char)))
(decl (csym::if_freenameindex) (fn void (ptr (struct csym::if_nameindex))))
(decl (csym::if_indextoname) (fn (ptr char) unsigned-int (ptr char)))
(decl (csym::if_nameindex) (fn (ptr (struct csym::if_nameindex)) void))
(decl (csym::if_nametoindex) (fn unsigned-int (ptr (const char))))
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
(%defconstant csym::__@TIME__ "21:30:02")
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
(%defconstant csym::_@NET_IF_H_ (c-exp ""))
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
(%defconstant csym::_@SYS_TIME_H_ (c-exp ""))
(%defconstant csym::_@SYS__TIMEVAL_H_ (c-exp ""))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
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
(%defconstant csym::__@TIME__ "21:30:02")
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
(%defconstant csym::_@NET_IF_H_ (c-exp ""))
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
(%defconstant csym::_@SYS_TIME_H_ (c-exp ""))
(%defconstant csym::_@SYS__TIMEVAL_H_ (c-exp ""))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
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
(%defconstant csym::@IF_NAMESIZE 16)
(%defconstant csym::@IFNAMSIZ csym::@IF_NAMESIZE)
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
(%defconstant csym::@IFF_ALTPHYS csym::@IFF_LINK2)
(%defconstant csym::@IFF_MULTICAST 32768)
(%defconstant csym::@IFF_POLLING 65536)
(%defconstant csym::@IFF_PPROMISC 131072)
(%defconstant csym::@IFF_MONITOR 262144)
(%defconstant csym::@IFF_STATICARP 524288)
(%defconstant csym::@IFF_NEEDSGIANT 1048576)
(%defconstant csym::@IFF_CANTCHANGE
 (bit-or csym::@IFF_BROADCAST csym::@IFF_POINTOPOINT csym::@IFF_RUNNING
  csym::@IFF_OACTIVE csym::@IFF_SIMPLEX csym::@IFF_MULTICAST
  csym::@IFF_ALLMULTI csym::@IFF_SMART csym::@IFF_PROMISC csym::@IFF_POLLING))
(%defconstant csym::@LINK_STATE_UNKNOWN 0)
(%defconstant csym::@LINK_STATE_DOWN 1)
(%defconstant csym::@LINK_STATE_UP 2)
(%defmacro csym::@IF_K@bps (csym::_arg0)
 (common-lisp:list '* csym::_arg0 '1000))
(%defmacro csym::@IF_M@bps (csym::_arg0)
 (common-lisp:list 'csym::@if_k@BPS (common-lisp:list '* csym::_arg0 '1000)))
(%defmacro csym::@IF_G@bps (csym::_arg0)
 (common-lisp:list 'csym::@if_m@BPS (common-lisp:list '* csym::_arg0 '1000)))
(%defconstant csym::@IFCAP_RXCSUM 1)
(%defconstant csym::@IFCAP_TXCSUM 2)
(%defconstant csym::@IFCAP_NETCONS 4)
(%defconstant csym::@IFCAP_VLAN_MTU 8)
(%defconstant csym::@IFCAP_VLAN_HWTAGGING 16)
(%defconstant csym::@IFCAP_JUMBO_MTU 32)
(%defconstant csym::@IFCAP_POLLING 64)
(%defconstant csym::@IFCAP_HWCSUM
 (bit-or csym::@IFCAP_RXCSUM csym::@IFCAP_TXCSUM))
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
(%defmacro csym::_@SIZEOF_ADDR_IFREQ (csym::_arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '>
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'fref
                                                                         csym::_arg0
                                                                         'csym::ifr_addr)
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
                                                                         csym::_arg0
                                                                         'csym::ifr_addr)
                                                       'csym::sa_len))
                   (common-lisp:list 'sizeof
                                     (common-lisp:list 'struct 'csym::ifreq))))
(%defconstant csym::@IFSTATMAX 800)
(%defconstant csym::ifc_buf (fref csym::ifc_ifcu csym::ifcu_buf))
(%defconstant csym::ifc_req (fref csym::ifc_ifcu csym::ifcu_req))
(%defconstant csym::@IFLR_PREFIX 32768)
