mpdircbot(1) -- control MPD playback through an IRC bot
=======================================================

## SYNOPSIS

`mpdircbot [OPTIONS]`

## DESCRIPTION

**mpdircbot** is a simple IRC bot that listens to private and channel
messages and controls the MPD playback accordingly. See [COMMANDS][] for the
complete list of recognized commands.

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

`-A, --irc-addr`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC server address (default `irc.freenode.net`).

`-P, --irc-port`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC server port (default `6667`).

`-N, --bot-nick`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The IRC nick for the bot (default `mpdircbot`).

`-S, --bot-secret`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
The password to access the bot (none by default).

`-h, --help`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Show the help message.

## COMMANDS ##

The supported commands are:

`play` starts the playback.

`pause` pauses the playback.

`toggle` toggles the playback.

`stop` stops the playback.

`prev` skips to the previous song.

`next` skips to the next song.

If a password has been defined with the `--bot-secret` option, all the
commands have to be prepended with that password, i.e. "<pass> <command>".

If invited in a channel the bot will join it and listen to commands sent
trough that channel too, so that everyone in the channel can control MPD. To
accept commands from a channel the mpdircbot nick has to be prepended to the
command itself, e.g.:

`mpdircbot, play`

`mpdircbot, pause`

`mpdircbot, toggle`

`...`

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
