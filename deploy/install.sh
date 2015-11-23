sudo mkdir /usr/share/kittenmash/
sudo ln -s "$(pwd)"/../www /usr/share/kittenmash/
sudo ln -s "$(pwd)"/../build/ /usr/lib/kittenmash
sudo ln -s "$(pwd)"/nginx.conf /etc/nginx/sites-enabled/kittenmash.conf
sudo nginx -s reload
