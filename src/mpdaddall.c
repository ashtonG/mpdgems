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
#include <string.h>
#include <mpd/client.h>

typedef struct NODE {
	void *value;

	struct NODE *prev;
	struct NODE *next;
} list_node_t;

#define list_foreach(ITEM, L) \
	for (ITEM = list_head(L); ITEM != L; ITEM = ITEM -> next)

static inline list_node_t *list_init() {
	list_node_t *t = (list_node_t *) malloc(sizeof(list_node_t));

	if (t == NULL)
		return NULL;

	t -> prev = t;
	t -> next = t;

	return t;
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

static struct option long_opts[] = {
	{ "addr",	required_argument,	0, 'a' },
	{ "port",	required_argument,	0, 'p' },
	{ "help",	no_argument,		0, 'h' },
	{ 0,		0,			0,  0  }
};

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdaddall [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}

int main(int argc, char *argv[]) {
	int opts;

	struct mpd_song *song = NULL;
	struct mpd_connection *mpd = NULL;

	list_node_t *songs_list = list_init(), *iter;

	char *mpd_addr = NULL;
	int   mpd_port = 0;

	while ((opts = getopt_long(argc, argv, "a:p:h", long_opts, 0)) != -1) {
		switch (opts) {
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

	mpd_send_list_all(mpd, "/");

	while ((song = mpd_recv_song(mpd))) {
		if (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS) {
			mpd_connection_free(mpd);
			return -1;
		}

		list_insert_tail(songs_list, strdup(mpd_song_get_uri(song)));

		mpd_song_free(song);
	}

	mpd_response_finish(mpd);

	list_foreach(iter, songs_list) {
		char *uri = iter -> value;
		mpd_run_add(mpd, uri);

		if (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS) {
			mpd_connection_free(mpd);
			return -1;
		}
	}

	mpd_connection_free(mpd);

	return 0;
}
