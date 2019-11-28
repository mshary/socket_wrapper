/**
 * @file socket_wrapper.c
 *
 * @brief Override socket operations of any program
 *
 * To load and apply this wrapper to any program,
 * LD_PRELOAD=path_to_wrapper_library/libsocket_wrapper.so ./your_program
 *
 * e.g.
 * LD_PRELOAD=/root/Work/libsocket_wrapper.so ssh root@sip-server.de
 *
 * Copyright (c) 2018, 2019, Burraq Technologies UG
 */

#include "socket_wrapper.h"

#ifdef WITH_REDIS
#include <hiredis/hiredis.h>

#define REDIS_HOST "/var/run/redis/redis-server.sock"
#endif

static int run_once = 0;

/** 
 * @brief override method - socket
 */
int socket(int domain, int type, int protocol) {
	char *msg;

	if (__socket == NULL) {
		__socket = dlsym(RTLD_NEXT, "socket");

#ifdef DEBUG
		fprintf(stderr, "Loading __socket => %p\n", __socket);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "socket: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

	return __socket(domain, type, protocol);
}

/** 
 * @brief override method - setsockopt
 */
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
	char *msg;

#ifdef WITH_REDIS
	int no_delay = -1;
	redisContext *c;
	redisReply *reply;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
#endif

	if (__setsockopt == NULL) {
		__setsockopt = dlsym(RTLD_NEXT, "setsockopt");

#ifdef DEBUG
		fprintf(stderr, "Loading __setsockopt => %p\n", __setsockopt);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "setsockopt: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

#ifdef WITH_REDIS
	// load tcp no-delay option from redis and apply it only once
	if (level == IPPROTO_TCP && run_once == 0) {
		c = redisConnectUnixWithTimeout(REDIS_HOST, timeout);
		if (c != NULL && !c->err) {
			reply = redisCommand(c,"GET tcp-no-delay");
			if (c != NULL && !c->err) {
				no_delay = reply->integer;
			}
			freeReplyObject(reply);
			redisFree(c);
		}
		if (no_delay >= 0) {
			fprintf(stderr, "setsockopt: TCP_NODELAY => %d\n", no_delay);
			fflush(stderr);

			__setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &no_delay, sizeof(no_delay));
		}
		run_once = 1;
	}
#endif

	return __setsockopt(sockfd, level, optname, optval, optlen);
}

/** 
 * @brief override method - bind
 */
int bind(int sockfd, const struct sockaddr *addr, socklen_t len) {
	char *msg;

	if (__bind == NULL) {
		__bind = dlsym(RTLD_NEXT, "bind");

#ifdef DEBUG
		fprintf(stderr, "Loading __bind => %p\n", __bind);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "bind: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

	return __bind(sockfd, addr, len);
}

/** 
 * @brief override method - listen
 */
int listen(int sockfd, int backlog) {
	char *msg;

	if (__listen == NULL) {
		__listen = dlsym(RTLD_NEXT, "listen");

#ifdef DEBUG
		fprintf(stderr, "Loading __listen => %p\n", __listen);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "listen: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

	return __listen(sockfd, backlog);
}

/** 
 * @brief override method - accept
 */
int accept(int sockfd, struct sockaddr *addr, socklen_t *len) {
	char *msg;

	if (__accept == NULL) {
		__accept = dlsym(RTLD_NEXT, "accept");

#ifdef DEBUG
		fprintf(stderr, "Loading __accept => %p\n", __accept);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "accept: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

	return __accept(sockfd, addr, len);
}

/** 
 * @brief override method - connect
 */
int connect(int sockfd, const struct sockaddr *addr, socklen_t len) {
	char *msg;

	if (__connect == NULL) {
		__connect = dlsym(RTLD_NEXT, "connect");

#ifdef DEBUG
		fprintf(stderr, "Loading __connect => %p\n", __connect);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "connect: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

	return __connect(sockfd, addr, len);
}

/** 
 * @brief override method - close
 */
int close(int sockfd) {
	char *msg;

	if (__close == NULL) {
		__close = dlsym(RTLD_NEXT, "close");

#ifdef DEBUG
		fprintf(stderr, "Loading __close => %p\n", __close);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "close: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

	return __close(sockfd);
}

/** 
 * @brief override method - read
 */
ssize_t read(int sockfd, void *buf, size_t c) {
	char *msg;

	if (__read == NULL) {
		__read = dlsym(RTLD_NEXT, "read");

#ifdef DEBUG
		fprintf(stderr, "Loading __read => %p\n", __read);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "read: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

	return __read(sockfd, buf, c);
}

ssize_t write(int sockfd, void *buf, size_t c) {
	char *msg;

	if (__write == NULL) {
		__write = dlsym(RTLD_NEXT, "write");

#ifdef DEBUG
		fprintf(stderr, "Loading __write => %p\n", __write);
		fflush(stderr);
#endif

		if ((msg = dlerror()) != NULL) {
			fprintf(stderr, "write: dlopen failed: %s\n", msg);
			fflush(stderr);
			exit(1);
		}
	}

	return __write(sockfd, buf, c);
}

