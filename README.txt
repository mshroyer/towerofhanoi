	     ------------------------------------------------
			      Tower of Hanoi

		    Mark Shroyer <code@markshroyer.com>
		       Sun Jun  8 19:20:22 EDT 2014
	     ------------------------------------------------


Overview
========

This program illustrates a recursive solution to the Tower of Hanoi,
written in C++11 and built on top of Qt 5.3.  If you are reading this file
as part of a source distribution, refer to towersolver.cpp for the
algorithm itself and tower.cpp for the tower model; the rest of the code is
about providing a GUI application to demonstrate the algorithm.

Visit the Bitbucket project page for the latest version:

    https://bitbucket.org/markshroyer/towerofhanoi

You can get the source code by cloning the project with Mercurial:

    $ hg clone https://bitbucket.org/markshroyer/towerofhanoi


Binary releases
===============

Application binaries for OS X (10.6 or newer, 64-bit Intel only) and
Windows (Vista or later) are available on the downloads page:

    https://bitbucket.org/markshroyer/towerofhanoi/downloads

To run the OS X release, just mount the provided disk image and open the
application inside.

To run the Windows release you will first need to extract the given ZIP
archive.  Next, double-click on towerofhanoi.exe inside the extracted
folder to run the application.


Building from source
====================

Compilation requires a recent GCC (4.7 or later) or Clang+LLVM (3.3 or
later) C++11 toolchain, as well as the Qt 5 development tools.  Some gcc
and clang-specific intrinsics and flags are used, so this will not build
under MSVC without modification.

The build has not been tested with versions of Qt prior to 5.3, but it
*might* work with Qt 5.1 or later.  (Qt 4 is not supported.)

The application can be built by opening towerofhanoi.pro in a properly
configured installation of Qt Creator, or on the command line using the
instructions for your operating system below:


Linux
-----

Run these commands to build on Linux, optionally specifying a non-default
C++ compiler by setting the QMAKE_CXX variable:

    $ qmake -spec linux-g++ QMAKE_CXX=/usr/bin/g++-4.8
    $ make -j2


Mac OS X
--------

You will need the Xcode command-line tools installed to provide the Apple
Clang+LLVM toolchain.  Assuming a Qt 5.3 installation downloaded from
qt-project.org, run qmake to generate a Makefile, then run make to build
the application:

    $ ~/Qt/5.3/clang_64/bin/qmake
    $ make -j2

You can also tell qmake to output an Xcode project if desired:

    $ ~/Qt/5.3/clang_64/bin/qmake -spec macx-xcode


Windows
-------

Use the MinGW version of Qt for Windows, which can be obtained from the Qt
online installer provided at qt-project.org.  (Be sure to select the MinGW
toolchain under the Tools group in the installer if you do not already have
your own MinGW toolchain installed.)  Open the Qt 5.3.0 for Desktop command
prompt in the start menu, change to the source directory, and then run
these commands, substituting the path of your own MinGW toolchain as
necessary:

    > qmake.exe
    > C:\Qt\Tools\mingw482_32\bin\mingw32-make.exe

After the build, towerofhanoi.exe will be located in the release
subdirectory.


Deployment
==========

The release binary bundles were created with Qt 5.3's deployment tools.  To
create your own release on Mac OS X, build the application as described
above and then use the macdeployqt command to copy the necessary Qt
frameworks and plugins into the application bundle:

    $ macdeployqt towerofhanoi.app
    
For a Windows deployment, copy towerofhanoi.exe into a new folder and then
run windeployqt to copy necessary parts of Qt into the same folder.  The
dependency on QtSvg for rendering the control button icons must be
explicitly specified with the -svg flag (whereas macdeployqt infers this
from the project file):

    > windeployqt.exe towerofhanoi.exe -svg


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
