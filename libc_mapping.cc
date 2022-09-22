#include "libc_mapping.h"

#include <asm/prctl.h>
#include <ctype.h>
#include <dirent.h>
#include <elf.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <getopt.h>
#include <grp.h>
#include <langinfo.h>
#include <libintl.h>
#include <pwd.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/capability.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/xattr.h>
#include <unistd.h>
#include <cinttypes>
#include <cstring>
#include <ctime>

namespace libc_mapping {
const char* (*strchr_c)(const char*, int) = strchr;
const char* (*strrchr_c)(const char*, int) = strrchr;
const void* (*rawmemchr_c)(const void*, int) = rawmemchr;

void sloader_libc_start_main(int (*main)(int, char**, char**), int argc, char* argv, void (*init)(void), void (*fini)(void),
                             void (*rtld_fini)(void), void* stack_end) {
    // Although glibc write this function in assembly, we can use a plain C function?
    // https://github.com/akawashiro/glibc/blob/0005e54f762b2ec65cee2c4ecf1e9d42612030f0/sysdeps/x86_64/start.S#L64-L76
    // TODO
    // After SYS_arch_prctl, we cannot use glog.
    // LOG(INFO) << LOG_BITS(main);
    exit(main(argc, &argv, NULL));
}

std::map<std::string, Elf64_Addr> sloader_libc_map = {
    {"_ITM_deregisterTMCloneTable", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"_ITM_registerTMCloneTable", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__asprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__assert_fail", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__ctype_b_loc", reinterpret_cast<Elf64_Addr>(&__ctype_b_loc)},
    {"__ctype_get_mb_cur_max", reinterpret_cast<Elf64_Addr>(&__ctype_get_mb_cur_max)},
    {"__ctype_tolower_loc", reinterpret_cast<Elf64_Addr>(&__ctype_tolower_loc)},
    {"__ctype_toupper_loc", reinterpret_cast<Elf64_Addr>(&__ctype_toupper_loc)},
    {"__cxa_atexit", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__cxa_finalize", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__errno_location", reinterpret_cast<Elf64_Addr>(&__errno_location)},
    {"__fpending", reinterpret_cast<Elf64_Addr>(&__fpending)},
    {"__fprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__freading", reinterpret_cast<Elf64_Addr>(&__freading)},
    {"__gmon_start__", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__libc_start_main", reinterpret_cast<Elf64_Addr>(&sloader_libc_start_main)},
    {"__mbstowcs_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__memcpy_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__overflow", reinterpret_cast<Elf64_Addr>(&__overflow)},
    {"__printf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__progname", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__progname_full", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__snprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__sprintf_chk", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"__stack_chk_fail", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"_exit", reinterpret_cast<Elf64_Addr>(&_exit)},
    {"_setjmp", reinterpret_cast<Elf64_Addr>(nullptr)},
    {"abort", reinterpret_cast<Elf64_Addr>(&abort)},
    {"bindtextdomain", reinterpret_cast<Elf64_Addr>(&bindtextdomain)},
    {"calloc", reinterpret_cast<Elf64_Addr>(&calloc)},
    {"capget", reinterpret_cast<Elf64_Addr>(&capget)},
    {"capset", reinterpret_cast<Elf64_Addr>(&capset)},
    {"chdir", reinterpret_cast<Elf64_Addr>(&chdir)},
    {"chroot", reinterpret_cast<Elf64_Addr>(&chroot)},
    {"clock_gettime", reinterpret_cast<Elf64_Addr>(&clock_gettime)},
    {"close", reinterpret_cast<Elf64_Addr>(&close)},
    {"closedir", reinterpret_cast<Elf64_Addr>(&closedir)},
    {"dcgettext", reinterpret_cast<Elf64_Addr>(&dcgettext)},
    {"dirfd", reinterpret_cast<Elf64_Addr>(&dirfd)},
    {"error", reinterpret_cast<Elf64_Addr>(&error)},
    {"execve", reinterpret_cast<Elf64_Addr>(&execve)},
    {"exit", reinterpret_cast<Elf64_Addr>(&exit)},
    {"fclose", reinterpret_cast<Elf64_Addr>(&fclose)},
    {"fflush", reinterpret_cast<Elf64_Addr>(&fflush)},
    {"fflush_unlocked", reinterpret_cast<Elf64_Addr>(&fflush_unlocked)},
    {"fgets", reinterpret_cast<Elf64_Addr>(&fgets)},
    {"fgetxattr", reinterpret_cast<Elf64_Addr>(&fgetxattr)},
    {"fileno", reinterpret_cast<Elf64_Addr>(&fileno)},
    {"fnmatch", reinterpret_cast<Elf64_Addr>(&fnmatch)},
    {"fopen64", reinterpret_cast<Elf64_Addr>(&fopen64)},
    {"fork", reinterpret_cast<Elf64_Addr>(&fork)},
    {"fputc_unlocked", reinterpret_cast<Elf64_Addr>(&fputc_unlocked)},
    {"fputs_unlocked", reinterpret_cast<Elf64_Addr>(&fputs_unlocked)},
    {"fread", reinterpret_cast<Elf64_Addr>(&fread)},
    {"free", reinterpret_cast<Elf64_Addr>(&free)},
    {"fremovexattr", reinterpret_cast<Elf64_Addr>(&fremovexattr)},
    {"fseeko", reinterpret_cast<Elf64_Addr>(&fseeko)},
    {"fsetxattr", reinterpret_cast<Elf64_Addr>(&fsetxattr)},
    {"fstat64", reinterpret_cast<Elf64_Addr>(&fstat64)},
    {"fwrite", reinterpret_cast<Elf64_Addr>(&fwrite)},
    {"fwrite_unlocked", reinterpret_cast<Elf64_Addr>(&fwrite_unlocked)},
    {"getcwd", reinterpret_cast<Elf64_Addr>(&getcwd)},
    {"getenv", reinterpret_cast<Elf64_Addr>(&getenv)},
    {"getgrgid", reinterpret_cast<Elf64_Addr>(&getgrgid)},
    {"gethostname", reinterpret_cast<Elf64_Addr>(&gethostname)},
    {"getopt_long", reinterpret_cast<Elf64_Addr>(&getopt_long)},
    {"getpwuid", reinterpret_cast<Elf64_Addr>(&getpwuid)},
    {"getxattr", reinterpret_cast<Elf64_Addr>(&getxattr)},
    {"gmtime_r", reinterpret_cast<Elf64_Addr>(&gmtime_r)},
    {"ioctl", reinterpret_cast<Elf64_Addr>(&ioctl)},
    {"isatty", reinterpret_cast<Elf64_Addr>(&isatty)},
    {"iswcntrl", reinterpret_cast<Elf64_Addr>(&iscntrl)},
    {"iswprint", reinterpret_cast<Elf64_Addr>(&isprint)},
    {"localeconv", reinterpret_cast<Elf64_Addr>(&localeconv)},
    {"localtime_r", reinterpret_cast<Elf64_Addr>(&localtime_r)},
    {"lseek", reinterpret_cast<Elf64_Addr>(&lseek)},
    {"lstat64", reinterpret_cast<Elf64_Addr>(&lstat64)},
    {"malloc", reinterpret_cast<Elf64_Addr>(&malloc)},
    {"mbrtowc", reinterpret_cast<Elf64_Addr>(&mbrtowc)},
    {"mbsinit", reinterpret_cast<Elf64_Addr>(&mbsinit)},
    {"mbstowcs", reinterpret_cast<Elf64_Addr>(&mbstowcs)},
    {"memcmp", reinterpret_cast<Elf64_Addr>(&memcmp)},
    {"memcpy", reinterpret_cast<Elf64_Addr>(&memcpy)},
    {"memmove", reinterpret_cast<Elf64_Addr>(&memmove)},
    {"mempcpy", reinterpret_cast<Elf64_Addr>(&mempcpy)},
    {"memset", reinterpret_cast<Elf64_Addr>(&memset)},
    {"nl_langinfo", reinterpret_cast<Elf64_Addr>(&nl_langinfo)},
    {"opendir", reinterpret_cast<Elf64_Addr>(&opendir)},
    {"optarg", reinterpret_cast<Elf64_Addr>(&optarg)},
    {"optind", reinterpret_cast<Elf64_Addr>(&optind)},
    {"perror", reinterpret_cast<Elf64_Addr>(&perror)},
    {"pipe2", reinterpret_cast<Elf64_Addr>(&pipe2)},
    {"prctl", reinterpret_cast<Elf64_Addr>(&prctl)},
    {"printf", reinterpret_cast<Elf64_Addr>(&printf)},
    {"program_invocation_name", reinterpret_cast<Elf64_Addr>(&program_invocation_name)},
    {"program_invocation_short_name", reinterpret_cast<Elf64_Addr>(&program_invocation_short_name)},
    {"putchar", reinterpret_cast<Elf64_Addr>(&putchar)},
    {"puts", reinterpret_cast<Elf64_Addr>(&puts)},
    {"raise", reinterpret_cast<Elf64_Addr>(&raise)},
    {"rawmemchr", reinterpret_cast<Elf64_Addr>(&rawmemchr_c)},
    {"read", reinterpret_cast<Elf64_Addr>(&read)},
    {"readdir", reinterpret_cast<Elf64_Addr>(&readdir)},
    {"readlink", reinterpret_cast<Elf64_Addr>(&readlink)},
    {"realloc", reinterpret_cast<Elf64_Addr>(&realloc)},
    {"reallocarray", reinterpret_cast<Elf64_Addr>(&reallocarray)},
    {"removexattr", reinterpret_cast<Elf64_Addr>(&removexattr)},
    {"sched_yield", reinterpret_cast<Elf64_Addr>(&sched_yield)},
    {"setenv", reinterpret_cast<Elf64_Addr>(&setenv)},
    {"setgid", reinterpret_cast<Elf64_Addr>(&setgid)},
    {"setgroups", reinterpret_cast<Elf64_Addr>(&setgroups)},
    {"setlocale", reinterpret_cast<Elf64_Addr>(&setlocale)},
    {"setuid", reinterpret_cast<Elf64_Addr>(&setuid)},
    {"setxattr", reinterpret_cast<Elf64_Addr>(&setxattr)},
    {"sigaction", reinterpret_cast<Elf64_Addr>(&sigaction)},
    {"sigaddset", reinterpret_cast<Elf64_Addr>(&sigaddset)},
    {"sigemptyset", reinterpret_cast<Elf64_Addr>(&sigemptyset)},
    {"sigismember", reinterpret_cast<Elf64_Addr>(&sigismember)},
    {"signal", reinterpret_cast<Elf64_Addr>(&signal)},
    {"sigprocmask", reinterpret_cast<Elf64_Addr>(&sigprocmask)},
    {"snprintf", reinterpret_cast<Elf64_Addr>(&snprintf)},
    {"stat", reinterpret_cast<Elf64_Addr>(&stat)},
    {"statx", reinterpret_cast<Elf64_Addr>(&statx)},
    {"stderr", reinterpret_cast<Elf64_Addr>(&stderr)},
    {"stdout", reinterpret_cast<Elf64_Addr>(&stdout)},
    {"stpncpy", reinterpret_cast<Elf64_Addr>(&stpncpy)},
    {"strchr", reinterpret_cast<Elf64_Addr>(&strchr_c)},
    {"strcmp", reinterpret_cast<Elf64_Addr>(&strcmp)},
    {"strcoll", reinterpret_cast<Elf64_Addr>(&strcoll)},
    {"strcpy", reinterpret_cast<Elf64_Addr>(&strcpy)},
    {"strftime", reinterpret_cast<Elf64_Addr>(&strftime)},
    {"strlen", reinterpret_cast<Elf64_Addr>(&strlen)},
    {"strncmp", reinterpret_cast<Elf64_Addr>(&strncmp)},
    {"strrchr", reinterpret_cast<Elf64_Addr>(&strrchr_c)},
    {"strtoul", reinterpret_cast<Elf64_Addr>(&strtoul)},
    {"strtoumax", reinterpret_cast<Elf64_Addr>(&strtoumax)},
    {"syscall", reinterpret_cast<Elf64_Addr>(&syscall)},
    {"tcgetpgrp", reinterpret_cast<Elf64_Addr>(&tcgetpgrp)},
    {"textdomain", reinterpret_cast<Elf64_Addr>(&textdomain)},
    {"tzset", reinterpret_cast<Elf64_Addr>(&tzset)},
    {"unsetenv", reinterpret_cast<Elf64_Addr>(&unsetenv)},
    {"waitpid", reinterpret_cast<Elf64_Addr>(&waitpid)},
    {"wcstombs", reinterpret_cast<Elf64_Addr>(&wcstombs)},
    {"wcswidth", reinterpret_cast<Elf64_Addr>(&wcswidth)},
    {"wcwidth", reinterpret_cast<Elf64_Addr>(&wcwidth)},
    {"write", reinterpret_cast<Elf64_Addr>(&write)},
};
}  // namespace libc_mapping
