#ifndef _COMMON_H_
#define _COMMON_H_

#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <sys/stat.h>
#include <fcntl.h>

#include "socket.h"
#include "errors.h"
#include "daemonize.h"

#endif //_COMMON_H_
