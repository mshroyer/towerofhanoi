			      Tower of Hanoi
		    Mark Shroyer <code@markshroyer.com>

This program illustrates a recursive solution to the Tower of Hanoi,
written in C++11 and built on top of Qt 5.3.  See towersolver.cpp for the
algorithm itself; the rest of the code provides a GUI application to
demonstrate it.


Binary releases
===============

Visit the downloads page to get binaries for OS X (10.6 or newer, 64-bit
Intel only) and Windows (Vista or later):

https://bitbucket.org/markshroyer/towerofhanoi/downloads

To run the OS X release, mount the provided disk image and open the
application inside.  To run the Windows release, extract the ZIP archive
and then double-click towerofhanoi.exe in the extracted folder.


Building from source
====================

Compilation requires a recent GCC (4.7 or later) or Clang+LLVM (3.3 or
later) C++11 toolchain, as well as the Qt 5 development tools.  Some gcc
and clang-specific intrinsics and flags are used, so this will not build
under MSVC without modification.

Run the following commands to build on Linux, optionally specifying a
non-default C++ compiler to qmake:

    $ qmake -spec linux-g++ QMAKE_CXX=/usr/bin/g++-4.8
    $ make

The release binary bundles were created with Qt 5.3's deployment tools.  To
deploy your own release on OS X after completing a release build, run:

    $ macdeployqt towerofhanoi.app -dmg
    
For Windows deployment, the dependency on QtSvg for the control button
icons must be explicitly specified on the command line (whereas
`macdeployqt` infers this from the project file):

    $ windeployqt towerofhanoi.exe -svg


Copyright and license
=====================

Copyright (c) 2014 Mark Shroyer <code@markshroyer.com>

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
