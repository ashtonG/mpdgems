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
#include <libircclient/libircclient.h>

static struct mpd_connection *mpd = NULL;

static char *irc_nick = "mpdircbot";
static char *irc_pass = NULL;

static inline void help();

static void run_mpd_cmd(irc_session_t *irc, const char *cmd, const char *orig);

#define IRC_CB_PARAMS irc_session_t *irc, const char *ev, \
	const char *orig, const char **params, unsigned int count

static void connect_cb(IRC_CB_PARAMS);
static void privmsg_cb(IRC_CB_PARAMS);
static void invited_cb(IRC_CB_PARAMS);
static void channel_cb(IRC_CB_PARAMS);

#define CHECK_MPD_CONN(mpd)	\
	assert(mpd_connection_get_error(mpd) == MPD_ERROR_SUCCESS);

int main(int argc, char *argv[]) {
	int opts;

	const char   *short_opts  = "a:p:s:A:P:N:S:h";
	struct option long_opts[] = {
		{ "addr",	required_argument,	0, 'a' },
		{ "port",	required_argument,	0, 'p' },
		{ "secret",	required_argument,	0, 's' },
		{ "irc-addr",	required_argument,	0, 'A' },
		{ "irc-port",	required_argument,	0, 'P' },
		{ "bot-secret",	required_argument,	0, 'S' },
		{ "bot-nick",	required_argument,	0, 'N' },
		{ "help",	no_argument,		0, 'h' },
		{ 0,		0,			0,  0  }
	};

	char *mpd_addr = getenv("MPD_HOST");
	int   mpd_port = getenv("MPD_PORT") ? atoi(getenv("MPD_PORT")) : 0;
	char *mpd_pass = NULL;

	char *irc_addr = "irc.freenode.net";
	int   irc_port = 6667;

	irc_session_t *irc = NULL;
	irc_callbacks_t cbs = {
		.event_connect = connect_cb,
		.event_privmsg = privmsg_cb,
		.event_invite  = invited_cb,
		.event_channel = channel_cb,
	};

	while ((opts = getopt_long(argc, argv, short_opts, long_opts, 0)) != -1) {
		switch (opts) {
			case 'a': { mpd_addr = optarg;		break;     }
			case 'p': { mpd_port = atoi(optarg);	break;     }
			case 's': { mpd_pass = optarg;		break;     }
			case 'A': { irc_addr = optarg;		break;     }
			case 'P': { irc_port = atoi(optarg);	break;     }
			case 'N': { irc_nick = optarg;		break;     }
			case 'S': { irc_pass = optarg;		break;     }
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

	irc = irc_create_session(&cbs);

	irc_option_set(irc, LIBIRC_OPTION_STRIPNICKS);

	irc_connect(irc, irc_addr, irc_port, NULL, irc_nick, NULL, NULL);

	puts("Connecting to the IRC server...");
	irc_run(irc);

	return 0;
}

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdircbot [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--secret",	"-s",	"The MPD password");
	CMD_HELP("--irc-addr",	"-A",	"The IRC server address");
	CMD_HELP("--irc-port",	"-P",	"The IRC server port");
	CMD_HELP("--bot-secret","-S",	"The password to access the bot");
	CMD_HELP("--bot-nick",	"-N",	"The IRC nick for the bot");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}

static void run_mpd_cmd(irc_session_t *irc, const char *p, const char *orig) {
	char *cmd = (char *) p;

	if (irc_pass) {
		if (!strncmp(cmd, irc_pass, strlen(irc_pass)))
			cmd += strlen(irc_pass) + 1;
		else
			return;
	}

	if (!strncmp("play", cmd, 4)) {
		mpd_run_play(mpd);
		irc_cmd_me(irc, orig, "started playback");
		return;
	}

	if (!strncmp("pause", cmd, 5)) {
		mpd_run_pause(mpd, 1);
		irc_cmd_me(irc, orig, "paused playback");
		return;
	}

	if (!strncmp("toggle", cmd, 6)) {
		mpd_run_toggle_pause(mpd);
		irc_cmd_me(irc, orig, "toggled playback");
		return;
	}

	if (!strncmp("stop", cmd, 4)) {
		mpd_run_stop(mpd);
		irc_cmd_me(irc, orig, "paused playback");
		return;
	}

	if (!strncmp("prev", cmd, 4)) {
		mpd_run_previous(mpd);
		irc_cmd_me(irc, orig, "skipped to the previous song");
		return;
	}

	if (!strncmp("next", cmd, 4)) {
		mpd_run_next(mpd);
		irc_cmd_me(irc, orig, "skipped to the next song");
		return;
	}

	/*if (!strncmp("quit", cmd, 4)) {
		irc_destroy_session(irc);
	}*/

	CHECK_MPD_CONN(mpd);
}

static void connect_cb(IRC_CB_PARAMS) {
	puts("Now connected, what are your orders, sir?");
}

static void privmsg_cb(IRC_CB_PARAMS) {
	run_mpd_cmd(irc, params[1], orig);
}

static void invited_cb(IRC_CB_PARAMS) {
	irc_cmd_join(irc, params[1], NULL);
}

static void channel_cb(IRC_CB_PARAMS) {
	char *text = (char *) params[1];

	if (!strncmp(text, irc_nick, strlen(irc_nick))) {
		text += strlen(irc_nick) + 2;

		run_mpd_cmd(irc, text, params[0]);
	}
}
