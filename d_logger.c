#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "d_logger.h"
#include "d_error.h"
#include "d_memory.h"
#include "d_types.h"
#include "d_list.h"
#include "d_socket.h"

#ifdef __ANDROID__
#include <android/log.h>
#endif

enum {
    LOGGER_TYPE_CONSOLE,
    LOGGER_TYPE_GROUP,
    LOGGER_TYPE_NETWORK
};

typedef struct _d_logger {
    void (*log)(DLogger* logger, int log_level, char* msg, va_list vl);
    void (*free)(DLogger* logger);
    int log_level;
    int logger_type;
} DLogger;

typedef struct _d_logger_console {
    DLogger logger;
} DLoggerConsole;

typedef struct _d_logger_group {
    DLogger logger;
    DSList* children;
} DLoggerGroup;

typedef struct _d_logger_network {
    DLogger logger;
    DSocket* socket;
} DLoggerNetwork;

static DLoggerGroup* g_default_logger = NULL;

char* build_message(int log_level, char* msg) {
    char* str = d_malloc(sizeof (char)*strlen(msg) + 32 * sizeof (char));

    switch (log_level) {
        case LOGLEVEL_DEBUG:
            sprintf(str, "DEBUG   : %s", msg);
            break;
        case LOGLEVEL_VERBOSE:
            sprintf(str, "VERBOSE : %s", msg);
            break;
        case LOGLEVEL_INFO:
            sprintf(str, "INFO    : %s", msg);
            break;
        case LOGLEVEL_WARNING:
            sprintf(str, "\e[34mWARNING : %s\e[0m", msg);
            break;
        case LOGLEVEL_ERROR:
            sprintf(str, "\e[31mERROR   : %s\e[0m", msg);
            break;
        case LOGLEVEL_CRITICAL:
            sprintf(str, "\e[1;4;31mCRITICAL: %s\e[0m", msg);
            break;
        case LOGLEVEL_FATAL:
            sprintf(str, "\e[1;4;5;31mFATAL   : %s\e[0m", msg);
            break;
        default:
            break;
    }

    return str;
}

/* Logging function for DLoggerConsole*/
void log_console(DLogger* logger, int log_level, char *msg, va_list vl) {
    if (log_level >= logger->log_level) {
        char* str = build_message(log_level, msg);
#ifdef __ANDROID__
        __android_log_vprint(ANDROID_LOG_DEBUG, "dgk", str, vl);
#else
        vprintf(str, vl);
        printf("\n");
#endif
        free(str);
    }
}

void log_group(DLogger* logger, int log_level, char *msg, va_list vl) {

    DLoggerGroup* logger_group = (DLoggerGroup*) logger;
    DSList* iter = logger_group->children;

    while (iter != NULL) {
        DLogger* l = iter->content;
        va_list dst;
        va_copy(dst, vl);
        l->log(l, log_level, msg, dst);
        iter = iter->next;
    }

}

void log_network(DLogger* logger, int log_level, char *msg, va_list vl) {
    if (log_level >= logger->log_level) {
        char* str = build_message(log_level, msg);
        va_list vl2;
        va_copy(vl2, vl);
        char* buffer = NULL;
        int count = vsnprintf(buffer, 0, str, vl);
        buffer = d_malloc(sizeof (char) * (count + 1));
        vsnprintf(buffer, count + 1, str, vl2);
        buffer[count] = '\n';
        DLoggerNetwork* logger_n = (DLoggerNetwork*) logger;
        DError* error = NULL;
        d_socket_send(logger_n->socket, buffer, count + 1, &error);
        if (error) {
#ifdef __ANDROID__
            __android_log_print(ANDROID_LOG_DEBUG, "dgk", "Network Logger -> %s\n", error->msg);
#else
            printf("Network Logger -> %s\n", error->msg);
#endif
        }

        free(buffer);
        free(str);
    }
}

void free_console(DLogger* logger) {
    free(logger);
}

void free_group(DLogger* logger) {
    DLoggerGroup* logger_g = (DLoggerGroup*) logger;
    DSList* iter = logger_g->children;
    while (iter != NULL) {
        d_logger_free(iter->content);
        iter = iter->next;
    }
    d_slist_free(logger_g->children);
    free(logger);
}

void free_network(DLogger* logger) {
    DLoggerNetwork* logger_n = (DLoggerNetwork*) logger;
    d_socket_close(logger_n->socket);
    free(logger_n);
}

DLoggerConsole* d_logger_console_new(int log_level) {
    DLoggerConsole* new_logger_console = d_malloc(sizeof (DLoggerConsole));
    new_logger_console->logger.log_level = log_level;
    new_logger_console->logger.log = log_console;
    new_logger_console->logger.free = free_console;
    new_logger_console->logger.logger_type = LOGGER_TYPE_CONSOLE;
    return new_logger_console;
}

DLoggerGroup* d_logger_group_new() {
    DLoggerGroup* new_logger_group = d_malloc(sizeof (DLoggerGroup));
    new_logger_group->logger.log_level = LOGLEVEL_VERBOSE;
    new_logger_group->logger.log = log_group;
    new_logger_group->logger.free = free_group;
    new_logger_group->logger.logger_type = LOGGER_TYPE_GROUP;
    new_logger_group->children = NULL;
    return new_logger_group;
}

DLoggerNetwork* d_logger_network_new(int log_level, char* ip, int port, DError** error) {
    DLoggerNetwork* new_logger_network = d_malloc(sizeof (DLoggerNetwork));
    new_logger_network->socket = d_socket_connect_by_ip(ip, port, error);

    if (error && *error)
        goto error;

    new_logger_network->logger.log_level = log_level;
    new_logger_network->logger.log = log_network;
    new_logger_network->logger.free= free_network;
    new_logger_network->logger.logger_type = LOGGER_TYPE_NETWORK;
    return new_logger_network;

error:
    if (new_logger_network) free(new_logger_network);
    return NULL;
}

void d_logger_free(DLogger* logger) {
    logger->free(logger);
}

void d_logger_set_loglevel(DLogger* logger, int log_level) {
    logger->log_level = log_level;
}

void d_logger_group_add_logger(DLoggerGroup* group, DLogger* logger) {
    group->children = d_slist_append(group->children, logger);
}

DLoggerGroup* d_logger_get_default_logger() {
    if (g_default_logger == NULL) {
        g_default_logger = d_logger_group_new();
        DLoggerConsole* console_logger = d_logger_console_new(0);
        d_logger_group_add_logger(g_default_logger, D_LOGGER(console_logger));
    }
    return g_default_logger;
}

void d_log(DLogger* logger, int log_level, char* msg, ...) {
    va_list args;
    va_start(args, msg);
    logger->log(logger, log_level, msg, args);
    va_end(args);
}

void d_log_error(DLogger* logger, DError* error, int log_level, char* msg, ...) {
    char* buffer;

    va_list args;
    va_start(args, msg);
    int count = vsnprintf(buffer, 0, msg, args);
    va_end(args);
    int count2 = snprintf(buffer, 0, " : %s -> In file %s at line %d ", error->msg, error->file, error->line);

    buffer = d_malloc(sizeof (char)*(count + count2 + 2));

    va_list args2;
    va_start(args2, msg);
    vsnprintf(buffer, count + 1, msg, args2);
    va_end(args2);
    snprintf(buffer + count, count2 + 1, " : %s -> In file %s at line %d ", error->msg, error->file, error->line);

    d_log(logger, log_level, buffer);

    free(buffer);
}
