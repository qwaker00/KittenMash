echo 'Running'

sudo ls >/dev/null

sudo fastcgi-daemon2 --config=daemon.conf >daemon.out 2>daemon.err &
echo "$!" >daemon.pid
echo "Running daemon: pid $!"

/usr/lib/kittenmash/rating-updater >updater.out 2>updater.err &
echo "$!" >updater.pid
echo "Running updater: pid $!"

echo 'Done'
