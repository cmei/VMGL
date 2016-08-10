/* Copyright (c) 2001, Stanford University
 * All rights reserved.
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

#ifndef CR_ERROR_H
#define CR_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

#include "cr_config.h"

#ifndef __GNUC__
#define NORETURN_PRINTF
#define PRINTF
#else
#define NORETURN_PRINTF __attribute__ ((__noreturn__,format(printf,1,2)))
#define PRINTF __attribute__ ((format(printf,1,2)))
#define PRINTF_WITH_LINE __attribute__ ((format(printf,4,5)))
#define NORETURN_PRINTF_WITH_LINE __attribute__ ((__noreturn__,format(printf,4,5)))
#endif

void crEnableWarnings(int onOff);

void __crDebug(const char * file, int line, const char * func,
        char *format, ... ) PRINTF_WITH_LINE;
void __crWarning(const char * file, int line, const char * func,
        char *format, ... ) PRINTF_WITH_LINE;
void __crInfo(const char * file, int line, const char * func,
        char *format, ... ) PRINTF_WITH_LINE;
void __crError(const char * file, int line, const char * func,
        char *format, ... ) NORETURN_PRINTF_WITH_LINE;

#define crDebug(format, ...) __crDebug(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define crWarning(format, ...) __crWarning(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define crInfo(format, ...) __crInfo(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define crError(format, ...) __crError(__FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

#ifndef NDEBUG

#ifdef CR_LOOP_ON_ERROR
#define CRASSERT( PRED ) do { \
    if (PRED) \
        break; \
    crDebug( "Assertion failed: %s, file %s, line %d", #PRED, __FILE__, __LINE__); \
    crDebug("Looping forever and waiting for you to attach a debugger..."); \
    while (1) { \
        sleep(5); \
    } \
} while (1);
#else
#define CRASSERT( PRED ) ((PRED)?(void)0:crError( "Assertion failed: %s, file %s, line %d", #PRED, __FILE__, __LINE__))
#endif // CR_LOOP_ON_ERROR

#define THREADASSERT( PRED ) ((PRED)?(void)0:crError( "Are you trying to run a threaded app ?\nBuild with 'make threadsafe'\nAssertion failed: %s, file %s, line %d", #PRED, __FILE__, __LINE__))
#else
#define CRASSERT( PRED ) ((void)0)
#define THREADASSERT( PRED ) ((void)0)
#endif

/**************************** LOGGING *****************************/
#define DB_GL_RELOAD                   (1 << 1) 
#define DB_X11                         (2 << 1) 

// #define DB_FLAGS 0
#define DB_FLAGS (DB_GL_RELOAD | DB_X11)

#define _DB(dbfunc, dbflag, fmt, ...) do { \
    if (dbflag & DB_FLAGS) { \
        dbfunc(__FILE__, __LINE__, __func__, "[%s] " fmt, #dbflag, ##__VA_ARGS__); \
    } \
} while (0)
#define CR_DEBUG(dbflag, fmt, ...) _DB(__crDebug, dbflag, fmt, ##__VA_ARGS__)
#define CR_INFO(dbflag, fmt, ...) _DB(__crInfo, dbflag, fmt, ##__VA_ARGS__)
#define CR_ERROR(dbflag, fmt, ...) _DB(__crError, dbflag, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* CR_ERROR_H */
