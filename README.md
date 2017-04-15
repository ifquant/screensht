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
* [~~aww.moe~~](https://aww.moe)<sup>† 2017</sup>
* [pomf.space](https://pomf.space)
* [reich.io](https://reich.io)
* [sugoi.vidyagam.es](https://sugoi.vidyagam.es)
* [up.che.moe](http://sugoi.vidyagam.es)

## Options

* ### -h, --host

	Host, "random" for randomly chosen host.

	###### Default: `0x0.st`

		screensht -h safe.moe
		screensht -h random

* ### -q, --quality

	Quality, 0-100.

	###### Default: `99`

		screensht -q 100

* ### -f, --font

	Font, "random" for randomly chosen font.

	###### Default: `*x14`

		screensht -f *x24
		screensht -f random

* ### -c, --color

	Main color in ARGB hex format.

	###### Default: `fff0f0f0`

		screensht -c ffaa00ff
		screensht -c 0xffaa00ff

* ### -s, --secondary-color

	Secondary color.

	###### Default: `ff000000`

		screensht -s ffffffff
		screensht -s 0xffffffff

* ### -k, --keep

	Where to save the file. If not specified, the image won't be saved.

	###### Default: `None`

		screensht -k ./
		screensht -k ~/screenshots

* ### -n, --form-filename

	The filename that the screenshot is going to be uploaded with, can be used to get unusual file extensions. These may not work on some hosts.

	###### Default: `screensht.jpg`

		screensht -n example.cool

## Compatibility Mode

If you don't have a composite manager installed, you won't be able to use some features such as colors and the selection might glitch out once in a while. To combat that, install one of [these](https://wiki.archlinux.org/index.php/Xorg#Composite).

## Usage

Clone with `git clone --recursive https://github.com/nvllvs/screensht`.

Run `./install-deps.sh` if you use apt-get (Debian-based distros) or pacman (Arch-based distros). Else find the packages yourself.

Compile with `cmake .; make`.

Bind a key to start the program using [XFCE application shortcuts](https://wiki.manjaro.org/index.php?title=XFCE:Making_Keyboard_Shortcuts_for_Frequently_Used_Applications), [Xbindkeys](https://wiki.archlinux.org/index.php/Xbindkeys) or [Gnome keyboard shortcuts](https://help.gnome.org/users/gnome-help/stable/keyboard-shortcuts-set.html.en).

To cancel during selection, press a mouse button other than left click or press any key.
