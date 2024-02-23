exists () {
    if ! command -v &> /dev/null
    then
        echo "$1 not found. Please install it!"
        exit 1
    fi
}

deps=("make","tar","xz","curl","gcc","texinfo","gmp","mpfr","libmpc","nasm","xorriso","mtools")
for dep in $deps
do
    exists dep
done

mkdir -p $HOME/.local/opt
mkdir -p $HOME/.local/opt/cross
mkdir -p $HOME/.local/src
export PREFIX="$HOME/.local/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

cd $HOME/.local/src
curl https://ftp.gnu.org/gnu/binutils/binutils-2.41.tar.xz | tar -xJ

mkdir build-binutils
cd build-binutils
../binutils-2.41/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

make -j8
make install

cd $HOME/.local/src
command -v -- $TARGET-as || echo $TARGET-as is not in the PATH
curl https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz | tar -xJ
cd gcc-13.2.0
./contrib/download_prerequisites
cd ..

mkdir build-gcc
cd build-gcc
../gcc-13.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

make -j8 all-gcc
make -j8 all-target-libgcc
make install-gcc
make install-target-libgcc

export PATH="$HOME/.local/opt/cross/bin:$PATH"

echo """
CONFIGURATION COMPLETE! Please add the following line to your .bashrc, .zshrc, or whatever you use.
export PATH=\$HOME/.local/opt/cross/bin:\$PATH"""
