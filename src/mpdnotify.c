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
 *     * Neither the name of the mpdutils project, Alessandro Ghedini, nor the
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

#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <mpd/client.h>
#include <libnotify/notify.h>

static char *music_dir  = NULL;
static char *cover_file = "cover.jpg";
static int   nocover    = 0;

static void notify(NotifyNotification *n, struct mpd_song *song) {
	struct stat sb;

	const char *title  = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
	const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
	const char *album  = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);

	const char *uri    = mpd_song_get_uri(song);
	const char *dir    = dirname((char *) uri);

	char *body = NULL;
	size_t body_len = strlen("of ") + strlen(artist) + 1 +
		strlen("from ") + strlen(album);

	char *cover = NULL;
	size_t cover_len = strlen(music_dir) + 1 +
		strlen(dir) + 1 +
		strlen(cover_file);

	body = malloc(body_len + 1);
	sprintf(body, "of %s\nfrom %s", artist, album);

	notify_notification_close(n, NULL);

	if (!nocover && music_dir) {
		cover = malloc(cover_len + 1);
		sprintf(cover, "%s/%s/%s", music_dir, dir, cover_file);

		if (!stat(cover, &sb) && S_ISREG(sb.st_mode)) {
			GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(cover, NULL);
			notify_notification_set_image_from_pixbuf(n, pixbuf);
			gdk_pixbuf_unref(pixbuf);
		}
	}

	notify_notification_update(n, title, body, "media-playback-start");
	notify_notification_show(n, NULL);
}

static struct option long_opts[] = {
	{ "dir",	required_argument,	0, 'd' },
	{ "cover",	required_argument,	0, 'c' },
	{ "nocover",	no_argument,		0, 'C' },
	{ "addr",	required_argument,	0, 'a' },
	{ "port",	required_argument,	0, 'p' },
	{ "help",	no_argument,		0, 'h' },
	{ 0,		0,			0,  0  }
};

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdaddall [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--dir",	"-d",	"The \"root\" music directory");
	CMD_HELP("--cover",	"-c",	"The cover file name (default 'cover.jpg')");
	CMD_HELP("--nocover",	"-C",	"Do not use the album's cover image in the notifications");
	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}

int main(int argc, char *argv[]) {
	int opts;

	unsigned int last_seen = 0;
	struct mpd_connection *mpd;

	char *mpd_addr = NULL;
	int   mpd_port = 0;

	while ((opts = getopt_long(argc, argv, "d:c:Ca:p:h", long_opts, 0)) != -1) {
		switch (opts) {
			case 'd': { music_dir = optarg;		break;     }
			case 'c': { cover_file = optarg;	break;     }
			case 'C': { nocover = 1;		break;     }
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

	notify_init("mpdnotify");

	while (true) {
		NotifyNotification *n = notify_notification_new("", "", "");

		struct mpd_song *song = mpd_run_current_song(mpd);

		if (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS) {
			mpd_connection_free(mpd);
			return -1;
		}

		if (song != NULL) {
			unsigned int id = mpd_song_get_id(song);

			if (id != last_seen) {
				notify(n, song);
				last_seen = id;
			}

			mpd_song_free(song);
		}

		mpd_run_idle_mask(mpd, MPD_IDLE_PLAYER);

		if (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS) {
			mpd_connection_free(mpd);
			return -1;
		}
	}

	mpd_connection_free(mpd);
	notify_uninit();

	return 0;
}
