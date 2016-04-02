#pragma once

#include "d_error.h"
#include "d_list.h"

enum {
    LOGLEVEL_DEBUG,
    LOGLEVEL_VERBOSE,
    LOGLEVEL_INFO,
    LOGLEVEL_WARNING,
    LOGLEVEL_ERROR,
    LOGLEVEL_CRITICAL,
    LOGLEVEL_FATAL
};

/* Opaques Types*/
typedef struct _d_logger DLogger;

typedef struct _d_logger_console DLoggerConsole;

typedef struct _d_logger_group DLoggerGroup;

typedef struct _d_logger_network DLoggerNetwork;

/* CASTS */

#define D_LOGGER(X) ((DLogger*)X)

/* Function Prototypes*/

DLoggerConsole* d_logger_console_new(int log_level);

DLoggerGroup* d_logger_group_new();

DLoggerNetwork* d_logger_network_new(int log_level,char* ip,int port,DError** error);

void d_logger_free(DLogger* logger);

void d_logger_set_loglevel(DLogger* logger,int log_level);

void d_log(DLogger* logger,int log_level,char* msg, ...);

void d_log_error(DLogger* logger,DError* error,int log_level,char* msg,...);

void d_logger_group_add_logger(DLoggerGroup* group,DLogger* logger);

DLoggerGroup* d_logger_get_default_logger();


/* FAST LOGGING MACROS*/

#ifndef NO_DEBUG_LOG
#define DLOGD(msg,...) d_log(D_LOGGER(d_logger_get_default_logger()),LOGLEVEL_DEBUG,msg,##__VA_ARGS__)
#else
#define DLOGD(msg,...)
#endif

#define DLOGV(msg,...) d_log(D_LOGGER(d_logger_get_default_logger()),LOGLEVEL_VERBOSE,msg,##__VA_ARGS__)
#define DLOGI(msg,...) d_log(D_LOGGER(d_logger_get_default_logger()),LOGLEVEL_INFO,msg,##__VA_ARGS__)
#define DLOGW(msg,...) d_log(D_LOGGER(d_logger_get_default_logger()),LOGLEVEL_WARNING,msg,##__VA_ARGS__)
#define DLOGE(msg,...) d_log(D_LOGGER(d_logger_get_default_logger()),LOGLEVEL_ERROR,msg,##__VA_ARGS__)
#define DLOGC(msg,...) d_log(D_LOGGER(d_logger_get_default_logger()),LOGLEVEL_CRITICAL,msg,##__VA_ARGS__)
#define DLOGF(msg,...) d_log(D_LOGGER(d_logger_get_default_logger()),LOGLEVEL_FATAL,msg,##__VA_ARGS__)

#define DLOG_ERR_W(error,msg,...) d_log_error(D_LOGGER(d_logger_get_default_logger()),error,LOGLEVEL_WARNING,msg,##__VA_ARGS__)
#define DLOG_ERR_E(error,msg,...) d_log_error(D_LOGGER(d_logger_get_default_logger()),error,LOGLEVEL_ERROR,msg,##__VA_ARGS__)
#define DLOG_ERR_C(error,msg,...) d_log_error(D_LOGGER(d_logger_get_default_logger()),error,LOGLEVEL_CRITICAL,msg,##__VA_ARGS__)
#define DLOG_ERR_F(error,msg,...) d_log_error(D_LOGGER(d_logger_get_default_logger()),error,LOGLEVEL_FATAL,msg,##__VA_ARGS__)