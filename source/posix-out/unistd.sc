(%line 1 "csample/posix-header/unistd.h")
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
(deftype c2sc::$abstract csym::__pid_t)
(deftype csym::register_t csym::__register_t)
(deftype csym::rlim_t csym::__rlim_t)
(deftype csym::segsz_t csym::__segsz_t)
(deftype csym::size_t csym::__size_t)
(deftype csym::ssize_t csym::__ssize_t)
(deftype csym::suseconds_t csym::__suseconds_t)
(deftype c2sc::$abstract csym::__time_t)
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
(%line 1 "/usr/include/sys/unistd.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 318 "csample/posix-header/unistd.h")
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
(%line 353 "csample/posix-header/unistd.h")
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
(%line 398 "csample/posix-header/unistd.h")
(decl (csym::getlogin_r) (fn int (ptr char) int))
(decl (csym::fchown) (fn int int csym::uid_t csym::gid_t))
(decl (csym::readlink) (fn int (ptr (const char)) (ptr char) int))
(decl (csym::gethostname) (fn int (ptr char) csym::size_t))
(decl (csym::setegid) (fn int csym::gid_t))
(decl (csym::seteuid) (fn int csym::uid_t))
(%line 418 "csample/posix-header/unistd.h")
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
(%line 453 "csample/posix-header/unistd.h")
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
(decl (csym::mkstemp) (fn int (ptr char)))
(decl (csym::mkstemps) (fn int (ptr char) int))
(decl (csym::mktemp) (fn (ptr char) (ptr char)))
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
(%line 520 "csample/posix-header/unistd.h")
(decl (csym::setdomainname) (fn int (ptr (const char)) int))
(decl (csym::setgroups) (fn int int (ptr (const csym::gid_t))))
(decl (csym::sethostid) (fn void long))
(decl (csym::sethostname) (fn int (ptr (const char)) int))
(decl (csym::setkey) (fn int (ptr (const char))))
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
(%defconstant csym::__@TIME__ "15:59:12")
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
(%defconstant csym::_@UNISTD_H_ (c-exp ""))
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
(%defconstant csym::__@DATE__ "Feb 13 2005")
(%defconstant csym::__@TIME__ "15:59:12")
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
(%defconstant csym::_@UNISTD_H_ (c-exp ""))
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
(%defconstant csym::_@POSIX_VDISABLE 255)
(%defconstant csym::_@XOPEN_SHM 1)
(%defconstant csym::_@XOPEN_STREAMS (- 1))
(%defconstant csym::_@POSIX_VERSION (cast long 200112))
(%defconstant csym::@F_OK 0)
(%defconstant csym::@X_OK 1)
(%defconstant csym::@W_OK 2)
(%defconstant csym::@R_OK 4)
(%defconstant csym::@SEEK_SET 0)
(%defconstant csym::@SEEK_CUR 1)
(%defconstant csym::@SEEK_END 2)
(%defconstant csym::@L_SET csym::@SEEK_SET)
(%defconstant csym::@L_INCR csym::@SEEK_CUR)
(%defconstant csym::@L_XTND csym::@SEEK_END)
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
(%defconstant csym::@RFKERNELONLY
 (bit-or csym::@RFSTOPPED csym::@RFHIGHPID csym::@RFPPWAIT))
(%defconstant csym::@NULL (cast (ptr void) 0))
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
(%defconstant csym::_@SC_PAGE_SIZE csym::_@SC_PAGESIZE)
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
(%defconstant csym::_@MKSTEMP_DECLARED (c-exp ""))
(%defconstant csym::_@MKTEMP_DECLARED (c-exp ""))
(%defconstant csym::_@SETKEY_DECLARED (c-exp ""))
(%defconstant csym::_@OPTRESET_DECLARED (c-exp ""))
