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
#include <string.h>
#include <getopt.h>
#include <mpd/client.h>

typedef struct NODE {
	void *value;

	struct NODE *prev;
	struct NODE *next;
} list_node_t;

static inline void format_song(char *fmt, struct mpd_song *song);
static inline void help();

static inline list_node_t *list_init();
static inline list_node_t *list_head(list_node_t *l);
static inline void list_insert_tail(list_node_t *l, void *item);
static inline void list_remove(list_node_t *l);
static inline bool list_is_empty(list_node_t *l);

#define list_foreach_safe(ITEM, SAFE, L) \
	for (ITEM = list_head(L), SAFE = ITEM -> next; ITEM != L; ITEM = SAFE, SAFE = ITEM -> next)

#define CHECK_MPD_CONN(mpd)	\
	assert(mpd_connection_get_error(mpd) == MPD_ERROR_SUCCESS);

int main(int argc, char *argv[]) {
	int opts;

	const char   *short_opts  = "t:r:b:f:dla:p:s:h";
	struct option long_opts[] = {
		{ "title",	required_argument,	0, 't' },
		{ "artist",	required_argument,	0, 'r' },
		{ "album",	required_argument,	0, 'b' },
		{ "format",	required_argument,	0, 'f' },
		{ "db",		no_argument,		0, 'd' },
		{ "list",	no_argument,		0, 'l' },
		{ "addr",	required_argument,	0, 'a' },
		{ "port",	required_argument,	0, 'p' },
		{ "secret",	required_argument,	0, 's' },
		{ "help",	no_argument,		0, 'h' },
		{ 0,		0,			0,  0  }
	};

	struct mpd_song *song = NULL;
	struct mpd_connection *mpd = NULL;

	list_node_t *songs_list = list_init(), *iter, *safe;

	char *mpd_addr  = getenv("MPD_HOST");
	int   mpd_port  = getenv("MPD_PORT") ? atoi(getenv("MPD_PORT")) : 0;
	char *mpd_pass  = NULL;
	int   search_db = 0;
	int   playlist  = 0;

	char *title  = NULL;
	char *artist = NULL;
	char *album  = NULL;
	char *fmt    = "%r - %b - %t\n";

	if (argc == 1) {
		help();
		return -1;
	}

	while ((opts = getopt_long(argc, argv, short_opts, long_opts, 0)) != -1) {
		switch (opts) {
			case 't': { title = optarg;		break;     }
			case 'r': { artist = optarg;		break;     }
			case 'b': { album = optarg;		break;     }
			case 'f': { fmt = optarg;		break;     }
			case 'd': { search_db = 1;		break;     }
			case 'l': { playlist = 1;		break;     }
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
		mpd_search_db_songs(mpd, false);
	else
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
		const char *uri = mpd_song_get_uri(song);
		list_insert_tail(songs_list, strdup(uri));

		format_song(fmt, song);

		mpd_song_free(song);
	}

	mpd_response_finish(mpd);

	if (playlist) {
		mpd_run_clear(mpd);

		list_foreach_safe(iter, safe, songs_list) {
			char *uri = iter -> value;

			mpd_run_add(mpd, uri);
			CHECK_MPD_CONN(mpd);

			free(uri);
			list_remove(iter);
		}
	}

	mpd_connection_free(mpd);
	free(songs_list);

	return 0;
}

static inline void format_song(char *fmt, struct mpd_song *song) {
	const char *fmt_copy = fmt;

	while (*fmt_copy != 0) {
		if (*fmt_copy == '%') {
			const char *title	=
				mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
			const char *artist	=
				mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
			const char *album	=
				mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);
			const char *track	=
				mpd_song_get_tag(song, MPD_TAG_TRACK, 0);
			const char *date	=
				mpd_song_get_tag(song, MPD_TAG_DATE, 0);
			const char *genre	=
				mpd_song_get_tag(song, MPD_TAG_GENRE, 0);

			switch (*(fmt_copy + 1)) {
				case 't': printf("%s", title);	break;
				case 'r': printf("%s", artist);	break;
				case 'b': printf("%s", album);	break;
				case 'k': printf("%s", track);	break;
				case 'd': printf("%s", date);	break;
				case 'g': printf("%s", genre);	break;
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
}

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdfindby [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--title",	"-t",	"Match song titles");
	CMD_HELP("--artist",	"-r",	"Match song artists");
	CMD_HELP("--album",	"-b",	"Match song albums");
	CMD_HELP("--format",	"-f",	"The output format string");
	CMD_HELP("--db",	"-d",	"Search the MPD database instead of the current playlist");
	CMD_HELP("--list",	"-l",	"Create a playlist with the matching songs");
	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--secret",	"-s",	"The MPD password");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}

static inline list_node_t *list_init() {
	list_node_t *t = (list_node_t *) malloc(sizeof(list_node_t));

	if (t == NULL)
		return NULL;

	t -> prev = t;
	t -> next = t;

	return t;
}

static inline bool list_is_empty(list_node_t *l) {
	if (l == NULL || l == l -> next)
		return true;

	return false;
}

static inline list_node_t *list_head(list_node_t *l) {
	return l -> next;
}

static inline void list_insert_tail(list_node_t *l, void *item) {
	list_node_t *t = list_init();

	t -> value = item;

	t -> prev = l -> prev;
	t -> next = l;
	t -> prev -> next = t;

	l -> prev = t;
}

static inline void list_remove(list_node_t *l) {
	l -> prev -> next = l -> next;
	l -> next -> prev = l -> prev;

	if (!list_is_empty(l))
		free(l);
}
