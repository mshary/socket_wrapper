/**
 * @file mptcp_wrapper.c
 *
 * @brief MPTCP configuration setup on per socket basis
 *
 * Copyright (c) 2018, 2019, Burraq Technologies UG
 */
#include "mptcp_wrapper.h"
#include "socket_wrapper.h"

/**
 * @brief gets the MPTCP information from an existing socket
 *
 * @param[in]  fd    socket handle
 * @param[in]  level socket type e.g. SOL_TCP
 * @param[out] ret   the @c struct @mptcp_info to stored MPTCP info
 */
static int get_mptcp_info(int fd, int level, struct mptcp_info *ret) {
	struct mptcp_info minfo;
	struct mptcp_meta_info meta_info;
	struct tcp_info initial;
	struct tcp_info others[3];
	struct mptcp_sub_info others_info[3];

	minfo.tcp_info_len = sizeof(struct tcp_info);
	minfo.sub_len = sizeof(others);
	minfo.meta_len = sizeof(struct mptcp_meta_info);
	minfo.meta_info = &meta_info;
	minfo.initial = &initial;
	minfo.subflows = others;
	minfo.sub_info_len = sizeof(struct mptcp_sub_info);
	minfo.total_sub_info_len = sizeof(others_info);
	minfo.subflow_info = others_info;
	ret = &minfo;

	return getsockopt(fd, level, MPTCP_INFO, &minfo, (socklen_t *)sizeof(minfo));
}

/**
 * @brief sets optimal MPTCP configuration on an existing socket
 *
 * @param[in]  sockfd socket handle
 * @param[in]  level  socket type e.g. SOL_TCP
 * @return     value from original setsockopt method
 */
static int set_mptcp_options(int sockfd, int level) {
	if (sockfd != 0 && level == IPPROTO_TCP) {
		int enable = 1;
		int ret = __setsockopt(sockfd, level, MPTCP_ENABLED, &enable, sizeof(enable));

		if (ret < 0) {
			fprintf(stderr, "setsockopt: MPTCP_ENABLED error %s!\n", strerror(errno));
			fflush(stderr);
			return ret;
		}

		char pathmanager[] = "fullmesh";
		ret = __setsockopt(sockfd, level, MPTCP_PATH_MANAGER, pathmanager, sizeof(pathmanager));

		if (ret < 0) {
			fprintf(stderr, "setsockopt: MPTCP_PATH_MANAGER error %s!\n", strerror(errno));
			fflush(stderr);
			return ret;
		}

		char scheduler[] = "default";
		ret = __setsockopt(sockfd, level, MPTCP_SCHEDULER, scheduler, sizeof(scheduler));

		if (ret < 0) {
			fprintf(stderr, "setsockopt: MPTCP_SCHEDULER error %s!\n", strerror(errno));
			fflush(stderr);
			return ret;
		}

		int val = MPTCP_INFO_FLAG_SAVE_MASTER;
		ret = __setsockopt(sockfd, level, MPTCP_INFO, &val, sizeof(val));

		if (ret < 0) {
			fprintf(stderr, "setsockopt: MPTCP_INFO error %s!\n", strerror(errno));
			fflush(stderr);
		}

		return ret;
	}

	return 0;
}

