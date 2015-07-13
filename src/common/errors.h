#ifndef _ERRORS_H_
#define _ERRORS_H_

void err_msg(const char *format, ...);

#ifdef __GNUC__

#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

/* Display error message including 'errno' diagnostic, and
   terminate the process */
void err_exit(const char *format, ...) NORETURN;
void err_exit_en(int errnum, const char *format, ...) NORETURN;
void fatal(const char *format, ...) NORETURN;
void usage_err(const char *format, ...) NORETURN;
void cmd_line_err(const char *format, ...) NORETURN;

void syslog_exit(const char *message) NORETURN;
void syslog_nx(const char *message);

static char *ename[] = {
        /*   0 */ "",
        /*   1 */ "EPERM", "ENOENT", "ESRCH", "EINTR", "EIO", "ENXIO",
        /*   7 */ "E2BIG", "ENOEXEC", "EBADF", "ECHILD",
        /*  11 */ "EAGAIN/EWOULDBLOCK", "ENOMEM", "EACCES", "EFAULT",
        /*  15 */ "ENOTBLK", "EBUSY", "EEXIST", "EXDEV", "ENODEV",
        /*  20 */ "ENOTDIR", "EISDIR", "EINVAL", "ENFILE", "EMFILE",
        /*  25 */ "ENOTTY", "ETXTBSY", "EFBIG", "ENOSPC", "ESPIPE",
        /*  30 */ "EROFS", "EMLINK", "EPIPE", "EDOM", "ERANGE",
        /*  35 */ "EDEADLK/EDEADLOCK", "ENAMETOOLONG", "ENOLCK", "ENOSYS",
        /*  39 */ "ENOTEMPTY", "ELOOP", "", "ENOMSG", "EIDRM", "ECHRNG",
        /*  45 */ "EL2NSYNC", "EL3HLT", "EL3RST", "ELNRNG", "EUNATCH",
        /*  50 */ "ENOCSI", "EL2HLT", "EBADE", "EBADR", "EXFULL", "ENOANO",
        /*  56 */ "EBADRQC", "EBADSLT", "", "EBFONT", "ENOSTR", "ENODATA",
        /*  62 */ "ETIME", "ENOSR", "ENONET", "ENOPKG", "EREMOTE",
        /*  67 */ "ENOLINK", "EADV", "ESRMNT", "ECOMM", "EPROTO",
        /*  72 */ "EMULTIHOP", "EDOTDOT", "EBADMSG", "EOVERFLOW",
        /*  76 */ "ENOTUNIQ", "EBADFD", "EREMCHG", "ELIBACC", "ELIBBAD",
        /*  81 */ "ELIBSCN", "ELIBMAX", "ELIBEXEC", "EILSEQ", "ERESTART",
        /*  86 */ "ESTRPIPE", "EUSERS", "ENOTSOCK", "EDESTADDRREQ",
        /*  90 */ "EMSGSIZE", "EPROTOTYPE", "ENOPROTOOPT",
        /*  93 */ "EPROTONOSUPPORT", "ESOCKTNOSUPPORT",
        /*  95 */ "EOPNOTSUPP/ENOTSUP", "EPFNOSUPPORT", "EAFNOSUPPORT",
        /*  98 */ "EADDRINUSE", "EADDRNOTAVAIL", "ENETDOWN", "ENETUNREACH",
        /* 102 */ "ENETRESET", "ECONNABORTED", "ECONNRESET", "ENOBUFS",
        /* 106 */ "EISCONN", "ENOTCONN", "ESHUTDOWN", "ETOOMANYREFS",
        /* 110 */ "ETIMEDOUT", "ECONNREFUSED", "EHOSTDOWN", "EHOSTUNREACH",
        /* 114 */ "EALREADY", "EINPROGRESS", "ESTALE", "EUCLEAN",
        /* 118 */ "ENOTNAM", "ENAVAIL", "EISNAM", "EREMOTEIO", "EDQUOT",
        /* 123 */ "ENOMEDIUM", "EMEDIUMTYPE", "ECANCELED", "ENOKEY",
        /* 127 */ "EKEYEXPIRED", "EKEYREVOKED", "EKEYREJECTED",
        /* 130 */ "EOWNERDEAD", "ENOTRECOVERABLE", "ERFKILL", "EHWPOISON"
};

#define MAX_ENAME 133

#endif /* _ERRORS_H_ */
