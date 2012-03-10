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

	struct option long_opts[] = {
		{ "title",	required_argument,	0, 't' },
		{ "artist",	required_argument,	0, 'r' },
		{ "album",	required_argument,	0, 'b' },
		{ "format",	required_argument,	0, 'f' },
		{ "addr",	required_argument,	0, 'a' },
		{ "port",	required_argument,	0, 'p' },
		{ "secret",	required_argument,	0, 's' },
		{ "help",	no_argument,		0, 'h' },
		{ 0,		0,			0,  0  }
	};

	unsigned int id = 0;

	struct mpd_song *song = NULL;
	struct mpd_connection *mpd = NULL;

	char *mpd_addr = getenv("MPD_HOST");
	int   mpd_port = getenv("MPD_PORT") ? atoi(getenv("MPD_PORT")) : 0;
	char *mpd_pass = NULL;

	char *title  = NULL;
	char *artist = NULL;
	char *album  = NULL;
	char *fmt    = "%r - %b - %t\n";

	if (argc == 1) {
		help();
		return -1;
	}

	while ((opts = getopt_long(argc, argv, "t:r:b:f:a:p:s:h", long_opts, 0)) != -1) {
		switch (opts) {
			case 't': { title = optarg;		break;     }
			case 'r': { artist = optarg;		break;     }
			case 'b': { album = optarg;		break;     }
			case 'f': { fmt = optarg;		break;     }
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

	mpd_search_queue_songs(mpd, false);

	if (title) {
		mpd_search_add_tag_constraint(
			mpd, MPD_OPERATOR_DEFAULT,
			MPD_TAG_TITLE, title
		);
	}

	if (artist) {
		mpd_search_add_tag_constraint(
			mpd, MPD_OPERATOR_DEFAULT,
			MPD_TAG_ARTIST, artist
		);
	}

	if (album) {
		mpd_search_add_tag_constraint(
			mpd, MPD_OPERATOR_DEFAULT,
			MPD_TAG_ALBUM, album
		);
	}

	mpd_search_commit(mpd);

	while (song = mpd_recv_song(mpd)) {
		const char *fmt_copy;

		fmt_copy = fmt;

		CHECK_MPD_CONN(mpd);

		while (*fmt_copy != 0) {
			if (*fmt_copy == '%') {
				switch (*(fmt_copy + 1)) {
					case 't': printf("%s", mpd_song_get_tag(song, MPD_TAG_TITLE, 0));  break;
					case 'r': printf("%s", mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)); break;
					case 'b': printf("%s", mpd_song_get_tag(song, MPD_TAG_ALBUM, 0));  break;
					case 'k': printf("%s", mpd_song_get_tag(song, MPD_TAG_TRACK, 0));  break;
					case 'd': printf("%s", mpd_song_get_tag(song, MPD_TAG_DATE, 0));  break;
					case 'g': printf("%s", mpd_song_get_tag(song, MPD_TAG_GENRE, 0));  break;
				}

				fmt_copy += 2;
				if (*fmt_copy == '\0') break;
			}

			if (*fmt_copy == '\\') {
				switch (*(fmt_copy + 1)) {
					case 'n': printf("\n");  break;
				}

				fmt_copy += 2;
				if (*fmt_copy == '\0') break;
			}

			putchar(*fmt_copy);
			fmt_copy++;
		}

		mpd_song_free(song);
	}

	mpd_response_finish(mpd);

	if (song) {
		id = mpd_song_get_id(song);

		mpd_song_free(song);

		mpd_run_play_id(mpd, id);
		CHECK_MPD_CONN(mpd);
	}

	mpd_connection_free(mpd);

	return 0;
}

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdfindby [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--title",	"-t",	"Match song titles");
	CMD_HELP("--artist",	"-r",	"Match song artists");
	CMD_HELP("--album",	"-b",	"Match song albums");
	CMD_HELP("--format",	"-f",	"The output format string");
	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--secret",	"-s",	"The MPD password");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}
