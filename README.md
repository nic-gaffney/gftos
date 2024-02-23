# Basic setup for OSdev

These files are well commented and are meant to remind you how to actually develop an OS. read from osdev.org to learn more.
## Requirements
- git
- make
- tar
- xz
- curl
- C compiler (preferrably gcc)
- texinfo
- gmp
- mpfr
- libmpc
- nasm
- xorriso
- mtools
- qemu  (to run)
- bochs (for testing)

## Install dependencies
Void:
```sh
$ xbps-install git make tar xz curl gcc texinfo gmp mpfr libmpc nasm xorriso mtools
```

## Configure and build
```sh
$ ./configure.sh
$ make
```
