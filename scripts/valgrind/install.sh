cd ~/Downloads

sudo apt remove --purge valgrind -y

wget https://sourceware.org/pub/valgrind/valgrind-3.20.0.tar.bz2
tar xvf valgrind-3.20.0.tar.bz2

export CC=gcc-12

cd valgrind-3.20.0
./configure
make
sudo make install

cd ..
rm -r valgrind-3.20.0
rm -r valgrind-3.20.0.tar.bz2

echo ""
valgrind --version
