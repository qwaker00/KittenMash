## Installation guide

### Install dependencies
```
sudo apt-get install g++ git nginx mongodb cmake fastcgi-daemon2 libfastcgi-daemon2-dev mongodb-dev libboost-thread-dev boost-system-dev boost-regex-dev boost-filesystem-dev
```

### Checkout, build, install and run service
```
git clone https://github.com/qwaker00/KittenMash
cd KittenMash
mkdir build
cd build
cmake ../src/
make -j4
cd ../deploy/
sudo ./install.sh
sudo ./run.sh
```
