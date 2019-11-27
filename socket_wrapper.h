/**
 * @file socket_wrapper.h
 *
 * @brief Override socket operations of any program
 *
 * Copyright (c) 2018, 2019, Burraq Technologies UG
 */
#ifndef _SOCKET_WRAPPER_
#define _SOCKET_WRAPPER_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>     // For NULL.

#include <dlfcn.h>
#include <errno.h>
#include <linux/tcp.h>

#include <string.h>

/**
 * @brief original socket method
 */
static int (*__socket)(int domain, int type, int protocol) = NULL;

/**
 * @brief original setsockopt method
 */
static int (*__setsockopt)(int sockfd, int level, int optname, const void *optval, socklen_t optlen) = NULL;

/**
 * @brief original bind method
 */
static int (*__bind)(int sockfd, const struct sockaddr *addr, socklen_t len) = NULL;

/**
 * @brief original listen method
 */
static int (*__listen)(int sockfd, int backlog) = NULL;

/**
 * @brief original accept method
 */
static int (*__accept)(int sockfd, struct sockaddr *addr, socklen_t *len) = NULL;

/**
 * @brief original connect method
 */
static int (*__connect)(int sockfd, const struct sockaddr *addr, socklen_t len) = NULL;

/**
 * @brief original close method
 */
static int (*__close)(int sockfd) = NULL;

/**
 * @brief original read method
 */
static ssize_t (*__read)(int sockfd, void *buf, size_t c) = NULL;

/**
 * @brief original write method
 */
static ssize_t (*__write)(int sockfd, void *buf, size_t c) = NULL;

/**
 * @brief override method - socket
 */
int socket(int domain, int type, int protocol);

/**
 * @brief override method - setsockopt
 */
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

/**
 * @brief override method - bind
 */
int bind(int sockfd, const struct sockaddr *addr, socklen_t len);

/**
 * @brief override method - listen
 */
int listen(int sockfd, int backlog);

/**
 * @brief override method - accept
 */
int accept(int sockfd, struct sockaddr *addr, socklen_t *len);

/**
 * @brief override method - connect
 */
int connect(int sockfd, const struct sockaddr *addr, socklen_t len);

/**
 * @brief override method - close
 */
int close(int sockfd);

/**
 * @brief override method - read
 */
ssize_t read(int sockfd, void *buf, size_t c);

/**
 * @brief override method - write
 */
ssize_t write(int sockfd, void *buf, size_t c);

#endif
