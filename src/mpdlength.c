/*
 * Collection of more or less useful tools for the Music Player Daemon.
 *
 * Copyright (c) 2012, Alessandro Ghedini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the mpdgems project, Alessandro Ghedini, nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <mpd/client.h>

static inline void help();

#define CHECK_MPD_CONN(mpd)	\
	assert(mpd_connection_get_error(mpd) == MPD_ERROR_SUCCESS);

int main(int argc, char *argv[]) {
	int opts;

	const char   *short_opts  = "f:da:p:s:h";
	struct option long_opts[] = {
		{ "format",	required_argument,	0, 'f' },
		{ "db",		no_argument,		0, 'd' },
		{ "addr",	required_argument,	0, 'a' },
		{ "port",	required_argument,	0, 'p' },
		{ "secret",	required_argument,	0, 's' },
		{ "help",	no_argument,		0, 'h' },
		{ 0,		0,			0,  0  }
	};

	unsigned int tot = 0;
	struct mpd_song *song = NULL;
	struct mpd_connection *mpd = NULL;

	char *mpd_addr  = getenv("MPD_HOST");
	int   mpd_port  = getenv("MPD_PORT") ? atoi(getenv("MPD_PORT")) : 0;
	char *mpd_pass  = NULL;
	int   search_db = 0;

	char *fmt = "%d days, %h hours, %m mins and %s secs";

	while ((opts = getopt_long(argc, argv, short_opts, long_opts, 0)) != -1) {
		switch (opts) {
			case 'f': { fmt = optarg;		break;     }
			case 'd': { search_db = 1;		break;     }
			case 'a': { mpd_addr = optarg;		break;     }
			case 'p': { mpd_port = atoi(optarg);	break;     }
			case 's': { mpd_pass = optarg;		break;     }
			default :
			case 'h': { help();			return -1; }
		}
	}

	mpd = mpd_connection_new(mpd_addr, mpd_port, 30000);
	CHECK_MPD_CONN(mpd);

	if (mpd_pass != NULL) {
		mpd_run_password(mpd, mpd_pass);
		CHECK_MPD_CONN(mpd);
	}

	if (search_db)
		mpd_send_list_all_meta(mpd, "/");
	else
		mpd_send_list_queue_meta(mpd);

	while ((song = mpd_recv_song(mpd))) {
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

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdlength [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--format",	"-f",	"The output format string");
	CMD_HELP("--db",	"-d",	"Search the MPD database instead of the current playlist");
	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--secret",	"-s",	"The MPD password");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}
