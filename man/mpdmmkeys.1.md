mpdmmkeys(1) -- control MPD playback via the X11 multimedia keys
================================================================

## SYNOPSIS

`mpdmmkeys [OPTIONS]`

## DESCRIPTION

**mpdmmkeys** is a simple daemon that listens to multimedia keys events and
controls MPD playback accordingly.

The supported multimedia keys are:

`XF86AudioPlay` toggles the playback.

`XF86AudioStop` stops the playback

`XF86AudioPrev` skips to the previous song.

`XF86AudioNext` skips to the next song.

## OPTIONS ##

`-a, --addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server address.

`-p, --port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server port.

`-s, --secret`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD password.

`-h, --help`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Show the help message.

## ENVIRONMENT ##

`MPD_HOST`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server address (overridden by `--addr`).

`MPD_PORT`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server port (overridden by `--port`).

## AUTHOR ##

Alessandro Ghedini <al3xbio@gmail.com>

## COPYRIGHT ##

Copyright (C) 2012 Alessandro Ghedini <al3xbio@gmail.com>

This program is released under the 3 clause BSD license.
