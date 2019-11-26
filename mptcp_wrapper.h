/**
 * @file mptcp_wrapper.h
 *
 * @brief MPTCP configuration setup on per socket basis
 *
 * Copyright (c) 2018, 2019, Burraq Technologies UG
 */
#ifndef _MPTCP_WRAPPER_
#define _MPTCP_WRAPPER_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <linux/tcp.h>
#include <string.h>

#define MPTCP_INFO_FLAG_SAVE_MASTER 0x01

/**
 * @brief gets the MPTCP information from an existing socket
 *
 * @param[in]  fd    socket handle
 * @param[in]  level socket type e.g. SOL_TCP
 * @param[out] ret   the @c struct @mptcp_info to stored MPTCP info
 */
static int get_mptcp_info(int fd, int level, struct mptcp_info *ret);

/**
 * @brief sets optimal MPTCP configuration on an existing socket
 *
 * @param[in]  sockfd socket handle
 * @param[in]  level  socket type e.g. SOL_TCP
 * @return     value from original setsockopt method
 */
static int set_mptcp_options(int sockfd, int level);

#endif
