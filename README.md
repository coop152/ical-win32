Ical-win32
====
Ical-win32 is a Windows port of Sanjay Ghemawat's Unix calendar program, [Ical](https://en.wikipedia.org/wiki/Ical_(Unix)).
In addition to building on both Windows and Linux, it features:

* A Windows installer
* A delete history that allows you to restore deleted items
* An auto-clearing feature for said delete history, to clear it out regularly
* The ability to mass-delete items before a given date


This release of ical is designed to work with Tcl8.6, and hasn't been tested on any earlier version.
A copy of [IronTcl](https://www.irontcl.com/index.html) is included for the Windows platform, but the Linux
version uses the system Tcl/Tk installation.


See also: [The original distribution's README file](/README)


Compiling
============
### Windows
Compiling on Windows requires Visual Studio. Open the included solution (`ical-win32.sln`).

If you would like to debug, then compile and run the `ical-win32` project in the Debug profile.

If you would like to install the program, switch to the release profile
and compile the `installer` project. The resulting installer executable is found in `/installer/Release`.

### Linux
Compiling on Linux requires a standard GNU development environment (gcc, GNU configure, automake, etc.)
and an installation of Tcl/Tk 8.6. On Ubuntu you can use these packages:
```bash
sudo apt install tcl-dev tk-dev
```

Use autoreconf to generate a configure script:
```bash
autoreconf -ivf
```

Run the configure script:
```bash
./configure --prefix=/home/username/.local
```
Where the prefix is the base directory to install to.
Your user must have write permissions to this directory,
so somewhere in your home directory (e.g. `~/.local` on Ubuntu) is advisable.

Then, to compile and install:
```bash
make install
```
And the program will be installed in the prefix.

If you installed to `~/.local` on Ubuntu as in the example, then the program will already be on the PATH
and you can run it with `ical`. Otherwise, the executable is located in `$(prefix)/bin`.



Copyrights
==========
Copyright (c) 1993 by Sanjay Ghemawat

Most of the files are covered by the copyright in COPYRIGHT.ORIG.

The included IronTcl binaries are covered by the Tcl/Tk licensing terms (see license.terms).

The configure script is covered by the GNU Public License v2.