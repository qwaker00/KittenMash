## Installation guide

### Install dependencies
```
sudo apt-get install g++ git nginx mongodb cmake fastcgi-daemon2 libfastcgi-daemon2-dev mongodb-dev libboost-thread-dev boost-system-dev boost-regex-dev boost-filesystem-dev
```

### Checkout repo
```
git clone https://github.com/qwaker00/KittenMash
cd KittenMash
```

### Build backend
```
mkdir build
cd build
cmake ../src/
make -j4
```

### Install links to congigs and libs
```
cd ../deploy/
sudo ./install.sh
```
From now client is available at http://localhost:8080

### Run backend
```
sudo ./run.sh
```
Congrats! Service is up!

### Development
When code of client is changed it immediately available to users.
When code of server is chaged you need to rebuild it (just ```make``` in build directory ) and rerun (```./run.sh```).
