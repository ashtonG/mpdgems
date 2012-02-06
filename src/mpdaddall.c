/*
 * Collection of more or less useful MPD tools.
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

static inline void help();

static inline list_node_t *list_init();
static inline list_node_t *list_head(list_node_t *l);
static inline void list_insert_tail(list_node_t *l, void *item);

#define list_foreach(ITEM, L) \
	for (ITEM = list_head(L); ITEM != L; ITEM = ITEM -> next)

int main(int argc, char *argv[]) {
	int opts;

	struct option long_opts[] = {
		{ "addr",	required_argument,	0, 'a' },
		{ "port",	required_argument,	0, 'p' },
		{ "help",	no_argument,		0, 'h' },
		{ 0,		0,			0,  0  }
	};

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

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdaddall [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
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
