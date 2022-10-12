#include "libc_mapping.h"

#include <string.h>
#include <libgen.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <asm/prctl.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <elf.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <getopt.h>
#include <glob.h>
#include <grp.h>
#include <iconv.h>
#include <ifaddrs.h>
#include <langinfo.h>
#include <libintl.h>
#include <math.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <pwd.h>
#include <regex.h>
#include <resolv.h>
#include <sched.h>
#include <setjmp.h>
#include <signal.h>
#include <spawn.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/capability.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/xattr.h>
#include <syslog.h>
#include <termios.h>
#include <ucontext.h>
#include <unistd.h>
#include <cinttypes>
#include <cstring>
#include <ctime>

namespace libc_mapping {
const char* (*strchr_c)(const char*, int) = strchr;
const char* (*strrchr_c)(const char*, int) = strrchr;
const void* (*rawmemchr_c)(const void*, int) = rawmemchr;
const void* (*memchr_c)(const void*, int, size_t) = memchr;
const char* (*strstr_c)(const char*, const char*) = strstr;
const char* (*strpbrk_c)(const char*, const char*) = strpbrk;
double (*frexp_c)(double, int*) = frexp;
double (*ldexp_c)(double, int) = ldexp;
const char* (*strchrnul_c)(const char*, int) = strchrnul;

void sloader_dummy_fun(){
    ;
}

void sloader_libc_start_main(int (*main)(int, char**, char**), int argc, char** argv, void (*init)(void), void (*fini)(void),
                             void (*rtld_fini)(void), void* stack_end) {
    // Although glibc write this function in assembly, we can use a plain C function?
    // https://github.com/akawashiro/glibc/blob/0005e54f762b2ec65cee2c4ecf1e9d42612030f0/sysdeps/x86_64/start.S#L64-L76
    // TODO
    // After SYS_arch_prctl, we cannot use glog.
    // LOG(INFO) << LOG_BITS(main);
    // We need __libc_stack_end
    exit(main(argc, argv, NULL));
}

std::map<std::string, Elf64_Addr> sloader_libc_map = {
    // {"__memcpy_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__memmove_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__memset_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__progname", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__progname_full", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__rawmemchr", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__register_atfork", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__res_context_hostalias", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__res_context_query", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__res_context_search", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__res_get_nsaddr", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__res_iclose", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__resolv_context_get", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__resolv_context_get_override", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__resolv_context_put", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__strcpy_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__strncat_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__strncpy_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__syslog_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__syslog_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__tls_get_addr", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__vasprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__vfprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__vsnprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__vsnprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"__vsnprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    // {"_setjmp", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"ZSTD_trace_compress_begin", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"ZSTD_trace_compress_end", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"ZSTD_trace_decompress_begin", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"ZSTD_trace_decompress_end", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"_ITM_deregisterTMCloneTable", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"_ITM_registerTMCloneTable", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__asprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__assert_fail", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__ctype_b_loc", reinterpret_cast<Elf64_Addr>(&__ctype_b_loc)},
    {"__ctype_get_mb_cur_max", reinterpret_cast<Elf64_Addr>(&__ctype_get_mb_cur_max)},
    {"__ctype_tolower_loc", reinterpret_cast<Elf64_Addr>(&__ctype_tolower_loc)},
    {"__ctype_toupper_loc", reinterpret_cast<Elf64_Addr>(&__ctype_toupper_loc)},
    {"__cxa_atexit", reinterpret_cast<Elf64_Addr>(&sloader_dummy_fun)},
    {"__cxa_finalize", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__errno_location", reinterpret_cast<Elf64_Addr>(&__errno_location)},
    {"__explicit_bzero_chk", reinterpret_cast<Elf64_Addr>(&__explicit_bzero_chk)},
    {"__fdelt_chk", reinterpret_cast<Elf64_Addr>(&__fdelt_chk)},
    {"__fpending", reinterpret_cast<Elf64_Addr>(&__fpending)},
    {"__fprintf_chk", reinterpret_cast<Elf64_Addr>(&__fprintf_chk)},
    {"__freading", reinterpret_cast<Elf64_Addr>(&__freading)},
    {"__gmon_start__", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__gmtime_r", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__isoc99_sscanf", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__libc_dn_expand", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__libc_ns_makecanon", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__libc_ns_samename", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__libc_res_nameinquery", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__libc_res_queriesmatch", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__libc_single_threaded", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__libc_start_main", reinterpret_cast<Elf64_Addr>(&sloader_libc_start_main)},
    {"__mbstowcs_chk", reinterpret_cast<Elf64_Addr>(&__mbstowcs_chk)},
    {"__overflow", reinterpret_cast<Elf64_Addr>(&__overflow)},
    {"__poll_chk", reinterpret_cast<Elf64_Addr>(&__poll_chk)},
    {"__printf_chk", reinterpret_cast<Elf64_Addr>(&__printf_chk)},
    {"__res_nclose", reinterpret_cast<Elf64_Addr>(&__res_nclose)},
    {"__res_ninit", reinterpret_cast<Elf64_Addr>(&__res_ninit)},
    {"__sched_cpucount", reinterpret_cast<Elf64_Addr>(&__sched_cpucount)},
    {"__snprintf_chk", reinterpret_cast<Elf64_Addr>(&__snprintf_chk)},
    {"__sprintf_chk", reinterpret_cast<Elf64_Addr>(&__sprintf_chk)},
    {"__stack_chk_fail", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__vfprintf_chk", reinterpret_cast<Elf64_Addr>(&__vfprintf_chk)},
    {"__xpg_basename", reinterpret_cast<Elf64_Addr>(&__xpg_basename)},
    {"__xpg_strerror_r", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__xstat", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"_exit", reinterpret_cast<Elf64_Addr>(&_exit)},
    {"_longjmp", reinterpret_cast<Elf64_Addr>(&_longjmp)},
    {"abort", reinterpret_cast<Elf64_Addr>(&abort)},
    {"accept", reinterpret_cast<Elf64_Addr>(&accept)},
    {"access", reinterpret_cast<Elf64_Addr>(&access)},
    {"access", reinterpret_cast<Elf64_Addr>(&access)},
    {"atexit", reinterpret_cast<Elf64_Addr>(&sloader_dummy_fun)},
    {"atoi", reinterpret_cast<Elf64_Addr>(&atoi)},
    {"atol", reinterpret_cast<Elf64_Addr>(&atol)},
    {"basename", reinterpret_cast<Elf64_Addr>(&basename)},
    {"bind", reinterpret_cast<Elf64_Addr>(&bind)},
    {"bindtextdomain", reinterpret_cast<Elf64_Addr>(&bindtextdomain)},
    {"calloc", reinterpret_cast<Elf64_Addr>(&calloc)},
    {"capget", reinterpret_cast<Elf64_Addr>(&capget)},
    {"capset", reinterpret_cast<Elf64_Addr>(&capset)},
    {"chdir", reinterpret_cast<Elf64_Addr>(&chdir)},
    {"chmod", reinterpret_cast<Elf64_Addr>(&chmod)},
    {"chmod", reinterpret_cast<Elf64_Addr>(&chmod)},
    {"chroot", reinterpret_cast<Elf64_Addr>(&chroot)},
    {"clearerr", reinterpret_cast<Elf64_Addr>(&clearerr)},
    {"clock", reinterpret_cast<Elf64_Addr>(&clock)},
    {"clock", reinterpret_cast<Elf64_Addr>(&clock)},
    {"clock_gettime", reinterpret_cast<Elf64_Addr>(&clock_gettime)},
    {"close", reinterpret_cast<Elf64_Addr>(&close)},
    {"closedir", reinterpret_cast<Elf64_Addr>(&closedir)},
    {"closelog", reinterpret_cast<Elf64_Addr>(&closelog)},
    {"connect", reinterpret_cast<Elf64_Addr>(&connect)},
    {"connect", reinterpret_cast<Elf64_Addr>(&connect)},
    {"connect", reinterpret_cast<Elf64_Addr>(&connect)},
    {"connect", reinterpret_cast<Elf64_Addr>(&connect)},
    {"connect", reinterpret_cast<Elf64_Addr>(&connect)},
    {"connect", reinterpret_cast<Elf64_Addr>(&connect)},
    {"ctime_r", reinterpret_cast<Elf64_Addr>(&ctime_r)},
    {"dcgettext", reinterpret_cast<Elf64_Addr>(&dcgettext)},
    {"dcngettext", reinterpret_cast<Elf64_Addr>(&dcngettext)},
    {"difftime", reinterpret_cast<Elf64_Addr>(&difftime)},
    {"dirfd", reinterpret_cast<Elf64_Addr>(&dirfd)},
    {"dirname", reinterpret_cast<Elf64_Addr>(&dirname)},
    {"dladdr", reinterpret_cast<Elf64_Addr>(&dladdr)},
    {"dlclose", reinterpret_cast<Elf64_Addr>(&dlclose)},
    {"dlclose", reinterpret_cast<Elf64_Addr>(&dlclose)},
    {"dlclose", reinterpret_cast<Elf64_Addr>(&dlclose)},
    {"dlerror", reinterpret_cast<Elf64_Addr>(&dlerror)},
    {"dlerror", reinterpret_cast<Elf64_Addr>(&dlerror)},
    {"dlerror", reinterpret_cast<Elf64_Addr>(&dlerror)},
    {"dlopen", reinterpret_cast<Elf64_Addr>(&dlopen)},
    {"dlopen", reinterpret_cast<Elf64_Addr>(&dlopen)},
    {"dlopen", reinterpret_cast<Elf64_Addr>(&dlopen)},
    {"dlsym", reinterpret_cast<Elf64_Addr>(&dlsym)},
    {"dlsym", reinterpret_cast<Elf64_Addr>(&dlsym)},
    {"dlsym", reinterpret_cast<Elf64_Addr>(&dlsym)},
    {"dn_expand", reinterpret_cast<Elf64_Addr>(&dn_expand)},
    {"dn_skipname", reinterpret_cast<Elf64_Addr>(&dn_skipname)},
    {"dup", reinterpret_cast<Elf64_Addr>(&dup)},
    {"dup2", reinterpret_cast<Elf64_Addr>(&dup2)},
    {"error", reinterpret_cast<Elf64_Addr>(&error)},
    {"execl", reinterpret_cast<Elf64_Addr>(&execl)},
    {"execve", reinterpret_cast<Elf64_Addr>(&execve)},
    {"execvp", reinterpret_cast<Elf64_Addr>(&execvp)},
    {"exit", reinterpret_cast<Elf64_Addr>(&exit)},
    {"fchmod", reinterpret_cast<Elf64_Addr>(&fchmod)},
    {"fclose", reinterpret_cast<Elf64_Addr>(&fclose)},
    {"fcntl", reinterpret_cast<Elf64_Addr>(&fcntl)},
    {"fcntl64", reinterpret_cast<Elf64_Addr>(&fcntl64)},
    {"fdopen", reinterpret_cast<Elf64_Addr>(&fdopen)},
    {"feof", reinterpret_cast<Elf64_Addr>(&feof)},
    {"ferror", reinterpret_cast<Elf64_Addr>(&ferror)},
    {"fflush", reinterpret_cast<Elf64_Addr>(&fflush)},
    {"fflush_unlocked", reinterpret_cast<Elf64_Addr>(&fflush_unlocked)},
    {"fgets", reinterpret_cast<Elf64_Addr>(&fgets)},
    {"fgetxattr", reinterpret_cast<Elf64_Addr>(&fgetxattr)},
    {"fileno", reinterpret_cast<Elf64_Addr>(&fileno)},
    {"flock", reinterpret_cast<Elf64_Addr>(&flock)},
    {"fmemopen", reinterpret_cast<Elf64_Addr>(&fmemopen)},
    {"fnmatch", reinterpret_cast<Elf64_Addr>(&fnmatch)},
    {"fopen", reinterpret_cast<Elf64_Addr>(&fopen)},
    {"fopen64", reinterpret_cast<Elf64_Addr>(&fopen64)},
    {"fork", reinterpret_cast<Elf64_Addr>(&fork)},
    {"fprintf", reinterpret_cast<Elf64_Addr>(&fprintf)},
    {"fputc", reinterpret_cast<Elf64_Addr>(&fputc)},
    {"fputc_unlocked", reinterpret_cast<Elf64_Addr>(&fputc_unlocked)},
    {"fputs", reinterpret_cast<Elf64_Addr>(&fputs)},
    {"fputs_unlocked", reinterpret_cast<Elf64_Addr>(&fputs_unlocked)},
    {"fread", reinterpret_cast<Elf64_Addr>(&fread)},
    {"free", reinterpret_cast<Elf64_Addr>(&free)},
    {"freeaddrinfo", reinterpret_cast<Elf64_Addr>(&freeaddrinfo)},
    {"freeaddrinfo", reinterpret_cast<Elf64_Addr>(&freeaddrinfo)},
    {"freeaddrinfo", reinterpret_cast<Elf64_Addr>(&freeaddrinfo)},
    {"freeaddrinfo", reinterpret_cast<Elf64_Addr>(&freeaddrinfo)},
    {"freeaddrinfo", reinterpret_cast<Elf64_Addr>(&freeaddrinfo)},
    {"freeifaddrs", reinterpret_cast<Elf64_Addr>(&freeifaddrs)},
    {"fremovexattr", reinterpret_cast<Elf64_Addr>(&fremovexattr)},
    {"frexp", reinterpret_cast<Elf64_Addr>(frexp_c)},
    {"frexpl", reinterpret_cast<Elf64_Addr>(&frexpl)},
    {"fseek", reinterpret_cast<Elf64_Addr>(&fseek)},
    {"fseeko", reinterpret_cast<Elf64_Addr>(&fseeko)},
    {"fsetxattr", reinterpret_cast<Elf64_Addr>(&fsetxattr)},
    {"fstat", reinterpret_cast<Elf64_Addr>(&fstat)},
    {"fstat64", reinterpret_cast<Elf64_Addr>(&fstat64)},
    {"fsync", reinterpret_cast<Elf64_Addr>(&fsync)},
    {"ftell", reinterpret_cast<Elf64_Addr>(&ftell)},
    {"ftruncate64", reinterpret_cast<Elf64_Addr>(&ftruncate64)},
    {"fwrite", reinterpret_cast<Elf64_Addr>(&fwrite)},
    {"fwrite_unlocked", reinterpret_cast<Elf64_Addr>(&fwrite_unlocked)},
    {"gai_strerror", reinterpret_cast<Elf64_Addr>(&gai_strerror)},
    {"gai_strerror", reinterpret_cast<Elf64_Addr>(&gai_strerror)},
    {"gai_strerror", reinterpret_cast<Elf64_Addr>(&gai_strerror)},
    {"getaddrinfo", reinterpret_cast<Elf64_Addr>(&getaddrinfo)},
    {"getaddrinfo", reinterpret_cast<Elf64_Addr>(&getaddrinfo)},
    {"getaddrinfo", reinterpret_cast<Elf64_Addr>(&getaddrinfo)},
    {"getaddrinfo", reinterpret_cast<Elf64_Addr>(&getaddrinfo)},
    {"getaddrinfo", reinterpret_cast<Elf64_Addr>(&getaddrinfo)},
    {"getauxval", reinterpret_cast<Elf64_Addr>(&getauxval)},
    {"getc", reinterpret_cast<Elf64_Addr>(&getc)},
    {"getcontext", reinterpret_cast<Elf64_Addr>(&getcontext)},
    {"getcwd", reinterpret_cast<Elf64_Addr>(&getcwd)},
    {"getegid", reinterpret_cast<Elf64_Addr>(&getegid)},
    {"getegid", reinterpret_cast<Elf64_Addr>(&getegid)},
    {"getegid", reinterpret_cast<Elf64_Addr>(&getegid)},
    {"getentropy", reinterpret_cast<Elf64_Addr>(&getentropy)},
    {"getenv", reinterpret_cast<Elf64_Addr>(&getenv)},
    {"geteuid", reinterpret_cast<Elf64_Addr>(&geteuid)},
    {"getgid", reinterpret_cast<Elf64_Addr>(&getgid)},
    {"getgid", reinterpret_cast<Elf64_Addr>(&getgid)},
    {"getgrgid", reinterpret_cast<Elf64_Addr>(&getgrgid)},
    {"gethostbyaddr_r", reinterpret_cast<Elf64_Addr>(&gethostbyaddr_r)},
    {"gethostbyname", reinterpret_cast<Elf64_Addr>(&gethostbyname)},
    {"gethostbyname_r", reinterpret_cast<Elf64_Addr>(&gethostbyname_r)},
    {"gethostname", reinterpret_cast<Elf64_Addr>(&gethostname)},
    {"getifaddrs", reinterpret_cast<Elf64_Addr>(&getifaddrs)},
    {"getifaddrs", reinterpret_cast<Elf64_Addr>(&getifaddrs)},
    {"getloadavg", reinterpret_cast<Elf64_Addr>(&getloadavg)},
    {"getnameinfo", reinterpret_cast<Elf64_Addr>(&getnameinfo)},
    {"getnameinfo", reinterpret_cast<Elf64_Addr>(&getnameinfo)},
    {"getnameinfo", reinterpret_cast<Elf64_Addr>(&getnameinfo)},
    {"getnameinfo", reinterpret_cast<Elf64_Addr>(&getnameinfo)},
    {"getopt", reinterpret_cast<Elf64_Addr>(&getopt)},
    {"getopt_long", reinterpret_cast<Elf64_Addr>(&getopt_long)},
    {"getpeername", reinterpret_cast<Elf64_Addr>(&getpeername)},
    {"getpid", reinterpret_cast<Elf64_Addr>(&getpid)},
    {"getpwnam_r", reinterpret_cast<Elf64_Addr>(&getpwnam_r)},
    {"getpwnam_r", reinterpret_cast<Elf64_Addr>(&getpwnam_r)},
    {"getpwuid", reinterpret_cast<Elf64_Addr>(&getpwuid)},
    {"getpwuid_r", reinterpret_cast<Elf64_Addr>(&getpwuid_r)},
    {"getpwuid_r", reinterpret_cast<Elf64_Addr>(&getpwuid_r)},
    {"getpwuid_r", reinterpret_cast<Elf64_Addr>(&getpwuid_r)},
    {"getservbyname", reinterpret_cast<Elf64_Addr>(&getservbyname)},
    {"getsockname", reinterpret_cast<Elf64_Addr>(&getsockname)},
    {"getsockname", reinterpret_cast<Elf64_Addr>(&getsockname)},
    {"getsockname", reinterpret_cast<Elf64_Addr>(&getsockname)},
    {"getsockname", reinterpret_cast<Elf64_Addr>(&getsockname)},
    {"getsockopt", reinterpret_cast<Elf64_Addr>(&getsockopt)},
    {"getsockopt", reinterpret_cast<Elf64_Addr>(&getsockopt)},
    {"getsockopt", reinterpret_cast<Elf64_Addr>(&getsockopt)},
    {"gettimeofday", reinterpret_cast<Elf64_Addr>(&gettimeofday)},
    {"getuid", reinterpret_cast<Elf64_Addr>(&getuid)},
    {"getuid", reinterpret_cast<Elf64_Addr>(&getuid)},
    {"getuid", reinterpret_cast<Elf64_Addr>(&getuid)},
    {"getuid", reinterpret_cast<Elf64_Addr>(&getuid)},
    {"getxattr", reinterpret_cast<Elf64_Addr>(&getxattr)},
    {"glob", reinterpret_cast<Elf64_Addr>(&glob)},
    {"globfree", reinterpret_cast<Elf64_Addr>(&globfree)},
    {"gmtime_r", reinterpret_cast<Elf64_Addr>(&gmtime_r)},
    {"iconv", reinterpret_cast<Elf64_Addr>(&iconv)},
    {"iconv_close", reinterpret_cast<Elf64_Addr>(&iconv_close)},
    {"iconv_open", reinterpret_cast<Elf64_Addr>(&iconv_open)},
    {"if_nametoindex", reinterpret_cast<Elf64_Addr>(&if_nametoindex)},
    {"in6addr_any", reinterpret_cast<Elf64_Addr>(&in6addr_any)},
    {"in6addr_any", reinterpret_cast<Elf64_Addr>(&in6addr_any)},
    {"inet_aton", reinterpret_cast<Elf64_Addr>(&inet_aton)},
    {"inet_nsap_ntoa", reinterpret_cast<Elf64_Addr>(&inet_nsap_ntoa)},
    {"inet_ntop", reinterpret_cast<Elf64_Addr>(&inet_ntop)},
    {"inet_ntop", reinterpret_cast<Elf64_Addr>(&inet_ntop)},
    {"inet_ntop", reinterpret_cast<Elf64_Addr>(&inet_ntop)},
    {"inet_pton", reinterpret_cast<Elf64_Addr>(&inet_pton)},
    {"inet_pton", reinterpret_cast<Elf64_Addr>(&inet_pton)},
    {"inet_pton", reinterpret_cast<Elf64_Addr>(&inet_pton)},
    {"inet_pton", reinterpret_cast<Elf64_Addr>(&inet_pton)},
    {"ioctl", reinterpret_cast<Elf64_Addr>(&ioctl)},
    {"isalnum", reinterpret_cast<Elf64_Addr>(&isalnum)},
    {"isatty", reinterpret_cast<Elf64_Addr>(&isatty)},
    {"isdigit", reinterpret_cast<Elf64_Addr>(&isdigit)},
    {"iswcntrl", reinterpret_cast<Elf64_Addr>(&iscntrl)},
    {"iswprint", reinterpret_cast<Elf64_Addr>(&isprint)},
    {"isxdigit", reinterpret_cast<Elf64_Addr>(&isxdigit)},
    {"jrand48", reinterpret_cast<Elf64_Addr>(&jrand48)},
    {"kill", reinterpret_cast<Elf64_Addr>(&kill)},
    {"ldexp", reinterpret_cast<Elf64_Addr>(ldexp_c)},
    {"ldexpl", reinterpret_cast<Elf64_Addr>(&ldexpl)},
    {"link", reinterpret_cast<Elf64_Addr>(&link)},
    {"listen", reinterpret_cast<Elf64_Addr>(&listen)},
    {"listen", reinterpret_cast<Elf64_Addr>(&listen)},
    {"localeconv", reinterpret_cast<Elf64_Addr>(&localeconv)},
    {"localtime", reinterpret_cast<Elf64_Addr>(&localtime)},
    {"localtime_r", reinterpret_cast<Elf64_Addr>(&localtime_r)},
    {"lseek", reinterpret_cast<Elf64_Addr>(&lseek)},
    {"lseek64", reinterpret_cast<Elf64_Addr>(&lseek64)},
    {"lstat", reinterpret_cast<Elf64_Addr>(&lstat)},
    {"lstat64", reinterpret_cast<Elf64_Addr>(&lstat64)},
    {"madvise", reinterpret_cast<Elf64_Addr>(&madvise)},
    {"makecontext", reinterpret_cast<Elf64_Addr>(&makecontext)},
    {"malloc", reinterpret_cast<Elf64_Addr>(&malloc)},
    {"mbrtowc", reinterpret_cast<Elf64_Addr>(&mbrtowc)},
    {"mbsinit", reinterpret_cast<Elf64_Addr>(&mbsinit)},
    {"mbstowcs", reinterpret_cast<Elf64_Addr>(&mbstowcs)},
    {"mbtowc", reinterpret_cast<Elf64_Addr>(&mbtowc)},
    {"memchr", reinterpret_cast<Elf64_Addr>(memchr_c)},
    {"memcmp", reinterpret_cast<Elf64_Addr>(&memcmp)},
    {"memcpy", reinterpret_cast<Elf64_Addr>(&memcpy)},
    {"memmove", reinterpret_cast<Elf64_Addr>(&memmove)},
    {"mempcpy", reinterpret_cast<Elf64_Addr>(&mempcpy)},
    {"memset", reinterpret_cast<Elf64_Addr>(&memset)},
    {"mkdir", reinterpret_cast<Elf64_Addr>(&mkdir)},
    {"mkstemp", reinterpret_cast<Elf64_Addr>(&mkstemp)},
    {"mktime", reinterpret_cast<Elf64_Addr>(&mktime)},
    {"mlock", reinterpret_cast<Elf64_Addr>(&mlock)},
    {"mmap", reinterpret_cast<Elf64_Addr>(&mmap)},
    {"mprotect", reinterpret_cast<Elf64_Addr>(&mprotect)},
    {"munmap", reinterpret_cast<Elf64_Addr>(&munmap)},
    {"nl_langinfo", reinterpret_cast<Elf64_Addr>(&nl_langinfo)},
    {"ns_name_uncompress", reinterpret_cast<Elf64_Addr>(&ns_name_uncompress)},
    {"open", reinterpret_cast<Elf64_Addr>(&open)},
    {"open64", reinterpret_cast<Elf64_Addr>(&open64)},
    {"open_memstream", reinterpret_cast<Elf64_Addr>(&open_memstream)},
    {"opendir", reinterpret_cast<Elf64_Addr>(&opendir)},
    {"openlog", reinterpret_cast<Elf64_Addr>(&openlog)},
    {"optarg", reinterpret_cast<Elf64_Addr>(&optarg)},
    {"optind", reinterpret_cast<Elf64_Addr>(&optind)},
    {"perror", reinterpret_cast<Elf64_Addr>(&perror)},
    {"pipe", reinterpret_cast<Elf64_Addr>(&pipe)},
    {"pipe2", reinterpret_cast<Elf64_Addr>(&pipe2)},
    {"poll", reinterpret_cast<Elf64_Addr>(&poll)},
    {"posix_spawn", reinterpret_cast<Elf64_Addr>(&posix_spawn)},
    {"posix_spawn_file_actions_addclose", reinterpret_cast<Elf64_Addr>(&posix_spawn_file_actions_addclose)},
    {"posix_spawn_file_actions_adddup2", reinterpret_cast<Elf64_Addr>(&posix_spawn_file_actions_adddup2)},
    {"posix_spawn_file_actions_addopen", reinterpret_cast<Elf64_Addr>(&posix_spawn_file_actions_addopen)},
    {"posix_spawn_file_actions_destroy", reinterpret_cast<Elf64_Addr>(&posix_spawn_file_actions_destroy)},
    {"posix_spawn_file_actions_init", reinterpret_cast<Elf64_Addr>(&posix_spawn_file_actions_init)},
    {"posix_spawnattr_destroy", reinterpret_cast<Elf64_Addr>(&posix_spawnattr_destroy)},
    {"posix_spawnattr_init", reinterpret_cast<Elf64_Addr>(&posix_spawnattr_init)},
    {"posix_spawnattr_setflags", reinterpret_cast<Elf64_Addr>(&posix_spawnattr_setflags)},
    {"posix_spawnattr_setsigmask", reinterpret_cast<Elf64_Addr>(&posix_spawnattr_setsigmask)},
    {"prctl", reinterpret_cast<Elf64_Addr>(&prctl)},
    {"printf", reinterpret_cast<Elf64_Addr>(&printf)},
    {"program_invocation_name", reinterpret_cast<Elf64_Addr>(&program_invocation_name)},
    {"program_invocation_short_name", reinterpret_cast<Elf64_Addr>(&program_invocation_short_name)},
    {"pselect", reinterpret_cast<Elf64_Addr>(&pselect)},
    {"pthread_attr_destroy", reinterpret_cast<Elf64_Addr>(&pthread_attr_destroy)},
    {"pthread_attr_init", reinterpret_cast<Elf64_Addr>(&pthread_attr_init)},
    {"pthread_attr_setdetachstate", reinterpret_cast<Elf64_Addr>(&pthread_attr_setdetachstate)},
    {"pthread_attr_setguardsize", reinterpret_cast<Elf64_Addr>(&pthread_attr_setguardsize)},
    {"pthread_attr_setstacksize", reinterpret_cast<Elf64_Addr>(&pthread_attr_setstacksize)},
    {"pthread_cond_broadcast", reinterpret_cast<Elf64_Addr>(&pthread_cond_broadcast)},
    {"pthread_cond_broadcast", reinterpret_cast<Elf64_Addr>(&pthread_cond_broadcast)},
    {"pthread_cond_destroy", reinterpret_cast<Elf64_Addr>(&pthread_cond_destroy)},
    {"pthread_cond_destroy", reinterpret_cast<Elf64_Addr>(&pthread_cond_destroy)},
    {"pthread_cond_init", reinterpret_cast<Elf64_Addr>(&pthread_cond_init)},
    {"pthread_cond_init", reinterpret_cast<Elf64_Addr>(&pthread_cond_init)},
    {"pthread_cond_signal", reinterpret_cast<Elf64_Addr>(&pthread_cond_signal)},
    {"pthread_cond_signal", reinterpret_cast<Elf64_Addr>(&pthread_cond_signal)},
    {"pthread_cond_wait", reinterpret_cast<Elf64_Addr>(&pthread_cond_wait)},
    {"pthread_cond_wait", reinterpret_cast<Elf64_Addr>(&pthread_cond_wait)},
    {"pthread_create", reinterpret_cast<Elf64_Addr>(&pthread_create)},
    {"pthread_create", reinterpret_cast<Elf64_Addr>(&pthread_create)},
    {"pthread_create", reinterpret_cast<Elf64_Addr>(&pthread_create)},
    {"pthread_create", reinterpret_cast<Elf64_Addr>(&pthread_create)},
    {"pthread_detach", reinterpret_cast<Elf64_Addr>(&pthread_detach)},
    {"pthread_equal", reinterpret_cast<Elf64_Addr>(&pthread_equal)},
    {"pthread_exit", reinterpret_cast<Elf64_Addr>(&pthread_exit)},
    {"pthread_getconcurrency", reinterpret_cast<Elf64_Addr>(&pthread_getconcurrency)},
    {"pthread_getspecific", reinterpret_cast<Elf64_Addr>(&pthread_getspecific)},
    {"pthread_getspecific", reinterpret_cast<Elf64_Addr>(&pthread_getspecific)},
    {"pthread_getspecific", reinterpret_cast<Elf64_Addr>(&pthread_getspecific)},
    {"pthread_join", reinterpret_cast<Elf64_Addr>(&pthread_join)},
    {"pthread_join", reinterpret_cast<Elf64_Addr>(&pthread_join)},
    {"pthread_join", reinterpret_cast<Elf64_Addr>(&pthread_join)},
    {"pthread_join", reinterpret_cast<Elf64_Addr>(&pthread_join)},
    {"pthread_key_create", reinterpret_cast<Elf64_Addr>(&pthread_key_create)},
    {"pthread_key_create", reinterpret_cast<Elf64_Addr>(&pthread_key_create)},
    {"pthread_key_create", reinterpret_cast<Elf64_Addr>(&pthread_key_create)},
    {"pthread_key_delete", reinterpret_cast<Elf64_Addr>(&pthread_key_delete)},
    {"pthread_key_delete", reinterpret_cast<Elf64_Addr>(&pthread_key_delete)},
    {"pthread_key_delete", reinterpret_cast<Elf64_Addr>(&pthread_key_delete)},
    {"pthread_kill", reinterpret_cast<Elf64_Addr>(&pthread_kill)},
    {"pthread_mutex_destroy", reinterpret_cast<Elf64_Addr>(&pthread_mutex_destroy)},
    {"pthread_mutex_destroy", reinterpret_cast<Elf64_Addr>(&pthread_mutex_destroy)},
    {"pthread_mutex_destroy", reinterpret_cast<Elf64_Addr>(&pthread_mutex_destroy)},
    {"pthread_mutex_destroy", reinterpret_cast<Elf64_Addr>(&pthread_mutex_destroy)},
    {"pthread_mutex_init", reinterpret_cast<Elf64_Addr>(&pthread_mutex_init)},
    {"pthread_mutex_init", reinterpret_cast<Elf64_Addr>(&pthread_mutex_init)},
    {"pthread_mutex_init", reinterpret_cast<Elf64_Addr>(&pthread_mutex_init)},
    {"pthread_mutex_init", reinterpret_cast<Elf64_Addr>(&pthread_mutex_init)},
    {"pthread_mutex_init", reinterpret_cast<Elf64_Addr>(&pthread_mutex_init)},
    {"pthread_mutex_lock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_lock)},
    {"pthread_mutex_lock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_lock)},
    {"pthread_mutex_lock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_lock)},
    {"pthread_mutex_lock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_lock)},
    {"pthread_mutex_lock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_lock)},
    {"pthread_mutex_trylock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_trylock)},
    {"pthread_mutex_unlock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_unlock)},
    {"pthread_mutex_unlock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_unlock)},
    {"pthread_mutex_unlock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_unlock)},
    {"pthread_mutex_unlock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_unlock)},
    {"pthread_mutex_unlock", reinterpret_cast<Elf64_Addr>(&pthread_mutex_unlock)},
    {"pthread_mutexattr_destroy", reinterpret_cast<Elf64_Addr>(&pthread_mutexattr_destroy)},
    {"pthread_mutexattr_destroy", reinterpret_cast<Elf64_Addr>(&pthread_mutexattr_destroy)},
    {"pthread_mutexattr_init", reinterpret_cast<Elf64_Addr>(&pthread_mutexattr_init)},
    {"pthread_mutexattr_init", reinterpret_cast<Elf64_Addr>(&pthread_mutexattr_init)},
    {"pthread_mutexattr_settype", reinterpret_cast<Elf64_Addr>(&pthread_mutexattr_settype)},
    {"pthread_mutexattr_settype", reinterpret_cast<Elf64_Addr>(&pthread_mutexattr_settype)},
    {"pthread_once", reinterpret_cast<Elf64_Addr>(&pthread_once)},
    {"pthread_once", reinterpret_cast<Elf64_Addr>(&pthread_once)},
    {"pthread_once", reinterpret_cast<Elf64_Addr>(&pthread_once)},
    {"pthread_rwlock_destroy", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_destroy)},
    {"pthread_rwlock_destroy", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_destroy)},
    {"pthread_rwlock_init", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_init)},
    {"pthread_rwlock_init", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_init)},
    {"pthread_rwlock_init", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_init)},
    {"pthread_rwlock_rdlock", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_rdlock)},
    {"pthread_rwlock_rdlock", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_rdlock)},
    {"pthread_rwlock_tryrdlock", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_tryrdlock)},
    {"pthread_rwlock_trywrlock", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_trywrlock)},
    {"pthread_rwlock_unlock", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_unlock)},
    {"pthread_rwlock_unlock", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_unlock)},
    {"pthread_rwlock_wrlock", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_wrlock)},
    {"pthread_rwlock_wrlock", reinterpret_cast<Elf64_Addr>(&pthread_rwlock_wrlock)},
    {"pthread_rwlockattr_destroy", reinterpret_cast<Elf64_Addr>(&pthread_rwlockattr_destroy)},
    {"pthread_rwlockattr_init", reinterpret_cast<Elf64_Addr>(&pthread_rwlockattr_init)},
    {"pthread_rwlockattr_setkind_np", reinterpret_cast<Elf64_Addr>(&pthread_rwlockattr_setkind_np)},
    {"pthread_self", reinterpret_cast<Elf64_Addr>(&pthread_self)},
    {"pthread_self", reinterpret_cast<Elf64_Addr>(&pthread_self)},
    {"pthread_self", reinterpret_cast<Elf64_Addr>(&pthread_self)},
    {"pthread_setconcurrency", reinterpret_cast<Elf64_Addr>(&pthread_setconcurrency)},
    {"pthread_setspecific", reinterpret_cast<Elf64_Addr>(&pthread_setspecific)},
    {"pthread_setspecific", reinterpret_cast<Elf64_Addr>(&pthread_setspecific)},
    {"pthread_setspecific", reinterpret_cast<Elf64_Addr>(&pthread_setspecific)},
    {"putc", reinterpret_cast<Elf64_Addr>(&putc)},
    {"putc", reinterpret_cast<Elf64_Addr>(&putc)},
    {"putchar", reinterpret_cast<Elf64_Addr>(&putchar)},
    {"puts", reinterpret_cast<Elf64_Addr>(&puts)},
    {"qsort", reinterpret_cast<Elf64_Addr>(&qsort)},
    {"raise", reinterpret_cast<Elf64_Addr>(&raise)},
    {"rawmemchr", reinterpret_cast<Elf64_Addr>(rawmemchr_c)},
    {"read", reinterpret_cast<Elf64_Addr>(&read)},
    {"readdir", reinterpret_cast<Elf64_Addr>(&readdir)},
    {"readlink", reinterpret_cast<Elf64_Addr>(&readlink)},
    {"realloc", reinterpret_cast<Elf64_Addr>(&realloc)},
    {"reallocarray", reinterpret_cast<Elf64_Addr>(&reallocarray)},
    {"recv", reinterpret_cast<Elf64_Addr>(&recv)},
    {"recvfrom", reinterpret_cast<Elf64_Addr>(&recvfrom)},
    {"recvfrom", reinterpret_cast<Elf64_Addr>(&recvfrom)},
    {"regcomp", reinterpret_cast<Elf64_Addr>(&regcomp)},
    {"regexec", reinterpret_cast<Elf64_Addr>(&regexec)},
    {"regfree", reinterpret_cast<Elf64_Addr>(&regfree)},
    {"remove", reinterpret_cast<Elf64_Addr>(&remove)},
    {"removexattr", reinterpret_cast<Elf64_Addr>(&removexattr)},
    {"rename", reinterpret_cast<Elf64_Addr>(&rename)},
    {"res_dnok", reinterpret_cast<Elf64_Addr>(&res_dnok)},
    {"res_hnok", reinterpret_cast<Elf64_Addr>(&res_hnok)},
    {"res_nsearch", reinterpret_cast<Elf64_Addr>(&res_nsearch)},
    {"res_query", reinterpret_cast<Elf64_Addr>(&res_query)},
    {"rewind", reinterpret_cast<Elf64_Addr>(&rewind)},
    {"rewind", reinterpret_cast<Elf64_Addr>(&rewind)},
    {"sched_getaffinity", reinterpret_cast<Elf64_Addr>(&sched_getaffinity)},
    {"sched_yield", reinterpret_cast<Elf64_Addr>(&sched_yield)},
    {"secure_getenv", reinterpret_cast<Elf64_Addr>(&secure_getenv)},
    {"secure_getenv", reinterpret_cast<Elf64_Addr>(&secure_getenv)},
    {"secure_getenv", reinterpret_cast<Elf64_Addr>(&secure_getenv)},
    {"secure_getenv", reinterpret_cast<Elf64_Addr>(&secure_getenv)},
    {"select", reinterpret_cast<Elf64_Addr>(&select)},
    {"select", reinterpret_cast<Elf64_Addr>(&select)},
    {"send", reinterpret_cast<Elf64_Addr>(&send)},
    {"sendmsg", reinterpret_cast<Elf64_Addr>(&sendmsg)},
    {"sendto", reinterpret_cast<Elf64_Addr>(&sendto)},
    {"setbuf", reinterpret_cast<Elf64_Addr>(&setbuf)},
    {"setbuf", reinterpret_cast<Elf64_Addr>(&setbuf)},
    {"setcontext", reinterpret_cast<Elf64_Addr>(&setcontext)},
    {"setenv", reinterpret_cast<Elf64_Addr>(&setenv)},
    {"setgid", reinterpret_cast<Elf64_Addr>(&setgid)},
    {"setgroups", reinterpret_cast<Elf64_Addr>(&setgroups)},
    {"setlocale", reinterpret_cast<Elf64_Addr>(&setlocale)},
    {"setsockopt", reinterpret_cast<Elf64_Addr>(&setsockopt)},
    {"setsockopt", reinterpret_cast<Elf64_Addr>(&setsockopt)},
    {"setsockopt", reinterpret_cast<Elf64_Addr>(&setsockopt)},
    {"setsockopt", reinterpret_cast<Elf64_Addr>(&setsockopt)},
    {"setuid", reinterpret_cast<Elf64_Addr>(&setuid)},
    {"setvbuf", reinterpret_cast<Elf64_Addr>(&setvbuf)},
    {"setxattr", reinterpret_cast<Elf64_Addr>(&setxattr)},
    {"shmat", reinterpret_cast<Elf64_Addr>(&shmat)},
    {"shmdt", reinterpret_cast<Elf64_Addr>(&shmdt)},
    {"shmget", reinterpret_cast<Elf64_Addr>(&shmget)},
    {"shutdown", reinterpret_cast<Elf64_Addr>(&shutdown)},
    {"shutdown", reinterpret_cast<Elf64_Addr>(&shutdown)},
    {"shutdown", reinterpret_cast<Elf64_Addr>(&shutdown)},
    {"sigaction", reinterpret_cast<Elf64_Addr>(&sigaction)},
    {"sigaddset", reinterpret_cast<Elf64_Addr>(&sigaddset)},
    {"sigemptyset", reinterpret_cast<Elf64_Addr>(&sigemptyset)},
    {"sigismember", reinterpret_cast<Elf64_Addr>(&sigismember)},
    {"signal", reinterpret_cast<Elf64_Addr>(&signal)},
    {"sigpending", reinterpret_cast<Elf64_Addr>(&sigpending)},
    {"sigprocmask", reinterpret_cast<Elf64_Addr>(&sigprocmask)},
    {"sleep", reinterpret_cast<Elf64_Addr>(&sleep)},
    {"snprintf", reinterpret_cast<Elf64_Addr>(&snprintf)},
    {"socket", reinterpret_cast<Elf64_Addr>(&socket)},
    {"socketpair", reinterpret_cast<Elf64_Addr>(&socketpair)},
    {"sprintf", reinterpret_cast<Elf64_Addr>(&sprintf)},
    {"sprintf", reinterpret_cast<Elf64_Addr>(&sprintf)},
    {"stat", reinterpret_cast<Elf64_Addr>(&stat)},
    {"stat64", reinterpret_cast<Elf64_Addr>(&stat64)},
    {"statx", reinterpret_cast<Elf64_Addr>(&statx)},
    {"stderr", reinterpret_cast<Elf64_Addr>(&stderr)},
    {"stdin", reinterpret_cast<Elf64_Addr>(&stdin)},
    {"stdout", reinterpret_cast<Elf64_Addr>(&stdout)},
    {"stpcpy", reinterpret_cast<Elf64_Addr>(&stpcpy)},
    {"stpncpy", reinterpret_cast<Elf64_Addr>(&stpncpy)},
    {"strcasecmp", reinterpret_cast<Elf64_Addr>(&strcasecmp)},
    {"strcat", reinterpret_cast<Elf64_Addr>(&strcat)},
    {"strcat", reinterpret_cast<Elf64_Addr>(&strcat)},
    {"strcat", reinterpret_cast<Elf64_Addr>(&strcat)},
    {"strcat", reinterpret_cast<Elf64_Addr>(&strcat)},
    {"strchr", reinterpret_cast<Elf64_Addr>(strchr_c)},
    {"strchrnul", reinterpret_cast<Elf64_Addr>(strchrnul_c)},
    {"strcmp", reinterpret_cast<Elf64_Addr>(&strcmp)},
    {"strcoll", reinterpret_cast<Elf64_Addr>(&strcoll)},
    {"strcpy", reinterpret_cast<Elf64_Addr>(&strcpy)},
    {"strcspn", reinterpret_cast<Elf64_Addr>(&strcspn)},
    {"strdup", reinterpret_cast<Elf64_Addr>(&strdup)},
    {"strdup", reinterpret_cast<Elf64_Addr>(&strdup)},
    {"strerror", reinterpret_cast<Elf64_Addr>(&strerror)},
    {"strerror_r", reinterpret_cast<Elf64_Addr>(&strerror_r)},
    {"strftime", reinterpret_cast<Elf64_Addr>(&strftime)},
    {"strlen", reinterpret_cast<Elf64_Addr>(&strlen)},
    {"strncasecmp", reinterpret_cast<Elf64_Addr>(&strncasecmp)},
    {"strncasecmp", reinterpret_cast<Elf64_Addr>(&strncasecmp)},
    {"strncasecmp", reinterpret_cast<Elf64_Addr>(&strncasecmp)},
    {"strncasecmp", reinterpret_cast<Elf64_Addr>(&strncasecmp)},
    {"strncasecmp", reinterpret_cast<Elf64_Addr>(&strncasecmp)},
    {"strncasecmp", reinterpret_cast<Elf64_Addr>(&strncasecmp)},
    {"strncat", reinterpret_cast<Elf64_Addr>(&strncat)},
    {"strncat", reinterpret_cast<Elf64_Addr>(&strncat)},
    {"strncmp", reinterpret_cast<Elf64_Addr>(&strncmp)},
    {"strncpy", reinterpret_cast<Elf64_Addr>(&strncpy)},
    {"strndup", reinterpret_cast<Elf64_Addr>(&strndup)},
    {"strnlen", reinterpret_cast<Elf64_Addr>(&strnlen)},
    {"strpbrk", reinterpret_cast<Elf64_Addr>(strpbrk_c)},
    {"strptime", reinterpret_cast<Elf64_Addr>(&strptime)},
    {"strrchr", reinterpret_cast<Elf64_Addr>(strrchr_c)},
    {"strspn", reinterpret_cast<Elf64_Addr>(&strspn)},
    {"strstr", reinterpret_cast<Elf64_Addr>(strstr_c)},
    {"strtod", reinterpret_cast<Elf64_Addr>(&strtod)},
    {"strtok", reinterpret_cast<Elf64_Addr>(&strtok)},
    {"strtok_r", reinterpret_cast<Elf64_Addr>(&strtok_r)},
    {"strtok_r", reinterpret_cast<Elf64_Addr>(&strtok_r)},
    {"strtol", reinterpret_cast<Elf64_Addr>(&strtol)},
    {"strtold", reinterpret_cast<Elf64_Addr>(&strtold)},
    {"strtoll", reinterpret_cast<Elf64_Addr>(&strtoll)},
    {"strtoul", reinterpret_cast<Elf64_Addr>(&strtoul)},
    {"strtoull", reinterpret_cast<Elf64_Addr>(&strtoull)},
    {"strtoumax", reinterpret_cast<Elf64_Addr>(&strtoumax)},
    {"strverscmp", reinterpret_cast<Elf64_Addr>(&strverscmp)},
    {"strxfrm", reinterpret_cast<Elf64_Addr>(&strxfrm)},
    {"sync", reinterpret_cast<Elf64_Addr>(&sync)},
    {"syscall", reinterpret_cast<Elf64_Addr>(&syscall)},
    {"sysconf", reinterpret_cast<Elf64_Addr>(&sysconf)},
    {"tcgetattr", reinterpret_cast<Elf64_Addr>(&tcgetattr)},
    {"tcgetpgrp", reinterpret_cast<Elf64_Addr>(&tcgetpgrp)},
    {"tcsetattr", reinterpret_cast<Elf64_Addr>(&tcsetattr)},
    {"textdomain", reinterpret_cast<Elf64_Addr>(&textdomain)},
    {"time", reinterpret_cast<Elf64_Addr>(&time)},
    {"timegm", reinterpret_cast<Elf64_Addr>(&timegm)},
    {"truncate", reinterpret_cast<Elf64_Addr>(&truncate)},
    {"tzset", reinterpret_cast<Elf64_Addr>(&tzset)},
    {"uname", reinterpret_cast<Elf64_Addr>(&uname)},
    {"uname", reinterpret_cast<Elf64_Addr>(&uname)},
    {"unlink", reinterpret_cast<Elf64_Addr>(&unlink)},
    {"unsetenv", reinterpret_cast<Elf64_Addr>(&unsetenv)},
    {"uselocale", reinterpret_cast<Elf64_Addr>(&uselocale)},
    {"utimes", reinterpret_cast<Elf64_Addr>(&utimes)},
    {"vfprintf", reinterpret_cast<Elf64_Addr>(&vfprintf)},
    {"wait", reinterpret_cast<Elf64_Addr>(&wait)},
    {"waitpid", reinterpret_cast<Elf64_Addr>(&waitpid)},
    {"wcstombs", reinterpret_cast<Elf64_Addr>(&wcstombs)},
    {"wcswidth", reinterpret_cast<Elf64_Addr>(&wcswidth)},
    {"wctomb", reinterpret_cast<Elf64_Addr>(&wctomb)},
    {"wcwidth", reinterpret_cast<Elf64_Addr>(&wcwidth)},
    {"write", reinterpret_cast<Elf64_Addr>(&write)},
    {"writev", reinterpret_cast<Elf64_Addr>(&writev)},
};
}  // namespace libc_mapping
