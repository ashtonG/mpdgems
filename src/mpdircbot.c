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

#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <mpd/client.h>
#include <libircclient/libircclient.h>

static struct mpd_connection *mpd = NULL;

static inline void help();

#define IRC_CB_PARAMS irc_session_t *irc, const char *ev, \
	const char *orig, const char **params, unsigned int count

static void connect_cb(IRC_CB_PARAMS);
static void privmsg_cb(IRC_CB_PARAMS);

int main(int argc, char *argv[]) {
	int opts;

	struct option long_opts[] = {
		{ "addr",	required_argument,	0, 'a' },
		{ "port",	required_argument,	0, 'p' },
		{ "irc-addr",	required_argument,	0, 'A' },
		{ "irc-port",	required_argument,	0, 'P' },
		{ "nick",	required_argument,	0, 'N' },
		{ "help",	no_argument,		0, 'h' },
		{ 0,		0,			0,  0  }
	};

	char *mpd_addr = NULL;
	int   mpd_port = 0;

	char *irc_addr = "irc.freenode.net";
	int   irc_port = 6667;
	char *irc_nick = "mpdircbot";

	irc_session_t *irc = NULL;
	irc_callbacks_t cbs = {
		.event_connect = connect_cb,
		.event_privmsg = privmsg_cb,
	};

	while ((opts = getopt_long(argc, argv, "a:p:A:P:N:h", long_opts, 0)) != -1) {
		switch (opts) {
			case 'a': { mpd_addr = optarg;		break;     }
			case 'p': { mpd_port = atoi(optarg);	break;     }
			case 'A': { irc_addr = optarg;		break;     }
			case 'P': { irc_port = atoi(optarg);	break;     }
			case 'N': { irc_nick = optarg;		break;     }
			default :
			case 'h': { help();			return -1; }
		}
	}

	do {
		if (mpd != NULL) mpd_connection_free(mpd);
		mpd = mpd_connection_new(mpd_addr, mpd_port, 30000);
	} while (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS);

	irc = irc_create_session(&cbs);

	irc_connect(irc, irc_addr, irc_port, NULL, irc_nick, NULL, NULL);

	irc_run(irc);

	return 0;
}

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdircbot [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--irc-addr",	"-A",	"The IRC server address");
	CMD_HELP("--irc-port",	"-P",	"The IRC server port");
	CMD_HELP("--nick",	"-N",	"The IRC nick for the bot");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}

static void connect_cb(IRC_CB_PARAMS) {
	puts("Now connected, what are your order, sir?");
}

static void privmsg_cb(IRC_CB_PARAMS) {
	if (!strncmp("play", params[1], 4)) {
		mpd_run_play(mpd);
		return;
	}

	if (!strncmp("pause", params[1], 5)) {
		mpd_run_pause(mpd, 1);
		return;
	}

	if (!strncmp("toggle", params[1], 6)) {
		mpd_run_toggle_pause(mpd);
		return;
	}

	if (!strncmp("stop", params[1], 4)) {
		mpd_run_stop(mpd);
		return;
	}

	if (!strncmp("prev", params[1], 4)) {
		mpd_run_previous(mpd);
		return;
	}

	if (!strncmp("next", params[1], 4)) {
		mpd_run_next(mpd);
		return;
	}

	/*if (!strncmp("quit", params[1], 4)) {
		irc_destroy_session(irc);
	}*/
}
