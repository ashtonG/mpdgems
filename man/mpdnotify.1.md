mpdnotify(1) -- stupid desktop notifier for MPD
===============================================

## SYNOPSIS

`mpdnotify [OPTIONS]`

## DESCRIPTION

**mpdnotify** is a simple utility that polls the MPD server for song changes, and
shows a nice desktop notification when that happens.

## OPTIONS ##

`-d, --dir`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The "root" music directory.

`-c, --cover`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The cover file name (default `cover.jpg`).

`-C, --nocover`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Do not use the album's cover image in the notifications.

`-a, --addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server address.

`-p, --port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The MPD server port.

`-h, --help`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Show the help message.

## AUTHOR ##

Alessandro Ghedini <al3xbio@gmail.com>

## COPYRIGHT ##

Copyright (C) 2012 Alessandro Ghedini <al3xbio@gmail.com>

This program is released under the 3 clause BSD license.
