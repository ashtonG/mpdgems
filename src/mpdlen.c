/*
 * More or less useful MPD tools.
 *
 * Copyright (C) 2012 Alessandro Ghedini <al3xbio@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mpd/client.h>

static struct option long_opts[] = {
	{ "format",	required_argument,	0, 'f' },
	{ "addr",	required_argument,	0, 'a' },
	{ "port",	required_argument,	0, 'p' },
	{ "help",	no_argument,		0, 'h' },
	{ 0,		0,			0,  0  }
};

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdlen [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--format",	"-f",	"The output format string");
	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}

int main(int argc, char *argv[]) {
	int opts;

	unsigned int tot = 0;
	struct mpd_song *song = NULL;
	struct mpd_connection *mpd = NULL;

	char *mpd_addr = NULL;
	int   mpd_port = 0;

	char *fmt = "%d days, %h hours, %m mins and %s secs";

	while ((opts = getopt_long(argc, argv, "f:a:p:h", long_opts, 0)) != -1) {
		switch (opts) {
			case 'f': { fmt = optarg;		break;     }
			case 'a': { mpd_addr = optarg;		break;     }
			case 'p': { mpd_port = atoi(optarg);	break;     }
			default :
			case 'h': { help();			return -1; }
		}
	}

	mpd = mpd_connection_new(mpd_addr, mpd_port, 30000);

	if (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS) {
		mpd_connection_free(mpd);
		return -1;
	}

	mpd_send_list_all_meta(mpd, "/");

	while ((song = mpd_recv_song(mpd))) {
		if (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS) {
			mpd_connection_free(mpd);
			return -1;
		}

		tot += mpd_song_get_duration(song);

		mpd_song_free(song);
	}

	mpd_response_finish(mpd);
	mpd_connection_free(mpd);

	{
		unsigned int rest;
		unsigned int days, hrs, mins, secs;

		days = tot / 86400;
		rest = tot % 86400;

		hrs  = rest / 3600;
		rest = rest % 3600;

		mins = rest / 60;
		rest = rest % 60;

		secs = rest;

		while (*fmt != 0) {
			if (*fmt == '%') {
				switch (*(fmt + 1)) {
					case 'd': printf("%d", days); break;
					case 'h': printf("%d", hrs);  break;
					case 'm': printf("%d", mins); break;
					case 's': printf("%d", secs); break;
				}

				fmt += 2;
				if (*fmt == '\0') break;
			}

			putchar(*fmt);
			fmt++;
		}

		putchar('\n');
	}

	return 0;
}
