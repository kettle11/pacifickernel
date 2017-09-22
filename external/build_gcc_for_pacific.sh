echo "Building Binutils and GCC Cross Compiler for Pacific:"

# Based on the steps described at http://wiki.osdev.org/GCC_Cross-Compiler

# Add the installation prefix to the PATH of the current shell session
export PREFIX="$HOME/opt/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"

readonly GCCFOLDER=gcc-7.2.0
readonly BINUTILSFOLDER=binutils-2.28

echo "Building Binutils:"

# Build Binutils

mkdir build-binutils
cd build-binutils
../$BINUTILSFOLDER/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
cd ..

# 

echo "Building GCC:"

# Build GCC
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-gcc
cd build-gcc
../$GCCFOLDER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..