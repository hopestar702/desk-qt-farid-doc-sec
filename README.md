## Required for build:

* cmake >= 2.8.2
* Qt5 >= 5.7
* webenginewidgets

## Required for runtime:

* nmap + nping >= 6.00
* dig package (debian: dnsutils)

## Build on GNU/Linux:

    $ cd nmapsi4
    $ cd tools/
    $ ./cmake_verbose_script.sh

### OR

    $ cd nmapsi4
    $ mkdir build
    $ cd build/

### AND 

    $ make
    $ make install (only root) or create a package for your disto ;)
