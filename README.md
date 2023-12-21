# Basic setup for OSdev

These files are well commented and are meant to remind you how to actually develop an OS. read from osdev.org to learn more.
## quickstart
```sh
mkdir -p $HOME/.local/opt
mkdir -p $HOME/.local/opt/cross
mkdir -p $HOME/.local/src
export PREFIX="$HOME/.local/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

cd $HOME/.local/src
mkdir build-binutils
cd build-binutils
curl https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.xz | tar -xJ
../binutils-2.41/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

cd $HOME/.local/src
command -v -- $TARGET-as || echo $TARGET-as is not in the PATH
curl https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz | tar -xJ
mkdir build-gcc
cd build-gcc
../gcc-13.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

export PATH="$HOME/.local/opt/cross/bin:$PATH"
```
