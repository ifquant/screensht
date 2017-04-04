![screensht](http://i.imgur.com/Xb18RmX.png)

# screensht

A simple and quick selection screenshot uploader for Linux written in C using Xlib, libjpeg and libcurl.

## Hosts

* [0x0.st](https://0x0.st)
* [mixtape.moe](https://mixtape.moe)
* [nya.is](https://nya.is)
* [p.fuwafuwa.moe](https://p.fuwafuwa.moe)
* [safe.moe](https://safe.moe)
* [cocaine.ninja](https://cocaine.ninja)
* [comfy.moe](https://comfy.moe)
* [pomf.cat](https://pomf.cat)
* [lewd.se](https://lewd.se)
* [memenet.org](https://memenet.org)
* [uguu.se](https://uguu.se)
* [yiff.moe](https://yiff.moe)
* [vidga.me](https://vidga.me)
* [aww.moe](https://aww.moe)
* [pomf.space](https://pomf.space)
* [reich.io](https://reich.io)
* [sugoi.vidyagam.es](https://sugoi.vidyagam.es)
* [up.che.moe](http://sugoi.vidyagam.es)

## Options

* ### -h

	Host, "random" for randomly chosen host.

	###### Default: `0x0.st`

		screensht -h safe.moe
		screensht -h random

* ### -q

	Quality, 0-100.

	###### Default: `99`

		screensht -q 100

* ### -d

	Directory to save in. If the specified path doesn't exist, it's created.
	
	###### Default: `./`

		screensht -d /home/me/screenshots/

* ### -f

	Font, "random" for randomly chosen font.

	###### Default: `*x14`

		screensht -f *x24
		screensht -f random

* ### -c

	Main color in ARGB hex format.

	###### Default: `fff0f0f0`

		screensht -c ffaa00ff
		screensht -c 0xffaa00ff

* ### -s

	Secondary color.

	###### Default: `ff000000`

		screensht -s ffffffff
		screensht -s 0xffffffff

## Usage

Bind a key to start the program using [XFCE application shortcuts](https://wiki.manjaro.org/index.php?title=XFCE:Making_Keyboard_Shortcuts_for_Frequently_Used_Applications), [Xbindkeys](https://wiki.archlinux.org/index.php/Xbindkeys) or [Gnome keyboard shortcuts](https://help.gnome.org/users/gnome-help/stable/keyboard-shortcuts-set.html.en).

To cancel during selection, press a mouse button other than left click or press any key.
