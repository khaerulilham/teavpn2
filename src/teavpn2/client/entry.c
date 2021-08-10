// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2021  Ammar Faizi
 */

#include <ctype.h>
#include <getopt.h>
#include <teavpn2/client/common.h>

/* TODO: Write my own getopt function. */


static const struct option long_options[] = {
	{"help",        no_argument,       0, 'h'},
	{"version",     no_argument,       0, 'V'},
	{"verbose",     optional_argument, 0, 'v'},

	{"config",      required_argument, 0, 'c'},
	{"data-dir",    required_argument, 0, 'd'},
	{"thread",      required_argument, 0, 't'},

	{"sock-type",   required_argument, 0, 's'},
	{"server-addr", required_argument, 0, 'H'},
	{"server-port", required_argument, 0, 'P'},
	{"max-conn",    required_argument, 0, 'C'},
	{"backlog",     required_argument, 0, 'B'},
	{"encrypt",     no_argument,       0, 'E'},

	{"dev",         required_argument, 0, 'D'},

	{0, 0, 0, 0}
};
static const char short_opt[] = "hVv::c:d:t:s:H:P:C:B:E:D:";


static void show_help(void)
{
}


#define PR_CFG(C, FMT) printf("   " #C " = " FMT "\n", C)

static void dump_client_cfg(struct cli_cfg *cfg)
{
	puts("=============================================");
	puts("   Config dump   ");
	puts("=============================================");
	PR_CFG(cfg->sys.cfg_file, "%s");
	PR_CFG(cfg->sys.data_dir, "%s");
	PR_CFG(cfg->sys.thread_num, "%hhu");
	PR_CFG(cfg->sys.verbose_level, "%hhu");
	putchar('\n');
	PR_CFG(cfg->sock.use_encrypt, "%hhu");
	printf("   cfg->sock.type = %s\n",
		(cfg->sock.type == SOCK_TCP) ? "SOCK_TCP" :
		((cfg->sock.type == SOCK_UDP) ? "SOCK_UDP" : "unknown"));
	PR_CFG(cfg->sock.server_addr, "%s");
	PR_CFG(cfg->sock.server_port, "%hu");
	PR_CFG(cfg->sock.event_loop, "%s");
	PR_CFG(cfg->sock.max_conn, "%hu");
	PR_CFG(cfg->sock.backlog, "%d");
	putchar('\n');
	PR_CFG(cfg->iface.dev, "%s");
	puts("=============================================");
}


static int parse_argv(int argc, char *argv[], struct cli_cfg *cfg)
{
	int c;
	struct cli_cfg_sys *sys = &cfg->sys;
	struct cli_cfg_sock *sock = &cfg->sock;
	struct cli_cfg_iface *iface = &cfg->iface;

	while (1) {
		int opt_idx = 0;

		c = getopt_long(argc, argv, short_opt, long_options, &opt_idx);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			show_help();
			exit(0);
		case 'V':
			show_version();
			exit(0);
		case 'v': {
			uint8_t level = optarg ? (uint8_t)atoi(optarg) : 6u;
			set_notice_level(level);
			sys->verbose_level = level;
			break;
		}


		/*
		 * Sys config
		 */
		case 'c':
			sys->cfg_file = optarg;
			break;
		case 'd':
			sys->data_dir = optarg;
			break;
		case 't': {
			int tmp = atoi(optarg);
			if (tmp <= 0) {
				pr_err("Thread num argument must be greater than 0");
				return -EINVAL;
			}
			sys->thread_num = (uint8_t)tmp;
			break;
		}


		/*
		 * Sock config
		 */
		case 's': {
			char tmp[5], *p = tmp;

			strncpy(tmp, optarg, sizeof(tmp));
			tmp[sizeof(tmp) - 1] = '\0';

			while (*p) {
				*p = (char)tolower((int)(unsigned)*p);
				p++;
			}

			if (!strcmp(tmp, "tcp")) {
				sock->type = SOCK_TCP;
			} else if (!strcmp(tmp, "udp")) {
				sock->type = SOCK_UDP;
			} else {
				pr_err("Invalid socket type: %s", optarg);
				return -EINVAL;
			}
			break;
		}
		case 'H':
			sock->server_addr = optarg;
			break;
		case 'P':
			sock->server_port = (uint16_t)atoi(optarg);
			break;
		case 'C':
			sock->max_conn = (uint16_t)atoi(optarg);
			break;
		case 'B':
			sock->backlog = atoi(optarg);
			break;
		case 'E':
			sock->use_encrypt = atoi(optarg) ? true : false;
			break;

		/*
		 * Iface config
		 */
		case 'D':
			strncpy(iface->dev, optarg, sizeof(iface->dev));
			iface->dev[sizeof(iface->dev) - 1] = '\0';
			break;
		default:
			return -EINVAL;
		}
	}

	return 0;
}


int run_client(int argc, char *argv[])
{
	int ret;
	struct cli_cfg cfg;
	memset(&cfg, 0, sizeof(cfg));

	pr_debug("Parsing argv...");
	ret = parse_argv(argc, argv, &cfg);
	if (ret)
		return -ret;

	dump_client_cfg(&cfg);


	return 0;
}
