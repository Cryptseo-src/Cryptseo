
Debian
====================
This directory contains files used to package cryptseod/cryptseo-qt
for Debian-based Linux systems. If you compile cryptseod/cryptseo-qt yourself, there are some useful files here.

## cryptseo: URI support ##


cryptseo-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install cryptseo-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your cryptseoqt binary to `/usr/bin`
and the `../../share/pixmaps/cryptseo128.png` to `/usr/share/pixmaps`

cryptseo-qt.protocol (KDE)

