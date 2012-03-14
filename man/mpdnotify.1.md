mpdnotify(1) -- stupid desktop notifier for MPD
===============================================

## SYNOPSIS

`mpdnotify [OPTIONS]`

## DESCRIPTION

**mpdnotify** is a simple daemon that shows a desktop notification when the
song that MPD is playing changes. It can also optionally show the album
cover art if the music database is located on the local system.

## OPTIONS ##

`-d, --dir`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The "root" music directory. This is used to find album cover files.

`-c, --cover`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The cover file name (default `cover.jpg`).

`-C, --nocover`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Do not insert the cover images in the notifications.

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
