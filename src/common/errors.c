#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "errors.h"

#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif
static void terminate(bool exit3)
{
    /* Dump core if EF_DUMPCORE environment variable is defined and is a nonempty string;
     * otherwise call exit(3) or _exit(2), depending on the value of 'exit3'. */
    char *s = getenv("EF_DUMPCORE");
    if (s != NULL && *s != '\0')
        abort();
    else if (exit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

/* Diagnose 'errno' error by:
 * outputting a string containing the error name (if available in 'ename' array) corresponding to the value in 'err',
 * along with the corresponding error message from strerror(), and outputting the caller-supplied error message
 * specified in 'format' and 'ap'. */
static void output_error(bool useErr, int err, bool flushStdout, const char *format, va_list ap)
{
#define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]",
                 (err > 0 && err <= MAX_ENAME) ?
                 ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);

    if (flushStdout)
        fflush(stdout);       /* Flush any pending stdout */
    fputs(buf, stderr);
    fflush(stderr);           /* In case stderr is not line-buffered */
}

void err_message(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    output_error(true, errno, true, format, args);
    va_end(args);
}

void err_exit(const char *format, ...)
{
    err_message(format);
    terminate(true);
}

void syslog_nx(const char *message)
{
    syslog(LOG_ERR, "%s: %s", message, strerror(errno));
}

void syslog_exit(const char *message)
{
    syslog_nx(message);
    terminate(true);
}
