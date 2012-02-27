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

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <mpd/client.h>

static inline void help();

static inline void x11_grab_key(Display *dpy, Window root, const char *key_string);
static inline void x11_ungrab_key(Display *dpy, Window root, const char *key_string);
static inline void x11_handle_key(Display *dpy, XEvent ev, struct mpd_connection *mpd);

int main(int argc, char *argv[]) {
	int opts;

	struct option long_opts[] = {
		{ "addr",	required_argument,	0, 'a' },
		{ "port",	required_argument,	0, 'p' },
		{ "secret",	required_argument,	0, 's' },
		{ "help",	no_argument,		0, 'h' },
		{ 0,		0,			0,  0  }
	};

	struct mpd_connection *mpd = NULL;

	char *mpd_addr = getenv("MPD_HOST");
	int   mpd_port = getenv("MPD_PORT") ? atoi(getenv("MPD_PORT")) : 0;
	char *mpd_pass = NULL;

	Display	*dpy	= XOpenDisplay(0);
	Window	root	= DefaultRootWindow(dpy);

	while ((opts = getopt_long(argc, argv, "a:p:s:h", long_opts, 0)) != -1) {
		switch (opts) {
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

		if (mpd_connection_get_error(mpd) != MPD_ERROR_SUCCESS) {
			mpd_connection_free(mpd);
			return -1;
		}
	}

	XSelectInput(dpy, root, KeyPressMask);

	while (true) {
		XEvent	ev;

		x11_grab_key(dpy, root, "XF86AudioPlay");
		x11_grab_key(dpy, root, "XF86AudioStop");
		x11_grab_key(dpy, root, "XF86AudioPrev");
		x11_grab_key(dpy, root, "XF86AudioNext");

		/*
		x11_grab_key("XF86AudioMute", dpy, root);
		x11_grab_key("XF86AudioRaiseVolume", dpy, root);
		x11_grab_key("XF86AudioLowerVolume", dpy, root);
		*/

		XNextEvent(dpy, &ev);

		switch (ev.type) {
			case KeyPress:
				x11_handle_key(dpy, ev, mpd);
				break;
		}

		x11_ungrab_key(dpy, root, "XF86AudioPlay");
		x11_ungrab_key(dpy, root, "XF86AudioStop");
		x11_ungrab_key(dpy, root, "XF86AudioPrev");
		x11_ungrab_key(dpy, root, "XF86AudioNext");

		/*
		x11_ungrab_key("XF86AudioMute", dpy, root);
		x11_ungrab_key("XF86AudioRaiseVolume", dpy, root);
		x11_ungrab_key("XF86AudioLowerVolume", dpy, root);
		*/
	}

	XCloseDisplay(dpy);
	mpd_connection_free(mpd);

	return 0;
}

static inline void help() {
	#define CMD_HELP(CMDL, CMDS, MSG) printf("  %s, %s\t%s.\n", CMDS, CMDL, MSG);

	puts("Usage: mpdmmkeys [OPTIONS]\n");
	puts(" Options:");

	CMD_HELP("--addr",	"-a",	"The MPD server address");
	CMD_HELP("--port",	"-p",	"The MPD server port");
	CMD_HELP("--secret",	"-s",	"The MPD password");
	CMD_HELP("--help",	"-h",	"Show this help");

	puts("");
}

#define XKeyStringToKeycode(STR) XKeysymToKeycode(dpy, XStringToKeysym(STR))
#define XEvEqKeyString(EV, KEYSTR) (EV.xkey.keycode ==  XKeyStringToKeycode(KEYSTR))

static inline void x11_grab_key(Display *dpy, Window root, const char *key_string) {
	unsigned int keycode = XKeyStringToKeycode(key_string);

	XGrabKey(dpy, keycode, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);
}

static inline void x11_ungrab_key(Display *dpy, Window root, const char *key_string) {
	unsigned int keycode = XKeyStringToKeycode(key_string);

	XUngrabKey(dpy, keycode, AnyModifier, root);
}

static inline void x11_handle_key(Display *dpy, XEvent ev, struct mpd_connection *mpd) {
	if (XEvEqKeyString(ev, "XF86AudioPlay")) { mpd_run_toggle_pause(mpd);	return; }
	if (XEvEqKeyString(ev, "XF86AudioStop")) { mpd_run_stop(mpd);		return; }
	if (XEvEqKeyString(ev, "XF86AudioPrev")) { mpd_run_previous(mpd);	return; }
	if (XEvEqKeyString(ev, "XF86AudioNext")) { mpd_run_next(mpd);		return; }

	XAllowEvents(dpy, AsyncKeyboard, ev.xkey.time);
}
