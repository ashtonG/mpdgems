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
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <mpd/client.h>
#include <libnotify/notify.h>

static char *music_dir  = NULL;
static char *cover_file = "cover.jpg";
static int   nocover    = 0;

static inline void help();
static void notify(NotifyNotification *n, struct mpd_song *song);

#define CHECK_MPD_CONN(mpd)	\
	assert(mpd_connection_get_error(mpd) == MPD_ERROR_SUCCESS);

int main(int argc, char *argv[]) {
	int opts;

	const char   *short_opts  = "d:c:Ca:p:s:h";
	struct option long_opts[] = {
		{ "dir",	required_argument,	0, 'd' },
		{ "cover",	required_argument,	0, 'c' },
		{ "nocover",	no_argument,		0, 'C' },
		{ "addr",	required_argument,	0, 'a' },
		{ "port",	required_argument,	0, 'p' },
		{ "secret",	required_argument,	0, 's' },
		{ "help",	no_argument,		0, 'h' },
		{ 0,		0,			0,  0  }
	};

	unsigned int last_seen = 0;
	struct mpd_connection *mpd = NULL;

	char *mpd_addr = getenv("MPD_HOST");
	int   mpd_port = getenv("MPD_PORT") ? atoi(getenv("MPD_PORT")) : 0;
	char *mpd_pass = NULL;

	NotifyNotification *n = NULL;

	while ((opts = getopt_long(argc, argv, short_opts, long_opts, 0)) != -1) {
		switch (opts) {
			case 'd': { music_dir = optarg;		break;     }
			case 'c': { cover_file = optarg;	break;     }
			case 'C': { nocover = 1;		break;     }
			case 'a': { mpd_addr = optarg;		break;     }
			case 'p': { mpd_port = atoi(optarg);	break;     }
			case 's': { mpd_pass = optarg;		break;     }
			default :
			case 'h': { help();			return -1; }
		}
	}

	do {
		if (mpd != NULL) mpd_connection_free(mpd);
		mpd = mpd_connection_new(mpd_addr, mpd_port, 30000);
	} while (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS);

	if (mpd_pass != NULL) {
		mpd_run_password(mpd, mpd_pass);
		CHECK_MPD_CONN(mpd);
	}

	notify_init("mpdnotify");

	n = notify_notification_new("", "", "");

	while (true) {
		struct mpd_song *song = mpd_run_current_song(mpd);
		CHECK_MPD_CONN(mpd);

		if (song != NULL) {
			unsigned int id = mpd_song_get_id(song);

			if (id != last_seen) {
				notify(n, song);
				last_seen = id;
			}

			mpd_song_free(song);
		}

		mpd_run_idle_mask(mpd, MPD_IDLE_PLAYER);
		CHECK_MPD_CONN(mpd);
	}

	mpd_connection_free(mpd);
	notify_uninit();

	return 0;
}

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdnotify [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--dir",	"-d",	"The \"root\" music directory");
	CMD_HELP("--cover",	"-c",	"The cover file name (default 'cover.jpg')");
	CMD_HELP("--nocover",	"-C",	"Do not use the album's cover image in the notifications");
	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--secret",	"-s",	"The MPD password");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}

static void notify(NotifyNotification *n, struct mpd_song *song) {
	struct stat sb;

	const char *title  = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
	const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
	const char *album  = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);

	const char *uri    = mpd_song_get_uri(song);
	const char *dir    = dirname((char *) uri);

	char *body = NULL;
	size_t body_len = 0;

	if (artist)
		body_len += strlen("of ") + strlen(artist) + 1;

	if (album)
		body_len += strlen("from ") + strlen(album);

	body = calloc(body_len + 1, 1);

	if (artist) {
		body = strcat(body, "of ");
		body = strcat(body, artist);
		body = strcat(body, "\n");
	}

	if (album) {
		body = strcat(body, "from ");
		body = strcat(body, album);
	}

	notify_notification_close(n, NULL);

	if (!nocover && music_dir) {
		size_t cover_len = strlen(music_dir) + 1 +
			strlen(dir) + 1 +
			strlen(cover_file);

		char *cover = malloc(cover_len + 1);
		sprintf(cover, "%s/%s/%s", music_dir, dir, cover_file);

		if (!stat(cover, &sb) && S_ISREG(sb.st_mode)) {
			GdkPixbuf *scaled = NULL;
			GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(
				cover, NULL
			);

			char *template = "/tmp/mpdnotify.XXXXXX";
			char *tmpfile  = malloc(strlen(template) + 1);
			tmpfile        = strcpy(tmpfile, template);
			mkstemp(tmpfile);

			scaled = gdk_pixbuf_scale_simple(
				pixbuf, 50, 50, GDK_INTERP_HYPER
			);

			gdk_pixbuf_save(scaled, tmpfile, "jpeg", NULL, NULL);

			g_object_unref(pixbuf);
			g_object_unref(scaled);

			notify_notification_update(n, title, body, tmpfile);
			notify_notification_show(n, NULL);

			unlink(tmpfile);

			return;
		}
	}

	notify_notification_update(n, title, body, "media-playback-start");
	notify_notification_show(n, NULL);
}
